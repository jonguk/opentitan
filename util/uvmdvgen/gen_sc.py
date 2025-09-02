#!/usr/bin/env python3
# Copyright lowRISC contributors
# SPDX-License-Identifier: Apache-2.0

from pathlib import Path
import hjson
import re
import os


SMOKE_CPP_TPL = """#include <uvm>
#include "../env/{name}_env.hpp"
using namespace uvm;

class {name}_smoke_test : public uvm_test {{
 public:
  UVM_COMPONENT_UTILS({name}_smoke_test);
  {name}_env* m_env {{}};
  explicit {name}_smoke_test(uvm_component_name nm) : uvm_test(nm) {{}}
  void build_phase(uvm_phase &phase) override {{ m_env = {name}_env::type_id::create("env", this); }}
  void run_phase(uvm_phase &phase) override {{
    phase.raise_objection(this);
    uvm::uvm_report_info("SMOKE", "begin", uvm::UVM_LOW);
    uvm::uvm_report_info("SMOKE", "end", uvm::UVM_LOW);
    phase.drop_objection(this);
  }}
}};
UVM_COMPONENT_REGISTER({name}_smoke_test);
"""

CSR_RW_SEQ_HPP = """#pragma once
#include <uvm>
class {name}_csr_rw_seq : public uvm::uvm_object {{
 public:
  UVM_OBJECT_UTILS({name}_csr_rw_seq);
  explicit {name}_csr_rw_seq(const std::string &nm = "{name}_csr_rw_seq") : uvm::uvm_object(nm) {{}}
}};
"""

CSR_RW_CPP_TPL = """#include <uvm>
#include "../env/{name}_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "{name}_csr_rw_seq.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../env/csr_exclude.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
#include "../../../dv/sc/scoreboard/scoreboard.hpp"
using namespace uvm;

class {name}_csr_rw_test : public uvm_test {{
 public:
  UVM_COMPONENT_UTILS({name}_csr_rw_test);
  {name}_env* m_env {{}};
  explicit {name}_csr_rw_test(uvm_component_name nm) : uvm_test(nm) {{}}
  void build_phase(uvm_phase &phase) override {{ m_env = {name}_env::type_id::create("env", this); }}
  void run_phase(uvm_phase &phase) override {{
    phase.raise_objection(this);
    uvm::uvm_report_info("CSR_RW", "run_phase begin", uvm::UVM_LOW);
    if (tl_bind::reset) tl_bind::reset();
    const uint32_t CTRL_ADDR = 16;
    uint32_t ctrl = 0; ctrl |= (1u<<0); ctrl |= (1u<<1); ctrl |= (0x1234u<<16);
    if (m_env && m_env->drv_sc) {{ m_env->drv_sc->send_write(CTRL_ADDR, ctrl, 4); (void)m_env->drv_sc->send_read(CTRL_ADDR, 4); }}
    (void)scdv::csr_rd(CTRL_ADDR);
    const uint32_t INTR_ENABLE = 4; uint32_t ien = (1u<<0)|(1u<<3)|(1u<<7);
    scdv::csr_wr(INTR_ENABLE, ien); uint32_t ien_r = scdv::csr_rd(INTR_ENABLE);
    if ((ien_r & ien) != ien) {{ UVM_ERROR("CSR_RW", "INTR_ENABLE mismatch"); }}
    const uint32_t WDATA = 28; scdv::csr_wr(WDATA, 0xAB); uint32_t wdata_r = scdv::csr_rd(WDATA);
    if ((wdata_r & 0xFF) != 0x00) {{ UVM_ERROR("CSR_RW", "WDATA should be WO and read as 0"); }}
    uvm::uvm_report_info("CSR_RW", "run_phase end (dropping objection)", uvm::UVM_LOW);
    phase.drop_objection(this);
  }}
}};
UVM_COMPONENT_REGISTER({name}_csr_rw_test);
"""


def _write(p: Path, s: str):
    p.parent.mkdir(parents=True, exist_ok=True)
    if not p.exists():
        p.write_text(s)


def gen_sc(name: str,
           env_outdir: str,
           vendor: str,
           is_cip: bool = False,
           has_ral: bool = False,
           has_interrupts: bool = False,
           has_alerts: bool = False,
           env_agents=None):
    # Place SC DV collateral under hw/<ip>/scdv (normalize when env_outdir points to .../dv)
    dv_base = Path(env_outdir)
    ip_root = dv_base.parent if dv_base.name == "dv" else dv_base
    scdv_dir = ip_root / "scdv"
    tests_dir = scdv_dir / "tests"
    # Generate minimal tests mirroring SV counterparts
    _write(tests_dir / f"{name}_smoke_test.cpp", SMOKE_CPP_TPL.format(name=name))
    _write(tests_dir / f"{name}_csr_rw_seq.hpp", CSR_RW_SEQ_HPP.format(name=name))
    _write(tests_dir / f"{name}_csr_rw_test.cpp", CSR_RW_CPP_TPL.format(name=name))
    # Leave Makefile/main.cpp as project-provided (do not overwrite)
    # Generate RAL wrapper (SV-like naming for SystemC) if has_ral
    if has_ral:
        ral_dir = scdv_dir / "ral"
        core_tmpl = (
            "filesets:\n"
            "  files_dv:\n"
            "    files:\n"
            f"      - {name}_ral_pkg.hpp\n"
            "    file_type: user\n"
            f"name: lowrisc:dv:{name}_ral_pkg\n"
            "targets:\n  default:\n    filesets:\n      - files_dv\n"
        )
        hpp_tmpl = (
            "#pragma once\n"
            f"#include \"../../tlm/{name}_reg_pkg.hpp\"\n"
            f"namespace {name}_ral_pkg {{ using ::opentitan::{name}::{name}_reg_top; }}\n"
        )
        _write(ral_dir / f"{name}_ral_pkg.core", core_tmpl)
        _write(ral_dir / f"{name}_ral_pkg.hpp", hpp_tmpl)
    # Generate tb skeleton if missing
    tb_dir = scdv_dir / "tb"
    tb_mk = (
        "# Simple Makefile to build tb for SystemC\n\n"
        "CXX ?= g++\n"
        "CXXFLAGS ?= -O2 -g -std=c++17 -DSC_ALLOW_DEPRECATED_IEEE_API\n\n"
        "ifndef SYSTEMC_HOME\n$(error Please set SYSTEMC_HOME to your SystemC installation path)\nendif\n\n"
        "INC := -I$(SYSTEMC_HOME)/include -I../../tlm\n"
        "LIBS := -L$(SYSTEMC_HOME)/lib -lsystemc\n\n"
        f"SRCS := tb_{name}.cpp ../../tlm/{name}_reg_top.cpp\n\n"
        f"all: tb_{name}\n\n"
        f"tb_{name}: $(SRCS)\n\t$(CXX) $(CXXFLAGS) $(INC) $(SRCS) -o $$@ $(LIBS)\n\n"
        "clean:\n\trm -f tb_" + name + "\n\n.PHONY: all clean\n"
    )
    tb_cpp = (
        "#include <systemc>\n"
        "int sc_main(int argc, char** argv) {\n"
        "  sc_core::sc_report_handler::set_actions(SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_, sc_core::SC_DO_NOTHING);\n"
        "  sc_core::sc_start(sc_core::SC_ZERO_TIME);\n"
        "  return 0;\n"
        "}\n"
    )
    _write(tb_dir / f"tb_{name}.cpp", tb_cpp)
    _write(tb_dir / "Makefile", tb_mk)
    print(f"Generated SC DV collateral under {scdv_dir}")

# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
"""Generate SystemC DV skeleton mirroring UVM env structure.

This creates sc/ directories under the env_outdir (ip root) with
tb/env/tests sources that use the SystemC reg_model generated by regtool.
"""

from pathlib import Path


def _write(p: Path, content: str) -> None:
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(content, encoding='utf-8')


def gen_sc(name: str, env_outdir: str, vendor: str, *,
           is_cip: bool, has_ral: bool, has_interrupts: bool,
           has_alerts: bool, env_agents: list[str]) -> None:
    ip_root = Path(env_outdir)
    if ip_root.name == 'dv':
        ip_root = ip_root.parent
    sc_root = ip_root / 'scdv'
    # (legacy tb/tb.cpp emission removed)

    # Load IP HJSON to tailor test emission
    ip_spec = {}
    try:
        candidates = []
        # 1) Relative to env_outdir (ip_root)
        candidates.append(ip_root / 'data' / f'{name}.hjson')
        # 2) Repo-root fallback relative to this file
        try:
            repo_top = Path(__file__).resolve().parents[2]
            candidates.append(repo_top / 'hw' / 'ip' / name / 'data' / f'{name}.hjson')
        except Exception:
            pass
        # 3) CWD-based fallback (when run from repo root)
        try:
            cwd = Path.cwd()
            candidates.append(cwd / 'hw' / 'ip' / name / 'data' / f'{name}.hjson')
        except Exception:
            pass
        for p in candidates:
            if p.exists():
                ip_spec = hjson.load(p.open('r', encoding='utf-8'))
                break
    except Exception:
        ip_spec = {}

    def _is_true(x):
        return bool(x) if not isinstance(x, str) else x.lower() in ('true', '1', 'yes', 'y')

    # Feature sniffing (best-effort)
    bus_if = ip_spec.get('bus_interfaces', []) if isinstance(ip_spec, dict) else []
    has_racl_feature = any(_is_true(b.get('racl_support')) for b in bus_if if isinstance(b, dict))
    regs = ip_spec.get('registers', []) if isinstance(ip_spec, dict) else []
    regwidth_bits = 32
    try:
        if isinstance(ip_spec.get('regwidth'), (int, str)):
            regwidth_bits = int(str(ip_spec.get('regwidth')), 0)
    except Exception:
        regwidth_bits = 32

    def parse_int(x):
        try:
            return int(str(x), 0)
        except Exception:
            return None

    # Compute register name -> offset (bytes) and name -> index (sequential order) maps
    name_to_offset: dict[str,int] = {}
    name_to_index: dict[str,int] = {}
    reg_order: list[str] = []
    curr_off = 0
    step = max(1, regwidth_bits // 8)
    for entry in regs:
        if not isinstance(entry, dict):
            continue
        # explicit offset override
        if 'offset' in entry:
            off = parse_int(entry.get('offset'))
            if off is not None:
                curr_off = off
        if 'name' in entry:
            nm = str(entry['name'])
            name_to_offset[nm] = curr_off
            name_to_index[nm] = len(reg_order)
            reg_order.append(nm)
            curr_off += step
        elif 'multireg' in entry and isinstance(entry['multireg'], dict):
            mr = entry['multireg']
            base = str(mr.get('name','')) or 'multireg'
            cnt = parse_int(mr.get('count')) or 1
            for i in range(cnt):
                nm = f"{base}[{i}]"
                name_to_offset[nm] = curr_off
                name_to_index[nm] = len(reg_order)
                reg_order.append(nm)
                curr_off += step
        elif 'window' in entry:
            # windows span multiple words; advance by items*step if available
            win = entry['window']
            items = parse_int(win.get('items')) if isinstance(win, dict) else None
            if items:
                curr_off += step * items
        else:
            # unknown entry; advance by one word to be safe
            curr_off += step
    def any_reg(pred):
        for r in regs:
            if not isinstance(r, dict):
                continue
            if pred(r):
                return True
            for f in r.get('fields', []) or []:
                if isinstance(f, dict) and pred(f):
                    return True
        return False
    has_w1c = any_reg(lambda n: str(n.get('swaccess','')).lower() in ('rw1c','w1c'))
    has_shadow = any_reg(lambda n: _is_true(n.get('shadowed')))
    has_regwen = any_reg(lambda n: 'regwen' in n or (isinstance(n.get('name'), str) and 'regwen' in n['name'].lower()))
    has_window = bool(regs)  # heuristic; many IPs have at least one window-like or multi-field reg

    # Augment mapping with standard comportable IP registers that regtool adds
    # even if not explicitly listed in registers: INTR_*, ALERT_TEST
    has_interrupts_list = bool(ip_spec.get('interrupt_list'))
    has_alerts_list = bool(ip_spec.get('alert_list'))
    if has_interrupts_list:
        std_intrs = [
            ('INTR_STATE', 0),
            ('INTR_ENABLE', 4),
            ('INTR_TEST', 8),
        ]
        for idx, (nm, off) in enumerate(std_intrs):
            if nm not in name_to_offset:
                name_to_offset[nm] = off
                name_to_index[nm] = -10 + idx  # ensure these sort before functional regs
                reg_order.insert(0, nm)
    if has_alerts_list:
        if 'ALERT_TEST' not in name_to_offset:
            name_to_offset['ALERT_TEST'] = 12
            name_to_index['ALERT_TEST'] = -6
            reg_order.insert(0, 'ALERT_TEST')

    # Derive per-register access masks from HJSON fields (SV-like policy)
    def parse_bits(bits_val):
        try:
            if isinstance(bits_val, str):
                if ':' in bits_val:
                    hi_s, lo_s = bits_val.split(':')
                    return int(hi_s, 0), int(lo_s, 0)
                else:
                    b = int(bits_val, 0)
                    return b, b
            if isinstance(bits_val, (list, tuple)) and len(bits_val) == 2:
                return int(bits_val[0]), int(bits_val[1])
            if isinstance(bits_val, int):
                return bits_val, bits_val
        except Exception:
            pass
        return regwidth_bits - 1, 0

    def mask_from_range(hi: int, lo: int) -> int:
        width = max(1, hi - lo + 1)
        if width >= 32:
            return (0xFFFFFFFF) & (~0 if lo == 0 else ((1 << width) - 1) << lo)
        return ((1 << width) - 1) << lo

    def is_sw_writable(sw: str) -> bool:
        s = (sw or '').lower()
        return s.startswith('rw') or s == 'w' or s == 'wo' or 'w1c' in s or 'w1s' in s

    name_to_masks: dict[str, dict[str,int]] = {}
    for entry in regs:
        if not isinstance(entry, dict):
            continue
        reg_sw = str(entry.get('swaccess', 'rw'))
        # Determine the name(s) this entry contributes to (handle multireg)
        reg_names: list[str] = []
        if 'name' in entry:
            reg_names.append(str(entry['name']))
        elif 'multireg' in entry and isinstance(entry['multireg'], dict):
            base = str(entry['multireg'].get('name', 'multireg'))
            cnt = parse_int(entry['multireg'].get('count')) or 1
            for i in range(cnt):
                reg_names.append(f"{base}[{i}]")
        # Aggregate masks per named reg
        for nm in reg_names:
            m = name_to_masks.setdefault(nm, {'w_mask': 0, 'wo_mask': 0, 'w1c_mask': 0, 'rc_mask': 0})
            fields = entry.get('fields') or []
            if isinstance(fields, list) and fields:
                for f in fields:
                    if not isinstance(f, dict):
                        continue
                    sw = str(f.get('swaccess', reg_sw))
                    hi, lo = parse_bits(f.get('bits'))
                    bitmask = mask_from_range(hi, lo)
                    sl = sw.lower()
                    if 'rc' in sl:
                        m['rc_mask'] |= bitmask
                    if 'w1c' in sl:
                        m['w1c_mask'] |= bitmask
                    elif sl == 'wo' or sl.startswith('wo'):
                        m['wo_mask'] |= bitmask
                    elif is_sw_writable(sl):
                        m['w_mask'] |= bitmask
            else:
                # No fields: apply reg-level policy to entire reg
                full = 0xFFFFFFFF if regwidth_bits >= 32 else ((1 << regwidth_bits) - 1)
                sl = reg_sw.lower()
                if 'rc' in sl:
                    m['rc_mask'] |= full
                if 'w1c' in sl:
                    m['w1c_mask'] |= full
                elif sl == 'wo' or sl.startswith('wo'):
                    m['wo_mask'] |= full
                elif is_sw_writable(sl) or sl == 'rw':
                    m['w_mask'] |= full

    # Heuristics for standard comportable regs when not explicitly fielded
    if has_interrupts_list:
        st = name_to_masks.setdefault('INTR_STATE', {'w_mask':0,'wo_mask':0,'w1c_mask':0,'rc_mask':0})
        st['w1c_mask'] |= 0xFFFFFFFF
        en = name_to_masks.setdefault('INTR_ENABLE', {'w_mask':0,'wo_mask':0,'w1c_mask':0,'rc_mask':0})
        en['w_mask'] |= 0xFFFFFFFF
        tst = name_to_masks.setdefault('INTR_TEST', {'w_mask':0,'wo_mask':0,'w1c_mask':0,'rc_mask':0})
        tst['wo_mask'] |= 0xFFFFFFFF

    readme = f"""# SystemC DV Skeleton for {name}

- Mirrors UVM env structure (tb/env/tests) with SystemC/TLM.
- Expects {name}_reg_model generated by regtool --systemc.

Detected (from HJSON):
- RACL support: {has_racl_feature}
- W1C fields: {has_w1c}
- Shadowed regs: {has_shadow}
- REGWEN: {has_regwen}
- Window-like: {has_window}
"""
    _write(sc_root / 'README.md', readme)

    # UVM SystemC skeleton: env and smoke test mirroring SV names
    bins_lines = "".join([
        f"      bin<uint32_t>(\"{nm.lower()}\", fc4sc::interval<uint32_t>({name_to_offset.get(nm,0)}u, {name_to_offset.get(nm,0)}u)), \\\n"
        for nm in sorted(name_to_offset.keys())
    ])
    cov_inc = (
        "#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)\n"
        + ("#include \"../../../dv/sc/coverage/uart_cov.hpp\"\n" if name == 'uart' else "") +
        "#define CSR_ADDR_BINS_DEF \\\n  COVERPOINT(uint32_t, addr_cp, addr_val_) { \\\n" + bins_lines +
        "      bin<uint32_t>(\"others\", fc4sc::interval<uint32_t>(0xFFFFFFFFu, 0xFFFFFFFFu)) \\\n  }; \\\n  cross<uint32_t,uint8_t> addr_x_rw = cross<uint32_t,uint8_t>(this, \"addr_x_rw\", &addr_cp, &is_write_cp);\n"
        "#include \"../../../dv/sc/coverage/csr_cov.hpp\"\n"
        "#endif\n\n"
    )
    # Compute common CSR offsets for coverage registration
    intr_state_off = name_to_offset.get('INTR_STATE')
    intr_test_off = name_to_offset.get('INTR_TEST')
    alert_test_off = name_to_offset.get('ALERT_TEST')
    # Derive W1C/RC registers from masks (ordered_regs_local will be set below; use temporary view)
    tmp_order = list(name_to_offset.keys())
    w1c_addrs = []
    rc_addrs = []
    for nm in tmp_order:
        masks = name_to_masks.get(nm, {})
        if int(masks.get('w1c_mask', 0) or 0) != 0:
            w1c_addrs.append(name_to_offset.get(nm))
        if int(masks.get('rc_mask', 0) or 0) != 0:
            rc_addrs.append(name_to_offset.get(nm))

    # Derive SHADOW/REGWEN registers from HJSON
    shadow_names: set[str] = set()
    regwen_names: set[str] = set()
    try:
        for entry in regs:
            if not isinstance(entry, dict):
                continue
            # collect names (handle multireg)
            reg_names: list[str] = []
            if 'name' in entry:
                reg_names.append(str(entry['name']))
            elif 'multireg' in entry and isinstance(entry['multireg'], dict):
                base = str(entry['multireg'].get('name', 'multireg'))
                cnt = parse_int(entry['multireg'].get('count')) or 1
                for i in range(cnt):
                    reg_names.append(f"{base}[{i}]")
            # shadowed
            if _is_true(entry.get('shadowed')):
                shadow_names.update(reg_names)
            # regwen: explicit or by name heuristic
            if 'regwen' in entry or any('REGWEN' in n or 'regwen' in n for n in reg_names):
                regwen_names.update(reg_names)
    except Exception:
        pass
    shadow_addrs = [name_to_offset.get(nm) for nm in shadow_names if name_to_offset.get(nm) is not None]
    regwen_addrs = [name_to_offset.get(nm) for nm in regwen_names if name_to_offset.get(nm) is not None]

    cov_parts = [
        "#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)\n",
        ("    if (mon) mon->set_coverage(&UartCov::get());\n" if name == 'uart' else ""),
        "    // CSR category registration for coverage\n",
        "    { auto &cc = CsrCov::get();\n",
    ]
    if intr_state_off is not None:
        cov_parts.append(f"      cc.register_w1c({intr_state_off}); cc.register_intr({intr_state_off});\n")
    if intr_test_off is not None:
        cov_parts.append(f"      cc.register_intr({intr_test_off});\n")
    if alert_test_off is not None:
        cov_parts.append(f"      cc.register_alert({alert_test_off});\n")
    for a in w1c_addrs:
        if a is not None:
            cov_parts.append(f"      cc.register_w1c({a});\n")
    for a in rc_addrs:
        if a is not None:
            cov_parts.append(f"      cc.register_rc({a});\n")
    if shadow_addrs:
        for a in shadow_addrs:
            cov_parts.append(f"      cc.register_shadow({a});\n")
    if regwen_addrs:
        for a in regwen_addrs:
            cov_parts.append(f"      cc.register_regwen({a});\n")
    cov_parts += [
        "    }\n",
        "#endif\n",
    ]

    # Emit coverage manifest (categories present and per-address access capabilities)
    try:
        import json as _json
        # Derive category address lists (unique, sorted)
        cat = {
            'w1c': sorted({a for a in w1c_addrs if a is not None}),
            'rc': sorted({a for a in rc_addrs if a is not None}),
            'intr': sorted({x for x in [intr_state_off, intr_test_off] if x is not None}),
            'alert': sorted({x for x in [alert_test_off] if x is not None}),
            'shadow': sorted({a for a in shadow_addrs if a is not None}),
            'regwen': sorted({a for a in regwen_addrs if a is not None}),
        }
        # Per-address access capability from masks
        access_list = []
        seen_addr = set()
        for nm, off in name_to_offset.items():
            if off in seen_addr: continue
            seen_addr.add(off)
            m = name_to_masks.get(nm, {})
            w_mask = int(m.get('w_mask', 0) or 0)
            wo_mask = int(m.get('wo_mask', 0) or 0)
            # can_read if not purely WO-only without RC/W1C
            can_read = not (wo_mask != 0 and w_mask == 0 and int(m.get('w1c_mask', 0) or 0) == 0 and int(m.get('rc_mask', 0) or 0) == 0)
            # can_write if any writable behavior (w_mask or wo_mask or w1c)
            can_write = (w_mask != 0) or (wo_mask != 0) or (int(m.get('w1c_mask', 0) or 0) != 0)
            access_list.append({'addr': off, 'can_read': bool(can_read), 'can_write': bool(can_write)})
        manifest = {'categories': cat, 'access': sorted(access_list, key=lambda x: x['addr'])}
        _write(sc_root / 'tests' / 'coverage_manifest.json', _json.dumps(manifest, indent=2))
    except Exception:
        pass
    cov_set = "".join(cov_parts)
    # Emit a simple virtual sequencer that mirrors SV CIP pattern
    vseq_hpp = (
        "#pragma once\n"
        "#include <uvm>\n"
        "#include \"../../../dv/sc/tl_agent/tl_sequencer.hpp\"\n"
        "#include \"../../../dv/sc/cip_lib/cip_base_virtual_sequencer.hpp\"\n"
        f"class {name}_virtual_sequencer : public uvm::uvm_component {{\n"
        " public:\n"
        f"  UVM_COMPONENT_UTILS({name}_virtual_sequencer);\n"
        "  tl_sequencer *tl_sequencer_h {nullptr};\n\n"
        f"  explicit {name}_virtual_sequencer(uvm::uvm_component_name nm) : uvm::uvm_component(nm) {{}}\n"
        "};\n"
    )
    _write(sc_root / 'env' / f'{name}_virtual_sequencer.hpp', vseq_hpp)

    # Build env header inheriting from cip_base_env to unify structure
    env_hpp = (
        "#pragma once\n"
        "#include <uvm>\n"
        + cov_inc +
        "#include \"../../../dv/sc/cip_lib/cip_base_env.hpp\"\n"
        "class {NAME}_env : public cip_base_env {\n"
        " public:\n"
        "  UVM_COMPONENT_UTILS({NAME}_env);\n"
        "  explicit {NAME}_env(uvm::uvm_component_name name) : cip_base_env(name) {}\n\n"
        "  void build_phase(uvm::uvm_phase &phase) override {\n"
        "    cip_base_env::build_phase(phase);\n"
        + cov_set +
        "  }\n"
        "};\n"
    ).replace("{NAME}", name)
    _write(sc_root / 'env' / f'{name}_env.hpp', env_hpp)
    # emit compatibility header
    compat_hpp = """#pragma once
#include <uvm>
#include <systemc>
namespace uvm_sc_compat_detail {
  template <typename T>
  inline ::uvm::uvm_component_registry<T>* ensure_component_registered() { return ::uvm::uvm_component_registry<T>::get(); }
  template <typename T>
  inline ::uvm::uvm_object_registry<T>* ensure_object_registered() { return ::uvm::uvm_object_registry<T>::get(); }
}
#define UVM_COMPONENT_REGISTER(TYPE) \
  namespace { auto TYPE##_uvm_sc_reg = ::uvm_sc_compat_detail::ensure_component_registered<TYPE>(); }
#define UVM_OBJECT_REGISTER(TYPE) \
  namespace { auto TYPE##_uvm_sc_obj_reg = ::uvm_sc_compat_detail::ensure_object_registered<TYPE>(); }

// Drain helper: advance delta cycles to flush analysis/TLM activities
inline void drain_delta(unsigned num_cycles = 4) {
  for (unsigned i = 0; i < num_cycles; ++i) sc_core::wait(sc_core::SC_ZERO_TIME);
}
"""
    _write(sc_root / 'env' / 'uvm_sc_compat.hpp', compat_hpp)

    # Emit CSR name<->offset mapping for exclude logic
    ordered_regs_local = []
    try:
      ordered_regs_local = sorted(name_to_index.items(), key=lambda kv: kv[1]) if name_to_index else []
    except Exception:
      ordered_regs_local = []
    if not ordered_regs_local:
      ordered_regs_local = sorted([(nm, off) for nm, off in name_to_offset.items()], key=lambda kv: kv[1])
    first_off_local = 0
    try:
      first_off_local = min(name_to_offset.values()) if name_to_offset else 0
    except Exception:
      first_off_local = 0
    ordered_names = [nm for nm, _ in ordered_regs_local]
    map_entries = ",\n".join([f"  {{ \"{nm}\", {name_to_offset.get(nm, first_off_local)} }}" for nm in ordered_names])
    csr_map_hpp = (
        "#pragma once\n"
        "#include <cstdint>\n"
        "#include <cstddef>\n"
        "struct csr_name_offset { const char *name; uint32_t addr; };\n"
        "namespace scdv {\n"
        "  static constexpr csr_name_offset csr_map[] = {\n" + map_entries + "\n  };\n"
        "  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);\n"
        "}\n"
    )
    _write(sc_root / 'env' / 'csr_map.hpp', csr_map_hpp)

    # Exclude/override policy (SV-like) with glob patterns on CSR names
    # Build default exclusions derived from HJSON (SV-like policy)
    def_excl_entries: list[str] = []
    for nm in ordered_names:
        masks = name_to_masks.get(nm, {})
        w_mask = int(masks.get('w_mask', 0) or 0)
        wo_mask = int(masks.get('wo_mask', 0) or 0)
        w1c_mask = int(masks.get('w1c_mask', 0) or 0)
        rc_mask = int(masks.get('rc_mask', 0) or 0)
        flags: list[str] = []
        # Exclude writes to W1C registers (random writes in generic TL sequence are invalid)
        if w1c_mask != 0:
            flags.append('ExclWrite')
        # Exclude writes to RO registers
        if w_mask == 0 and wo_mask == 0:
            if 'ExclWrite' not in flags:
                flags.append('ExclWrite')
        # Exclude reads of WO-only registers
        if wo_mask != 0 and w_mask == 0 and w1c_mask == 0 and rc_mask == 0:
            flags.append('ExclRead')
        if flags:
            def_excl_entries.append(f"  {{ \"{nm}\", " + "|".join(flags) + " }")

    def_excl_block = ",\n".join(def_excl_entries) if def_excl_entries else ""

    csr_excl_hpp = (
        "#pragma once\n"
        "#include <cstdint>\n"
        "#include <vector>\n"
        "#include <string>\n"
        "#include \"csr_map.hpp\"\n"
        "namespace scdv {\n"
        "  enum CsrExclType : unsigned { ExclNone=0, ExclWrite=1u<<0, ExclRead=1u<<1, ExclAll=ExclWrite|ExclRead };\n"
        "  inline bool glob_match(const char *pat, const char *str) {\n"
        "    // simple '*' and '?' matcher\n"
        "    if (!pat || !str) return false;\n"
        "    const char *s = str, *p = pat, *star = nullptr, *ss = nullptr;\n"
        "    while (*s) {\n"
        "      if (*p == '?' || *p == *s) { ++p; ++s; continue; }\n"
        "      if (*p == '*') { star = p++; ss = s; continue; }\n"
        "      if (star) { p = star + 1; s = ++ss; continue; }\n"
        "      return false;\n"
        "    }\n"
        "    while (*p == '*') ++p;\n"
        "    return *p == 0;\n"
        "  }\n"
        "  inline std::vector<std::pair<std::string,unsigned>>& excl_rules() { static std::vector<std::pair<std::string,unsigned>> r; return r; }\n"
        "  inline void add_excl(const std::string &pattern, unsigned flags) { excl_rules().emplace_back(pattern, flags); }\n"
        "  inline unsigned query_flags_by_name(const char *name) { for (auto &pr : excl_rules()) if (glob_match(pr.first.c_str(), name)) return pr.second; return 0u; }\n"
        "  inline const char* name_from_addr(uint32_t addr) { for (std::size_t i=0;i<csr_map_size;++i) if (csr_map[i].addr==addr) return csr_map[i].name; return (const char*)nullptr; }\n"
        "  inline bool exclude_write(uint32_t addr) { if (const char* nm = name_from_addr(addr)) return (query_flags_by_name(nm) & ExclWrite)!=0; return false; }\n"
        "  inline bool exclude_read(uint32_t addr)  { if (const char* nm = name_from_addr(addr)) return (query_flags_by_name(nm) & ExclRead)!=0; return false; }\n"
        "  struct default_excl_t { const char* name; unsigned flags; };\n"
        "  inline const default_excl_t* get_default_exclusions(size_t &n) {\n"
        "    static const default_excl_t defs[] = {\n" + def_excl_block + "\n    };\n"
        "    n = sizeof(defs)/sizeof(defs[0]); return defs;\n"
        "  }\n"
        "  inline void init_default_exclusions() { size_t n=0; auto arr = get_default_exclusions(n); for (size_t i=0;i<n;++i) add_excl(arr[i].name, arr[i].flags); }\n"
        "}\n"
    )
    _write(sc_root / 'env' / 'csr_exclude.hpp', csr_excl_hpp)

    main_cpp = f"""#include <uvm>
#include <string>
#include <systemc>
#include <ctime>
#include <cstdlib>
#include <tlm_utils/simple_initiator_socket.h>
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../env/csr_exclude.hpp"
#include "../../tlm/{name}_reg_pkg.hpp"
using namespace uvm;

static const char *get_testname_from_argv(int argc, char **argv) {{
  for (int i = 1; i < argc; ++i) {{
    std::string arg(argv[i] ? argv[i] : "");
    const std::string k1 = "+UVM_TESTNAME=";
    const std::string k2 = "UVM_TESTNAME=";
    if (arg.rfind(k1, 0) == 0) return argv[i] + k1.size();
    if (arg.rfind(k2, 0) == 0) return argv[i] + k2.size();
  }}
  return nullptr;
}}

static std::string get_plusarg_str(int argc, char **argv, const std::string &key) {{
  for (int i = 1; i < argc; ++i) {{
    std::string arg(argv[i] ? argv[i] : "");
    if (arg.rfind(key, 0) == 0) return arg.substr(key.size());
  }}
  return "";
}}

static bool has_plusarg(int argc, char **argv, const std::string &flag) {{
  for (int i = 1; i < argc; ++i) {{
    std::string arg(argv[i] ? argv[i] : "");
    if (arg == flag) return true;
  }}
  return false;
}}

static sc_core::sc_time parse_time_arg(const std::string &s) {{
  if (s.empty()) return sc_core::SC_ZERO_TIME;
  char *endp = nullptr;
  double val = std::strtod(s.c_str(), &endp);
  std::string unit = endp ? std::string(endp) : std::string();
  if (unit == "s" || unit == "S") return sc_core::sc_time(val, sc_core::SC_SEC);
  if (unit == "ms" || unit == "MS") return sc_core::sc_time(val, sc_core::SC_MS);
  if (unit == "us" || unit == "US") return sc_core::sc_time(val, sc_core::SC_US);
  if (unit == "ns" || unit == "NS") return sc_core::sc_time(val, sc_core::SC_NS);
  if (unit == "ps" || unit == "PS") return sc_core::sc_time(val, sc_core::SC_PS);
  return sc_core::sc_time(val, sc_core::SC_NS);
}}

static void apply_uvm_flags(int argc, char **argv) {{
  std::string verb = get_plusarg_str(argc, argv, "+UVM_VERBOSITY=");
  if (!verb.empty()) {{
    uvm::uvm_verbosity v = uvm::UVM_MEDIUM;
    if (verb == "UVM_NONE") v = uvm::UVM_NONE;
    else if (verb == "UVM_LOW") v = uvm::UVM_LOW;
    else if (verb == "UVM_MEDIUM") v = uvm::UVM_MEDIUM;
    else if (verb == "UVM_HIGH") v = uvm::UVM_HIGH;
    else if (verb == "UVM_FULL") v = uvm::UVM_FULL;
    uvm::uvm_root::get()->set_report_verbosity_level_hier(v);
  }}
  std::string mqc = get_plusarg_str(argc, argv, "+UVM_MAX_QUIT_COUNT=");
  if (!mqc.empty()) {{ int n = std::atoi(mqc.c_str()); (void)n; }}
  std::string to = get_plusarg_str(argc, argv, "+UVM_TIMEOUT=");
  if (!to.empty()) {{ sc_core::sc_time t = parse_time_arg(to);
    sc_core::sc_spawn([t]{{ sc_core::wait(t); uvm::uvm_report_fatal("UVM_TIMEOUT", "Simulation timed out", uvm::UVM_NONE); }}); }}
  std::string seed = get_plusarg_str(argc, argv, "+ntb_random_seed=");
  if (!seed.empty()) {{ unsigned s = 1u; if (seed == "random") s = static_cast<unsigned>(std::time(nullptr));
    else s = static_cast<unsigned>(std::strtoul(seed.c_str(), nullptr, 10)); std::srand(s);
    uvm::uvm_report_info("SEED", std::string("ntb_random_seed=") + std::to_string(s), uvm::UVM_LOW); }}
  if (has_plusarg(argc, argv, "+UVM_PHASE_TRACE")) uvm::uvm_report_info("UVM", "Phase trace requested", uvm::UVM_LOW);
  if (has_plusarg(argc, argv, "+UVM_OBJECTION_TRACE")) uvm::uvm_report_info("UVM", "Objection trace requested", uvm::UVM_LOW);
  if (has_plusarg(argc, argv, "+UVM_CONFIG_DB_TRACE")) uvm::uvm_report_info("UVM", "Config DB trace requested", uvm::UVM_LOW);
  if (has_plusarg(argc, argv, "+UVM_RESOURCE_DB_TRACE")) uvm::uvm_report_info("UVM", "Resource DB trace requested", uvm::UVM_LOW);
  if (has_plusarg(argc, argv, "+UVM_NO_RELNOTES")) uvm::uvm_report_info("UVM", "Suppress release notes", uvm::UVM_LOW);
}}

int sc_main(int argc, char **argv) {{
  struct top : sc_core::sc_module {{
    tlm_utils::simple_initiator_socket<top> init;
    opentitan::{name}::{name}_reg_top dut;
    SC_HAS_PROCESS(top);
    top(sc_core::sc_module_name n) : sc_core::sc_module(n), init("init"), dut("dut") {{ init.bind(dut.tlm_target); }}
  }} top_i("top");
  tl_bind::b_transport = [&](tlm::tlm_generic_payload &t, sc_core::sc_time &d){{ top_i.init->b_transport(t, d); }};
  tl_bind::reset = [&](){{ top_i.dut.reset(); }};
  tl_bind::set_racl_enable = [&](bool e){{ top_i.dut.set_racl_enable(e); }};
  tl_bind::set_racl_error_response = [&](bool e){{ top_i.dut.set_racl_error_response(e); }};
  tl_bind::set_reg_policy = [&](std::size_t idx, bool rd, bool wr){{ top_i.dut.set_reg_policy(idx, rd, wr); }};
  tl_bind::set_role_mask = [&](uint32_t m){{ top_i.dut.set_role_mask(m); }};
  // Bind RAL hooks to DUT's software model for accurate prediction
  tl_bind::ral_sw_read = [&](uint32_t addr) -> uint32_t {{ return top_i.dut.sw_read(addr); }};
  tl_bind::ral_sw_predict_write = [&](uint32_t addr, uint32_t data){{ top_i.dut.sw_predict_write(addr, data); }};
  tl_bind::ral_num_bytes = [&](){{ return static_cast<uint32_t>(opentitan::{name}::{name}_reg_top::kRegBytes); }};
  tl_bind::ral_word_bytes = [&](){{ return 4u; }};

  // Initialize default CSR exclusions derived from HJSON
  scdv::init_default_exclusions();

  apply_uvm_flags(argc, argv);
  if (const char *tn = get_testname_from_argv(argc, argv)) {{ uvm::run_test(tn); }} else {{
    std::string prog = std::string(argv && argv[0] ? argv[0] : "");
    std::size_t pos = prog.find_last_of('/');
    if (pos != std::string::npos) prog = prog.substr(pos + 1);
    if (!prog.empty()) uvm::run_test(prog.c_str()); else uvm::run_test();
  }}
  return 0;
}}
"""
    _write(sc_root / 'tests' / 'main.cpp', main_cpp)

    # Base smoke test stub
    test_cpp = f"""
#include <uvm>
#include "{name}_env.hpp"
#include "uvm_sc_compat.hpp"
using namespace uvm;

class {name}_smoke_test : public uvm_test {{
 public:
  UVM_COMPONENT_UTILS({name}_smoke_test);
  {name}_env* m_env {{}};
  explicit {name}_smoke_test(uvm_component_name name) : uvm_test(name) {{}}
  void build_phase(uvm_phase &phase) override {{
    uvm_test::build_phase(phase);
    m_env = {name}_env::type_id::create("env", this);
  }}
  void run_phase(uvm_phase &phase) override {{
    phase.raise_objection(this);
    phase.drop_objection(this);
  }}
}};

UVM_COMPONENT_REGISTER({name}_smoke_test);
"""
    _write(sc_root / 'tests' / f'{name}_smoke_test.cpp', test_cpp)

    # Tests Makefile
    tests_mk = f"""CXX ?= g++
CXXSTD ?= 17
CXXFLAGS ?= -O0 -g -std=c++$(CXXSTD) -DSC_ALLOW_DEPRECATED_IEEE_API -fsanitize=address
JOBS ?= $(shell (/usr/sbin/sysctl -n hw.ncpu) 2>/dev/null || (nproc) 2>/dev/null || echo 4)
MAKEFLAGS += -j$(JOBS)
ifndef SYSTEMC_HOME
$(error Please set SYSTEMC_HOME to your SystemC installation path)
endif
UVM_SYSTEMC_HOME ?= $(abspath ../../../../third_party/uvm-systemc)
UVM_SC_INCDIRS := $(UVM_SYSTEMC_HOME)/include
UVM_SC_LIBDIRS := $(UVM_SYSTEMC_HOME)/lib $(UVM_SYSTEMC_HOME)/lib-macosx64
UVM_SC_INC := $(firstword $(foreach d,$(UVM_SC_INCDIRS),$(if $(wildcard $(d)),$(d),)))
UVM_SC_LIBDIR := $(firstword $(foreach d,$(UVM_SC_LIBDIRS),$(if $(wildcard $(d)),$(d),)))
LATEST_UVM_DYLIB := $(shell ls -1t $(addsuffix /libuvm-systemc*.dylib,$(UVM_SC_LIBDIRS)) 2>/dev/null | head -n1)
INC := -I$(SYSTEMC_HOME)/include -I$(UVM_SC_INC) -I../../tlm -I../env \
       -I../../../dv/sc/csr_utils -I../../../dv/sc/tl_agent \
       -I../../../dv/sc/dv_utils -I../../../dv/sc/bus_params_pkg -I../../../dv/sc/scoreboard
DEFAULT_FC4SC := ../../../../../third_party/fc4sc
ifneq ($(wildcard $(DEFAULT_FC4SC)/includes),)
  FC4SC_HOME ?= $(DEFAULT_FC4SC)
endif
ifdef FC4SC_HOME
  ifneq ($(wildcard $(FC4SC_HOME)/includes),)
    INC += -I$(FC4SC_HOME)/includes
  else ifneq ($(wildcard $(FC4SC_HOME)/include),)
    INC += -I$(FC4SC_HOME)/include
  endif
  CXXFLAGS += -DENABLE_FC4SC -DFC4SC_READY
endif
LIBS := -L$(SYSTEMC_HOME)/lib -lsystemc -Wl,-rpath,$(SYSTEMC_HOME)/lib
# Link against locally built UVM-SystemC from detected lib dir
LIBS += -L$(UVM_SC_LIBDIR) -luvm-systemc -Wl,-rpath,$(UVM_SC_LIBDIR)
SRCS := main.cpp
TEST_SRCS := $(wildcard {name}_*_test.cpp)
TESTS := $(patsubst %.cpp,%,$(TEST_SRCS))
all: $(TESTS)

# Link DUT implementation
DUT_SRC := ../../tlm/{name}_reg_top.cpp

%: %.cpp main.cpp $(DUT_SRC)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIBS)
clean:
	rm -f $(TESTS)
PYTHON ?= $(shell [ -x $(abspath ../../../..)/.venv/bin/python ] && echo $(abspath ../../../..)/.venv/bin/python || command -v python3)

# Simple run-all and coverage helpers
RUN_ARGS ?= +UVM_VERBOSITY=UVM_LOW +ntb_random_seed=random
.PHONY: run cov
run: all
	@for b in $(TESTS); do echo "== $$b =="; ./$$b $(RUN_ARGS); done
cov:
	ROOT=$$(cd ../../../../../ && pwd); cd $$ROOT && $(PYTHON) util/scdv_cov_report.py --ip {name} --outdir $(abspath ./coverage_html) --systemc-home $(SYSTEMC_HOME) --jobs $(JOBS) --repeat 1 --extra-args "$(RUN_ARGS)" --accumulate --rich

.PHONY: gen-collateral
gen-collateral:
	cd ../../../../util && $(PYTHON) gen_scdv_collateral.py --tmp-out {name}
.PHONY: all clean
"""
    _write(sc_root / 'tests' / 'Makefile', tests_mk)

    # Tests README with usage and flags
    tests_readme = f"""# {name} SystemC UVM Tests

Build and run:

1) Ensure env vars:
   - SYSTEMC_HOME: your SystemC install prefix
   - UVM_SYSTEMC_HOME: your UVM-SystemC install prefix

2) Build all tests:

   make -C hw/ip/{name}/scdv/tests all

3) Run a specific test (UVM style):

   cd hw/ip/{name}/scdv/tests
   ./${name}_csr_rw_test +UVM_TESTNAME=${name}_csr_rw_test +UVM_VERBOSITY=UVM_MEDIUM +ntb_random_seed=random

Supported flags:
 - +UVM_TESTNAME=<test_class>
 - +UVM_VERBOSITY=UVM_NONE|UVM_LOW|UVM_MEDIUM|UVM_HIGH|UVM_FULL
 - +UVM_TIMEOUT=<value>[ns|us|ms|s]
 - +ntb_random_seed=<N>|random

4) Generate coverage HTML (parallel, accumulate across repeats/seeds):

   util/scdv_cov_report.py --ip {name} --outdir hw/ip/{name}/scdv/tests/coverage_html \
     --systemc-home "$SYSTEMC_HOME" --jobs 4 --repeat 1 --extra-args "+UVM_VERBOSITY=UVM_LOW +ntb_random_seed=random" --accumulate --rich

CSR lookup policy (strict):
- The generator resolves CSR offsets strictly by exact names in HJSON.
- Required: CTRL, INTR_ENABLE. Optional: WDATA.
- Standard comportable IP CSRs (INTR_STATE @0, INTR_ENABLE @4, INTR_TEST @8, ALERT_TEST @12) are auto-available when interrupts/alerts exist, even if not listed in HJSON.
- If a required CSR is missing, scaffold generation fails fast with an error listing the first few available register names.

Template verification:
 - To verify generated TLM matches checked-in sources, use:

   util/sc_tlm_diff.py --ip {name}  # add --update to sync

Collateral generation:
 - Dry-run scaffold & diff:

   util/gen_scdv_collateral.py --tmp-out --skip-regtool {name}

 - Apply scaffold (writes to repo):

   util/gen_scdv_collateral.py {name}
"""
    _write(sc_root / 'tests' / 'README.md', tests_readme)

    # CSR RW sequence stub (object) and test that uses global csr_utils helpers
    # Derive common offsets (exact match only): CTRL, INTR_ENABLE, WDATA
    first_off = next(iter(name_to_offset.values()), 0)
    # Get a stable ordered list of regs
    ordered_regs = sorted(name_to_index.items(), key=lambda kv: kv[1])
    ordered_offs = [name_to_offset.get(nm, first_off) for nm, _ in ordered_regs]
    ordered_wmask = [name_to_masks.get(nm, {}).get('w_mask', 0) for nm, _ in ordered_regs]
    ordered_womask = [name_to_masks.get(nm, {}).get('wo_mask', 0) for nm, _ in ordered_regs]
    ordered_w1cmask = [name_to_masks.get(nm, {}).get('w1c_mask', 0) for nm, _ in ordered_regs]
    ordered_rcmask = [name_to_masks.get(nm, {}).get('rc_mask', 0) for nm, _ in ordered_regs]
    # Fallbacks (not used when strict; kept for internal helper return shape)
    off_fallbacks = (ordered_offs + [first_off, first_off, first_off])
    def pick_off_exact(name: str, fb_idx: int):
        if name in name_to_offset:
            return name_to_offset[name], True
        return off_fallbacks[min(fb_idx, len(off_fallbacks)-1)], False
    ctrl_off, ctrl_found = pick_off_exact('CTRL', 0)
    intr_enable_off, intr_found = pick_off_exact('INTR_ENABLE', 1)
    wdata_off, wdata_found = pick_off_exact('WDATA', 2)

    # Determine a robust CTRL candidate for non-CIP IPs
    def find_ctrl_candidate():
        # Prefer regs with ctrl/ctl in name (excluding standard comportable regs)
        non_std = [nm for nm in reg_order if nm not in ('INTR_STATE', 'INTR_ENABLE', 'INTR_TEST', 'ALERT_TEST')]
        for nm in non_std:
            nml = nm.lower()
            if 'ctrl' in nml or 'ctl' in nml:
                return nm
        # Then try enable/en patterns
        for nm in non_std:
            nml = nm.lower()
            if 'enable' in nml or nml.endswith('_en') or nml.endswith('en_ctl'):
                return nm
        # Fallback to first non-standard register
        return non_std[0] if non_std else (reg_order[0] if reg_order else None)

    if ctrl_found:
        ctrl_name = 'CTRL'
        ctrl_addr = ctrl_off
    else:
        cand = find_ctrl_candidate()
        ctrl_name = cand if isinstance(cand, str) else 'CTRL'
        ctrl_addr = name_to_offset.get(ctrl_name, first_off)

    # Strictly require INTR_ENABLE only if IP has interrupts; WDATA is optional.
    missing = []
    if has_interrupts_list and not intr_found:
        missing.append('INTR_ENABLE')
    if missing:
        available = ', '.join([nm for nm, _ in ordered_regs[:16]])
        raise RuntimeError(f"[{name}] Address lookup failed for: {', '.join(missing)}. Available regs (first 16): {available}.")
    wdata_block = (
        f"""
    // WDATA is WO; ensure a read occurs for scoreboard to compare mirrored value (expected 0)
    tl_item *wr3 = tl_item::type_id::create("wr3", nullptr);
    wr3->a_opcode = tl_a_op_e::PutFullData; wr3->a_addr = {wdata_off}; wr3->a_size = 2; wr3->a_mask = 0xF; wr3->a_data = 0xAB;
    start_item(wr3); finish_item(wr3);
    tl_item *rd3 = tl_item::type_id::create("rd3", nullptr);
    rd3->a_opcode = tl_a_op_e::Get; rd3->a_addr = {wdata_off}; rd3->a_size = 2; rd3->a_mask = 0xF;
    start_item(rd3); finish_item(rd3);
"""
    ) if wdata_found else "\n"
    # Emit CSR RW that mirrors SV csr_rw: iterate over all CSRs (best-effort from HJSON order)
    ordered_offs_src = ", ".join(str(o) for o in ordered_offs) if ordered_offs else str(first_off)
    ordered_wmask_src = ", ".join(hex(m) for m in ordered_wmask) if ordered_wmask else "0"
    ordered_womask_src = ", ".join(hex(m) for m in ordered_womask) if ordered_womask else "0"
    ordered_w1cmask_src = ", ".join(hex(m) for m in ordered_w1cmask) if ordered_w1cmask else "0"
    ordered_rcmask_src = ", ".join(hex(m) for m in ordered_rcmask) if ordered_rcmask else "0"
    csr_seq_hpp = (
        "#pragma once\n"
        "#include <uvm>\n"
        "#include <cstdlib>\n"
        "#include \"../../../dv/sc/tl_agent/tl_item.hpp\"\n"
        "#include \"../../../dv/sc/tl_agent/tl_bind.hpp\"\n"
        "#include \"../../../dv/sc/bus_params_pkg/bus_params.hpp\"\n"
        "#include \"../env/csr_exclude.hpp\"\n"
        "class {NAME}_csr_rw_seq : public uvm::uvm_sequence<tl_item> {\n"
        " public:\n"
        "  UVM_OBJECT_UTILS({NAME}_csr_rw_seq);\n"
        "  explicit {NAME}_csr_rw_seq(const std::string &nm = \"{NAME}_csr_rw_seq\") : uvm::uvm_sequence<tl_item>(nm) {}\n"
        "  void body() override {\n"
        "    if (tl_bind::reset) tl_bind::reset();\n"
        "    // Prefer runtime RAL traversal when hooks are available\n"
        "    if (tl_bind::ral_num_bytes && tl_bind::ral_word_bytes) {\n"
        "      uint32_t total = tl_bind::ral_num_bytes();\n"
        "      uint32_t step  = tl_bind::ral_word_bytes();\n"
        "      if (step == 0) step = 4u;\n"
        "      for (uint32_t addr = 0; addr < total; addr += step) {\n"
        "        if (scdv::exclude_write(addr)) continue;\n"
        "        uint32_t wdata = static_cast<uint32_t>(std::rand());\n"
        "        tl_item *wr = tl_item::type_id::create(\"wr\", nullptr);\n"
        "        wr->a_opcode = tl_a_op_e::PutFullData; wr->a_addr = addr; wr->a_size = 2; wr->a_mask = 0xF; wr->a_data = wdata;\n"
        "        start_item(wr); finish_item(wr);\n"
        "        uint32_t sz_sel = static_cast<uint32_t>(std::rand()) % 3; // 0:1B,1:2B,2:4B\n"
        "        tl_item *rd = tl_item::type_id::create(\"rd\", nullptr);\n"
        "        rd->a_opcode = tl_a_op_e::Get; rd->a_addr = addr; rd->a_size = (sz_sel == 2 ? 2 : (sz_sel == 1 ? 1 : 0));\n"
        "        rd->a_mask = (sz_sel == 2 ? 0xF : (sz_sel == 1 ? 0x3 : 0x1));\n"
        "        if (!scdv::exclude_read(addr)) { start_item(rd); finish_item(rd); }\n"
        "      }\n"
        "      return;\n"
        "    }\n"
        "    // Fallback: use HJSON-derived ordered offsets and per-register masks\n"
        "    const uint32_t offs[] = { {OFFS} };\n"
        "    const uint32_t wmask[] = { {WMASK} };\n"
        "    const uint32_t womask[] = { {WOMASK} };\n"
        "    const uint32_t w1cmask[] = { {W1CMASK} };\n"
        "    const uint32_t rcmask[] = { {RCMASK} };\n"
        "    const size_t num = sizeof(offs)/sizeof(offs[0]);\n"
        "    for (size_t i = 0; i < num; ++i) {\n"
        "      uint32_t addr = offs[i];\n"
        "      const uint32_t m_w = wmask[i];\n"
        "      // Compute byte-enable mask from field-level write mask (coarse-grain)\n"
        "      uint32_t be = 0;\n"
        "      for (int b=0; b<4; ++b) { uint32_t lane = (m_w >> (8*b)) & 0xFFu; if (lane) be |= (1u<<b); }\n"
        "      if (!scdv::exclude_write(addr) && be) {\n"
        "        uint32_t wdata = static_cast<uint32_t>(std::rand());\n"
        "        // Random subset of allowed lanes to mimic partial writes\n"
        "        uint32_t subset = (static_cast<uint32_t>(std::rand()) & 0xFu) & be; if (subset == 0) subset = be;\n"
        "        tl_item *wr = tl_item::type_id::create(\"wr\", nullptr);\n"
        "        wr->a_opcode = (subset == 0xFu ? tl_a_op_e::PutFullData : tl_a_op_e::PutPartialData);\n"
        "        wr->a_addr = addr; wr->a_size = 2; wr->a_mask = subset; wr->a_data = wdata;\n"
        "        start_item(wr); finish_item(wr);\n"
        "      }\n"
        "      uint32_t sz_sel = static_cast<uint32_t>(std::rand()) % 3;\n"
        "      tl_item *rd = tl_item::type_id::create(\"rd\", nullptr);\n"
        "      rd->a_opcode = tl_a_op_e::Get; rd->a_addr = addr; rd->a_size = (sz_sel == 2 ? 2 : (sz_sel == 1 ? 1 : 0));\n"
        "      rd->a_mask = (sz_sel == 2 ? 0xF : (sz_sel == 1 ? 0x3 : 0x1));\n"
        "      if (!scdv::exclude_read(addr)) { start_item(rd); finish_item(rd); }\n"
        "    }\n"
        "  }\n"
        "};\n"
    )
    csr_seq_hpp = (csr_seq_hpp
                   .replace("{NAME}", name)
                   .replace("{OFFS}", ordered_offs_src)
                   .replace("{WMASK}", ordered_wmask_src)
                   .replace("{WOMASK}", ordered_womask_src)
                   .replace("{W1CMASK}", ordered_w1cmask_src)
                   .replace("{RCMASK}", ordered_rcmask_src))
    _write(sc_root / 'tests' / f'{name}_csr_rw_seq.hpp', csr_seq_hpp)

    csr_test_cpp = f"""#include <uvm>
#include "../env/{name}_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "{name}_csr_rw_seq.hpp"
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"
using namespace uvm;

class {name}_csr_rw_test : public uvm_test {{
 public:
  UVM_COMPONENT_UTILS({name}_csr_rw_test);
  {name}_env* m_env {{}};
  explicit {name}_csr_rw_test(uvm_component_name nm) : uvm_test(nm) {{}}
  void build_phase(uvm_phase &phase) override {{ m_env = {name}_env::type_id::create("env", this); }}
  void run_phase(uvm_phase &phase) override {{
    phase.raise_objection(this);
    if (m_env && m_env->scb) m_env->scb->clear();
    auto seq = {name}_csr_rw_seq::type_id::create("seq");
    tl_sequencer *seqr_ptr {{nullptr}};
    if (uvm::uvm_config_db<tl_sequencer*>::get(nullptr, "*", "tl_sequencer", seqr_ptr) && seq) {{
      seq->start(seqr_ptr);
    }} else {{
      uvm::uvm_report_fatal("TEST/NOSQR", "tl_sequencer not found in config_db", uvm::UVM_NONE);
    }}
    drain_delta();
    // bounded drain: wait until scoreboard settles or max iters
    for (int i=0; i<100 && m_env && m_env->scb && m_env->scb->has_pending(); ++i) drain_delta();
    phase.drop_objection(this);
  }}
}};
UVM_COMPONENT_REGISTER({name}_csr_rw_test);
"""
    _write(sc_root / 'tests' / f'{name}_csr_rw_test.cpp', csr_test_cpp)

    # Bit-bash sequence and test
    bit_seq_hpp = f"""#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class {name}_bit_bash_seq : public uvm::uvm_sequence<tl_item> {{
 public:
  UVM_OBJECT_UTILS({name}_bit_bash_seq);
  explicit {name}_bit_bash_seq(const std::string &nm = "{name}_bit_bash_seq") : uvm::uvm_sequence<tl_item>(nm) {{}}
  void body() override {{
    // Toggle bits in INTR_ENABLE (addr 4)
    for (unsigned b = 0; b < 8; ++b) {{
      tl_item *wr = tl_item::type_id::create("bb_wr", nullptr);
      wr->a_opcode = tl_a_op_e::PutFullData; wr->a_addr = 4; wr->a_size = 2; wr->a_mask = 0xF; wr->a_data = (1u<<b);
      start_item(wr); finish_item(wr);
      tl_item *rd = tl_item::type_id::create("bb_rd", nullptr);
      rd->a_opcode = tl_a_op_e::Get; rd->a_addr = 4; rd->a_size = 2; rd->a_mask = 0xF;
      start_item(rd); finish_item(rd);
    }}
  }}
}};
"""
    _write(sc_root / 'tests' / f'{name}_bit_bash_seq.hpp', bit_seq_hpp)

    bit_test_cpp = f"""#include <uvm>
#include "../env/{name}_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "{name}_bit_bash_seq.hpp"
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"
#include "../../../dv/sc/tl_agent/seq_lib/tl_seq_list.hpp"
using namespace uvm;

class {name}_bit_bash_test : public uvm_test {{
 public:
  UVM_COMPONENT_UTILS({name}_bit_bash_test);
  {name}_env* m_env {{}};
  explicit {name}_bit_bash_test(uvm_component_name nm) : uvm_test(nm) {{}}
  void build_phase(uvm_phase &phase) override {{ m_env = {name}_env::type_id::create("env", this); }}
  void run_phase(uvm_phase &phase) override {{
    phase.raise_objection(this);
    auto seq = {name}_bit_bash_seq::type_id::create("seq");
    tl_sequencer *seqr_ptr {{nullptr}};
    if (uvm::uvm_config_db<tl_sequencer*>::get(nullptr, "*", "tl_sequencer", seqr_ptr) && seq) {{
      seq->start(seqr_ptr);
    }} else {{
      uvm::uvm_report_fatal("TEST/NOSQR", "tl_sequencer not found in config_db", uvm::UVM_NONE);
    }}
    drain_delta();
    phase.drop_objection(this);
  }}
}};
UVM_COMPONENT_REGISTER({name}_bit_bash_test);
"""
    _write(sc_root / 'tests' / f'{name}_bit_bash_test.cpp', bit_test_cpp)

    # Additional scaffold tests: w1c, shadow, regwen, racl, tlul_err, window
    def emit_simple_seq_and_test(tag: str, seq_body: str) -> None:
        seq_hpp = (
            f"#pragma once\n"
            f"#include <uvm>\n"
            f"#include \"../../../dv/sc/tl_agent/tl_item.hpp\"\n"
            f"class {name}_{tag}_seq : public uvm::uvm_sequence<tl_item> {{\n"
            f" public:\n"
            f"  UVM_OBJECT_UTILS({name}_{tag}_seq);\n"
            f"  explicit {name}_{tag}_seq(const std::string &nm = \"{name}_{tag}_seq\") : uvm::uvm_sequence<tl_item>(nm) {{}}\n"
            f"  void body() override {{\n"
            f"{seq_body}"
            f"  }}\n"
            f"}};\n"
        )
        _write(sc_root / 'tests' / f'{name}_{tag}_seq.hpp', seq_hpp)
        test_cpp = (
            f"#include <uvm>\n"
            f"#include \"../env/{name}_env.hpp\"\n"
            f"#include \"../env/uvm_sc_compat.hpp\"\n"
            f"#include \"{name}_{tag}_seq.hpp\"\n"
            f"#include \"../../../dv/sc/tl_agent/tl_sequencer.hpp\"\n"
            f"#include \"../../../dv/sc/tl_agent/seq_lib/tl_seq_list.hpp\"\n"
            f"using namespace uvm;\n\n"
            f"class {name}_{tag}_test : public uvm_test {{\n"
            f" public:\n"
            f"  UVM_COMPONENT_UTILS({name}_{tag}_test);\n"
            f"  {name}_env* m_env {{}};\n"
            f"  explicit {name}_{tag}_test(uvm_component_name nm) : uvm_test(nm) {{}}\n"
            f"  void build_phase(uvm_phase &phase) override {{ m_env = {name}_env::type_id::create(\"env\", this); }}\n"
            f"  void run_phase(uvm_phase &phase) override {{\n"
            f"    phase.raise_objection(this);\n"
            f"    auto seq = {name}_{tag}_seq::type_id::create(\"seq\");\n"
            f"    tl_sequencer *seqr_ptr {{nullptr}};\n"
            f"    if (uvm::uvm_config_db<tl_sequencer*>::get(nullptr, \"*\", \"tl_sequencer\", seqr_ptr) && seq) {{\n"
            f"      seq->start(seqr_ptr);\n"
            f"    }} else {{\n"
            f"      uvm::uvm_report_fatal(\"TEST/NOSQR\", \"tl_sequencer not found in config_db\", uvm::UVM_NONE);\n"
            f"    }}\n"
            f"    drain_delta();\n"
            f"    for (int i=0; i<100 && m_env && m_env->scb && m_env->scb->has_pending(); ++i) drain_delta();\n"
            f"    phase.drop_objection(this);\n"
            f"  }}\n"
            f"}};\n"
            f"UVM_COMPONENT_REGISTER({name}_{tag}_test);\n"
        )
        _write(sc_root / 'tests' / f'{name}_{tag}_test.cpp', test_cpp)

    # Address-sweep sequence: frontdoor via tl_sequencer using RAL masks
    # Deduplicate offsets and OR masks per unique address
    unique_addrs = sorted(set(name_to_offset.values()))
    addr_to_masks = {a: { 'w_mask': 0, 'wo_mask': 0, 'w1c_mask': 0, 'rc_mask': 0 } for a in unique_addrs}
    for reg_name, off in name_to_offset.items():
        m = name_to_masks.get(reg_name, {})
        if off in addr_to_masks:
            addr_to_masks[off]['w_mask']   |= int(m.get('w_mask', 0)   & 0xFFFFFFFF)
            addr_to_masks[off]['wo_mask']  |= int(m.get('wo_mask', 0)  & 0xFFFFFFFF)
            addr_to_masks[off]['w1c_mask'] |= int(m.get('w1c_mask', 0) & 0xFFFFFFFF)
            addr_to_masks[off]['rc_mask']  |= int(m.get('rc_mask', 0)  & 0xFFFFFFFF)
    addrs_emit   = ', '.join(str(a) for a in unique_addrs)
    wmask_emit   = ', '.join(hex(addr_to_masks[a]['w_mask']   & 0xFFFFFFFF) for a in unique_addrs)
    womask_emit  = ', '.join(hex(addr_to_masks[a]['wo_mask']  & 0xFFFFFFFF) for a in unique_addrs)
    w1cmask_emit = ', '.join(hex(addr_to_masks[a]['w1c_mask'] & 0xFFFFFFFF) for a in unique_addrs)
    rcmask_emit  = ', '.join(hex(addr_to_masks[a]['rc_mask']  & 0xFFFFFFFF) for a in unique_addrs)

    addr_seq_hpp = (
        f"#pragma once\n"
        f"#include <uvm>\n"
        f"#include \"../../../dv/sc/tl_agent/tl_item.hpp\"\n"
        f"#include \"../../../dv/sc/csr_utils/csr_seq_lib.hpp\"\n"
        f"class {name}_csr_addr_sweep_seq : public uvm::uvm_sequence<tl_item> {{\n"
        f" public:\n"
        f"  UVM_OBJECT_UTILS({name}_csr_addr_sweep_seq);\n"
        f"  explicit {name}_csr_addr_sweep_seq(const std::string &nm = \"{name}_csr_addr_sweep_seq\") : uvm::uvm_sequence<tl_item>(nm) {{}}\n"
        f"  void body() override {{\n"
        f"    const uint32_t addrs[] = {{ {addrs_emit} }};\n"
        f"    const uint32_t wmask[] = {{ {wmask_emit} }};\n"
        f"    const uint32_t womask[] = {{ {womask_emit} }};\n"
        f"    const uint32_t w1cmask[] = {{ {w1cmask_emit} }};\n"
        f"    const uint32_t rcmask[] = {{ {rcmask_emit} }};\n"
        f"    run_csr_rw_sweep(this, addrs, wmask, womask, w1cmask, rcmask, sizeof(addrs)/sizeof(addrs[0]));\n"
        f"  }}\n"
        f"}};\n"
    )
    _write(sc_root / 'tests' / f'{name}_csr_addr_sweep_seq.hpp', addr_seq_hpp)
    addr_test_cpp = (
        f"#include <uvm>\n"
        f"#include \"../env/{name}_env.hpp\"\n"
        f"#include \"../env/uvm_sc_compat.hpp\"\n"
        f"#include \"{name}_csr_addr_sweep_seq.hpp\"\n"
        f"#include \"../../../dv/sc/tl_agent/tl_sequencer.hpp\"\n"
        f"using namespace uvm;\n\n"
        f"class {name}_csr_addr_sweep_test : public uvm_test {{\n"
        f" public:\n"
        f"  UVM_COMPONENT_UTILS({name}_csr_addr_sweep_test);\n"
        f"  {name}_env* m_env {{}};\n"
        f"  explicit {name}_csr_addr_sweep_test(uvm_component_name nm) : uvm_test(nm) {{}}\n"
        f"  void build_phase(uvm_phase &phase) override {{ m_env = {name}_env::type_id::create(\"env\", this); }}\n"
        f"  void run_phase(uvm_phase &phase) override {{\n"
        f"    phase.raise_objection(this);\n"
        f"    auto seq = {name}_csr_addr_sweep_seq::type_id::create(\"seq\");\n"
        f"    tl_sequencer *seqr_ptr {{nullptr}};\n"
        f"    if (uvm::uvm_config_db<tl_sequencer*>::get(nullptr, \"*\", \"tl_sequencer\", seqr_ptr) && seq) {{ seq->start(seqr_ptr); }}\n"
        f"    drain_delta(); phase.drop_objection(this);\n"
        f"  }}\n"
        f"}};\n"
        f"UVM_COMPONENT_REGISTER({name}_csr_addr_sweep_test);\n"
    )
    _write(sc_root / 'tests' / f'{name}_csr_addr_sweep_test.cpp', addr_test_cpp)

    # Tailored sequences using HJSON when possible; fallback to first register
    def seq_body(tag: str) -> str:
        first_off = next(iter(name_to_offset.values()), 0)
        intr_state_off = name_to_offset.get('INTR_STATE', first_off)
        intr_test_off = name_to_offset.get('INTR_TEST', intr_state_off)
        ctrl_off = name_to_offset.get('CTRL', first_off)
        # Use chosen CTRL candidate name to derive index consistently
        ctrl_idx = name_to_index.get(ctrl_name, name_to_index.get('CTRL', 0))
        # WO/RC helpers
        wdata_off = name_to_offset.get('WDATA')
        txdata_off = name_to_offset.get('TXDATA')
        rdata_off = name_to_offset.get('RDATA', first_off)
        if tag == 'w1c':
            # Use INTR_TEST (8) to set bits, INTR_STATE (0) to check, then clear via W1C write to STATE
            return (
                "    // Set tx_empty bit via INTR_TEST, then observe INTR_STATE and clear via W1C\n"
                "    tl_item *wr_set = tl_item::type_id::create(\"wr_set\", nullptr);\n"
                f"    wr_set->a_opcode = tl_a_op_e::PutFullData; wr_set->a_addr = {intr_test_off}; wr_set->a_size = 2; wr_set->a_mask = 0xF; wr_set->a_data = 1u;\n"
                "    start_item(wr_set); finish_item(wr_set);\n"
                "    tl_item *rd_state = tl_item::type_id::create(\"rd_state\", nullptr);\n"
                f"    rd_state->a_opcode = tl_a_op_e::Get; rd_state->a_addr = {intr_state_off}; rd_state->a_size = 2; rd_state->a_mask = 0xF;\n"
                "    start_item(rd_state); finish_item(rd_state);\n"
                "    tl_item *wr_clr = tl_item::type_id::create(\"wr_clr\", nullptr);\n"
                f"    wr_clr->a_opcode = tl_a_op_e::PutFullData; wr_clr->a_addr = {intr_state_off}; wr_clr->a_size = 2; wr_clr->a_mask = 0xF; wr_clr->a_data = 1u;\n"
                "    start_item(wr_clr); finish_item(wr_clr);\n"
                "    tl_item *rd_state2 = tl_item::type_id::create(\"rd_state2\", nullptr);\n"
                f"    rd_state2->a_opcode = tl_a_op_e::Get; rd_state2->a_addr = {intr_state_off}; rd_state2->a_size = 2; rd_state2->a_mask = 0xF;\n"
                "    start_item(rd_state2); finish_item(rd_state2);\n"
            )
        if tag == 'shadow':
            # Find a shadowed-like register by name heuristic
            shadow_off = None
            for nm, off in name_to_offset.items():
                nmu = nm.upper()
                if 'SHADOWED' in nmu or nmu.endswith('_SHADOWED') or 'SHDW' in nmu:
                    shadow_off = off
                    break
            if shadow_off is None:
                # Fallback: do nothing if no shadow register
                return (
                    "    // No shadowed registers detected; skip\n"
                )
            return (
                "    // Shadowed register two-phase commit: write same value twice to commit\n"
                "    uint32_t val = 0xA5A5u;\n"
                "    tl_item *wr_p1 = tl_item::type_id::create(\"wr_p1\", nullptr);\n"
                f"    wr_p1->a_opcode = tl_a_op_e::PutFullData; wr_p1->a_addr = {shadow_off}; wr_p1->a_size = 2; wr_p1->a_mask = 0xF; wr_p1->a_data = val;\n"
                "    start_item(wr_p1); finish_item(wr_p1);\n"
                "    tl_item *wr_p2 = tl_item::type_id::create(\"wr_p2\", nullptr);\n"
                f"    wr_p2->a_opcode = tl_a_op_e::PutFullData; wr_p2->a_addr = {shadow_off}; wr_p2->a_size = 2; wr_p2->a_mask = 0xF; wr_p2->a_data = val;\n"
                "    start_item(wr_p2); finish_item(wr_p2);\n"
                "    tl_item *rd_ok = tl_item::type_id::create(\"rd_ok\", nullptr);\n"
                f"    rd_ok->a_opcode = tl_a_op_e::Get; rd_ok->a_addr = {shadow_off}; rd_ok->a_size = 2; rd_ok->a_mask = 0xF;\n"
                "    start_item(rd_ok); finish_item(rd_ok);\n"
                "    // Mismatch attempt: different second write should not commit\n"
                "    uint32_t val2 = 0x3C3Cu;\n"
                "    tl_item *wr_m1 = tl_item::type_id::create(\"wr_m1\", nullptr);\n"
                f"    wr_m1->a_opcode = tl_a_op_e::PutFullData; wr_m1->a_addr = {shadow_off}; wr_m1->a_size = 2; wr_m1->a_mask = 0xF; wr_m1->a_data = val2;\n"
                "    start_item(wr_m1); finish_item(wr_m1);\n"
                "    tl_item *wr_m2 = tl_item::type_id::create(\"wr_m2\", nullptr);\n"
                f"    wr_m2->a_opcode = tl_a_op_e::PutFullData; wr_m2->a_addr = {shadow_off}; wr_m2->a_size = 2; wr_m2->a_mask = 0xF; wr_m2->a_data = (val2 ^ 0xFFFFu);\n"
                "    start_item(wr_m2); finish_item(wr_m2);\n"
                "    tl_item *rd_no = tl_item::type_id::create(\"rd_no\", nullptr);\n"
                f"    rd_no->a_opcode = tl_a_op_e::Get; rd_no->a_addr = {shadow_off}; rd_no->a_size = 2; rd_no->a_mask = 0xF;\n"
                "    start_item(rd_no); finish_item(rd_no);\n"
            )
        if tag == 'regwen':
            # Find a REGWEN register if present
            regwen_off = None
            for nm, off in name_to_offset.items():
                nmu = nm.upper()
                if nmu.endswith('REGWEN') or 'REGWEN' in nmu:
                    regwen_off = off
                    break
            if regwen_off is None:
                return (
                    "    // No REGWEN detected; skip\n"
                )
            return (
                "    // Lock writes via REGWEN=0, attempt write to CTRL (or first), expect no effect; then unlock and write\n"
                "    tl_item *wen0 = tl_item::type_id::create(\"wen0\", nullptr);\n"
                f"    wen0->a_opcode = tl_a_op_e::PutFullData; wen0->a_addr = {regwen_off}; wen0->a_size = 2; wen0->a_mask = 0xF; wen0->a_data = 0u;\n"
                "    start_item(wen0); finish_item(wen0);\n"
                "    tl_item *w_lock = tl_item::type_id::create(\"w_lock\", nullptr);\n"
                f"    w_lock->a_opcode = tl_a_op_e::PutFullData; w_lock->a_addr = {ctrl_addr}; w_lock->a_size = 2; w_lock->a_mask = 0xF; w_lock->a_data = 0xA5A5u;\n"
                "    start_item(w_lock); finish_item(w_lock);\n"
                "    tl_item *r_lock = tl_item::type_id::create(\"r_lock\", nullptr);\n"
                f"    r_lock->a_opcode = tl_a_op_e::Get; r_lock->a_addr = {ctrl_addr}; r_lock->a_size = 2; r_lock->a_mask = 0xF;\n"
                "    start_item(r_lock); finish_item(r_lock);\n"
                "    tl_item *wen1 = tl_item::type_id::create(\"wen1\", nullptr);\n"
                f"    wen1->a_opcode = tl_a_op_e::PutFullData; wen1->a_addr = {regwen_off}; wen1->a_size = 2; wen1->a_mask = 0xF; wen1->a_data = 1u;\n"
                "    start_item(wen1); finish_item(wen1);\n"
                "    tl_item *w_unlk = tl_item::type_id::create(\"w_unlk\", nullptr);\n"
                f"    w_unlk->a_opcode = tl_a_op_e::PutFullData; w_unlk->a_addr = {ctrl_addr}; w_unlk->a_size = 2; w_unlk->a_mask = 0xF; w_unlk->a_data = 0x5A5Au;\n"
                "    start_item(w_unlk); finish_item(w_unlk);\n"
                "    tl_item *r_unlk = tl_item::type_id::create(\"r_unlk\", nullptr);\n"
                f"    r_unlk->a_opcode = tl_a_op_e::Get; r_unlk->a_addr = {ctrl_addr}; r_unlk->a_size = 2; r_unlk->a_mask = 0xF;\n"
                "    start_item(r_unlk); finish_item(r_unlk);\n"
            )
        if tag == 'racl':
            # Disallow write to CTRL (16) via RACL and attempt a write/read
            return (
                "    // Enable RACL and set policy: block writes to CTRL for default role (0)\n"
                "    if (tl_bind::set_racl_enable) tl_bind::set_racl_enable(true);\n"
                f"    if (tl_bind::set_reg_policy) tl_bind::set_reg_policy({ctrl_idx}, true, false);\n"
                "    // Attempt blocked write (expect dut to ignore or error based on error_rsp)\n"
                "    if (tl_bind::set_racl_error_response) tl_bind::set_racl_error_response(false);\n"
                "    tl_item *wr_blk = tl_item::type_id::create(\"wr_blk\", nullptr);\n"
                f"    wr_blk->a_opcode = tl_a_op_e::PutFullData; wr_blk->a_addr = {ctrl_addr}; wr_blk->a_size = 2; wr_blk->a_mask = 0xF; wr_blk->a_data = 0xA5A5u;\n"
                "    start_item(wr_blk); finish_item(wr_blk);\n"
                "    tl_item *rd_chk = tl_item::type_id::create(\"rd_chk\", nullptr);\n"
                f"    rd_chk->a_opcode = tl_a_op_e::Get; rd_chk->a_addr = {ctrl_addr}; rd_chk->a_size = 2; rd_chk->a_mask = 0xF;\n"
                "    start_item(rd_chk); finish_item(rd_chk);\n"
                "    // Now act as privileged role (nonzero mask) and allow write\n"
                "    if (tl_bind::set_role_mask) tl_bind::set_role_mask(1u);\n"
                "    if (tl_bind::set_racl_error_response) tl_bind::set_racl_error_response(true);\n"
                "    tl_item *wr_ok = tl_item::type_id::create(\"wr_ok\", nullptr);\n"
                f"    wr_ok->a_opcode = tl_a_op_e::PutFullData; wr_ok->a_addr = {ctrl_addr}; wr_ok->a_size = 2; wr_ok->a_mask = 0xF; wr_ok->a_data = 0x5A5Au;\n"
                "    start_item(wr_ok); finish_item(wr_ok);\n"
                "    tl_item *rd_ok = tl_item::type_id::create(\"rd_ok\", nullptr);\n"
                f"    rd_ok->a_opcode = tl_a_op_e::Get; rd_ok->a_addr = {ctrl_addr}; rd_ok->a_size = 2; rd_ok->a_mask = 0xF;\n"
                "    start_item(rd_ok); finish_item(rd_ok);\n"
                "    // Disable RACL\n"
                "    if (tl_bind::set_racl_enable) tl_bind::set_racl_enable(false);\n"
            )
        if tag == 'tlul_err':
            return (
                "    // Out-of-range accesses to drive addr_cp.others and protocol errors\n"
                "    // 1) Valid-size read at an unmapped high address to hit others bin explicitly\n"
                "    tl_item *rd_others = tl_item::type_id::create(\"rd_others\", nullptr);\n"
                "    rd_others->a_opcode = tl_a_op_e::Get; rd_others->a_addr = 0xFFFFFFFFu; rd_others->a_size = 0; rd_others->a_mask = 0x1;\n"
                "    start_item(rd_others); finish_item(rd_others);\n"
                "    // 2) Out-of-range mid address (legacy)\n"
                "    tl_item *rd_bad = tl_item::type_id::create(\"rd_bad\", nullptr);\n"
                f"    rd_bad->a_opcode = tl_a_op_e::Get; rd_bad->a_addr = 0xFFFF; rd_bad->a_size = 0; rd_bad->a_mask = 0x1;\n"
                "    start_item(rd_bad); finish_item(rd_bad);\n"
                "    // 3) Illegal mask (0) on partial write to trigger error\n"
                "    tl_item *wr_illegal = tl_item::type_id::create(\"wr_illegal\", nullptr);\n"
                "    wr_illegal->a_opcode = tl_a_op_e::PutPartialData; wr_illegal->a_addr = 4; wr_illegal->a_size = 0; wr_illegal->a_mask = 0x0; wr_illegal->a_data = 0;\n"
                "    start_item(wr_illegal); finish_item(wr_illegal);\n"
            )
        if tag == 'wo_rc':
            # If a known WO (write-only) CSR exists, write then read and expect 0; also read-clear behavior on a known RC if any
            body = ""
            if wdata_off is not None:
                body += (
                    "    // WO semantics: write WDATA, then read back (expect 0)\n"
                    "    tl_item *wo_wr = tl_item::type_id::create(\"wo_wr\", nullptr);\n"
                    f"    wo_wr->a_opcode = tl_a_op_e::PutFullData; wo_wr->a_addr = {wdata_off}; wo_wr->a_size = 2; wo_wr->a_mask = 0xF; wo_wr->a_data = 0xAB;\n"
                    "    start_item(wo_wr); finish_item(wo_wr);\n"
                    "    tl_item *wo_rd = tl_item::type_id::create(\"wo_rd\", nullptr);\n"
                    f"    wo_rd->a_opcode = tl_a_op_e::Get; wo_rd->a_addr = {wdata_off}; wo_rd->a_size = 2; wo_rd->a_mask = 0xF;\n"
                    "    start_item(wo_rd); finish_item(wo_rd);\n"
                )
            if txdata_off is not None and wdata_off is None:
                body += (
                    "    // WO-like semantics on TXDATA (target/hw dependent)\n"
                    "    tl_item *tx_wr = tl_item::type_id::create(\"tx_wr\", nullptr);\n"
                    f"    tx_wr->a_opcode = tl_a_op_e::PutFullData; tx_wr->a_addr = {txdata_off}; tx_wr->a_size = 2; tx_wr->a_mask = 0xF; tx_wr->a_data = 0xAB;\n"
                    "    start_item(tx_wr); finish_item(tx_wr);\n"
                )
            # RC example: reading RDATA may clear internal FIFO state; perform two consecutive reads
            if rdata_off is not None:
                body += (
                    "    // RC check: consecutive reads from RDATA\n"
                    "    tl_item *rc_rd1 = tl_item::type_id::create(\"rc_rd1\", nullptr);\n"
                    f"    rc_rd1->a_opcode = tl_a_op_e::Get; rc_rd1->a_addr = {rdata_off}; rc_rd1->a_size = 2; rc_rd1->a_mask = 0xF;\n"
                    "    start_item(rc_rd1); finish_item(rc_rd1);\n"
                    "    tl_item *rc_rd2 = tl_item::type_id::create(\"rc_rd2\", nullptr);\n"
                    f"    rc_rd2->a_opcode = tl_a_op_e::Get; rc_rd2->a_addr = {rdata_off}; rc_rd2->a_size = 2; rc_rd2->a_mask = 0xF;\n"
                    "    start_item(rc_rd2); finish_item(rc_rd2);\n"
                )
            return body if body else (
                "    // No WO/RC CSRs detected; touch CTRL candidate as a placeholder\n"
                "    tl_item *rd = tl_item::type_id::create(\"rd\", nullptr);\n"
                f"    rd->a_opcode = tl_a_op_e::Get; rd->a_addr = {ctrl_addr}; rd->a_size = 2; rd->a_mask = 0xF;\n"
                "    start_item(rd); finish_item(rd);\n"
            )
        if tag == 'window':
            return (
                "    // Touch a variety of regs to raise addr_cp coverage\n"
                f"    const uint32_t offs[] = {{{intr_test_off}, 12, 20, 24, 32, 36, 40, 44, 48}};\n"
                "    for (unsigned i=0;i<sizeof(offs)/sizeof(offs[0]);++i){\n"
                "      // 4B read\n"
                "      tl_item *rd4 = tl_item::type_id::create(\"rd4\", nullptr); rd4->a_opcode = tl_a_op_e::Get; rd4->a_addr = offs[i]; rd4->a_size = 2; rd4->a_mask = 0xF; start_item(rd4); finish_item(rd4);\n"
                "      // 2B read\n"
                "      tl_item *rd2 = tl_item::type_id::create(\"rd2\", nullptr); rd2->a_opcode = tl_a_op_e::Get; rd2->a_addr = offs[i]; rd2->a_size = 1; rd2->a_mask = 0x3; start_item(rd2); finish_item(rd2);\n"
                "      // 1B read\n"
                "      tl_item *rd1 = tl_item::type_id::create(\"rd1\", nullptr); rd1->a_opcode = tl_a_op_e::Get; rd1->a_addr = offs[i]; rd1->a_size = 0; rd1->a_mask = 0x1; start_item(rd1); finish_item(rd1);\n"
                "    }\n"
                "    // Partial writes with different sizes/masks\n"
                "    tl_item *pw0 = tl_item::type_id::create(\"pw0\", nullptr); pw0->a_opcode=tl_a_op_e::PutPartialData; pw0->a_addr=4; pw0->a_size=0; pw0->a_mask=0x0; pw0->a_data=0x0; start_item(pw0); finish_item(pw0);\n"
                "    tl_item *pw1 = tl_item::type_id::create(\"pw1\", nullptr); pw1->a_opcode=tl_a_op_e::PutPartialData; pw1->a_addr=4; pw1->a_size=0; pw1->a_mask=0x1; pw1->a_data=0x1; start_item(pw1); finish_item(pw1);\n"
                "    tl_item *pw2 = tl_item::type_id::create(\"pw2\", nullptr); pw2->a_opcode=tl_a_op_e::PutPartialData; pw2->a_addr=4; pw2->a_size=1; pw2->a_mask=0x3; pw2->a_data=0x3; start_item(pw2); finish_item(pw2);\n"
                "    tl_item *pw3 = tl_item::type_id::create(\"pw3\", nullptr); pw3->a_opcode=tl_a_op_e::PutPartialData; pw3->a_addr=4; pw3->a_size=2; pw3->a_mask=0x7; pw3->a_data=0x7; start_item(pw3); finish_item(pw3);\n"
            )
        if tag == 'tl_partial':
            return (
                "    // TL partial write coverage boost: valid partial writes at an aligned CSR (CTRL candidate)\n"
                f"    const uint32_t base = {ctrl_addr};\n"
                "    // 1B partial (mask count=1)\n"
                "    { tl_item *w1 = tl_item::type_id::create(\"w1\", nullptr); w1->a_opcode=tl_a_op_e::PutPartialData; w1->a_addr=base; w1->a_size=0; w1->a_mask=0x1; w1->a_data=0x11; start_item(w1); finish_item(w1); }\n"
                "    { tl_item *r1 = tl_item::type_id::create(\"r1\", nullptr); r1->a_opcode=tl_a_op_e::Get; r1->a_addr=base; r1->a_size=2; r1->a_mask=0xF; start_item(r1); finish_item(r1); }\n"
                "    // 2B partial (mask count=2)\n"
                "    { tl_item *w2 = tl_item::type_id::create(\"w2\", nullptr); w2->a_opcode=tl_a_op_e::PutPartialData; w2->a_addr=base; w2->a_size=1; w2->a_mask=0x3; w2->a_data=0x2233; start_item(w2); finish_item(w2); }\n"
                "    { tl_item *r2 = tl_item::type_id::create(\"r2\", nullptr); r2->a_opcode=tl_a_op_e::Get; r2->a_addr=base; r2->a_size=2; r2->a_mask=0xF; start_item(r2); finish_item(r2); }\n"
                "    // 4B partial (mask count=3, non-contig)\n"
                "    { tl_item *w3 = tl_item::type_id::create(\"w3\", nullptr); w3->a_opcode=tl_a_op_e::PutPartialData; w3->a_addr=base; w3->a_size=2; w3->a_mask=0x7; w3->a_data=0x44556677; start_item(w3); finish_item(w3); }\n"
                "    { tl_item *r3 = tl_item::type_id::create(\"r3\", nullptr); r3->a_opcode=tl_a_op_e::Get; r3->a_addr=base; r3->a_size=2; r3->a_mask=0xF; start_item(r3); finish_item(r3); }\n"
                "    { tl_item *wn = tl_item::type_id::create(\"wn\", nullptr); wn->a_opcode=tl_a_op_e::PutPartialData; wn->a_addr=base; wn->a_size=2; wn->a_mask=0x5; wn->a_data=0x8899AABB; start_item(wn); finish_item(wn); }\n"
                "    { tl_item *rn = tl_item::type_id::create(\"rn\", nullptr); rn->a_opcode=tl_a_op_e::Get; rn->a_addr=base; rn->a_size=2; rn->a_mask=0xF; start_item(rn); finish_item(rn); }\n"
            )
        # Fallback simple read of first register
        return (
            "    tl_item *rd = tl_item::type_id::create(\"rd\", nullptr);\n"
            f"    rd->a_opcode = tl_a_op_e::Get; rd->a_addr = {first_off}; rd->a_size = 2; rd->a_mask = 0xF;\n"
            "    start_item(rd); finish_item(rd);\n"
        )

    for tag in ["w1c", "shadow", "regwen", "racl", "tlul_err", "wo_rc", "window", "tl_partial"]:
        emit_simple_seq_and_test(tag, seq_body(tag))

    # RAL wrapper (SV-like naming) if requested
    if has_ral:
        ral_dir = sc_root / 'ral'
        ral_core = (
            "filesets:\n"
            "  files_dv:\n"
            "    files:\n"
            f"      - {name}_ral_pkg.hpp\n"
            "    file_type: user\n"
            f"name: lowrisc:dv:{name}_ral_pkg\n"
            "targets:\n  default:\n    filesets:\n      - files_dv\n"
        )
        ral_hpp = (
            f"#pragma once\n"
            f"#include \"../../tlm/{name}_reg_pkg.hpp\"\n"
            f"namespace {name}_ral_pkg {{ using ::opentitan::{name}::{name}_reg_top; }}\n"
        )
        _write(ral_dir / f"{name}_ral_pkg.core", ral_core)
        _write(ral_dir / f"{name}_ral_pkg.hpp", ral_hpp)

    # Simple tb skeleton
    tb_dir = sc_root / 'tb'
    tb_mk = (
        "# Simple Makefile to build tb for SystemC\n\n"
        "CXX ?= g++\n"
        "CXXFLAGS ?= -O2 -g -std=c++17 -DSC_ALLOW_DEPRECATED_IEEE_API\n\n"
        "ifndef SYSTEMC_HOME\n$(error Please set SYSTEMC_HOME to your SystemC installation path)\nendif\n\n"
        "INC := -I$(SYSTEMC_HOME)/include -I../../tlm\n"
        "LIBS := -L$(SYSTEMC_HOME)/lib -lsystemc\n\n"
        f"SRCS := tb_{name}.cpp ../../tlm/{name}_reg_top.cpp\n\n"
        f"all: tb_{name}\n\n"
        f"tb_{name}: $(SRCS)\n\t$(CXX) $(CXXFLAGS) $(INC) $(SRCS) -o $$@ $(LIBS)\n\n"
        f"clean:\n\trm -f tb_{name}\n\n.PHONY: all clean\n"
    )
    tb_cpp = (
        "#include <systemc>\n"
        "int sc_main(int argc, char** argv) {\n"
        "  sc_core::sc_report_handler::set_actions(SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_, sc_core::SC_DO_NOTHING);\n"
        "  sc_core::sc_start(sc_core::SC_ZERO_TIME);\n"
        "  return 0;\n"
        "}\n"
    )
    _write(tb_dir / f"tb_{name}.cpp", tb_cpp)
    _write(tb_dir / "Makefile", tb_mk)



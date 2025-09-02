// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

<% from reggen import gen_rtl %>
<% 
lblock = block.name.lower()
ublock = lblock.upper()
kbytes = sum(len(rb.flat_regs) for rb in block.reg_blocks.values()) * (block.regwidth // 8)
%>

#pragma once

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <cstdint>
#include <array>

namespace opentitan {
namespace ${lblock} {

// ----- Types mirrored from ${lblock}_reg_pkg.sv -----
<%def name="typedefs_for_iface(iface_name, iface_desc, for_iface, rb)">\
<%
   hdr = gen_rtl.make_box_quote('Typedefs for registers' + for_iface)
%>\
% for r in rb.all_regs:
  % if r.get_n_bits(["q"]):
    % if hdr:

// ${hdr}
    % endif
<%
    r0 = gen_rtl.get_r0(r)
    hdr = None
%>\

  struct ${gen_rtl.get_reg_tx_type(block, r, False)} {
    % if r.is_homogeneous():
<%
      field = r.get_field_list()[0]
      field_q_width = field.get_n_bits(r0.hwext, r0.hwre, ['q'])
%>\
    sc_dt::sc_bv<${field_q_width}> q;
      % if field.hwqe:
    bool qe;
      % endif
      % if r0.hwre or (r0.shadowed and r0.hwext):
    bool re;
      % endif
    % else:
      % for f in reversed(r0.fields):
<%
          field_q_width = f.get_n_bits(r0.hwext, r0.hwre, ["q"])
%>\
        % if field_q_width:
<%
            struct_name = f.name.lower()
%>\
    struct {
      sc_dt::sc_bv<${field_q_width}> q;
          % if f.hwqe:
      bool        qe;
          % endif
          % if r0.hwre or (r0.shadowed and r0.hwext):
      bool        re;
          % endif
    } ${struct_name};
        %endif
      %endfor
    %endif
  };
  %endif
% endfor
% for r in rb.all_regs:
  % if r.get_n_bits(["d"]):
<%
    r0 = gen_rtl.get_r0(r)
%>\

  struct ${gen_rtl.get_reg_tx_type(block, r, True)} {
    % if r.is_homogeneous():
<%
      field = r.get_field_list()[0]
      field_d_width = field.get_n_bits(r0.hwext, r0.hwre, ['d'])
%>\
    sc_dt::sc_bv<${field_d_width}> d;
      % if not r0.hwext:
    bool        de;
      % endif
    % else:
      % for f in reversed(r0.fields):
<%
          field_d_width = f.get_n_bits(r0.hwext, r0.hwre, ["d"])
%>\
        % if field_d_width:
<%
            struct_name = f.name.lower()
%>\
    struct {
      sc_dt::sc_bv<${field_d_width}> d;
          % if not r0.hwext:
      bool        de;
          % endif
    } ${struct_name};
        %endif
      %endfor
    %endif
  };
  % endif
% endfor
% if block.expose_reg_if:
<%
    lpfx = gen_rtl.get_type_name_pfx(block, iface_name)
    addr_width = rb.get_addr_width()
    data_width = block.regwidth
    data_byte_width = data_width // 8
%>\

  struct ${lpfx}_reg2hw_reg_if_t {
    bool        reg_we;
    bool        reg_re;
    sc_dt::sc_bv<${addr_width}> reg_addr;
    sc_dt::sc_bv<${data_width}> reg_wdata;
    sc_dt::sc_bv<${data_byte_width}> reg_be;
  };
% endif
</%def>\

<%def name="reg2hw_for_iface(iface_name, iface_desc, for_iface, rb)">\
<%
lpfx = gen_rtl.get_type_name_pfx(block, iface_name)
nbits = rb.get_n_bits(["q", "qe", "re"])
%>\
% if nbits > 0:

  // Register -> HW type${for_iface}
  struct ${gen_rtl.get_iface_tx_type(block, iface_name, False)} {
% for r in rb.all_regs:
  % if r.get_n_bits(["q"]):
<%
    r0 = gen_rtl.get_r0(r)
    struct_type = gen_rtl.get_reg_tx_type(block, r, False)
%>\
  % if hasattr(r, 'pregs'):
    std::array<${struct_type}, ${len(r.pregs)}> ${r0.name.lower()};
  % else:
    ${struct_type} ${r0.name.lower()};
  % endif
  % endif
% endfor
  };
% endif
</%def>\
<%def name="hw2reg_for_iface(iface_name, iface_desc, for_iface, rb)">\
<%
nbits = rb.get_n_bits(["d", "de"])
%>\
% if nbits > 0:

  // HW -> register type${for_iface}
  struct ${gen_rtl.get_iface_tx_type(block, iface_name, True)} {
% for r in rb.all_regs:
  % if r.get_n_bits(["d"]):
<%
    r0 = gen_rtl.get_r0(r)
    struct_type = gen_rtl.get_reg_tx_type(block, r, True)
%>\
  % if hasattr(r, 'pregs'):
    std::array<${struct_type}, ${len(r.pregs)}> ${r0.name.lower()};
  % else:
    ${struct_type} ${r0.name.lower()};
  % endif
  % endif
% endfor
  };
% endif
</%def>\

class ${lblock}_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<${lblock}_reg_top> tlm_target;

  SC_HAS_PROCESS(${lblock}_reg_top);
  explicit ${lblock}_reg_top(sc_core::sc_module_name name);

  // Register reset helper
  void reset();

  // Soft-access helpers for DV: bypass TLM socket to read/predict mirror
  void sw_predict_write(uint32_t addr, uint32_t data);
  uint32_t sw_read(uint32_t addr);

  // RACL controls (optional; default disabled and all allowed)
  void set_racl_enable(bool enable) { enable_racl_ = enable; }
  void set_racl_error_response(bool enable) { racl_error_rsp_ = enable; }
  void set_role_mask(uint32_t role_mask) { role_mask_ = role_mask; }
  void set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write);

  // Expose total register bytes for DV convenience
  static constexpr std::size_t kRegBytes = ${kbytes};

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = ${block.regwidth};
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
% for iface_name, rb in block.reg_blocks.items():
  enum class reg_${iface_name or 'default'}_e : uint32_t {
%   for r in rb.flat_regs:
    ${r.name.upper()} = ${r.offset},
%   endfor
  };
% endfor

  // Aggregate structs mirroring ${lblock}_reg_pkg
% for iface_name, rb in block.reg_blocks.items():
<%
  iface_desc = iface_name or 'default'
  just_default = len(block.reg_blocks) == 1 and None in block.reg_blocks
  for_iface = '' if just_default else ' for {} interface'.format(iface_desc)
%>\
${typedefs_for_iface(iface_name, iface_desc, for_iface, rb)}\
${reg2hw_for_iface(iface_name, iface_desc, for_iface, rb)}\
${hw2reg_for_iface(iface_name, iface_desc, for_iface, rb)}\
% endfor

  // Flat register storage
% for iface_name, rb in block.reg_blocks.items():
  std::array<uint32_t, ${len(rb.flat_regs)}> regs_${iface_name or 'default'}_;
% endfor

  // Shadow register support (per-register staged/phase; only used when applicable)
% for iface_name, rb in block.reg_blocks.items():
  std::array<uint32_t, ${len(rb.flat_regs)}> shadow_stage_${iface_name or 'default'}_{};
  std::array<uint8_t,  ${len(rb.flat_regs)}> shadow_phase_${iface_name or 'default'}_{}; // 0:first,1:second
% endfor

  // RACL per-register policy (true means allowed)
% for iface_name, rb in block.reg_blocks.items():
  std::array<bool, ${len(rb.flat_regs)}> racl_allow_rd_${iface_name or 'default'}_{};
  std::array<bool, ${len(rb.flat_regs)}> racl_allow_wr_${iface_name or 'default'}_{};
% endfor

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata, uint32_t be_mask);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
% for iface_name, rb in block.reg_blocks.items():
<%
  var_suffix = ('' if iface_name is None else '_' + iface_name.lower())
%>\
  ${gen_rtl.get_iface_tx_type(block, iface_name, False)} reg2hw${var_suffix};
  ${gen_rtl.get_iface_tx_type(block, iface_name, True)}  hw2reg${var_suffix};
% endfor

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace ${lblock}
}  // namespace opentitan



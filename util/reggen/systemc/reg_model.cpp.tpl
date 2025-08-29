// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

<% lblock = block.name.lower() %>

#include "${lblock}_reg_pkg.hpp"
#include <cstring>

namespace opentitan {
namespace ${lblock} {

${lblock}_reg_top::${lblock}_reg_top(sc_core::sc_module_name name)
    : sc_core::sc_module(name), tlm_target("tlm_target") {
  tlm_target.register_b_transport(this, &${lblock}_reg_top::b_transport);
  reset();
}

void ${lblock}_reg_top::reset() {
% for iface_name, rb in block.reg_blocks.items():
  regs_${iface_name or 'default'}_.fill(0u);
  shadow_stage_${iface_name or 'default'}_.fill(0u);
  shadow_phase_${iface_name or 'default'}_.fill(0u);
  racl_allow_rd_${iface_name or 'default'}_.fill(true);
  racl_allow_wr_${iface_name or 'default'}_.fill(true);
% endfor
}

void ${lblock}_reg_top::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
  (void)delay;
  const uint64_t addr = trans.get_address();
  unsigned char *data = trans.get_data_ptr();
  const unsigned len = trans.get_data_length();
  const bool is_write = (trans.get_command() == tlm::TLM_WRITE_COMMAND);

  // Word-aligned, full-width accesses are expected.
  if (len != kWordBytes || (addr % kWordBytes) != 0) {
    trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
    return;
  }

  // RACL enforcement
  if (enable_racl_ && !check_racl(is_write, static_cast<uint32_t>(addr))) {
    if (racl_error_rsp_) {
      trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
      return;
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    return;
  }

  // Decode to interface/register arrays by offset
% for iface_name, rb in block.reg_blocks.items():
  if (addr < ${rb.offset}) {
    const uint32_t index = static_cast<uint32_t>(addr / kWordBytes);
    if (index >= regs_${iface_name or 'default'}_.size()) {
      trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
      return;
    }
    if (is_write) {
      uint32_t w;
      std::memcpy(&w, data, sizeof(uint32_t));
      if (!apply_sw_write(static_cast<uint32_t>(addr), w)) {
        trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
        return;
      }
    } else {
      uint32_t r = read_sw(static_cast<uint32_t>(addr));
      std::memcpy(data, &r, sizeof(uint32_t));
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    return;
  }
% endfor

  trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
}

bool ${lblock}_reg_top::check_racl(bool is_write, uint32_t addr) const {
% for iface_name, rb in block.reg_blocks.items():
  if (addr < ${rb.offset}) {
    const uint32_t index = addr / kWordBytes;
    return is_write ? racl_allow_wr_${iface_name or 'default'}_[index]
                    : racl_allow_rd_${iface_name or 'default'}_[index];
  }
% endfor
  return true;
}

bool ${lblock}_reg_top::apply_sw_write(uint32_t addr, uint32_t wdata) {
  const uint32_t index = addr / kWordBytes;
% for iface_name, rb in block.reg_blocks.items():
  if (addr < ${rb.offset}) {
    auto &regs = regs_${iface_name or 'default'}_;
    uint32_t oldv = regs[index];
    uint32_t newv = oldv;
    bool allow = true;
    switch (index) {
%   for i, r in enumerate(rb.flat_regs):
      case ${i}: {
%       if r.regwen:
<%
  regwen_idx = None
  regwen_width = 1
  regwen_lsb = 0
  regwen_true = 1
  for j, rr in enumerate(rb.flat_regs):
      if r.regwen and rr.name == r.regwen:
          regwen_idx = j
          fw = rr.fields[0]
          regwen_width = fw.bits.width()
          regwen_lsb = fw.bits.lsb
          regwen_true = fw.resval or 1
          break
%>
        {
          uint32_t gw = regs[${regwen_idx}];
          uint32_t gate = (gw >> ${regwen_lsb}) & ((${1 << regwen_width}u) - 1u);
          allow = (gate == ${regwen_true}u);
        }
%       endif
        if (!allow) break;
%       if r.shadowed and not r.hwext:
        {
          auto &phase = shadow_phase_${iface_name or 'default'}_[index];
          auto &stage = shadow_stage_${iface_name or 'default'}_[index];
          if (phase == 0) { stage = wdata; phase = 1; break; }
          if (wdata != stage) { phase = 0; break; }
          // commit on match; fallthrough to apply transformations using staged raw data
        }
%       endif
%       for f in r.fields:
<%
  width = f.bits.width()
  lsb = f.bits.lsb
  mask = ((1 << width) - 1)
%>
        {
          constexpr uint32_t mask = (static_cast<uint32_t>(${mask}) << ${lsb});
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
%         if f.swaccess.key == 'rw':
          newv = (newv & ~mask) | raw;
%         elif f.swaccess.key == 'ro':
          // ignore writes
%         elif f.swaccess.key == 'wo':
          newv = (newv & ~mask) | raw;
%         elif f.swaccess.key == 'rw1c' or f.swaccess.key == 'r0w1c':
          newv = newv & ~raw;
%         elif f.swaccess.key == 'rw1s':
          newv = newv | raw;
%         elif f.swaccess.key == 'rw0c':
          newv = newv & ~(~raw & mask);
%         elif f.swaccess.key == 'rc':
          // no effect on write; clears on read
%         else:
          // default: no-op
%         endif
        }
%       endfor
%       if r.name.lower() == 'intr_test':
<%
  intr_state_idx = None
  intr_state_mask = 0
  for j2, rr2 in enumerate(rb.flat_regs):
      if rr2.name and rr2.name.lower() == 'intr_state':
          intr_state_idx = j2
          m2 = 0
          for f2 in rr2.fields:
              width2 = f2.bits.width()
              lsb2 = f2.bits.lsb
              m2 |= ((1 << width2) - 1) << lsb2
          intr_state_mask = m2
          break
%>
%         if intr_state_idx is not None:
        // Writing INTR_TEST should set corresponding bits in INTR_STATE (SV behavior)
        regs[${intr_state_idx}] |= (wdata & ${intr_state_mask});
%         endif
%       endif
        regs[index] = newv;
        break;
      }
%   endfor
      default: break;
    }
    return true;
  }
% endfor
  return false;
}

uint32_t ${lblock}_reg_top::read_sw(uint32_t addr) {
  const uint32_t index = addr / kWordBytes;
% for iface_name, rb in block.reg_blocks.items():
  if (addr < ${rb.offset}) {
    auto &regs = regs_${iface_name or 'default'}_;
    uint32_t rdata = regs[index];
    switch (index) {
%   for i, r in enumerate(rb.flat_regs):
      case ${i}: {
%       for f in r.fields:
<%
  width = f.bits.width()
  lsb = f.bits.lsb
  mask = ((1 << width) - 1)
%>
        {
          constexpr uint32_t mask = (static_cast<uint32_t>(${mask}) << ${lsb});
%         if f.swaccess.key == 'wo':
          rdata &= ~mask;
%         elif f.swaccess.key == 'r0w1c':
          rdata &= ~mask;
%         elif f.swaccess.key == 'rc':
          // clear-on-read
          regs[index] &= ~mask;
%         endif
        }
%       endfor
%       if r.shadowed and not r.hwext:
        // reading resets shadow phase for non-RO types
        shadow_phase_${iface_name or 'default'}_[index] = 0;
%       endif
        break;
      }
%   endfor
      default: break;
    }
    return rdata;
  }
% endfor
  return 0u;
}

void ${lblock}_reg_top::set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write) {
% for iface_name, rb in block.reg_blocks.items():
  if (reg_index < racl_allow_rd_${iface_name or 'default'}_.size()) {
    racl_allow_rd_${iface_name or 'default'}_[reg_index] = allow_read;
    racl_allow_wr_${iface_name or 'default'}_[reg_index] = allow_write;
    return;
  }
% endfor
}

}  // namespace ${lblock}
}  // namespace opentitan



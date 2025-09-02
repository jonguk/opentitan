// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0



#include "entropy_src_reg_pkg.hpp"
#include <cstring>

namespace opentitan {
namespace entropy_src {

entropy_src_reg_top::entropy_src_reg_top(sc_core::sc_module_name name)
    : sc_core::sc_module(name), tlm_target("tlm_target") {
  tlm_target.register_b_transport(this, &entropy_src_reg_top::b_transport);
  reset();
}

void entropy_src_reg_top::reset() {
  regs_default_.fill(0u);
  shadow_stage_default_.fill(0u);
  shadow_phase_default_.fill(0u);
  racl_allow_rd_default_.fill(true);
  racl_allow_wr_default_.fill(true);
}

void entropy_src_reg_top::sw_predict_write(uint32_t addr, uint32_t data) {
  (void)apply_sw_write(addr, data);
}

uint32_t entropy_src_reg_top::sw_read(uint32_t addr) {
  return read_sw(addr);
}

void entropy_src_reg_top::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
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
  if (addr < 224) {
    const uint32_t index = static_cast<uint32_t>(addr / kWordBytes);
    if (index >= regs_default_.size()) {
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

  trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
}

bool entropy_src_reg_top::check_racl(bool is_write, uint32_t addr) const {
  if (addr < 224) {
    const uint32_t index = addr / kWordBytes;
    return is_write ? racl_allow_wr_default_[index]
                    : racl_allow_rd_default_[index];
  }
  return true;
}

bool entropy_src_reg_top::apply_sw_write(uint32_t addr, uint32_t wdata) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 224) {
    auto &regs = regs_default_;
    uint32_t oldv = regs[index];
    uint32_t newv = oldv;
    bool allow = true;
    switch (index) {
      case 0: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw;
        }
        regs[index] = newv;
        break;
      }
      case 1: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 2: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        // Writing INTR_TEST should set corresponding bits in INTR_STATE (SV behavior)
        regs[0] |= (wdata & 15);
        regs[index] = newv;
        break;
      }
      case 3: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 4: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }
        regs[index] = newv;
        break;
      }
      case 5: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }
        regs[index] = newv;
        break;
      }
      case 6: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 7: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 8: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 8);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 12);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 20);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(255) << 24);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 9: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 10: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 11: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 12: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 13: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 14: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 15: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 16: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 17: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 18: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 19: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 20: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 21: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 22: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 23: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 24: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 25: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 26: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 27: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 28: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 29: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 30: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 31: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 32: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 33: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 34: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 35: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 36: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 37: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 38: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 39: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 40: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 41: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 8);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 12);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 20);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 24);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 28);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 42: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 43: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 44: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 45: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 46: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 47: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 48: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 49: {

        {
          uint32_t gw = regs[6];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 50: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 51: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 3);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 6);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 8);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 9);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 17);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 52: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 5);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 8);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 9);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 10);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 11);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 12);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 13);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 14);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 15);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 17);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 18);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 31);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw & mask);
        }
        regs[index] = newv;
        break;
      }
      case 53: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 20);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 21);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 22);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 23);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 24);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 28);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 29);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 30);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 54: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(31) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 55: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(511) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      default: break;
    }
    return true;
  }
  return false;
}

uint32_t entropy_src_reg_top::read_sw(uint32_t addr) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 224) {
    auto &regs = regs_default_;
    uint32_t rdata = regs[index];
    switch (index) {
      case 0: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
        }
        break;
      }
      case 1: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
        }
        break;
      }
      case 2: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          rdata &= ~mask;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          rdata &= ~mask;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          rdata &= ~mask;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
          rdata &= ~mask;
        }
        break;
      }
      case 3: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          rdata &= ~mask;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          rdata &= ~mask;
        }
        break;
      }
      case 4: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 5: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 6: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 7: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
        }
        break;
      }
      case 8: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 8);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 12);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 16);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 20);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(255) << 24);
        }
        break;
      }
      case 9: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
        }
        break;
      }
      case 10: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 11: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 12: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 13: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 14: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 15: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 16: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 17: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 18: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 19: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 20: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 21: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 22: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 23: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 24: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 25: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 26: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 27: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 28: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 29: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 30: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 31: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 32: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 33: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 34: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 35: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 36: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 37: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 38: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 39: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 40: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }
        break;
      }
      case 41: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 8);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 12);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 16);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 20);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 24);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 28);
        }
        break;
      }
      case 42: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
        }
        break;
      }
      case 43: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 4);
        }
        break;
      }
      case 44: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(15) << 0);
        }
        break;
      }
      case 45: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 46: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 47: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 48: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 49: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 0);
        }
        break;
      }
      case 50: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 0);
        }
        break;
      }
      case 51: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 3);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 6);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 8);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 9);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 16);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 17);
        }
        break;
      }
      case 52: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 5);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 8);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 9);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 10);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 11);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 12);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 13);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 14);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 15);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 16);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 17);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 18);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 31);
        }
        break;
      }
      case 53: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 20);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 21);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 22);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 23);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 24);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 28);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 29);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 30);
        }
        break;
      }
      case 54: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(31) << 0);
        }
        break;
      }
      case 55: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(511) << 0);
        }
        break;
      }
      default: break;
    }
    return rdata;
  }
  return 0u;
}

void entropy_src_reg_top::set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write) {
  if (reg_index < racl_allow_rd_default_.size()) {
    racl_allow_rd_default_[reg_index] = allow_read;
    racl_allow_wr_default_[reg_index] = allow_write;
    return;
  }
}

}  // namespace entropy_src
}  // namespace opentitan



// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0



#include "otp_macro_reg_pkg.hpp"
#include <cstring>

namespace opentitan {
namespace otp_macro {

otp_macro_reg_top::otp_macro_reg_top(sc_core::sc_module_name name)
    : sc_core::sc_module(name), tlm_target("tlm_target") {
  tlm_target.register_b_transport(this, &otp_macro_reg_top::b_transport);
  reset();
}

void otp_macro_reg_top::reset() {
  regs_prim_.fill(0u);
  shadow_stage_prim_.fill(0u);
  shadow_phase_prim_.fill(0u);
  racl_allow_rd_prim_.fill(true);
  racl_allow_wr_prim_.fill(true);
}

void otp_macro_reg_top::sw_predict_write(uint32_t addr, uint32_t data) {
  (void)apply_sw_write(addr, data);
}

uint32_t otp_macro_reg_top::sw_read(uint32_t addr) {
  return read_sw(addr);
}

void otp_macro_reg_top::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
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
  if (addr < 32) {
    const uint32_t index = static_cast<uint32_t>(addr / kWordBytes);
    if (index >= regs_prim_.size()) {
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

bool otp_macro_reg_top::check_racl(bool is_write, uint32_t addr) const {
  if (addr < 32) {
    const uint32_t index = addr / kWordBytes;
    return is_write ? racl_allow_wr_prim_[index]
                    : racl_allow_rd_prim_[index];
  }
  return true;
}

bool otp_macro_reg_top::apply_sw_write(uint32_t addr, uint32_t wdata) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 32) {
    auto &regs = regs_prim_;
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
          constexpr uint32_t mask = (static_cast<uint32_t>(1023) << 4);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(2047) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 1: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(127) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(127) << 8);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 15);
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
      case 2: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 3: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1023) << 4);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 16);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 17);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 18);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 19);
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
        regs[index] = newv;
        break;
      }
      case 4: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1023) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 12);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 13);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 14);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }
        regs[index] = newv;
        break;
      }
      case 5: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 6);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 8);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 9);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 12);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 13);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
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
      case 6: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1023) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 11);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 12);
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
      case 7: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 8);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 14);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 15);
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

uint32_t otp_macro_reg_top::read_sw(uint32_t addr) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 32) {
    auto &regs = regs_prim_;
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
          constexpr uint32_t mask = (static_cast<uint32_t>(1023) << 4);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(2047) << 16);
        }
        break;
      }
      case 1: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(127) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(127) << 8);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 15);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 2: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 3: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1023) << 4);
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
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 19);
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
        break;
      }
      case 4: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1023) << 0);
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
        break;
      }
      case 5: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 6);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 8);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 9);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 12);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 13);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 6: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1023) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 11);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 12);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 16);
        }
        break;
      }
      case 7: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 8);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 14);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 15);
        }
        break;
      }
      default: break;
    }
    return rdata;
  }
  return 0u;
}

void otp_macro_reg_top::set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write) {
  if (reg_index < racl_allow_rd_prim_.size()) {
    racl_allow_rd_prim_[reg_index] = allow_read;
    racl_allow_wr_prim_[reg_index] = allow_write;
    return;
  }
}

}  // namespace otp_macro
}  // namespace opentitan



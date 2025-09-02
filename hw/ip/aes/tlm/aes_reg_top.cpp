// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0



#include "aes_reg_pkg.hpp"
#include <cstring>

namespace opentitan {
namespace aes {

aes_reg_top::aes_reg_top(sc_core::sc_module_name name)
    : sc_core::sc_module(name), tlm_target("tlm_target") {
  tlm_target.register_b_transport(this, &aes_reg_top::b_transport);
  reset();
}

void aes_reg_top::reset() {
  regs_default_.fill(0u);
  shadow_stage_default_.fill(0u);
  shadow_phase_default_.fill(0u);
  racl_allow_rd_default_.fill(true);
  racl_allow_wr_default_.fill(true);
}

void aes_reg_top::sw_predict_write(uint32_t addr, uint32_t data) {
  (void)apply_sw_write(addr, data);
}

uint32_t aes_reg_top::sw_read(uint32_t addr) {
  return read_sw(addr);
}

void aes_reg_top::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
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
  if (addr < 136) {
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

bool aes_reg_top::check_racl(bool is_write, uint32_t addr) const {
  if (addr < 136) {
    const uint32_t index = addr / kWordBytes;
    return is_write ? racl_allow_wr_default_[index]
                    : racl_allow_rd_default_[index];
  }
  return true;
}

bool aes_reg_top::apply_sw_write(uint32_t addr, uint32_t wdata) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 136) {
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
      case 1: {
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
      case 2: {
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
      case 3: {
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
      case 4: {
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
      case 5: {
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
      case 6: {
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
      case 7: {
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
      case 8: {
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
      case 9: {
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
      case 10: {
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
      case 11: {
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
      case 12: {
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
      case 13: {
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
      case 14: {
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
      case 15: {
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
      case 16: {
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
      case 17: {
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
      case 18: {
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
      case 19: {
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
      case 20: {
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
      case 21: {
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
      case 22: {
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
      case 23: {
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
      case 24: {
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
      case 25: {
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
      case 26: {
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
      case 27: {
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
      case 28: {
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
      case 29: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 0);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 2);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 8);
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
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 12);
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
        regs[index] = newv;
        break;
      }
      case 30: {

        {
          uint32_t gw = regs[31];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;
        {
          auto &phase = shadow_phase_default_[index];
          auto &stage = shadow_stage_default_[index];
          if (phase == 0) { stage = wdata; phase = 1; break; }
          if (wdata != stage) { phase = 0; break; }
          // commit on match; fallthrough to apply transformations using staged raw data
        }

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
      case 31: {
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
      case 32: {
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
      case 33: {
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
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 4);
          const uint32_t raw = wdata & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 5);
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
        regs[index] = newv;
        break;
      }
      default: break;
    }
    return true;
  }
  return false;
}

uint32_t aes_reg_top::read_sw(uint32_t addr) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 136) {
    auto &regs = regs_default_;
    uint32_t rdata = regs[index];
    switch (index) {
      case 0: {

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
      case 1: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 2: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 3: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 4: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 5: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 6: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 7: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 8: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 9: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 10: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 11: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 12: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 13: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 14: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 15: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 16: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 17: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 18: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 19: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 20: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 21: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 22: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 23: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 24: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 25: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 26: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 27: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 28: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 29: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(63) << 2);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 8);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 11);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 12);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 15);
        }
        break;
      }
      case 30: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
        }
        // reading resets shadow phase for non-RO types
        shadow_phase_default_[index] = 0;
        break;
      }
      case 31: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 32: {

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
      case 33: {

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
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 4);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 5);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 6);
        }
        break;
      }
      default: break;
    }
    return rdata;
  }
  return 0u;
}

void aes_reg_top::set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write) {
  if (reg_index < racl_allow_rd_default_.size()) {
    racl_allow_rd_default_[reg_index] = allow_read;
    racl_allow_wr_default_[reg_index] = allow_write;
    return;
  }
}

}  // namespace aes
}  // namespace opentitan



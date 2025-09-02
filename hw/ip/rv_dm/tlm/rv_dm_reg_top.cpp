// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0



#include "rv_dm_reg_pkg.hpp"
#include <cstring>

namespace opentitan {
namespace rv_dm {

rv_dm_reg_top::rv_dm_reg_top(sc_core::sc_module_name name)
    : sc_core::sc_module(name), tlm_target("tlm_target") {
  tlm_target.register_b_transport(this, &rv_dm_reg_top::b_transport);
  reset();
}

void rv_dm_reg_top::reset() {
  regs_regs_.fill(0u);
  shadow_stage_regs_.fill(0u);
  shadow_phase_regs_.fill(0u);
  racl_allow_rd_regs_.fill(true);
  racl_allow_wr_regs_.fill(true);
  regs_mem_.fill(0u);
  shadow_stage_mem_.fill(0u);
  shadow_phase_mem_.fill(0u);
  racl_allow_rd_mem_.fill(true);
  racl_allow_wr_mem_.fill(true);
  regs_dbg_.fill(0u);
  shadow_stage_dbg_.fill(0u);
  shadow_phase_dbg_.fill(0u);
  racl_allow_rd_dbg_.fill(true);
  racl_allow_wr_dbg_.fill(true);
}

void rv_dm_reg_top::sw_predict_write(uint32_t addr, uint32_t data) {
  (void)apply_sw_write(addr, data);
}

uint32_t rv_dm_reg_top::sw_read(uint32_t addr) {
  return read_sw(addr);
}

void rv_dm_reg_top::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
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
  if (addr < 12) {
    const uint32_t index = static_cast<uint32_t>(addr / kWordBytes);
    if (index >= regs_regs_.size()) {
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
  if (addr < 4096) {
    const uint32_t index = static_cast<uint32_t>(addr / kWordBytes);
    if (index >= regs_mem_.size()) {
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
  if (addr < 512) {
    const uint32_t index = static_cast<uint32_t>(addr / kWordBytes);
    if (index >= regs_dbg_.size()) {
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

bool rv_dm_reg_top::check_racl(bool is_write, uint32_t addr) const {
  if (addr < 12) {
    const uint32_t index = addr / kWordBytes;
    return is_write ? racl_allow_wr_regs_[index]
                    : racl_allow_rd_regs_[index];
  }
  if (addr < 4096) {
    const uint32_t index = addr / kWordBytes;
    return is_write ? racl_allow_wr_mem_[index]
                    : racl_allow_rd_mem_[index];
  }
  if (addr < 512) {
    const uint32_t index = addr / kWordBytes;
    return is_write ? racl_allow_wr_dbg_[index]
                    : racl_allow_rd_dbg_[index];
  }
  return true;
}

bool rv_dm_reg_top::apply_sw_write(uint32_t addr, uint32_t wdata) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 12) {
    auto &regs = regs_regs_;
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
        regs[index] = newv;
        break;
      }
      case 1: {
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
      case 2: {

        {
          uint32_t gw = regs[1];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
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
      default: break;
    }
    return true;
  }
  if (addr < 4096) {
    auto &regs = regs_mem_;
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
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
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
          // ignore writes
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
          // ignore writes
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
          // ignore writes
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
          // ignore writes
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
          // ignore writes
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
          // ignore writes
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
      case 12: {
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
      case 13: {
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
      case 14: {
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
      case 15: {
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
      case 16: {
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
      case 17: {
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
      case 18: {
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
      case 19: {
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
      case 20: {
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
      case 21: {
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
      case 22: {
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
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
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
      case 40: {
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
      case 41: {
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
      case 42: {
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
      case 43: {
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
      case 44: {
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
      case 45: {
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
      case 46: {
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
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 49: {
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
      case 50: {
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
      case 51: {
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
      case 52: {
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
      case 53: {
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
      case 54: {
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
      case 55: {
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
      case 56: {
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
      case 57: {
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
      case 58: {
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
      case 59: {
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
      case 60: {
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
      case 61: {
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
      case 62: {
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
      case 63: {
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
      case 64: {
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
      case 65: {
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
      case 66: {
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
      case 67: {
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
      case 68: {
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
      case 69: {
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
      case 70: {
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
      case 71: {
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
      case 72: {
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
      case 73: {
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
      case 74: {
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
      case 75: {
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
      case 76: {
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
      case 77: {
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
      case 78: {
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
      case 79: {
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
      case 80: {
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
      case 81: {
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
      case 82: {
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
      case 83: {
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
      case 84: {
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
      case 85: {
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
      case 86: {
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
      case 87: {
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
      case 88: {
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
      case 89: {
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
      case 90: {
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
      case 91: {
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
      case 92: {
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
      case 93: {
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
      case 94: {
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
      case 95: {
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
      case 96: {
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
      case 97: {
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
      case 98: {
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
      case 99: {
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
      case 100: {
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
      case 101: {
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
      case 102: {
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
      case 103: {
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
      case 104: {
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
      case 105: {
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
      case 106: {
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
      case 107: {
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
      case 108: {
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
      case 109: {
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
      case 110: {
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
      case 111: {
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
      case 112: {
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
      case 113: {
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
      case 114: {
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
      case 115: {
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
      case 116: {
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
      case 117: {
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
      case 118: {
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
      case 119: {
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
      case 120: {
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
      case 121: {
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
      case 122: {
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
      case 123: {
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
      case 124: {
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
      case 125: {
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
      case 126: {
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
      case 127: {
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
      case 128: {
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
      case 129: {
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
      case 130: {
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
      case 131: {
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
      case 132: {
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
      case 133: {
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
      case 134: {
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
      case 135: {
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
      case 136: {
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
      case 137: {
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
      case 138: {
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
      case 139: {
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
      case 140: {
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
      case 141: {
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
      case 142: {
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
      case 143: {
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
      case 144: {
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
      case 145: {
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
      case 146: {
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
      case 147: {
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
      case 148: {
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
      case 149: {
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
      case 150: {
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
      case 151: {
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
      case 152: {
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
      case 153: {
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
      case 154: {
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
      case 155: {
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
      case 156: {
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
      case 157: {
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
      case 158: {
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
      case 159: {
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
      case 160: {
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
      case 161: {
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
      case 162: {
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
      case 163: {
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
      case 164: {
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
      case 165: {
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
      case 166: {
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
      case 167: {
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
      case 168: {
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
      case 169: {
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
      case 170: {
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
      case 171: {
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
      case 172: {
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
      case 173: {
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
      case 174: {
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
      case 175: {
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
      case 176: {
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
      case 177: {
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
      case 178: {
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
      case 179: {
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
      case 180: {
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
      case 181: {
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
      case 182: {
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
      case 183: {
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
      case 184: {
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
      case 185: {
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
      case 186: {
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
      case 187: {
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
      case 188: {
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
      case 189: {
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
      case 190: {
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
      case 191: {
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
      case 192: {
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
      case 193: {
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
      case 194: {
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
      case 195: {
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
      case 196: {
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
      case 197: {
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
      case 198: {
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
      case 199: {
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
      case 200: {
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
      case 201: {
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
      case 202: {
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
      case 203: {
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
      case 204: {
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
      case 205: {
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
      case 206: {
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
      case 207: {
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
      case 208: {
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
      case 209: {
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
      case 210: {
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
      case 211: {
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
      case 212: {
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
      case 213: {
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
      case 214: {
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
      case 215: {
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
      case 216: {
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
      case 217: {
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
      case 218: {
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
      case 219: {
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
      case 220: {
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
      case 221: {
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
      case 222: {
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
      case 223: {
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
      case 224: {
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
      case 225: {
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
      case 226: {
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
      case 227: {
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
      case 228: {
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
      case 229: {
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
      case 230: {
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
      case 231: {
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
      case 232: {
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
      case 233: {
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
      case 234: {
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
      case 235: {
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
      case 236: {
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
      case 237: {
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
      case 238: {
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
      case 239: {
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
      case 240: {
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
      case 241: {
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
      case 242: {
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
      case 243: {
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
      case 244: {
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
      case 245: {
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
      case 246: {
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
      case 247: {
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
      case 248: {
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
      case 249: {
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
      case 250: {
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
      case 251: {
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
      case 252: {
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
      case 253: {
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
      case 254: {
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
      case 255: {
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
      case 256: {
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
      case 257: {
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
      case 258: {
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
      case 259: {
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
      case 260: {
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
      case 261: {
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
      case 262: {
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
      case 263: {
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
      case 264: {
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
      case 265: {
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
      case 266: {
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
      case 267: {
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
      case 268: {
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
      case 269: {
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
      case 270: {
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
      case 271: {
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
      case 272: {
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
      case 273: {
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
      case 274: {
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
      case 275: {
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
      case 276: {
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
      case 277: {
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
      case 278: {
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
      case 279: {
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
      case 280: {
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
      default: break;
    }
    return true;
  }
  if (addr < 512) {
    auto &regs = regs_dbg_;
    uint32_t oldv = regs[index];
    uint32_t newv = oldv;
    bool allow = true;
    switch (index) {
      default: break;
    }
    return true;
  }
  return false;
}

uint32_t rv_dm_reg_top::read_sw(uint32_t addr) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 12) {
    auto &regs = regs_regs_;
    uint32_t rdata = regs[index];
    switch (index) {
      case 0: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 1: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 2: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      default: break;
    }
    return rdata;
  }
  if (addr < 4096) {
    auto &regs = regs_mem_;
    uint32_t rdata = regs[index];
    switch (index) {
      case 0: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 1: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 2: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 3: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          rdata &= ~mask;
        }
        break;
      }
      case 4: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 5: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 6: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 7: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 8: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 9: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
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
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 12: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 13: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 14: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 15: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 16: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
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
        }
        break;
      }
      case 22: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 23: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 24: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
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
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
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
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 40: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 41: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 42: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 43: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 44: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 45: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 46: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
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
        }
        break;
      }
      case 49: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 50: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 51: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 52: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 53: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 54: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 55: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 56: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 57: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 58: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 59: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 60: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 61: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 62: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 63: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 64: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 65: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 66: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 67: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 68: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 69: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 70: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 71: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 72: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 73: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 74: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 75: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 76: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 77: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 78: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 79: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 80: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 81: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 82: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 83: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 84: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 85: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 86: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 87: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 88: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 89: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 90: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 91: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 92: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 93: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 94: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 95: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 96: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 97: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 98: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 99: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 100: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 101: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 102: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 103: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 104: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 105: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 106: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 107: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 108: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 109: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 110: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 111: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 112: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 113: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 114: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 115: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 116: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 117: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 118: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 119: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 120: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 121: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 122: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 123: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 124: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 125: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 126: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 127: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 128: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 129: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 130: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 131: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 132: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 133: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 134: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 135: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 136: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 137: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 138: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 139: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 140: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 141: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 142: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 143: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 144: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 145: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 146: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 147: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 148: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 149: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 150: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 151: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 152: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 153: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 154: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 155: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 156: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 157: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 158: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 159: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 160: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 161: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 162: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 163: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 164: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 165: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 166: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 167: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 168: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 169: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 170: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 171: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 172: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 173: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 174: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 175: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 176: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 177: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 178: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 179: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 180: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 181: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 182: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 183: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 184: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 185: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 186: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 187: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 188: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 189: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 190: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 191: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 192: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 193: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 194: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 195: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 196: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 197: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 198: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 199: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 200: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 201: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 202: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 203: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 204: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 205: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 206: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 207: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 208: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 209: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 210: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 211: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 212: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 213: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 214: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 215: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 216: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 217: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 218: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 219: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 220: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 221: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 222: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 223: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 224: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 225: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 226: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 227: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 228: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 229: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 230: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 231: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 232: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 233: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 234: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 235: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 236: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 237: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 238: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 239: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 240: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 241: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 242: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 243: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 244: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 245: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 246: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 247: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 248: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 249: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 250: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 251: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 252: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 253: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 254: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 255: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 256: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 257: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 258: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 259: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 260: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 261: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 262: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 263: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 264: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 265: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 266: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 267: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 268: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 269: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 270: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 271: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 272: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 273: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 274: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 275: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 276: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 277: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 278: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 279: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      case 280: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        break;
      }
      default: break;
    }
    return rdata;
  }
  if (addr < 512) {
    auto &regs = regs_dbg_;
    uint32_t rdata = regs[index];
    switch (index) {
      default: break;
    }
    return rdata;
  }
  return 0u;
}

void rv_dm_reg_top::set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write) {
  if (reg_index < racl_allow_rd_regs_.size()) {
    racl_allow_rd_regs_[reg_index] = allow_read;
    racl_allow_wr_regs_[reg_index] = allow_write;
    return;
  }
  if (reg_index < racl_allow_rd_mem_.size()) {
    racl_allow_rd_mem_[reg_index] = allow_read;
    racl_allow_wr_mem_[reg_index] = allow_write;
    return;
  }
  if (reg_index < racl_allow_rd_dbg_.size()) {
    racl_allow_rd_dbg_[reg_index] = allow_read;
    racl_allow_wr_dbg_[reg_index] = allow_write;
    return;
  }
}

}  // namespace rv_dm
}  // namespace opentitan



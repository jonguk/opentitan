// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0



#include "keymgr_reg_pkg.hpp"
#include <cstring>

namespace opentitan {
namespace keymgr {

keymgr_reg_top::keymgr_reg_top(sc_core::sc_module_name name)
    : sc_core::sc_module(name), tlm_target("tlm_target") {
  tlm_target.register_b_transport(this, &keymgr_reg_top::b_transport);
  reset();
}

void keymgr_reg_top::reset() {
  regs_default_.fill(0u);
  shadow_stage_default_.fill(0u);
  shadow_phase_default_.fill(0u);
  racl_allow_rd_default_.fill(true);
  racl_allow_wr_default_.fill(true);
}

void keymgr_reg_top::sw_predict_write(uint32_t addr, uint32_t data) {
  (void)apply_sw_write(addr, data, 0xFu);
}

uint32_t keymgr_reg_top::sw_read(uint32_t addr) {
  return read_sw(addr);
}

void keymgr_reg_top::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
  (void)delay;
  const uint64_t addr = trans.get_address();
  unsigned char *data = trans.get_data_ptr();
  const unsigned len = trans.get_data_length();
  const bool is_write = (trans.get_command() == tlm::TLM_WRITE_COMMAND);

  // Allow 1/2/4-byte accesses; require alignment to transfer size
  if (!((len == 1) || (len == 2) || (len == kWordBytes)) || (addr % len) != 0) {
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
  if (addr < 252) {
    const uint32_t index = static_cast<uint32_t>(addr / kWordBytes);
    if (index >= regs_default_.size()) {
      trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
      return;
    }
    if (is_write) {
      uint32_t w;
      std::memcpy(&w, data, sizeof(uint32_t));
      // Compose TL-UL byte-enable mask (per-lane) from TLM byte enables
      uint32_t be_mask = 0xFu;
      if (auto be_ptr = trans.get_byte_enable_ptr()) {
        be_mask = 0u;
        const unsigned bel = trans.get_byte_enable_length();
        const unsigned n = bel ? bel : len;
        for (unsigned i = 0; i < n && i < 4; ++i) {
          if (be_ptr[i] != 0) be_mask |= (1u << i);
        }
      }
      if (!apply_sw_write(static_cast<uint32_t>(addr), w, be_mask)) {
        trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
        return;
      }
    } else {
      uint32_t r = read_sw(static_cast<uint32_t>(addr));
      std::memcpy(data, &r, len);
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    return;
  }

  trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
}

bool keymgr_reg_top::check_racl(bool is_write, uint32_t addr) const {
  if (addr < 252) {
    const uint32_t index = addr / kWordBytes;
    return is_write ? racl_allow_wr_default_[index]
                    : racl_allow_rd_default_[index];
  }
  return true;
}

bool keymgr_reg_top::apply_sw_write(uint32_t addr, uint32_t wdata, uint32_t be_mask) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 252) {
    auto &regs = regs_default_;
    uint32_t oldv = regs[index];
    uint32_t newv = oldv;
    bool allow = true;
    // Expand lane mask (1 bit per byte) to 32-bit byte mask
    uint32_t byte_mask32 = 0;
    for (int i = 0; i < 4; ++i) { if ((be_mask >> i) & 0x1u) byte_mask32 |= (0xFFu << (8 * i)); }
    switch (index) {
      case 0: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 1: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 2: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }

        // Writing INTR_TEST should set corresponding bits in INTR_STATE (SV behavior)
        regs[0] |= (wdata & 1 & byte_mask32);
        regs[index] = newv;
        break;
      }
      case 3: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 4: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 5: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 6: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;
        {
          auto &phase = shadow_phase_default_[index];
          auto &stage = shadow_stage_default_[index];
          uint32_t wmasked = (wdata & byte_mask32);
          if (phase == 0) { stage = wmasked; phase = 1; break; }
          if (wmasked != stage) { phase = 0; break; }
          // commit on match; fallthrough to apply transformations using staged raw data
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 4);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 12);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
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
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 8: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }
        regs[index] = newv;
        break;
      }
      case 9: {

        {
          uint32_t gw = regs[8];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;
        {
          auto &phase = shadow_phase_default_[index];
          auto &stage = shadow_stage_default_[index];
          uint32_t wmasked = (wdata & byte_mask32);
          if (phase == 0) { stage = wmasked; phase = 1; break; }
          if (wmasked != stage) { phase = 0; break; }
          // commit on match; fallthrough to apply transformations using staged raw data
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 10: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }
        regs[index] = newv;
        break;
      }
      case 11: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 12: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 13: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 14: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 15: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 16: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 17: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 18: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 19: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 20: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 21: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 22: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 23: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 24: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 25: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 26: {

        {
          uint32_t gw = regs[10];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 27: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 28: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 29: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 30: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 31: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 32: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 33: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 34: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 35: {

        {
          uint32_t gw = regs[4];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 36: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }
        regs[index] = newv;
        break;
      }
      case 37: {

        {
          uint32_t gw = regs[36];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;
        {
          auto &phase = shadow_phase_default_[index];
          auto &stage = shadow_stage_default_[index];
          uint32_t wmasked = (wdata & byte_mask32);
          if (phase == 0) { stage = wmasked; phase = 1; break; }
          if (wmasked != stage) { phase = 0; break; }
          // commit on match; fallthrough to apply transformations using staged raw data
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 38: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }
        regs[index] = newv;
        break;
      }
      case 39: {

        {
          uint32_t gw = regs[38];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;
        {
          auto &phase = shadow_phase_default_[index];
          auto &stage = shadow_stage_default_[index];
          uint32_t wmasked = (wdata & byte_mask32);
          if (phase == 0) { stage = wmasked; phase = 1; break; }
          if (wmasked != stage) { phase = 0; break; }
          // commit on match; fallthrough to apply transformations using staged raw data
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 40: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }
        regs[index] = newv;
        break;
      }
      case 41: {

        {
          uint32_t gw = regs[40];
          uint32_t gate = (gw >> 0) & ((2u) - 1u);
          allow = (gate == 1u);
        }
        if (!allow) break;
        {
          auto &phase = shadow_phase_default_[index];
          auto &stage = shadow_stage_default_[index];
          uint32_t wmasked = (wdata & byte_mask32);
          if (phase == 0) { stage = wmasked; phase = 1; break; }
          if (wmasked != stage) { phase = 0; break; }
          // commit on match; fallthrough to apply transformations using staged raw data
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = (newv & ~mask) | raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 42: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 43: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 44: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 45: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 46: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 47: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 48: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 49: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 50: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 51: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 52: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 53: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 54: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 55: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 56: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 57: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // no effect on write; clears on read
        }
        regs[index] = newv;
        break;
      }
      case 58: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 59: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 60: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw_clamped;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw_clamped;
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~raw_clamped;
        }
        regs[index] = newv;
        break;
      }
      case 61: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 4);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 5);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 6);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 8);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 9);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 10);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 11);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 12);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 13);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          // ignore writes
        }
        regs[index] = newv;
        break;
      }
      case 62: {
        if (!allow) break;

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 3);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 4);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 5);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 6);
          // Apply byte-enable and clamp to field width; prevent upper-bit artifacts
          const uint32_t raw = (wdata & byte_mask32) & mask;
          const uint32_t raw_clamped = raw & mask;
          const uint32_t cur = newv & mask;
          newv = newv & ~(~raw_clamped & mask);
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

uint32_t keymgr_reg_top::read_sw(uint32_t addr) {
  const uint32_t index = addr / kWordBytes;
  if (addr < 252) {
    auto &regs = regs_default_;
    uint32_t rdata = regs[index];
    switch (index) {
      case 0: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
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
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 4);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 7);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 12);
        }
        // reading resets shadow phase for non-RO types
        shadow_phase_default_[index] = 0;
        break;
      }
      case 7: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 0);
        }
        break;
      }
      case 8: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 9: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(65535) << 0);
        }
        // reading resets shadow phase for non-RO types
        shadow_phase_default_[index] = 0;
        break;
      }
      case 10: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
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
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 37: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        // reading resets shadow phase for non-RO types
        shadow_phase_default_[index] = 0;
        break;
      }
      case 38: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 39: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        // reading resets shadow phase for non-RO types
        shadow_phase_default_[index] = 0;
        break;
      }
      case 40: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }
        break;
      }
      case 41: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
        }
        // reading resets shadow phase for non-RO types
        shadow_phase_default_[index] = 0;
        break;
      }
      case 42: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 43: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 44: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 45: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 46: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 47: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 48: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 49: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 50: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 51: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 52: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 53: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 54: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 55: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 56: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 57: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(4294967295) << 0);
          // clear-on-read
          regs[index] &= ~mask;
        }
        break;
      }
      case 58: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(7) << 0);
        }
        break;
      }
      case 59: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(3) << 0);
        }
        break;
      }
      case 60: {

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 0);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 1);
        }

        {
          constexpr uint32_t mask = (static_cast<uint32_t>(1) << 2);
        }
        break;
      }
      case 61: {

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
        break;
      }
      case 62: {

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

void keymgr_reg_top::set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write) {
  if (reg_index < racl_allow_rd_default_.size()) {
    racl_allow_rd_default_[reg_index] = allow_read;
    racl_allow_wr_default_[reg_index] = allow_write;
    return;
  }
}

}  // namespace keymgr
}  // namespace opentitan



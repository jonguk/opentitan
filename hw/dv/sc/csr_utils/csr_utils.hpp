#pragma once
#include <cstdint>
#include <functional>
#include <sstream>
#include <uvm>
#include "../tl_agent/tl_bind.hpp"

// Generic, reusable CSR utilities for SystemC DV.
// Mirrors common SV DV csr_utils helpers (read, write, set/clear bits, poll).

namespace scdv {
inline bool csr_log_enabled = true;

// Generic bus concept: must provide read32(addr)->uint32_t and write32(addr, data)
template <typename Bus>
inline void csr_wr(Bus &bus, uint32_t addr, uint32_t data) {
  bus.write32(addr, data);
}

template <typename Bus>
inline uint32_t csr_rd(Bus &bus, uint32_t addr) {
  return bus.read32(addr);
}

template <typename Bus>
inline void csr_set_bits(Bus &bus, uint32_t addr, uint32_t set_mask) {
  uint32_t v = bus.read32(addr);
  v |= set_mask;
  bus.write32(addr, v);
}

template <typename Bus>
inline void csr_clr_bits(Bus &bus, uint32_t addr, uint32_t clr_mask) {
  uint32_t v = bus.read32(addr);
  v &= ~clr_mask;
  bus.write32(addr, v);
}

template <typename Bus>
inline void csr_update_field(Bus &bus, uint32_t addr, uint32_t field_mask, unsigned lsb, uint32_t value) {
  uint32_t v = bus.read32(addr);
  v = (v & ~field_mask) | ((value << lsb) & field_mask);
  bus.write32(addr, v);
}

template <typename Bus>
inline bool csr_poll(Bus &bus, uint32_t addr, uint32_t mask, uint32_t expected, unsigned max_iters = 1000) {
  for (unsigned i = 0; i < max_iters; ++i) {
    if ((bus.read32(addr) & mask) == (expected & mask)) return true;
  }
  return false;
}

// Global TLM-based helpers (no explicit Bus) using tl_bind hook
inline void csr_wr(uint32_t addr, uint32_t data) {
  if (!tl_bind::b_transport) return;
  tlm::tlm_generic_payload t; sc_core::sc_time d = sc_core::SC_ZERO_TIME;
  unsigned char buf[4]; std::memcpy(buf, &data, 4);
  t.set_command(tlm::TLM_WRITE_COMMAND); t.set_address(addr);
  t.set_data_length(4); t.set_streaming_width(4); t.set_byte_enable_ptr(nullptr);
  t.set_data_ptr(buf); tl_bind::b_transport(t, d);
  if (t.get_response_status() == tlm::TLM_OK_RESPONSE) {
    for (auto &fn : tl_bind::observers) { fn(addr, data, true); }
    uvm::uvm_report_info("CSR_UTIL", (std::string("WR notify @0x") + std::to_string(addr) + " =0x" + std::to_string(data)).c_str(), uvm::UVM_LOW);
  }
  if (csr_log_enabled) {
    std::ostringstream oss; oss << "WR @0x" << std::hex << addr << " = 0x" << data;
    uvm::uvm_report_info("CSR_WR", oss.str(), uvm::UVM_MEDIUM);
  }
}

inline uint32_t csr_rd(uint32_t addr) {
  if (!tl_bind::b_transport) return 0;
  tlm::tlm_generic_payload t; sc_core::sc_time d = sc_core::SC_ZERO_TIME;
  unsigned char buf[4]{};
  t.set_command(tlm::TLM_READ_COMMAND); t.set_address(addr);
  t.set_data_length(4); t.set_streaming_width(4); t.set_byte_enable_ptr(nullptr);
  t.set_data_ptr(buf); tl_bind::b_transport(t, d);
  uint32_t v=0; std::memcpy(&v, buf, 4);
  if (t.get_response_status() == tlm::TLM_OK_RESPONSE) {
    for (auto &fn : tl_bind::observers) { fn(addr, v, false); }
    uvm::uvm_report_info("CSR_UTIL", (std::string("RD notify @0x") + std::to_string(addr) + " ->0x" + std::to_string(v)).c_str(), uvm::UVM_LOW);
  }
  if (csr_log_enabled) {
    std::ostringstream oss; oss << "RD @0x" << std::hex << addr << " -> 0x" << v;
    uvm::uvm_report_info("CSR_RD", oss.str(), uvm::UVM_MEDIUM);
  }
  return v;
}

inline void csr_rd_check(uint32_t addr, uint32_t mask, uint32_t expected, const char *msg = "") {
  uint32_t obs = csr_rd(addr) & mask;
  uint32_t exp = expected & mask;
  if (obs != exp) {
    std::ostringstream oss; oss << msg << " obs=0x" << std::hex << obs << " exp=0x" << exp;
    uvm::uvm_report_error("CSR_RD_CHECK", oss.str(), uvm::UVM_NONE);
  }
}

inline bool csr_spinwait(uint32_t addr, uint32_t mask, uint32_t expected,
                         unsigned max_iters = 10000, sc_core::sc_time delay = sc_core::SC_ZERO_TIME) {
  for (unsigned i = 0; i < max_iters; ++i) {
    if ((csr_rd(addr) & mask) == (expected & mask)) return true;
    if (delay != sc_core::SC_ZERO_TIME) sc_core::wait(delay);
  }
  std::ostringstream oss; oss << "spinwait timeout @0x" << std::hex << addr
                               << " mask=0x" << mask << " exp=0x" << expected;
  uvm::uvm_report_error("CSR_SPINWAIT", oss.str(), uvm::UVM_NONE);
  return false;
}

// SV dv_utils get_masked_data equivalent: merge write data with mirrored CSR bytes
// When mask bit is 0, take byte from csr_mirrored; when 1, take from wdata.
inline uint32_t get_masked_data(uint32_t wdata, uint32_t mask, uint32_t csr_mirrored) {
  uint32_t merged = wdata;
  for (unsigned i = 0; i < 4; ++i) {
    if (((mask >> i) & 0x1u) == 0u) {
      // clear target byte and insert from mirrored
      merged &= ~(0xFFu << (i * 8));
      merged |= ((csr_mirrored >> (i * 8)) & 0xFFu) << (i * 8);
    }
  }
  return merged;
}

}  // namespace scdv



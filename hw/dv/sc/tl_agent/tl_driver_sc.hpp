#pragma once
#include <systemc>
#include <tlm>
#include <uvm>
#include "tl_bind.hpp"

struct tl_driver_sc : public sc_core::sc_module {
  SC_HAS_PROCESS(tl_driver_sc);
  explicit tl_driver_sc(sc_core::sc_module_name name) : sc_core::sc_module(name) {}

  static bool is_power_of_two(unsigned x){ return x && !(x & (x-1)); }

  void send_write(uint64_t addr, uint32_t data, unsigned size_bytes = 4) {
    if (!tl_bind::b_transport) return;
    if (!is_power_of_two(size_bytes) || (addr % size_bytes) != 0) {
      uvm::uvm_report_error("TL_ALIGN", "Unaligned or invalid transfer size", uvm::UVM_NONE);
      return;
    }
    tlm::tlm_generic_payload trans; sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    trans.set_address(addr); trans.set_data_length(size_bytes); trans.set_streaming_width(size_bytes);
    trans.set_byte_enable_ptr(nullptr); trans.set_dmi_allowed(false);
    unsigned char buf[8]{}; std::memcpy(buf, &data, std::min<unsigned>(size_bytes, sizeof(uint32_t)));
    trans.set_command(tlm::TLM_WRITE_COMMAND); trans.set_data_ptr(buf);
    tl_bind::b_transport(trans, delay);
    if (trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
      uvm::uvm_report_error("TL_RESP", "TLM error response from DUT", uvm::UVM_NONE);
      return;
    }
    for (auto &fn : tl_bind::observers) { fn(static_cast<uint32_t>(addr), data, true); }
  }

  uint32_t send_read(uint64_t addr, unsigned size_bytes = 4) {
    uint32_t data_val = 0;
    if (!tl_bind::b_transport) return data_val;
    if (!is_power_of_two(size_bytes) || (addr % size_bytes) != 0) {
      uvm::uvm_report_error("TL_ALIGN", "Unaligned or invalid transfer size", uvm::UVM_NONE);
      return data_val;
    }
    tlm::tlm_generic_payload trans; sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    trans.set_address(addr); trans.set_data_length(size_bytes); trans.set_streaming_width(size_bytes);
    trans.set_byte_enable_ptr(nullptr); trans.set_dmi_allowed(false);
    unsigned char buf[8]{}; trans.set_command(tlm::TLM_READ_COMMAND); trans.set_data_ptr(buf);
    tl_bind::b_transport(trans, delay);
    if (trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
      uvm::uvm_report_error("TL_RESP", "TLM error response from DUT", uvm::UVM_NONE);
      return data_val;
    }
    std::memcpy(&data_val, buf, std::min<unsigned>(size_bytes, sizeof(uint32_t)));
    for (auto &fn : tl_bind::observers) { fn(static_cast<uint32_t>(addr), data_val, false); }
    return data_val;
  }
};



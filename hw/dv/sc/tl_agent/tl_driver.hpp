#pragma once
#include <uvm>
#include <tlm>
#include "tl_item.hpp"
#include "tl_bind.hpp"

class tl_driver : public uvm::uvm_driver<tl_item> {
 public:
  UVM_COMPONENT_UTILS(tl_driver);

  explicit tl_driver(uvm::uvm_component_name name) : uvm::uvm_driver<tl_item>(name) {}

  void run_phase(uvm::uvm_phase &phase) override {
    while (true) {
      tl_item *req {nullptr};
      seq_item_port->get_next_item(req);
      if (req) do_b_transport(*req);
      seq_item_port->item_done();
    }
  }

  static bool is_power_of_two(unsigned x){ return x && !(x & (x-1)); }

  void do_b_transport(const tl_item &req) {
    if (!tl_bind::b_transport) return;
    // Size/alignment checks
    if (!is_power_of_two(req.size_bytes) || (req.addr % req.size_bytes) != 0) {
      uvm::uvm_report_error("TL_ALIGN", "Unaligned or invalid transfer size", uvm::UVM_NONE);
      return;
    }
    tlm::tlm_generic_payload trans;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    trans.set_address(req.addr);
    trans.set_data_length(req.size_bytes);
    trans.set_streaming_width(req.size_bytes);
    trans.set_byte_enable_ptr(nullptr);
    trans.set_dmi_allowed(false);
    unsigned char buf[8]{};
    if (req.is_write) {
      std::memcpy(buf, &req.data, std::min<unsigned>(req.size_bytes, sizeof(uint32_t)));
      trans.set_command(tlm::TLM_WRITE_COMMAND);
      trans.set_data_ptr(buf);
    } else {
      trans.set_command(tlm::TLM_READ_COMMAND);
      trans.set_data_ptr(buf);
    }
    tl_bind::b_transport(trans, delay);
    if (trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
      uvm::uvm_report_error("TL_RESP", "TLM error response from DUT", uvm::UVM_NONE);
      return;
    }
    uint32_t data_val = 0;
    if (!req.is_write) std::memcpy(&data_val, buf, std::min<unsigned>(req.size_bytes, sizeof(uint32_t)));
    else data_val = req.data;
    for (auto &fn : tl_bind::observers) { fn(static_cast<uint32_t>(req.addr), data_val, req.is_write); }
  }
};

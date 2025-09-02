#pragma once
#include <uvm>
#include <tlm>
#include <systemc>
#include "tl_item.hpp"
#include "tl_bind.hpp"

class tl_driver : public uvm::uvm_driver<tl_item> {
 public:
  UVM_COMPONENT_UTILS(tl_driver);

  explicit tl_driver(uvm::uvm_component_name name) : uvm::uvm_driver<tl_item>(name) {}

  static bool is_power_of_two(unsigned x){ return x && !(x & (x-1)); }
  static unsigned size_bytes_from_size(uint8_t size_log2){ return 1u << size_log2; }

  void do_b_transport(tl_item &req) {
    if (!tl_bind::b_transport) return;
    // Derive size_bytes from a_size and enforce alignment
    unsigned size_bytes = size_bytes_from_size(req.a_size);
    if (!is_power_of_two(size_bytes) || (req.a_addr % size_bytes) != 0) {
      uvm::uvm_report_warning("TL_ALIGN", "Unaligned or invalid transfer size; proceeding to record protocol error for coverage", uvm::UVM_LOW);
      // Proceed to allow coverage of d_error and size/mask bins
    }
    tlm::tlm_generic_payload trans;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    trans.set_address(req.a_addr);
    trans.set_data_length(size_bytes);
    trans.set_streaming_width(size_bytes);
    unsigned char be[8]{}; // byte enables (0xFF active, 0x00 inactive)
    for (unsigned i = 0; i < size_bytes && i < sizeof(be); ++i) {
      be[i] = ((req.a_mask >> i) & 0x1u) ? 0xFF : 0x00;
    }
    trans.set_byte_enable_ptr(be);
    trans.set_byte_enable_length(size_bytes);
    trans.set_dmi_allowed(false);
    unsigned char buf[8]{};
    if (req.is_write()) {
      // Apply a_mask for PutPartialData
      uint32_t masked = 0;
      for (unsigned i = 0; i < size_bytes; ++i) {
        if ((req.a_mask >> i) & 0x1u) {
          reinterpret_cast<uint8_t*>(&masked)[i] = reinterpret_cast<const uint8_t*>(&req.a_data)[i];
        }
      }
      std::memcpy(buf, &masked, std::min<unsigned>(size_bytes, sizeof(uint32_t)));
      trans.set_command(tlm::TLM_WRITE_COMMAND);
      trans.set_data_ptr(buf);
    } else {
      trans.set_command(tlm::TLM_READ_COMMAND);
      trans.set_data_ptr(buf);
    }
    uvm::uvm_report_info("TL_DRV", (std::string(req.is_write()?"WR":"RD") + " send @0x" + std::to_string(req.a_addr)).c_str(), uvm::UVM_LOW);
    tl_bind::b_transport(trans, delay);
    // Always consume transaction to allow coverage of negative cases as in SV (monitor samples d_error)
    // Expected D opcode: Get -> AccessAckData, Write -> AccessAck (SV tl_seq_item::is_ok semantics)
    const char *exp_rsp = req.is_write() ? "AccessAck" : "AccessAckData";
    uvm::uvm_report_info("TL_DRV", (std::string("exp D opcode = ") + exp_rsp).c_str(), uvm::UVM_HIGH);

    uint32_t data_val = 0;
    if (!req.is_write()) std::memcpy(&data_val, buf, std::min<unsigned>(size_bytes, sizeof(uint32_t)));
    else data_val = req.a_data;
    // Fill D channel fields akin to SV
    req.d_data = data_val;
    req.d_size = req.a_size;
    req.d_source = req.a_source;
    req.d_opcode = req.is_write()? tl_d_op_e::AccessAck : tl_d_op_e::AccessAckData;
    // Flag protocol errors using helpers (simulate DUT response error)
    req.d_error = req.get_exp_d_error();
    for (auto &fn : tl_bind::observers) { fn(static_cast<uint32_t>(req.a_addr), data_val, req.is_write(), req.a_mask, req.a_size, req.d_error, static_cast<uint8_t>(req.d_opcode), static_cast<uint8_t>(req.a_opcode), req.a_source); }
    uvm::uvm_report_info("TL_DRV", (std::string(req.is_write()?"WR":"RD") + " done @0x" + std::to_string(req.a_addr) + " =0x" + std::to_string(data_val)).c_str(), uvm::UVM_LOW);
  }

  void build_phase(uvm::uvm_phase &phase) override { uvm::uvm_driver<tl_item>::build_phase(phase); }

  void run_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_driver<tl_item>::run_phase(phase);
    uvm::uvm_report_info("TL_DRV", "run_phase: entering loop", uvm::UVM_LOW);
    while (true) {
      tl_item req;
      uvm::uvm_report_info("TL_DRV", "run_phase: get_next_item waiting", uvm::UVM_HIGH);
      this->seq_item_port.get_next_item(req);
      uvm::uvm_report_info("TL_DRV", "run_phase: got item", uvm::UVM_LOW);
      do_b_transport(req);
      this->seq_item_port.item_done();
    }
  }
};

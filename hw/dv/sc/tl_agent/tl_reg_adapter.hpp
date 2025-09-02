#pragma once
#include <uvm>
#include <cstdint>
#include "../scoreboard/scoreboard.hpp" // for csr_event
#include "../bus_params_pkg/bus_params.hpp"

// Minimal SC version of uvm_reg_adapter: converts csr_event -> reg_bus_op
struct reg_bus_op_sc {
  uint32_t address {0};
  uint32_t data {0};
  bool     is_write {false};
  uint32_t byte_en_mask {0xFFFFFFFFu};
  uint8_t  size_log2 {2};
  uint8_t  a_opcode {static_cast<uint8_t>(tlul::Get)};
  uint8_t  d_opcode {static_cast<uint8_t>(tlul::AccessAck)};
  bool     d_error {false};
  uint8_t  a_source {0};
  uint8_t  d_source {0};
};

class tl_reg_adapter_sc : public uvm::uvm_component {
 public:
  UVM_COMPONENT_UTILS(tl_reg_adapter_sc);
  uvm::uvm_analysis_imp<csr_event, tl_reg_adapter_sc> imp;
  uvm::uvm_analysis_port<reg_bus_op_sc> ap {"ap"};

  explicit tl_reg_adapter_sc(uvm::uvm_component_name nm)
      : uvm::uvm_component(nm), imp("imp", this) {}

  void write(const csr_event &ev) {
    reg_bus_op_sc op;
    op.address = ev.addr;
    op.data = ev.data;
    op.is_write = ev.is_write;
    op.byte_en_mask = ev.mask;
    op.size_log2 = ev.size_log2;
    op.d_error = ev.d_error;
    // Populate opcode/size to mirror SV bus2reg/reg2bus semantics
    op.a_opcode = ev.is_write ? static_cast<uint8_t>(tlul::PutFullData) : static_cast<uint8_t>(tlul::Get);
    op.d_opcode = ev.is_write ? static_cast<uint8_t>(tlul::AccessAck) : static_cast<uint8_t>(tlul::AccessAckData);
    // carry source from event if available
    op.a_source = ev.a_source;
    op.d_source = ev.d_source;
    ap.write(op);
  }
};



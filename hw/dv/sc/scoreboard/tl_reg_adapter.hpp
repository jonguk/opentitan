#pragma once
#include <uvm>
#include <cstdint>
#include "scoreboard.hpp" // for csr_event

// Minimal SC version of uvm_reg_adapter: converts csr_event -> reg_bus_op
struct reg_bus_op_sc {
  uint32_t address {0};
  uint32_t data {0};
  bool     is_write {false};
  uint32_t byte_en_mask {0xFFFFFFFFu};
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
    ap.write(op);
  }
};



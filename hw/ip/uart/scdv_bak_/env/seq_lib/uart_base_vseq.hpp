#pragma once
#include <uvm>
#include "../uart_virtual_sequencer.hpp"

class uart_base_vseq : public uvm::uvm_sequence<> {
 public:
  UVM_OBJECT_UTILS(uart_base_vseq);

  explicit uart_base_vseq(const std::string &name = "uart_base_vseq")
      : uvm::uvm_sequence<>(name) {}

 protected:
  uart_virtual_sequencer* get_vseqr() const {
    return dynamic_cast<uart_virtual_sequencer*>(this->get_sequencer());
  }
};



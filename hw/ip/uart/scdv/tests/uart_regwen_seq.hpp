#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class uart_regwen_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_regwen_seq);
  explicit uart_regwen_seq(const std::string &nm = "uart_regwen_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // No REGWEN detected; skip
  }
};

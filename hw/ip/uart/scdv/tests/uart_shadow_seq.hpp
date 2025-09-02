#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class uart_shadow_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_shadow_seq);
  explicit uart_shadow_seq(const std::string &nm = "uart_shadow_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // No shadowed registers detected; skip
  }
};

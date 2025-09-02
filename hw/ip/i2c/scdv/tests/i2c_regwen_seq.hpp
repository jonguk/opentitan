#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class i2c_regwen_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(i2c_regwen_seq);
  explicit i2c_regwen_seq(const std::string &nm = "i2c_regwen_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // No REGWEN detected; skip
  }
};

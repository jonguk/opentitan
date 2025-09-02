#pragma once
#include <uvm>
#include "cip_base_env.hpp"

class cip_base_test : public uvm::uvm_test {
 public:
  UVM_COMPONENT_UTILS(cip_base_test);
  cip_base_env* m_env {};
  explicit cip_base_test(uvm::uvm_component_name nm) : uvm::uvm_test(nm) {}
  void build_phase(uvm::uvm_phase &phase) override {
    m_env = cip_base_env::type_id::create("env", this);
  }
};



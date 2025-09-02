#pragma once
#include <uvm>
#include "../../../dv/sc/cip_lib/cip_base_env.hpp"

class entropy_src_env : public cip_base_env {
 public:
  UVM_COMPONENT_UTILS(entropy_src_env);
  explicit entropy_src_env(uvm::uvm_component_name name) : cip_base_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    cip_base_env::build_phase(phase);
  }
};

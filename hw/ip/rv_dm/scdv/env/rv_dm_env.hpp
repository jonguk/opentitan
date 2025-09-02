#pragma once
#include <uvm>
#include "../../../dv/sc/cip_lib/cip_base_env.hpp"

class rv_dm_env : public cip_base_env {
 public:
  UVM_COMPONENT_UTILS(rv_dm_env);
  explicit rv_dm_env(uvm::uvm_component_name name) : cip_base_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    cip_base_env::build_phase(phase);
  }
};

#pragma once
#include <uvm>
#include "../../../dv/sc/cip_lib/cip_base_env.hpp"

class rom_ctrl_env : public cip_base_env {
 public:
  UVM_COMPONENT_UTILS(rom_ctrl_env);
  explicit rom_ctrl_env(uvm::uvm_component_name name) : cip_base_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    cip_base_env::build_phase(phase);
  }
};

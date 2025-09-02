#pragma once
#include <uvm>

class cip_base_env_cfg : public uvm::uvm_object {
 public:
  UVM_OBJECT_UTILS(cip_base_env_cfg);
  explicit cip_base_env_cfg(const std::string &nm = "cip_base_env_cfg") : uvm::uvm_object(nm) {}
};



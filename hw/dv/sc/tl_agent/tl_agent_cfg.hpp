#pragma once
#include <uvm>

class tl_agent_cfg : public uvm::uvm_object {
 public:
  UVM_OBJECT_UTILS(tl_agent_cfg);
  bool is_host {true};
  explicit tl_agent_cfg(const std::string &name = "tl_agent_cfg") : uvm::uvm_object(name) {}
};



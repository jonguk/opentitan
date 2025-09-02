#pragma once
#include <uvm>
#include "../tl_agent/tl_sequencer.hpp"

class cip_base_virtual_sequencer : public uvm::uvm_component {
 public:
  UVM_COMPONENT_UTILS(cip_base_virtual_sequencer);
  tl_sequencer *tl_sequencer_h {nullptr};

  explicit cip_base_virtual_sequencer(uvm::uvm_component_name nm) : uvm::uvm_component(nm) {}
};



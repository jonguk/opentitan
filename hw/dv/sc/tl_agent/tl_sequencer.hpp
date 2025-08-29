#pragma once
#include <uvm>
#include "tl_item.hpp"

class tl_sequencer : public uvm::uvm_sequencer<tl_item> {
 public:
  UVM_COMPONENT_UTILS(tl_sequencer);
  explicit tl_sequencer(uvm::uvm_component_name name) : uvm::uvm_sequencer<tl_item>(name) {}
};

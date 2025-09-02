#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"
#include "../../../dv/sc/cip_lib/cip_base_virtual_sequencer.hpp"
class uart_virtual_sequencer : public uvm::uvm_component {
 public:
  UVM_COMPONENT_UTILS(uart_virtual_sequencer);
  tl_sequencer *tl_sequencer_h {nullptr};

  explicit uart_virtual_sequencer(uvm::uvm_component_name nm) : uvm::uvm_component(nm) {}
};

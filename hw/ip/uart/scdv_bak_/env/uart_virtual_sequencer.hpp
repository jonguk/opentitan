#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"

// CIP-style virtual sequencer for UART SC DV
class uart_virtual_sequencer : public uvm::uvm_sequencer<> {
 public:
  UVM_COMPONENT_UTILS(uart_virtual_sequencer);

  // Handle to TL-UL sequencer (shared bus agent)
  tl_sequencer *tl_sequencer_h {nullptr};

  explicit uart_virtual_sequencer(uvm::uvm_component_name name)
      : uvm::uvm_sequencer<>(name) {}
};



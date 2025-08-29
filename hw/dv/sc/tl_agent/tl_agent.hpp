#pragma once
#include <uvm>
#include "tl_item.hpp"
#include "tl_sequencer.hpp"
#include "tl_driver.hpp"
#include "tl_monitor.hpp"

class tl_agent : public uvm::uvm_agent {
 public:
  UVM_COMPONENT_UTILS(tl_agent);
  tl_sequencer *m_sequencer {nullptr};
  tl_driver    *m_driver {nullptr};
  tl_monitor   *m_monitor {nullptr};

  explicit tl_agent(uvm::uvm_component_name name) : uvm::uvm_agent(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_agent::build_phase(phase);
    m_sequencer = tl_sequencer::type_id::create("sequencer", this);
    m_driver    = tl_driver::type_id::create("driver", this);
    m_monitor   = tl_monitor::type_id::create("monitor", this);
  }

  void connect_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_agent::connect_phase(phase);
    m_driver->seq_item_port.connect(m_sequencer->seq_item_export);
  }
};

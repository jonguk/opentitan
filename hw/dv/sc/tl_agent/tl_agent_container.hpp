#pragma once
#include <systemc>
#include <uvm>
#include "tl_driver.hpp"
#include "tl_sequencer.hpp"

// Container sc_module to construct sequencer/driver within module context
struct tl_agent_container : sc_core::sc_module {
  tl_sequencer *seqr  {nullptr};
  tl_driver    *drv   {nullptr};
  uvm::uvm_component *parent {nullptr};
  SC_HAS_PROCESS(tl_agent_container);
  tl_agent_container(sc_core::sc_module_name nm, uvm::uvm_component *p = nullptr)
      : sc_core::sc_module(nm), parent(p) {
    seqr = tl_sequencer::type_id::create("sqr", parent);
    drv  = tl_driver::type_id::create("drv", parent);
  }
};



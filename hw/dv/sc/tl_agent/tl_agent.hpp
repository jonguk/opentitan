#pragma once
#include <uvm>
#include "tl_sequencer.hpp"
#include "tl_driver.hpp"
#include "tl_device_driver.hpp"
#include "tl_monitor.hpp"
#include "tl_agent_cfg.hpp"
#include "tl_agent_container.hpp"

class tl_agent : public uvm::uvm_agent {
 public:
  UVM_COMPONENT_UTILS(tl_agent);
  tl_sequencer *seqr {nullptr};
  tl_driver *drv {nullptr};
  tl_device_driver *dev_drv {nullptr};
  tl_monitor *mon {nullptr};
  tl_agent_container *container {nullptr};

  explicit tl_agent(uvm::uvm_component_name name) : uvm::uvm_agent(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_agent::build_phase(phase);
    // Standard UVM composition under agent
    seqr = tl_sequencer::type_id::create("sqr", this);
    drv  = tl_driver::type_id::create("drv", this);
    mon  = tl_monitor::type_id::create("mon", this);
    tl_agent_cfg *cfg = nullptr;
    if (uvm::uvm_config_db<tl_agent_cfg*>::get(this, "", "cfg", cfg) && cfg && !cfg->is_host) {
      dev_drv = tl_device_driver::type_id::create("dev_drv", this);
    }
  }

  void connect_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_agent::connect_phase(phase);
    if (drv && seqr) drv->seq_item_port.connect(seqr->seq_item_export);
  }
};

#pragma once
#include <uvm>
#include "../tl_agent/tl_monitor.hpp"
#include "../tl_agent/tl_sequencer.hpp"
#include "../tl_agent/tl_driver.hpp"
#include "../scoreboard/scoreboard.hpp"
#include "../tl_agent/tl_reg_adapter.hpp"
#include "../csr_utils/uvm_reg_predictor.hpp"
#include "cip_base_virtual_sequencer.hpp"

// Minimal CIP-like base env for SystemC
class cip_base_env : public uvm::uvm_env {
 public:
  UVM_COMPONENT_UTILS(cip_base_env);
  simple_scoreboard *scb {nullptr};
  tl_monitor *mon {nullptr};
  tl_sequencer *seqr {nullptr};
  tl_driver *drv {nullptr};
  tl_reg_adapter_sc *reg_adapter {nullptr};
  uvm_reg_predictor *reg_predictor {nullptr};
  cip_base_virtual_sequencer *vsqr {nullptr};

  explicit cip_base_env(uvm::uvm_component_name name) : uvm::uvm_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_env::build_phase(phase);
    scb  = simple_scoreboard::type_id::create("scb", this);
    mon  = tl_monitor::type_id::create("mon", this);
    seqr = tl_sequencer::type_id::create("seqr", this);
    drv  = tl_driver::type_id::create("drv", this);
    reg_adapter   = tl_reg_adapter_sc::type_id::create("reg_adapter", this);
    reg_predictor = uvm_reg_predictor::type_id::create("reg_predictor", this);
    vsqr = cip_base_virtual_sequencer::type_id::create("vsqr", this);
  }

  void connect_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_env::connect_phase(phase);
    if (mon && scb) mon->connect_scoreboard(scb);
    if (mon && scb) mon->ap.connect(scb->item_collected_export);
    if (mon && reg_adapter) mon->ap.connect(reg_adapter->imp);
    if (reg_adapter && reg_predictor) reg_adapter->ap.connect(reg_predictor->imp);
    if (drv && seqr) drv->seq_item_port.connect(seqr->seq_item_export);
    if (seqr) uvm::uvm_config_db<tl_sequencer*>::set(nullptr, "*", "tl_sequencer", seqr);
    if (vsqr) vsqr->tl_sequencer_h = seqr;
    if (vsqr) uvm::uvm_config_db<cip_base_virtual_sequencer*>::set(nullptr, "*", "virtual_sequencer", vsqr);
  }
};



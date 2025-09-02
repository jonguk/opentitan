
#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_agent.hpp"
#include "../../../dv/sc/scoreboard/scoreboard.hpp"
#include "../../../dv/sc/csr_utils/uvm_reg_predictor.hpp"
#include "../../../dv/sc/tl_agent/tl_reg_adapter.hpp"
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include "../../../dv/sc/coverage/uart_cov.hpp"
#endif
#include "uart_virtual_sequencer.hpp"
#include "uart_scoreboard.hpp"
#include "../../../dv/sc/tl_agent/tl_agent_cfg.hpp"

class uart_env : public uvm::uvm_env {
 public:
  UVM_COMPONENT_UTILS(uart_env);
  uart_scoreboard *scb {nullptr};
  tl_agent *agent {nullptr};
  uvm_reg_predictor *pred {nullptr};
  uart_virtual_sequencer *vseqr {nullptr};

  explicit uart_env(uvm::uvm_component_name name) : uvm::uvm_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_env::build_phase(phase);
    scb  = uart_scoreboard::type_id::create("scb", this);
    agent = tl_agent::type_id::create("agent", this);
    pred = uvm_reg_predictor::type_id::create("pred", this);
    adapter_ = tl_reg_adapter_sc::type_id::create("adapter", this);
    vseqr = uart_virtual_sequencer::type_id::create("vseqr", this);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    if (agent && agent->mon) agent->mon->set_coverage(&UartCov::get());
#endif
    // Configure agent as host-mode by default (SV tl_agent_cfg parity)
    auto *cfg = tl_agent_cfg::type_id::create("cfg");
    cfg->is_host = true;
    uvm::uvm_config_db<tl_agent_cfg*>::set(this, "agent", "cfg", cfg);
  }

  void connect_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_env::connect_phase(phase);
    // SV-like direct wiring: monitor.ap -> adapter -> predictor, and monitor.ap -> scoreboard
    if (agent && agent->mon && pred && adapter_) {
      agent->mon->ap.connect(adapter_->imp);
      adapter_->ap.connect(pred->imp);
    }
    if (agent && agent->mon && scb) {
      agent->mon->ap.connect(scb->item_collected_export);
    }
    if (vseqr && agent && agent->seqr) vseqr->tl_sequencer_h = agent->seqr;
    // Expose tl_sequencer for SV-like flows
    if (agent && agent->seqr) {
      uvm::uvm_config_db<tl_sequencer*>::set(nullptr, "*", "tl_sequencer", agent->seqr);
    }
  }
 private:
  tl_reg_adapter_sc *adapter_ {nullptr};
};

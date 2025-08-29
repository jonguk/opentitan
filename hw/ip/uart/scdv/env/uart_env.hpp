
#pragma once
#include <uvm>
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include "../../../dv/sc/coverage/uart_cov.hpp"
#include <fc4sc/includes/fc4sc.hpp>
#include <fc4sc/includes/xml_printer.hpp>
#endif
#include "../../../dv/sc/tl_agent/tl_monitor.hpp"
#include "../../../dv/sc/scoreboard/scoreboard.hpp"

class uart_env : public uvm::uvm_env {
 public:
  UVM_COMPONENT_UTILS(uart_env);
  simple_scoreboard *scb {nullptr};
  tl_monitor *mon {nullptr};

  explicit uart_env(uvm::uvm_component_name name) : uvm::uvm_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_env::build_phase(phase);
    scb = simple_scoreboard::type_id::create("scb", this);
    mon = tl_monitor::type_id::create("mon", this);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    (void)UartCov::get();
#endif
  }

  void connect_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_env::connect_phase(phase);
    if (mon && scb) mon->connect_scoreboard(scb);
  }
  void final_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_env::final_phase(phase);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    try {
      xml_printer::coverage_save("/tmp/fc4sc_cov.ucis.xml", fc4sc::global::getter());
      xml_printer::coverage_save("fc4sc_cov.ucis.xml", fc4sc::global::getter());
    } catch (...) {}
#endif
  }
};

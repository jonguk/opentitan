#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "uart_window_seq.hpp"
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"
using namespace uvm;

class uart_window_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_window_test);
  uart_env* m_env {};
  explicit uart_window_test(uvm_component_name nm) : uvm_test(nm) {}
  void build_phase(uvm_phase &phase) override { m_env = uart_env::type_id::create("env", this); }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    auto seq = uart_window_seq::type_id::create("seq");
    tl_sequencer *seqr_ptr {nullptr};
    if (uvm::uvm_config_db<tl_sequencer*>::get(nullptr, "*", "tl_sequencer", seqr_ptr) && seq) {
      uvm::uvm_report_info("TEST", "Starting uart_window_seq on tl_sequencer", uvm::UVM_LOW);
      seq->start(seqr_ptr);
    } else {
      uvm::uvm_report_fatal("TEST/NOSQR", "tl_sequencer not found in config_db", uvm::UVM_NONE);
    }
    phase.drop_objection(this);
  }
};
UVM_COMPONENT_REGISTER(uart_window_test);

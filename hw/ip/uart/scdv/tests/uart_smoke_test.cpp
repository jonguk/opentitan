
#include <uvm>
#include "uart_env.hpp"
#include "uvm_sc_compat.hpp"
#include "../../../dv/sc/scoreboard/scoreboard.hpp"
using namespace uvm;

class uart_smoke_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_smoke_test);
  uart_env* m_env {};
  simple_scoreboard *scb {};
  explicit uart_smoke_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
    scb = simple_scoreboard::type_id::create("scb", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    // Push expected and observe a mismatch to verify error path
    scb->push_expected(0x0, 0x1);
    scb->observe(0x0, 0x2);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_smoke_test);

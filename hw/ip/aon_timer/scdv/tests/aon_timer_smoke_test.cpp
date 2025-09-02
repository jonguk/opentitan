
#include <uvm>
#include "aon_timer_env.hpp"
#include "uvm_sc_compat.hpp"
using namespace uvm;

class aon_timer_smoke_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(aon_timer_smoke_test);
  aon_timer_env* m_env {};
  explicit aon_timer_smoke_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = aon_timer_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(aon_timer_smoke_test);

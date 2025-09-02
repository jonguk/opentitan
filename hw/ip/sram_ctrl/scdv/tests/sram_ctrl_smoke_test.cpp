
#include <uvm>
#include "sram_ctrl_env.hpp"
#include "uvm_sc_compat.hpp"
using namespace uvm;

class sram_ctrl_smoke_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(sram_ctrl_smoke_test);
  sram_ctrl_env* m_env {};
  explicit sram_ctrl_smoke_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = sram_ctrl_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(sram_ctrl_smoke_test);


#include <uvm>
#include "i2c_env.hpp"
#include "uvm_sc_compat.hpp"
using namespace uvm;

class i2c_smoke_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(i2c_smoke_test);
  i2c_env* m_env {};
  explicit i2c_smoke_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = i2c_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(i2c_smoke_test);


#include <uvm>
#include "keymgr_env.hpp"
#include "uvm_sc_compat.hpp"
using namespace uvm;

class keymgr_smoke_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(keymgr_smoke_test);
  keymgr_env* m_env {};
  explicit keymgr_smoke_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = keymgr_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(keymgr_smoke_test);

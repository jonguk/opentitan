#include <uvm>
#include "../env/i2c_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "i2c_csr_addr_sweep_seq.hpp"
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"
using namespace uvm;

class i2c_csr_addr_sweep_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(i2c_csr_addr_sweep_test);
  i2c_env* m_env {};
  explicit i2c_csr_addr_sweep_test(uvm_component_name nm) : uvm_test(nm) {}
  void build_phase(uvm_phase &phase) override { m_env = i2c_env::type_id::create("env", this); }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    auto seq = i2c_csr_addr_sweep_seq::type_id::create("seq");
    tl_sequencer *seqr_ptr {nullptr};
    if (uvm::uvm_config_db<tl_sequencer*>::get(nullptr, "*", "tl_sequencer", seqr_ptr) && seq) { seq->start(seqr_ptr); }
    drain_delta(); phase.drop_objection(this);
  }
};
UVM_COMPONENT_REGISTER(i2c_csr_addr_sweep_test);

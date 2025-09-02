#include <uvm>
#include "../env/hmac_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "hmac_csr_rw_seq.hpp"
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"
using namespace uvm;

class hmac_csr_rw_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(hmac_csr_rw_test);
  hmac_env* m_env {};
  explicit hmac_csr_rw_test(uvm_component_name nm) : uvm_test(nm) {}
  void build_phase(uvm_phase &phase) override { m_env = hmac_env::type_id::create("env", this); }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    if (m_env && m_env->scb) m_env->scb->clear();
    auto seq = hmac_csr_rw_seq::type_id::create("seq");
    tl_sequencer *seqr_ptr {nullptr};
    if (uvm::uvm_config_db<tl_sequencer*>::get(nullptr, "*", "tl_sequencer", seqr_ptr) && seq) {
      seq->start(seqr_ptr);
    } else {
      uvm::uvm_report_fatal("TEST/NOSQR", "tl_sequencer not found in config_db", uvm::UVM_NONE);
    }
    drain_delta();
    // bounded drain: wait until scoreboard settles or max iters
    for (int i=0; i<100 && m_env && m_env->scb && m_env->scb->has_pending(); ++i) drain_delta();
    phase.drop_objection(this);
  }
};
UVM_COMPONENT_REGISTER(hmac_csr_rw_test);

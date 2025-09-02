#include <uvm>
#include "../env/otbn_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "otbn_bit_bash_seq.hpp"
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"
#include "../../../dv/sc/tl_agent/seq_lib/tl_seq_list.hpp"
using namespace uvm;

class otbn_bit_bash_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(otbn_bit_bash_test);
  otbn_env* m_env {};
  explicit otbn_bit_bash_test(uvm_component_name nm) : uvm_test(nm) {}
  void build_phase(uvm_phase &phase) override { m_env = otbn_env::type_id::create("env", this); }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    auto seq = otbn_bit_bash_seq::type_id::create("seq");
    tl_sequencer *seqr_ptr {nullptr};
    if (uvm::uvm_config_db<tl_sequencer*>::get(nullptr, "*", "tl_sequencer", seqr_ptr) && seq) {
      seq->start(seqr_ptr);
    } else {
      uvm::uvm_report_fatal("TEST/NOSQR", "tl_sequencer not found in config_db", uvm::UVM_NONE);
    }
    drain_delta();
    phase.drop_objection(this);
  }
};
UVM_COMPONENT_REGISTER(otbn_bit_bash_test);

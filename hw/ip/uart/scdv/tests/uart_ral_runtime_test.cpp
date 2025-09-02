#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_sequencer.hpp"
#include "../../../dv/sc/tl_agent/tl_item.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
using namespace uvm;

class uart_ral_runtime_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_ral_runtime_seq);
  explicit uart_ral_runtime_seq(const std::string &nm = "uart_ral_runtime_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    if (tl_bind::reset) tl_bind::reset();
    uint32_t total = tl_bind::ral_num_bytes ? tl_bind::ral_num_bytes() : 64u;
    uint32_t step  = tl_bind::ral_word_bytes ? tl_bind::ral_word_bytes() : 4u;
    for (uint32_t addr = 0; addr < total; addr += step) {
      tl_item *wr = tl_item::type_id::create("wr", nullptr);
      wr->a_opcode = tl_a_op_e::PutFullData; wr->a_addr = addr; wr->a_size = 2; wr->a_mask = 0xF; wr->a_data = 0xA5A50000u | (addr & 0xFFFF);
      start_item(wr); finish_item(wr);
      tl_item *rd = tl_item::type_id::create("rd", nullptr);
      rd->a_opcode = tl_a_op_e::Get; rd->a_addr = addr; rd->a_size = 2; rd->a_mask = 0xF;
      start_item(rd); finish_item(rd);
    }
  }
};

class uart_ral_runtime_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_ral_runtime_test);
  uart_env* m_env {};
  explicit uart_ral_runtime_test(uvm_component_name nm) : uvm_test(nm) {}
  void build_phase(uvm_phase &phase) override { m_env = uart_env::type_id::create("env", this); }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    tl_sequencer *seqr_ptr {nullptr};
    if (!uvm::uvm_config_db<tl_sequencer*>::get(nullptr, "*", "tl_sequencer", seqr_ptr)) {
      uvm::uvm_report_fatal("TEST/NOSQR", "tl_sequencer not found in config_db", uvm::UVM_NONE);
    }
    if (m_env && m_env->scb) m_env->scb->clear();
    auto seq = uart_ral_runtime_seq::type_id::create("ral_seq");
    seq->start(seqr_ptr);
    drain_delta();
    phase.drop_objection(this);
  }
};
UVM_COMPONENT_REGISTER(uart_ral_runtime_test);

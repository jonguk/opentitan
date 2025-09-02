#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class keymgr_regwen_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(keymgr_regwen_seq);
  explicit keymgr_regwen_seq(const std::string &nm = "keymgr_regwen_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Lock writes via REGWEN=0, attempt write to CTRL (or first), expect no effect; then unlock and write
    tl_item *wen0 = tl_item::type_id::create("wen0", nullptr);
    wen0->a_opcode = tl_a_op_e::PutFullData; wen0->a_addr = 0; wen0->a_size = 2; wen0->a_mask = 0xF; wen0->a_data = 0u;
    start_item(wen0); finish_item(wen0);
    tl_item *w_lock = tl_item::type_id::create("w_lock", nullptr);
    w_lock->a_opcode = tl_a_op_e::PutFullData; w_lock->a_addr = 0; w_lock->a_size = 2; w_lock->a_mask = 0xF; w_lock->a_data = 0xA5A5u;
    start_item(w_lock); finish_item(w_lock);
    tl_item *r_lock = tl_item::type_id::create("r_lock", nullptr);
    r_lock->a_opcode = tl_a_op_e::Get; r_lock->a_addr = 0; r_lock->a_size = 2; r_lock->a_mask = 0xF;
    start_item(r_lock); finish_item(r_lock);
    tl_item *wen1 = tl_item::type_id::create("wen1", nullptr);
    wen1->a_opcode = tl_a_op_e::PutFullData; wen1->a_addr = 0; wen1->a_size = 2; wen1->a_mask = 0xF; wen1->a_data = 1u;
    start_item(wen1); finish_item(wen1);
    tl_item *w_unlk = tl_item::type_id::create("w_unlk", nullptr);
    w_unlk->a_opcode = tl_a_op_e::PutFullData; w_unlk->a_addr = 0; w_unlk->a_size = 2; w_unlk->a_mask = 0xF; w_unlk->a_data = 0x5A5Au;
    start_item(w_unlk); finish_item(w_unlk);
    tl_item *r_unlk = tl_item::type_id::create("r_unlk", nullptr);
    r_unlk->a_opcode = tl_a_op_e::Get; r_unlk->a_addr = 0; r_unlk->a_size = 2; r_unlk->a_mask = 0xF;
    start_item(r_unlk); finish_item(r_unlk);
  }
};

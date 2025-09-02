#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class rv_dm_wo_rc_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(rv_dm_wo_rc_seq);
  explicit rv_dm_wo_rc_seq(const std::string &nm = "rv_dm_wo_rc_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // RC check: consecutive reads from RDATA
    tl_item *rc_rd1 = tl_item::type_id::create("rc_rd1", nullptr);
    rc_rd1->a_opcode = tl_a_op_e::Get; rc_rd1->a_addr = 12; rc_rd1->a_size = 2; rc_rd1->a_mask = 0xF;
    start_item(rc_rd1); finish_item(rc_rd1);
    tl_item *rc_rd2 = tl_item::type_id::create("rc_rd2", nullptr);
    rc_rd2->a_opcode = tl_a_op_e::Get; rc_rd2->a_addr = 12; rc_rd2->a_size = 2; rc_rd2->a_mask = 0xF;
    start_item(rc_rd2); finish_item(rc_rd2);
  }
};

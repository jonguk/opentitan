#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class uart_wo_rc_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_wo_rc_seq);
  explicit uart_wo_rc_seq(const std::string &nm = "uart_wo_rc_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // WO semantics: write WDATA, then read back (expect 0)
    tl_item *wo_wr = tl_item::type_id::create("wo_wr", nullptr);
    wo_wr->a_opcode = tl_a_op_e::PutFullData; wo_wr->a_addr = 12; wo_wr->a_size = 2; wo_wr->a_mask = 0xF; wo_wr->a_data = 0xAB;
    start_item(wo_wr); finish_item(wo_wr);
    tl_item *wo_rd = tl_item::type_id::create("wo_rd", nullptr);
    wo_rd->a_opcode = tl_a_op_e::Get; wo_rd->a_addr = 12; wo_rd->a_size = 2; wo_rd->a_mask = 0xF;
    start_item(wo_rd); finish_item(wo_rd);
    // RC check: consecutive reads from RDATA
    tl_item *rc_rd1 = tl_item::type_id::create("rc_rd1", nullptr);
    rc_rd1->a_opcode = tl_a_op_e::Get; rc_rd1->a_addr = 8; rc_rd1->a_size = 2; rc_rd1->a_mask = 0xF;
    start_item(rc_rd1); finish_item(rc_rd1);
    tl_item *rc_rd2 = tl_item::type_id::create("rc_rd2", nullptr);
    rc_rd2->a_opcode = tl_a_op_e::Get; rc_rd2->a_addr = 8; rc_rd2->a_size = 2; rc_rd2->a_mask = 0xF;
    start_item(rc_rd2); finish_item(rc_rd2);
  }
};

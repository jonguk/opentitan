#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class i2c_wo_rc_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(i2c_wo_rc_seq);
  explicit i2c_wo_rc_seq(const std::string &nm = "i2c_wo_rc_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // WO-like semantics on TXDATA (target/hw dependent)
    tl_item *tx_wr = tl_item::type_id::create("tx_wr", nullptr);
    tx_wr->a_opcode = tl_a_op_e::PutFullData; tx_wr->a_addr = 76; tx_wr->a_size = 2; tx_wr->a_mask = 0xF; tx_wr->a_data = 0xAB;
    start_item(tx_wr); finish_item(tx_wr);
    // RC check: consecutive reads from RDATA
    tl_item *rc_rd1 = tl_item::type_id::create("rc_rd1", nullptr);
    rc_rd1->a_opcode = tl_a_op_e::Get; rc_rd1->a_addr = 8; rc_rd1->a_size = 2; rc_rd1->a_mask = 0xF;
    start_item(rc_rd1); finish_item(rc_rd1);
    tl_item *rc_rd2 = tl_item::type_id::create("rc_rd2", nullptr);
    rc_rd2->a_opcode = tl_a_op_e::Get; rc_rd2->a_addr = 8; rc_rd2->a_size = 2; rc_rd2->a_mask = 0xF;
    start_item(rc_rd2); finish_item(rc_rd2);
  }
};

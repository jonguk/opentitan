#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class uart_racl_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_racl_seq);
  explicit uart_racl_seq(const std::string &nm = "uart_racl_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    if (tl_bind::set_racl_enable) tl_bind::set_racl_enable(true);
    if (tl_bind::set_reg_policy) tl_bind::set_reg_policy(0, true, false); // CTRL index
    tl_item *wr_blk = tl_item::type_id::create("wr_blk", nullptr);
    wr_blk->a_opcode = tl_a_op_e::PutFullData; wr_blk->a_addr = 0; wr_blk->a_size = 2; wr_blk->a_mask = 0xF; wr_blk->a_data = 0xA5A5u;
    start_item(wr_blk); finish_item(wr_blk);
    tl_item *rd_chk = tl_item::type_id::create("rd_chk", nullptr);
    rd_chk->a_opcode = tl_a_op_e::Get; rd_chk->a_addr = 0; rd_chk->a_size = 2; rd_chk->a_mask = 0xF;
    start_item(rd_chk); finish_item(rd_chk);
    if (tl_bind::set_racl_enable) tl_bind::set_racl_enable(false);
  }
};

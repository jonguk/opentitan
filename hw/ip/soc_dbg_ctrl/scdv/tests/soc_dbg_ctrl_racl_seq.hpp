#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class soc_dbg_ctrl_racl_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(soc_dbg_ctrl_racl_seq);
  explicit soc_dbg_ctrl_racl_seq(const std::string &nm = "soc_dbg_ctrl_racl_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Enable RACL and set policy: block writes to CTRL for default role (0)
    if (tl_bind::set_racl_enable) tl_bind::set_racl_enable(true);
    if (tl_bind::set_reg_policy) tl_bind::set_reg_policy(-6, true, false);
    // Attempt blocked write (expect dut to ignore or error based on error_rsp)
    if (tl_bind::set_racl_error_response) tl_bind::set_racl_error_response(false);
    tl_item *wr_blk = tl_item::type_id::create("wr_blk", nullptr);
    wr_blk->a_opcode = tl_a_op_e::PutFullData; wr_blk->a_addr = 12; wr_blk->a_size = 2; wr_blk->a_mask = 0xF; wr_blk->a_data = 0xA5A5u;
    start_item(wr_blk); finish_item(wr_blk);
    tl_item *rd_chk = tl_item::type_id::create("rd_chk", nullptr);
    rd_chk->a_opcode = tl_a_op_e::Get; rd_chk->a_addr = 12; rd_chk->a_size = 2; rd_chk->a_mask = 0xF;
    start_item(rd_chk); finish_item(rd_chk);
    // Now act as privileged role (nonzero mask) and allow write
    if (tl_bind::set_role_mask) tl_bind::set_role_mask(1u);
    if (tl_bind::set_racl_error_response) tl_bind::set_racl_error_response(true);
    tl_item *wr_ok = tl_item::type_id::create("wr_ok", nullptr);
    wr_ok->a_opcode = tl_a_op_e::PutFullData; wr_ok->a_addr = 12; wr_ok->a_size = 2; wr_ok->a_mask = 0xF; wr_ok->a_data = 0x5A5Au;
    start_item(wr_ok); finish_item(wr_ok);
    tl_item *rd_ok = tl_item::type_id::create("rd_ok", nullptr);
    rd_ok->a_opcode = tl_a_op_e::Get; rd_ok->a_addr = 12; rd_ok->a_size = 2; rd_ok->a_mask = 0xF;
    start_item(rd_ok); finish_item(rd_ok);
    // Disable RACL
    if (tl_bind::set_racl_enable) tl_bind::set_racl_enable(false);
  }
};

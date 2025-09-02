#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class adc_ctrl_w1c_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(adc_ctrl_w1c_seq);
  explicit adc_ctrl_w1c_seq(const std::string &nm = "adc_ctrl_w1c_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Set tx_empty bit via INTR_TEST, then observe INTR_STATE and clear via W1C
    tl_item *wr_set = tl_item::type_id::create("wr_set", nullptr);
    wr_set->a_opcode = tl_a_op_e::PutFullData; wr_set->a_addr = 8; wr_set->a_size = 2; wr_set->a_mask = 0xF; wr_set->a_data = 1u;
    start_item(wr_set); finish_item(wr_set);
    tl_item *rd_state = tl_item::type_id::create("rd_state", nullptr);
    rd_state->a_opcode = tl_a_op_e::Get; rd_state->a_addr = 0; rd_state->a_size = 2; rd_state->a_mask = 0xF;
    start_item(rd_state); finish_item(rd_state);
    tl_item *wr_clr = tl_item::type_id::create("wr_clr", nullptr);
    wr_clr->a_opcode = tl_a_op_e::PutFullData; wr_clr->a_addr = 0; wr_clr->a_size = 2; wr_clr->a_mask = 0xF; wr_clr->a_data = 1u;
    start_item(wr_clr); finish_item(wr_clr);
    tl_item *rd_state2 = tl_item::type_id::create("rd_state2", nullptr);
    rd_state2->a_opcode = tl_a_op_e::Get; rd_state2->a_addr = 0; rd_state2->a_size = 2; rd_state2->a_mask = 0xF;
    start_item(rd_state2); finish_item(rd_state2);
  }
};

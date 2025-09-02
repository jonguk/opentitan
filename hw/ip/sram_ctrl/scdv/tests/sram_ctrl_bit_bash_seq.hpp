#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class sram_ctrl_bit_bash_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(sram_ctrl_bit_bash_seq);
  explicit sram_ctrl_bit_bash_seq(const std::string &nm = "sram_ctrl_bit_bash_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Toggle bits in INTR_ENABLE (addr 4)
    for (unsigned b = 0; b < 8; ++b) {
      tl_item *wr = tl_item::type_id::create("bb_wr", nullptr);
      wr->a_opcode = tl_a_op_e::PutFullData; wr->a_addr = 4; wr->a_size = 2; wr->a_mask = 0xF; wr->a_data = (1u<<b);
      start_item(wr); finish_item(wr);
      tl_item *rd = tl_item::type_id::create("bb_rd", nullptr);
      rd->a_opcode = tl_a_op_e::Get; rd->a_addr = 4; rd->a_size = 2; rd->a_mask = 0xF;
      start_item(rd); finish_item(rd);
    }
  }
};

#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class sram_ctrl_shadow_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(sram_ctrl_shadow_seq);
  explicit sram_ctrl_shadow_seq(const std::string &nm = "sram_ctrl_shadow_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    tl_item *rd = tl_item::type_id::create("rd", nullptr);
    rd->a_opcode = tl_a_op_e::Get; rd->a_addr = 12; rd->a_size = 2; rd->a_mask = 0xF;
    start_item(rd); finish_item(rd);
  }
};

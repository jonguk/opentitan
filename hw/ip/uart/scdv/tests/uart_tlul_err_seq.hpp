#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class uart_tlul_err_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_tlul_err_seq);
  explicit uart_tlul_err_seq(const std::string &nm = "uart_tlul_err_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Out-of-range accesses to drive addr_cp.others and protocol errors
    // 1) Valid-size read at an unmapped high address to hit others bin explicitly
    tl_item *rd_others = tl_item::type_id::create("rd_others", nullptr);
    rd_others->a_opcode = tl_a_op_e::Get; rd_others->a_addr = 0xFFFFFFFFu; rd_others->a_size = 0; rd_others->a_mask = 0x1;
    start_item(rd_others); finish_item(rd_others);
    // 2) Out-of-range mid address (legacy)
    tl_item *rd_bad = tl_item::type_id::create("rd_bad", nullptr);
    rd_bad->a_opcode = tl_a_op_e::Get; rd_bad->a_addr = 0xFFFF; rd_bad->a_size = 0; rd_bad->a_mask = 0x1;
    start_item(rd_bad); finish_item(rd_bad);
    // 3) Illegal mask (0) on partial write to trigger error
    tl_item *wr_illegal = tl_item::type_id::create("wr_illegal", nullptr);
    wr_illegal->a_opcode = tl_a_op_e::PutPartialData; wr_illegal->a_addr = 4; wr_illegal->a_size = 0; wr_illegal->a_mask = 0x0; wr_illegal->a_data = 0;
    start_item(wr_illegal); finish_item(wr_illegal);
  }
};

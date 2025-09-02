#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class uart_tl_partial_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_tl_partial_seq);
  explicit uart_tl_partial_seq(const std::string &nm = "uart_tl_partial_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // TL partial write coverage boost: valid partial writes at an aligned CSR (CTRL candidate)
    const uint32_t base = 0;
    // 1B partial (mask count=1)
    { tl_item *w1 = tl_item::type_id::create("w1", nullptr); w1->a_opcode=tl_a_op_e::PutPartialData; w1->a_addr=base; w1->a_size=0; w1->a_mask=0x1; w1->a_data=0x11; start_item(w1); finish_item(w1); }
    { tl_item *r1 = tl_item::type_id::create("r1", nullptr); r1->a_opcode=tl_a_op_e::Get; r1->a_addr=base; r1->a_size=2; r1->a_mask=0xF; start_item(r1); finish_item(r1); }
    // 2B partial (mask count=2)
    { tl_item *w2 = tl_item::type_id::create("w2", nullptr); w2->a_opcode=tl_a_op_e::PutPartialData; w2->a_addr=base; w2->a_size=1; w2->a_mask=0x3; w2->a_data=0x2233; start_item(w2); finish_item(w2); }
    { tl_item *r2 = tl_item::type_id::create("r2", nullptr); r2->a_opcode=tl_a_op_e::Get; r2->a_addr=base; r2->a_size=2; r2->a_mask=0xF; start_item(r2); finish_item(r2); }
    // 4B partial (mask count=3, non-contig)
    { tl_item *w3 = tl_item::type_id::create("w3", nullptr); w3->a_opcode=tl_a_op_e::PutPartialData; w3->a_addr=base; w3->a_size=2; w3->a_mask=0x7; w3->a_data=0x44556677; start_item(w3); finish_item(w3); }
    { tl_item *r3 = tl_item::type_id::create("r3", nullptr); r3->a_opcode=tl_a_op_e::Get; r3->a_addr=base; r3->a_size=2; r3->a_mask=0xF; start_item(r3); finish_item(r3); }
    { tl_item *wn = tl_item::type_id::create("wn", nullptr); wn->a_opcode=tl_a_op_e::PutPartialData; wn->a_addr=base; wn->a_size=2; wn->a_mask=0x5; wn->a_data=0x8899AABB; start_item(wn); finish_item(wn); }
    { tl_item *rn = tl_item::type_id::create("rn", nullptr); rn->a_opcode=tl_a_op_e::Get; rn->a_addr=base; rn->a_size=2; rn->a_mask=0xF; start_item(rn); finish_item(rn); }
  }
};

#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class rv_dm_window_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(rv_dm_window_seq);
  explicit rv_dm_window_seq(const std::string &nm = "rv_dm_window_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Touch a variety of regs to raise addr_cp coverage
    const uint32_t offs[] = {12, 12, 20, 24, 32, 36, 40, 44, 48};
    for (unsigned i=0;i<sizeof(offs)/sizeof(offs[0]);++i){
      // 4B read
      tl_item *rd4 = tl_item::type_id::create("rd4", nullptr); rd4->a_opcode = tl_a_op_e::Get; rd4->a_addr = offs[i]; rd4->a_size = 2; rd4->a_mask = 0xF; start_item(rd4); finish_item(rd4);
      // 2B read
      tl_item *rd2 = tl_item::type_id::create("rd2", nullptr); rd2->a_opcode = tl_a_op_e::Get; rd2->a_addr = offs[i]; rd2->a_size = 1; rd2->a_mask = 0x3; start_item(rd2); finish_item(rd2);
      // 1B read
      tl_item *rd1 = tl_item::type_id::create("rd1", nullptr); rd1->a_opcode = tl_a_op_e::Get; rd1->a_addr = offs[i]; rd1->a_size = 0; rd1->a_mask = 0x1; start_item(rd1); finish_item(rd1);
    }
    // Partial writes with different sizes/masks
    tl_item *pw0 = tl_item::type_id::create("pw0", nullptr); pw0->a_opcode=tl_a_op_e::PutPartialData; pw0->a_addr=4; pw0->a_size=0; pw0->a_mask=0x0; pw0->a_data=0x0; start_item(pw0); finish_item(pw0);
    tl_item *pw1 = tl_item::type_id::create("pw1", nullptr); pw1->a_opcode=tl_a_op_e::PutPartialData; pw1->a_addr=4; pw1->a_size=0; pw1->a_mask=0x1; pw1->a_data=0x1; start_item(pw1); finish_item(pw1);
    tl_item *pw2 = tl_item::type_id::create("pw2", nullptr); pw2->a_opcode=tl_a_op_e::PutPartialData; pw2->a_addr=4; pw2->a_size=1; pw2->a_mask=0x3; pw2->a_data=0x3; start_item(pw2); finish_item(pw2);
    tl_item *pw3 = tl_item::type_id::create("pw3", nullptr); pw3->a_opcode=tl_a_op_e::PutPartialData; pw3->a_addr=4; pw3->a_size=2; pw3->a_mask=0x7; pw3->a_data=0x7; start_item(pw3); finish_item(pw3);
  }
};

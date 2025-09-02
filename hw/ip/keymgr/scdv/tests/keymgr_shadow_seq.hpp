#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class keymgr_shadow_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(keymgr_shadow_seq);
  explicit keymgr_shadow_seq(const std::string &nm = "keymgr_shadow_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Shadowed register two-phase commit: write same value twice to commit
    uint32_t val = 0xA5A5u;
    tl_item *wr_p1 = tl_item::type_id::create("wr_p1", nullptr);
    wr_p1->a_opcode = tl_a_op_e::PutFullData; wr_p1->a_addr = 8; wr_p1->a_size = 2; wr_p1->a_mask = 0xF; wr_p1->a_data = val;
    start_item(wr_p1); finish_item(wr_p1);
    tl_item *wr_p2 = tl_item::type_id::create("wr_p2", nullptr);
    wr_p2->a_opcode = tl_a_op_e::PutFullData; wr_p2->a_addr = 8; wr_p2->a_size = 2; wr_p2->a_mask = 0xF; wr_p2->a_data = val;
    start_item(wr_p2); finish_item(wr_p2);
    tl_item *rd_ok = tl_item::type_id::create("rd_ok", nullptr);
    rd_ok->a_opcode = tl_a_op_e::Get; rd_ok->a_addr = 8; rd_ok->a_size = 2; rd_ok->a_mask = 0xF;
    start_item(rd_ok); finish_item(rd_ok);
    // Mismatch attempt: different second write should not commit
    uint32_t val2 = 0x3C3Cu;
    tl_item *wr_m1 = tl_item::type_id::create("wr_m1", nullptr);
    wr_m1->a_opcode = tl_a_op_e::PutFullData; wr_m1->a_addr = 8; wr_m1->a_size = 2; wr_m1->a_mask = 0xF; wr_m1->a_data = val2;
    start_item(wr_m1); finish_item(wr_m1);
    tl_item *wr_m2 = tl_item::type_id::create("wr_m2", nullptr);
    wr_m2->a_opcode = tl_a_op_e::PutFullData; wr_m2->a_addr = 8; wr_m2->a_size = 2; wr_m2->a_mask = 0xF; wr_m2->a_data = (val2 ^ 0xFFFFu);
    start_item(wr_m2); finish_item(wr_m2);
    tl_item *rd_no = tl_item::type_id::create("rd_no", nullptr);
    rd_no->a_opcode = tl_a_op_e::Get; rd_no->a_addr = 8; rd_no->a_size = 2; rd_no->a_mask = 0xF;
    start_item(rd_no); finish_item(rd_no);
  }
};

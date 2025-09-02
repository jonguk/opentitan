#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class usbdev_tlul_err_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(usbdev_tlul_err_seq);
  explicit usbdev_tlul_err_seq(const std::string &nm = "usbdev_tlul_err_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Out-of-range access to trigger d_error (read)
    tl_item *rd_bad = tl_item::type_id::create("rd_bad", nullptr);
    rd_bad->a_opcode = tl_a_op_e::Get; rd_bad->a_addr = 0xFFFF; rd_bad->a_size = 0; rd_bad->a_mask = 0x1;
    start_item(rd_bad); finish_item(rd_bad);
    // Illegal mask (0) on partial write to trigger error
    tl_item *wr_illegal = tl_item::type_id::create("wr_illegal", nullptr);
    wr_illegal->a_opcode = tl_a_op_e::PutPartialData; wr_illegal->a_addr = 4; wr_illegal->a_size = 0; wr_illegal->a_mask = 0x0; wr_illegal->a_data = 0;
    start_item(wr_illegal); finish_item(wr_illegal);
  }
};

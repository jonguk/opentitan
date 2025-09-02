#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
class uart_tlul_err_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_tlul_err_seq);
  explicit uart_tlul_err_seq(const std::string &nm = "uart_tlul_err_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    // Generate protocol errors similar to SV tl_host_protocol_err_seq
    // 1) Misaligned read
    tl_item *misalign = tl_item::type_id::create("misalign", nullptr);
    misalign->a_opcode = tl_a_op_e::Get; misalign->a_addr = 3; misalign->a_size = 2; // 4-byte op at addr 3
    start_item(misalign); finish_item(misalign);
    // 2) Size over max
    tl_item *oversize = tl_item::type_id::create("oversize", nullptr);
    oversize->a_opcode = tl_a_op_e::Get; oversize->a_addr = 0; oversize->a_size = 3;
    start_item(oversize); finish_item(oversize);
    // 3) PutFull with mask mismatch (not all lanes active)
    tl_item *mask_mismatch = tl_item::type_id::create("mask_mismatch", nullptr);
    mask_mismatch->a_opcode = tl_a_op_e::PutFullData; mask_mismatch->a_addr = 16; mask_mismatch->a_size = 2; mask_mismatch->a_mask = 0x3; mask_mismatch->a_data = 0xDEAD'BEEF;
    start_item(mask_mismatch); finish_item(mask_mismatch);
  }
};

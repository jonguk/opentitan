#pragma once
#include <uvm>
#include "../tl_item.hpp"

// Common host base sequence with helpers to create TL-UL items
class tl_host_base_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(tl_host_base_seq);
  explicit tl_host_base_seq(const std::string &nm = "tl_host_base_seq") : uvm::uvm_sequence<tl_item>(nm) {}

 protected:
  static tl_item* make_rd(uint32_t addr, uint8_t size_log2, uint8_t mask) {
    tl_item *it = tl_item::type_id::create("rd", nullptr);
    it->a_opcode = tl_a_op_e::Get; it->a_addr = addr; it->a_size = size_log2; it->a_mask = mask;
    return it;
  }
  static tl_item* make_wr(uint32_t addr, uint8_t size_log2, uint8_t mask, uint32_t data) {
    tl_item *it = tl_item::type_id::create("wr", nullptr);
    it->a_opcode = (mask == 0xF && size_log2 == 2) ? tl_a_op_e::PutFullData : tl_a_op_e::PutPartialData;
    it->a_addr = addr; it->a_size = size_log2; it->a_mask = mask; it->a_data = data;
    return it;
  }
};



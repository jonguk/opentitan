#pragma once
#include <uvm>
#include <tlm>

class tl_item : public uvm::uvm_sequence_item {
 public:
  UVM_OBJECT_UTILS(tl_item);
  // Basic TL-UL fields (subset)
  bool    is_write {false};
  uint64_t addr {0};
  uint32_t data {0};
  unsigned size_bytes {4};

  tl_item(const std::string &name = "tl_item") : uvm::uvm_sequence_item(name) {}
};

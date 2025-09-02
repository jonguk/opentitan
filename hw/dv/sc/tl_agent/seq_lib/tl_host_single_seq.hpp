#pragma once
#include <uvm>
#include "tl_host_base_seq.hpp"

class tl_host_single_seq : public tl_host_base_seq {
 public:
  UVM_OBJECT_UTILS(tl_host_single_seq);
  explicit tl_host_single_seq(const std::string &nm = "tl_host_single_seq") : tl_host_base_seq(nm) {}

  uint32_t addr {0};
  bool do_write {false};
  uint8_t size_log2 {2};
  uint8_t mask {0xF};
  uint32_t data {0};

  void body() override {
    if (do_write) {
      tl_item *wr = make_wr(addr, size_log2, mask, data);
      start_item(wr); finish_item(wr);
    } else {
      tl_item *rd = make_rd(addr, size_log2, mask);
      start_item(rd); finish_item(rd);
    }
  }
};

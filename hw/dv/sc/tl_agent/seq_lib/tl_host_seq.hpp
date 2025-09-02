#pragma once
#include <uvm>
#include "tl_host_base_seq.hpp"

class tl_host_seq : public tl_host_base_seq {
 public:
  UVM_OBJECT_UTILS(tl_host_seq);
  explicit tl_host_seq(const std::string &nm = "tl_host_seq") : tl_host_base_seq(nm) {}

  uint32_t base_addr {0};

  void body() override {
    // Simple pattern: RD/WR a few words around base
    for (int i = 0; i < 4; ++i) {
      uint32_t a = base_addr + static_cast<uint32_t>(i * 4);
      tl_item *rd = make_rd(a, 2, 0xF);
      start_item(rd); finish_item(rd);
      tl_item *wr = make_wr(a, 2, 0xF, 0xA5A5A5A5u ^ a);
      start_item(wr); finish_item(wr);
    }
  }
};

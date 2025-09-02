#pragma once
#include <uvm>
#include "tl_host_base_seq.hpp"

// Sequence to exercise TL-UL protocol errors (mask=0, misaligned, OOB)
class tl_host_protocol_err_seq : public tl_host_base_seq {
 public:
  UVM_OBJECT_UTILS(tl_host_protocol_err_seq);
  explicit tl_host_protocol_err_seq(const std::string &nm = "tl_host_protocol_err_seq") : tl_host_base_seq(nm) {}

  void body() override {
    // Out-of-range read
    {
      tl_item *rd_bad = make_rd(0xFFFF, 0, 0x1);
      start_item(rd_bad); finish_item(rd_bad);
    }
    // Illegal mask=0 on partial write
    {
      tl_item *wr_zero = make_wr(4, 0, 0x0, 0);
      start_item(wr_zero); finish_item(wr_zero);
    }
    // Misaligned 2B access
    {
      tl_item *rd_mis = make_rd(0x13, 1, 0x3);
      start_item(rd_mis); finish_item(rd_mis);
    }
  }
};



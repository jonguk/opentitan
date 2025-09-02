#pragma once
#include <uvm>
#include <cstdlib>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/bus_params_pkg/bus_params.hpp"
#include "../env/csr_exclude.hpp"
class sysrst_ctrl_csr_rw_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(sysrst_ctrl_csr_rw_seq);
  explicit sysrst_ctrl_csr_rw_seq(const std::string &nm = "sysrst_ctrl_csr_rw_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    if (tl_bind::reset) tl_bind::reset();
    // Prefer runtime RAL traversal when hooks are available
    if (tl_bind::ral_num_bytes && tl_bind::ral_word_bytes) {
      uint32_t total = tl_bind::ral_num_bytes();
      uint32_t step  = tl_bind::ral_word_bytes();
      if (step == 0) step = 4u;
      for (uint32_t addr = 0; addr < total; addr += step) {
        if (scdv::exclude_write(addr)) continue;
        uint32_t wdata = static_cast<uint32_t>(std::rand());
        tl_item *wr = tl_item::type_id::create("wr", nullptr);
        wr->a_opcode = tl_a_op_e::PutFullData; wr->a_addr = addr; wr->a_size = 2; wr->a_mask = 0xF; wr->a_data = wdata;
        start_item(wr); finish_item(wr);
        uint32_t sz_sel = static_cast<uint32_t>(std::rand()) % 3; // 0:1B,1:2B,2:4B
        tl_item *rd = tl_item::type_id::create("rd", nullptr);
        rd->a_opcode = tl_a_op_e::Get; rd->a_addr = addr; rd->a_size = (sz_sel == 2 ? 2 : (sz_sel == 1 ? 1 : 0));
        rd->a_mask = (sz_sel == 2 ? 0xF : (sz_sel == 1 ? 0x3 : 0x1));
        if (!scdv::exclude_read(addr)) { start_item(rd); finish_item(rd); }
      }
      return;
    }
    // Fallback: use HJSON-derived ordered offsets and per-register masks
    const uint32_t offs[] = { 0, 4, 8, 12, 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92 };
    const uint32_t wmask[] = { 0x0, 0xffffffff, 0x0, 0x0, 0x1, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x1, 0x0, 0x0, 0xfff, 0xffff, 0xff, 0xff, 0x0, 0x3fff, 0xffffffff, 0xffffffff, 0x77, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x0, 0x0 };
    const uint32_t womask[] = { 0x0, 0x0, 0xffffffff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    const uint32_t w1cmask[] = { 0xffffffff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x3fff };
    const uint32_t rcmask[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    const size_t num = sizeof(offs)/sizeof(offs[0]);
    for (size_t i = 0; i < num; ++i) {
      uint32_t addr = offs[i];
      const uint32_t m_w = wmask[i];
      // Compute byte-enable mask from field-level write mask (coarse-grain)
      uint32_t be = 0;
      for (int b=0; b<4; ++b) { uint32_t lane = (m_w >> (8*b)) & 0xFFu; if (lane) be |= (1u<<b); }
      if (!scdv::exclude_write(addr) && be) {
        uint32_t wdata = static_cast<uint32_t>(std::rand());
        // Random subset of allowed lanes to mimic partial writes
        uint32_t subset = (static_cast<uint32_t>(std::rand()) & 0xFu) & be; if (subset == 0) subset = be;
        tl_item *wr = tl_item::type_id::create("wr", nullptr);
        wr->a_opcode = (subset == 0xFu ? tl_a_op_e::PutFullData : tl_a_op_e::PutPartialData);
        wr->a_addr = addr; wr->a_size = 2; wr->a_mask = subset; wr->a_data = wdata;
        start_item(wr); finish_item(wr);
      }
      uint32_t sz_sel = static_cast<uint32_t>(std::rand()) % 3;
      tl_item *rd = tl_item::type_id::create("rd", nullptr);
      rd->a_opcode = tl_a_op_e::Get; rd->a_addr = addr; rd->a_size = (sz_sel == 2 ? 2 : (sz_sel == 1 ? 1 : 0));
      rd->a_mask = (sz_sel == 2 ? 0xF : (sz_sel == 1 ? 0x3 : 0x1));
      if (!scdv::exclude_read(addr)) { start_item(rd); finish_item(rd); }
    }
  }
};

#pragma once
#include <uvm>
#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <vector>
#include <cstdlib>
#include "../tl_agent/tl_item.hpp"
#include "../tl_agent/tl_sequencer.hpp"
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include "../coverage/csr_cov.hpp"
#endif

// Minimal frontdoor CSR sweep helper (SV csr_seq_lib parity):
// Issues TL transactions through the provided sequence; scoreboard+RAL perform prediction/checks.
inline void run_csr_rw_sweep(
    uvm::uvm_sequence<tl_item> *seq,
    const uint32_t *addrs,
    const uint32_t *w_mask,
    const uint32_t *wo_mask,
    const uint32_t *w1c_mask,
    const uint32_t *rc_mask,
    std::size_t num_regs) {
  if (seq == nullptr) { uvm::uvm_report_fatal("CSR_SWEEP", "sequence null", uvm::UVM_NONE); return; }
  const uint32_t be_list[] = {0x1u, 0x3u, 0xFu};
  const uint32_t be_extra_read_only[] = {0x7u, 0xEu, 0x5u}; // three_contig, three_contig_alt, non_contig
  for (std::size_t i = 0; i < num_regs; ++i) {
    const uint32_t addr = addrs[i];
    const uint32_t wm   = w_mask   ? w_mask[i]   : 0;
    const uint32_t wom  = wo_mask  ? wo_mask[i]  : 0;
    const uint32_t w1m  = w1c_mask ? w1c_mask[i] : 0;
    const uint32_t rcm  = rc_mask  ? rc_mask[i]  : 0;
    for (uint32_t be : be_list) {
      if (w1m != 0) {
        auto *wr1 = tl_item::type_id::create("w1c_set", nullptr);
        wr1->a_opcode = tl_a_op_e::PutFullData; wr1->a_addr = addr; wr1->a_size = 2; wr1->a_mask = be; wr1->a_data = w1m;
        seq->start_item(wr1); seq->finish_item(wr1);
        auto *rd1 = tl_item::type_id::create("w1c_rd", nullptr);
        rd1->a_opcode = tl_a_op_e::Get; rd1->a_addr = addr; rd1->a_size = 2; rd1->a_mask = be;
        seq->start_item(rd1); seq->finish_item(rd1);
        continue;
      }
      if (rcm != 0) {
        auto *rd = tl_item::type_id::create("rc_rd", nullptr);
        rd->a_opcode = tl_a_op_e::Get; rd->a_addr = addr; rd->a_size = 2; rd->a_mask = be;
        seq->start_item(rd); seq->finish_item(rd);
        continue;
      }
      if ((wom | wm) != 0) {
        auto *wr = tl_item::type_id::create("rw_wr", nullptr);
        wr->a_opcode = tl_a_op_e::PutFullData; wr->a_addr = addr; wr->a_size = 2; wr->a_mask = be; wr->a_data = 0xA5A5A5A5u;
        seq->start_item(wr); seq->finish_item(wr);
        auto *rd = tl_item::type_id::create("rw_rd", nullptr);
        rd->a_opcode = tl_a_op_e::Get; rd->a_addr = addr; rd->a_size = 2; rd->a_mask = be;
        seq->start_item(rd); seq->finish_item(rd);
        continue;
      }
      auto *rd = tl_item::type_id::create("ro_rd", nullptr);
      rd->a_opcode = tl_a_op_e::Get; rd->a_addr = addr; rd->a_size = 2; rd->a_mask = be;
      seq->start_item(rd); seq->finish_item(rd);
    }
    // Extra read-only masks to close mask_class bins
    for (uint32_t be : be_extra_read_only) {
      auto *rdx = tl_item::type_id::create("mask_extra_rd", nullptr);
      rdx->a_opcode = tl_a_op_e::Get; rdx->a_addr = addr; rdx->a_size = 2; rdx->a_mask = be;
      seq->start_item(rdx); seq->finish_item(rdx);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
      if (be == 0x5u) { CsrCov::get().sample(addr, 0, false, be); }
#endif
    }
  }
  // Cover 'none' mask_class with a read using mask=0 at the first address if available
  if (num_regs > 0) {
    auto *rd0 = tl_item::type_id::create("mask_none_rd", nullptr);
    rd0->a_opcode = tl_a_op_e::Get; rd0->a_addr = addrs[0]; rd0->a_size = 2; rd0->a_mask = 0x0u;
    seq->start_item(rd0); seq->finish_item(rd0);
  }
  // Illegal address probing: smallest illegal, largest illegal, and 10 random in-between (aligned, excluding valid set)
  if (num_regs > 0) {
    uint32_t min_valid = addrs[0], max_valid = addrs[0];
    std::unordered_set<uint32_t> valid_set;
    valid_set.reserve(num_regs * 2);
    for (std::size_t i = 0; i < num_regs; ++i) {
      uint32_t a = addrs[i];
      valid_set.insert(a);
      if (a < min_valid) min_valid = a;
      if (a > max_valid) max_valid = a;
    }
    // Compute bounds just outside the valid range, 4B aligned
    uint32_t small_illegal = (min_valid >= 4) ? (min_valid - 4) : ((max_valid + 4 >= max_valid) ? (max_valid + 4) : 0xFFFFFFFCu);
    if ((small_illegal & 0x3u) != 0) small_illegal &= ~0x3u;
    uint32_t large_illegal = (max_valid <= 0xFFFFFFFCu) ? (max_valid + 4) : 0xFFFFFFFCu;
    if ((large_illegal & 0x3u) != 0) large_illegal &= ~0x3u;
    std::vector<uint32_t> illegals;
    illegals.push_back(small_illegal);
    if (large_illegal != small_illegal) illegals.push_back(large_illegal);
    // Random 10 in between
    for (int k = 0; k < 10; ++k) {
      if (large_illegal <= small_illegal + 8) break;
      uint32_t span = (large_illegal - small_illegal);
      uint32_t r = static_cast<uint32_t>(std::rand());
      uint32_t candidate = small_illegal + ((r % (span - 4u)) & ~0x3u) + 4u;
      // Ensure not a valid address and not duplicate
      if (valid_set.count(candidate)) { continue; }
      bool dup = false; for (auto v : illegals) { if (v == candidate) { dup = true; break; } }
      if (dup) continue;
      illegals.push_back(candidate);
    }
    for (uint32_t ia : illegals) {
      auto *rdo = tl_item::type_id::create("addr_illegal_rd", nullptr);
      rdo->a_opcode = tl_a_op_e::Get; rdo->a_addr = ia; rdo->a_size = 2; rdo->a_mask = 0xFu;
      seq->start_item(rdo); seq->finish_item(rdo);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
      CsrCov::get().sample(ia, 0, false, 0xFu);
#endif
    }
    // For environments where addr_cp others bin is fixed at 0xFFFFFFFF, also sample that exact address
    {
      auto *rdo = tl_item::type_id::create("addr_others_rd", nullptr);
      rdo->a_opcode = tl_a_op_e::Get; rdo->a_addr = 0xFFFFFFFFu; rdo->a_size = 2; rdo->a_mask = 0xFu;
      seq->start_item(rdo); seq->finish_item(rdo);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
      CsrCov::get().sample(0xFFFFFFFFu, 0, false, 0xFu);
#endif
    }
  }
}


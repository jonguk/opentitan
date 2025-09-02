#pragma once
#include <uvm>
#include "uart_base_vseq.hpp"
#include "../../dv/sc/tl_agent/tl_item.hpp"

class uart_smoke_vseq : public uart_base_vseq {
 public:
  UVM_OBJECT_UTILS(uart_smoke_vseq);
  explicit uart_smoke_vseq(const std::string &name = "uart_smoke_vseq") : uart_base_vseq(name) {}

  void body() override {
    auto vseqr = get_vseqr();
    if (!vseqr || !vseqr->tl_sequencer_h) {
      uvm::uvm_report_fatal("UART_VSEQ", "virtual sequencer or tl_sequencer_h is null", uvm::UVM_NONE);
      return;
    }
    // Simple CSR write/read smoke via TL-UL
    tl_item wr("wr"); wr.a_opcode = tl_a_op_e::PutFullData; wr.a_addr = 16; wr.a_size = 2; wr.a_mask = 0xF; wr.a_data = 0x1;
    start_item(&wr, -1, vseqr->tl_sequencer_h); finish_item(&wr, -1);
    tl_item rd("rd"); rd.a_opcode = tl_a_op_e::Get; rd.a_addr = 16; rd.a_size = 2;
    start_item(&rd, -1, vseqr->tl_sequencer_h); finish_item(&rd, -1);
  }
};



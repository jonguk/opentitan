#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include "../../../dv/sc/coverage/uart_cov.hpp"
#endif
class uart_csr_rw_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_csr_rw_seq);
  explicit uart_csr_rw_seq(const std::string &nm = "uart_csr_rw_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void pre_body() override {
    if (tl_bind::reset) tl_bind::reset();
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    scdv::coverage_ready = true;
#endif
  }
  void body() override {
    // Write CTRL
    tl_item *wr = tl_item::type_id::create("wr", nullptr);
    wr->a_opcode = tl_a_op_e::PutFullData; wr->a_addr = 16; wr->a_size = 2; wr->a_mask = 0xF; wr->a_data = (1u<<0)|(1u<<1)|(0x1234u<<16);
    start_item(wr); finish_item(wr);
    uvm::uvm_report_info("SEQ", "sent wr", uvm::UVM_MEDIUM);
    // Read CTRL
    tl_item *rd = tl_item::type_id::create("rd", nullptr);
    rd->a_opcode = tl_a_op_e::Get; rd->a_addr = 16; rd->a_size = 2; rd->a_mask = 0xF;
    start_item(rd); finish_item(rd);
    uvm::uvm_report_info("SEQ", "sent rd", uvm::UVM_MEDIUM);
    // Program INTR_ENABLE and sanity readback via a read transfer (scoreboard/monitor checks)
    tl_item *wr2 = tl_item::type_id::create("wr2", nullptr);
    wr2->a_opcode = tl_a_op_e::PutFullData; wr2->a_addr = 4; wr2->a_size = 2; wr2->a_mask = 0xF; wr2->a_data = (1u<<0)|(1u<<3)|(1u<<7);
    start_item(wr2); finish_item(wr2);
    uvm::uvm_report_info("SEQ", "sent wr2", uvm::UVM_MEDIUM);
    tl_item *rd2 = tl_item::type_id::create("rd2", nullptr);
    rd2->a_opcode = tl_a_op_e::Get; rd2->a_addr = 4; rd2->a_size = 2; rd2->a_mask = 0xF;
    start_item(rd2); finish_item(rd2);
    uvm::uvm_report_info("SEQ", "sent rd2", uvm::UVM_MEDIUM);
    // WDATA is WO; ensure a read occurs for scoreboard to compare mirrored value (expected 0)
    tl_item *wr3 = tl_item::type_id::create("wr3", nullptr);
    wr3->a_opcode = tl_a_op_e::PutPartialData; wr3->a_addr = 28; wr3->a_size = 2; wr3->a_mask = 0x1; wr3->a_data = 0xAB;
    start_item(wr3); finish_item(wr3);
    uvm::uvm_report_info("SEQ", "sent wr3", uvm::UVM_MEDIUM);
    tl_item *rd3 = tl_item::type_id::create("rd3", nullptr);
    rd3->a_opcode = tl_a_op_e::Get; rd3->a_addr = 28; rd3->a_size = 2; rd3->a_mask = 0xF;
    start_item(rd3); finish_item(rd3);
    uvm::uvm_report_info("SEQ", "sent rd3", uvm::UVM_MEDIUM);
  }
};

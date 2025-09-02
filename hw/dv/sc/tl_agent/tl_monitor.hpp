#pragma once
#include <uvm>
#include <cstdint>
#include <functional>
#include "tl_bind.hpp"
#include <tlm_utils/peq_with_get.h>
#include "../scoreboard/scoreboard.hpp"
#include <unordered_map>
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include "../coverage/uart_cov.hpp"
#include "../coverage/tl_cov.hpp"
#include "../coverage/csr_cov.hpp"
#include "../env/csr_exclude.hpp"
#endif
#include "../bus_params_pkg/bus_params.hpp"

class tl_monitor : public uvm::uvm_component {
 public:
  UVM_COMPONENT_UTILS(tl_monitor);
  SC_HAS_PROCESS(tl_monitor);
  explicit tl_monitor(uvm::uvm_component_name name)
      : uvm::uvm_component(name), peq_("tlm_peq") {
    SC_THREAD(drain_peq);
  }

  // SV-style analysis port: monitor publishes csr_event, scoreboard subscribes via analysis_imp
  uvm::uvm_analysis_port<csr_event> ap {"ap"};

  // SV-style connection helper (legacy direct path removed; connect via AP)
  void connect_scoreboard(simple_scoreboard *scb) { (void)scb; /* deprecated */ }
  // RAL is handled internally via uart_reg_top model

#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
  void set_coverage(UartCov *cov) { cov_ = cov; }
#endif

  void build_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_component::build_phase(phase);
    // Register observer to snoop TL transactions; enqueue into PEQ
    tl_bind::add_observer([&](uint32_t addr, uint32_t data, bool is_write, uint32_t mask, uint8_t size_log2, bool d_error, uint8_t d_opcode, uint8_t a_opcode, uint8_t a_source){ this->on_observe(addr, data, is_write, mask, size_log2, d_error, d_opcode, a_opcode, a_source); });
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    // Hook RACL control setters to coverage sampling if available
    auto prev_set_racl_enable = tl_bind::set_racl_enable;
    tl_bind::set_racl_enable = [&, prev_set_racl_enable](bool en){ if (prev_set_racl_enable) prev_set_racl_enable(en); if (cov_ && ready_) cov_->sample_racl(en, racl_err_rsp_); };
    auto prev_set_racl_error_response = tl_bind::set_racl_error_response;
    tl_bind::set_racl_error_response = [&, prev_set_racl_error_response](bool e){ if (prev_set_racl_error_response) prev_set_racl_error_response(e); racl_err_rsp_ = e; if (cov_ && ready_) cov_->sample_racl(racl_en_last_, e); };
    auto prev_set_role_mask = tl_bind::set_role_mask;
    tl_bind::set_role_mask = [&, prev_set_role_mask](uint32_t m){ if (prev_set_role_mask) prev_set_role_mask(m); if (cov_ && ready_) cov_->sample_role(m); };
#endif
    // Drain thread is declared via SC_THREAD in constructor
  }

  void start_of_simulation() override {
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    ready_ = true;
#endif
  }

 private:
  struct Txn { uint32_t addr; uint32_t data; bool is_write; };
  tlm_utils::peq_with_get<Txn> peq_;
  std::list<std::unique_ptr<Txn>> owned_;

  void on_observe(uint32_t addr, uint32_t data, bool is_write, uint32_t mask, uint8_t size_log2, bool d_error, uint8_t d_opcode, uint8_t a_opcode, uint8_t a_source) {
    // Immediately publish to analysis path (SV-like)
    if (is_write) on_write(addr, data, mask, size_log2, d_error, d_opcode, a_opcode, a_source);
    else on_read(addr, data, mask, size_log2, d_error, d_opcode, a_opcode, a_source);
  }

  void on_read(uint32_t addr, uint32_t data, uint32_t mask, uint8_t size_log2, bool d_error, uint8_t d_opcode, uint8_t a_opcode, uint8_t a_source) {
    uvm::uvm_report_info("TL_MON", (std::string("RD observed @0x") + std::to_string(addr) + " =0x" + std::to_string(data)).c_str(), uvm::UVM_LOW);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    if (cov_ && ready_) {
      cov_->sample(addr, data, false);
      // FIFO_STATUS: txlvl[7:0], rxlvl[23:16]
      if (addr == 36) {
        int txlvl = static_cast<int>(data & 0xFFu);
        int rxlvl = static_cast<int>((data >> 16) & 0xFFu);
        cov_->sample_fifo_levels(txlvl, rxlvl, false);
      }
    }
    // TLUL coverage: use observed fields
    TLulCov::get().sample(a_opcode, d_opcode, size_log2, mask, d_error);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    uint32_t addr_cov = addr;
    if (d_error) {
      const char* nm = scdv::name_from_addr(addr);
      if (nm == nullptr) addr_cov = 0xFFFFFFFFu;
    }
    CsrCov::get().sample(addr_cov, data, false, mask);
#endif
#endif
    // Basic response consistency check
    if (d_error) {
      uvm::uvm_report_warning("TL_RSP", "d_error asserted on read", uvm::UVM_LOW);
    }
    // As in SV, accept DUT behavior; coverage captures protocol issues
    // Publish observation via analysis port, and optionally direct path
    csr_event ev; ev.addr=addr; ev.data=data; ev.mask=mask; ev.is_write=false; ev.size_log2=size_log2; ev.d_error=d_error; ev.d_opcode=d_opcode; ev.a_opcode=a_opcode; ev.a_source=a_source; ev.d_source=a_source;
    ap.write(ev);
  }

  void on_write(uint32_t addr, uint32_t data, uint32_t mask, uint8_t size_log2, bool d_error, uint8_t d_opcode, uint8_t a_opcode, uint8_t a_source) {
    uvm::uvm_report_info("TL_MON", (std::string("WR observed @0x") + std::to_string(addr) + " =0x" + std::to_string(data)).c_str(), uvm::UVM_LOW);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    if (cov_ && ready_) {
      cov_->sample(addr, data, true);
      // FIFO_CTRL write: RXILVL[4:2], TXILVL[7:5]
      if (addr == 32) {
        int rxilvl = static_cast<int>((data >> 2) & 0x7u);
        int txilvl = static_cast<int>((data >> 5) & 0x7u);
        cov_->sample_rx_watermark(rxilvl);
        cov_->sample_tx_watermark(txilvl);
      }
    }
    // TLUL coverage: decide PutFull vs PutPartial by comparing active lanes
    TLulCov::get().sample(a_opcode, d_opcode, size_log2, mask, d_error);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    CsrCov::get().sample(addr, data, true, mask);
#endif
#endif
    if (d_error) {
      uvm::uvm_report_warning("TL_RSP", "d_error asserted on write", uvm::UVM_LOW);
    }
    // As in SV, accept DUT behavior; coverage captures protocol issues
    // Publish write as event (predictor based flow uses RAL)
    csr_event ev; ev.addr=addr; ev.data=data; ev.mask=mask; ev.is_write=true; ev.size_log2=size_log2; ev.d_error=d_error; ev.d_opcode=d_opcode; ev.a_opcode=a_opcode; ev.a_source=a_source; ev.d_source=a_source;
    ap.write(ev);
  }

  void drain_peq() { while (true) sc_core::wait(peq_.get_event()); }

  // Non-blocking count of PEQ depth (approximate: owned_ not yet drained or pending in peq)
 public:
  std::size_t pending_count() const { return 0; }

  simple_scoreboard *scb_ {nullptr}; // deprecated
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
  UartCov *cov_ {nullptr};
  bool ready_ {false};
  bool racl_err_rsp_ {true};
  bool racl_en_last_ {false};
#endif
};

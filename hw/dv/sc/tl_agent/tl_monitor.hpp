#pragma once
#include <uvm>
#include <cstdint>
#include <functional>
#include "tl_bind.hpp"
#include "../scoreboard/scoreboard.hpp"
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include "../coverage/uart_cov.hpp"
#endif

class tl_monitor : public uvm::uvm_component {
 public:
  UVM_COMPONENT_UTILS(tl_monitor);
  explicit tl_monitor(uvm::uvm_component_name name) : uvm::uvm_component(name) {}

  // SV-style connection via analysis port/export
  void connect_scoreboard(simple_scoreboard *scb) { scb_ = scb; }

  void build_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_component::build_phase(phase);
    // Register observer to snoop TL transactions
    tl_bind::add_observer([&](uint32_t addr, uint32_t data, bool is_write){ this->on_observe(addr, data, is_write); });
  }

 private:
  void on_observe(uint32_t addr, uint32_t data, bool is_write) {
    if (!scb_) return;
    if (!is_write) on_read(addr, data);
    else on_write(addr, data);
  }
  void on_read(uint32_t addr, uint32_t data) {
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    UartCov::get().sample(addr, data, false);
    // FIFO_STATUS: txlvl[7:0], rxlvl[23:16]
    if (addr == 36) {
      int txlvl = static_cast<int>(data & 0xFFu);
      int rxlvl = static_cast<int>((data >> 16) & 0xFFu);
      UartCov::get().sample_fifo_levels(txlvl, rxlvl, false);
    }
#endif
    if (!scb_) return;
    csr_event ev{addr, data, 0xFFFFFFFFu, false};
    scb_->write(ev);
  }
  void on_write(uint32_t addr, uint32_t data) {
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    UartCov::get().sample(addr, data, true);
    // FIFO_CTRL write: RXILVL[4:2], TXILVL[7:5]
    if (addr == 32) {
      int rxilvl = static_cast<int>((data >> 2) & 0x7u);
      int txilvl = static_cast<int>((data >> 5) & 0x7u);
      UartCov::get().sample_rx_watermark(rxilvl);
      UartCov::get().sample_tx_watermark(txilvl);
    }
#endif
    // SV-style: scoreboard expectations are driven by predictor/test, not raw writes
  }
  simple_scoreboard *scb_ {nullptr};
};

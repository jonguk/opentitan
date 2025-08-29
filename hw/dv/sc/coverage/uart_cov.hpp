#pragma once

#ifdef ENABLE_FC4SC
#include <fc4sc/includes/fc4sc.hpp>
#include <cstdint>

class UartCov : public covergroup {
 public:
  CG_CONS(UartCov) {
    // Crosses between existing coverpoints
    addr_x_write = cross<uint32_t, bool>(this, &addr_cp, &write_cp);
    rx_lvl_x_rst = cross<int, bool>(this, &rx_lvl_cp, &rst_cp);
    tx_lvl_x_rst = cross<int, bool>(this, &tx_lvl_cp, &rst_cp);
    baud_x_clk   = cross<int, int>(this, &baud_cp, &clk_cp);
  }

  static UartCov &get() {
    static UartCov inst;
    return inst;
  }

  // TL observation
  void sample(uint32_t addr, uint32_t data, bool is_write) {
    addr_val = addr;
    write_val = is_write;
    data_lo_val = data & 0xFFu;
    addr_cp.sample();
    write_cp.sample();
    data_lo_cp.sample();
    addr_x_write.sample();
  }

  // FIFO levels with reset
  void sample_fifo_levels(int txlvl, int rxlvl, bool rst) {
    tx_lvl_val = txlvl;
    rx_lvl_val = rxlvl;
    rst_val = rst;
    tx_lvl_cp.sample();
    rx_lvl_cp.sample();
    rst_cp.sample();
    tx_lvl_x_rst.sample();
    rx_lvl_x_rst.sample();
  }

  void sample_baud_clk(int baud, int clk_mhz) {
    baud_val = baud;
    clk_val = clk_mhz;
    baud_cp.sample();
    clk_cp.sample();
    baud_x_clk.sample();
  }

  void sample_tx_watermark(int lvl) { tx_wm_val = lvl; tx_wm_cp.sample(); }
  void sample_rx_watermark(int lvl) { rx_wm_val = lvl; rx_wm_cp.sample(); }
  void sample_break_level(int lvl) { brk_lvl_val = lvl; brk_lvl_cp.sample(); }
  void sample_timeout(int val) { timeout_val = val; timeout_cp.sample(); }
  void sample_parity(bool parity) { parity_val = parity; parity_cp.sample(); }
  void sample_noise(bool rx_sync, bool rx_sync_q1, bool rx_sync_q2) {
    uint32_t code = (rx_sync ? 1u : 0u) | ((rx_sync_q1 ? 1u : 0u) << 1) | ((rx_sync_q2 ? 1u : 0u) << 2);
    noise_val = code;
    noise_cp.sample();
  }

 private:
  // TL
  // sample points
  uint32_t addr_val{};
  bool     write_val{};
  uint32_t data_lo_val{};
  int      tx_lvl_val{};
  int      rx_lvl_val{};
  bool     rst_val{};
  int      tx_wm_val{};
  int      rx_wm_val{};
  int      baud_val{};
  int      clk_val{};
  int      brk_lvl_val{};
  int      timeout_val{};
  bool     parity_val{};
  uint32_t noise_val{};

  // TL
  coverpoint<uint32_t> addr_cp{this, addr_val};
  coverpoint<bool>     write_cp{this, write_val};
  coverpoint<uint32_t> data_lo_cp{this, data_lo_val};
  cross<uint32_t, bool> addr_x_write;
  // FIFO + reset
  coverpoint<int> rx_lvl_cp{this, rx_lvl_val};
  coverpoint<int> tx_lvl_cp{this, tx_lvl_val};
  coverpoint<bool> rst_cp{this, rst_val};
  cross<int, bool> rx_lvl_x_rst;
  cross<int, bool> tx_lvl_x_rst;
  // Watermarks
  coverpoint<int> tx_wm_cp{this, tx_wm_val};
  coverpoint<int> rx_wm_cp{this, rx_wm_val};
  // Baud x clk
  coverpoint<int> baud_cp{this, baud_val};
  coverpoint<int> clk_cp{this, clk_val};
  cross<int, int> baud_x_clk;
  // Break/timeout/parity
  coverpoint<int>  brk_lvl_cp{this, brk_lvl_val};
  coverpoint<int>  timeout_cp{this, timeout_val};
  coverpoint<bool> parity_cp{this, parity_val};
  // Noise
  coverpoint<uint32_t> noise_cp{this, noise_val};
};

#endif // ENABLE_FC4SC



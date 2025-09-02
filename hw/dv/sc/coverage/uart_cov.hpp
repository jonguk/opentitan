#pragma once

#ifdef ENABLE_FC4SC
#include <fc4sc.hpp>
#include <cstdint>

namespace scdv { inline bool coverage_ready = false; }

class UartCov : public covergroup {
 public:
  CG_CONS(UartCov) { }

  static UartCov &get() {
    static UartCov inst;
    return inst;
  }

  // TL observation
  void sample(uint32_t addr, uint32_t data, bool is_write) {
    addr_val = addr;
    write_val = is_write;
    data_lo_val = data & 0xFFu;
    covergroup::sample();
  }

  // FIFO levels with reset
  void sample_fifo_levels(int txlvl, int rxlvl, bool rst) {
    tx_lvl_val = txlvl;
    rx_lvl_val = rxlvl;
    rst_val = rst;
    covergroup::sample();
  }

  void sample_baud_clk(int baud, int clk_mhz) {
    baud_val = baud;
    clk_val = clk_mhz;
    covergroup::sample();
  }

  // RACL flags (enable/error response)
  void sample_racl(bool en, bool err_rsp) {
    racl_en_val = en;
    racl_err_val = err_rsp;
    covergroup::sample();
  }
  void sample_role(uint32_t mask) { role_mask_val = mask; covergroup::sample(); }

  void sample_tx_watermark(int lvl) { tx_wm_val = lvl; covergroup::sample(); }
  void sample_rx_watermark(int lvl) { rx_wm_val = lvl; covergroup::sample(); }
  void sample_break_level(int lvl) { brk_lvl_val = lvl; covergroup::sample(); }
  void sample_timeout(int val) { timeout_val = val; covergroup::sample(); }
  void sample_parity(bool parity) { parity_val = parity; covergroup::sample(); }
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
  bool     racl_en_val{};
  bool     racl_err_val{};
  uint32_t role_mask_val{};

  // TL coverpoints (COVERPOINT macro binds to member sample vars)
  COVERPOINT(uint32_t, addr_cp, addr_val) {
    // UART register offsets (word addressed)
    bin<uint32_t>("intr_state",    fc4sc::interval<uint32_t>(0u, 0u)),
    bin<uint32_t>("intr_enable",   fc4sc::interval<uint32_t>(4u, 4u)),
    bin<uint32_t>("intr_test",     fc4sc::interval<uint32_t>(8u, 8u)),
    bin<uint32_t>("alert_test",    fc4sc::interval<uint32_t>(12u, 12u)),
    bin<uint32_t>("ctrl",          fc4sc::interval<uint32_t>(16u, 16u)),
    bin<uint32_t>("status",        fc4sc::interval<uint32_t>(20u, 20u)),
    bin<uint32_t>("rdata",         fc4sc::interval<uint32_t>(24u, 24u)),
    bin<uint32_t>("wdata",         fc4sc::interval<uint32_t>(28u, 28u)),
    bin<uint32_t>("fifo_ctrl",     fc4sc::interval<uint32_t>(32u, 32u)),
    bin<uint32_t>("fifo_status",   fc4sc::interval<uint32_t>(36u, 36u)),
    bin<uint32_t>("ovrd",          fc4sc::interval<uint32_t>(40u, 40u)),
    bin<uint32_t>("val",           fc4sc::interval<uint32_t>(44u, 44u)),
    bin<uint32_t>("timeout_ctrl",  fc4sc::interval<uint32_t>(48u, 48u))
  };
  COVERPOINT(bool, rw_cp, write_val) {
    bin<bool>("read",  fc4sc::interval<bool>(0, 0)),
    bin<bool>("write", fc4sc::interval<bool>(1, 1))
  };
  COVERPOINT(uint32_t, data_lo_cp, data_lo_val) {
    bin<uint32_t>("data_lo_all", fc4sc::interval<uint32_t>(0u, 0xFFu))
  };
  cross<uint32_t, bool> addr_x_rw = cross<uint32_t, bool>(this, "addr_x_rw", &addr_cp, &rw_cp);
  // FIFO + reset
  COVERPOINT(int, rx_lvl_cp, rx_lvl_val) {
    bin<int>("rxlvl_all", fc4sc::interval<int>(0, 255))
  };
  COVERPOINT(int, tx_lvl_cp, tx_lvl_val) {
    bin<int>("txlvl_all", fc4sc::interval<int>(0, 255))
  };
  COVERPOINT(bool, rst_cp, rst_val) {
    bin<bool>("rst_all", fc4sc::interval<bool>(0, 1))
  };
  cross<int, bool> rx_lvl_x_rst = cross<int, bool>(this, "rx_lvl_x_rst", &rx_lvl_cp, &rst_cp);
  cross<int, bool> tx_lvl_x_rst = cross<int, bool>(this, "tx_lvl_x_rst", &tx_lvl_cp, &rst_cp);
  // Watermarks
  COVERPOINT(int, tx_wm_cp, tx_wm_val) {
    bin<int>("txwm_all", fc4sc::interval<int>(0, 7))
  };
  COVERPOINT(int, rx_wm_cp, rx_wm_val) {
    bin<int>("rxwm_all", fc4sc::interval<int>(0, 7))
  };
  // Baud x clk
  COVERPOINT(int, baud_cp, baud_val) {
    bin<int>("baud_all", fc4sc::interval<int>(0, 100000000))
  };
  COVERPOINT(int, clk_cp, clk_val) {
    bin<int>("clk_all", fc4sc::interval<int>(0, 1000))
  };
  cross<int, int> baud_x_clk = cross<int, int>(this, "baud_x_clk", &baud_cp, &clk_cp);
  // Break/timeout/parity
  COVERPOINT(int,  brk_lvl_cp, brk_lvl_val) {
    bin<int>("brk_all", fc4sc::interval<int>(0, 255))
  };
  COVERPOINT(int,  timeout_cp,  timeout_val) {
    bin<int>("tout_all", fc4sc::interval<int>(0, 0x7FFFFFFF))
  };
  COVERPOINT(bool, parity_cp, parity_val) {
    bin<bool>("parity_all", fc4sc::interval<bool>(0, 1))
  };
  // Noise
  COVERPOINT(uint32_t, noise_cp, noise_val) {
    bin<uint32_t>("noise_all", fc4sc::interval<uint32_t>(0u, 7u))
  };

  // RACL coverpoints
  COVERPOINT(bool, racl_en_cp, racl_en_val) {
    bin<bool>("disabled", fc4sc::interval<bool>(0, 0)),
    bin<bool>("enabled",  fc4sc::interval<bool>(1, 1))
  };
  COVERPOINT(bool, racl_err_cp, racl_err_val) {
    bin<bool>("no_error", fc4sc::interval<bool>(0, 0)),
    bin<bool>("error",    fc4sc::interval<bool>(1, 1))
  };
  COVERPOINT(uint32_t, racl_role_cp, role_mask_val) {
    bin<uint32_t>("none", fc4sc::interval<uint32_t>(0u, 0u)),
    bin<uint32_t>("nonzero", fc4sc::interval<uint32_t>(1u, 0xFFFFFFFFu))
  };
  cross<bool, bool> rw_x_racl_en = cross<bool, bool>(this, "rw_x_racl_en", &rw_cp, &racl_en_cp);
  cross<uint32_t, bool> addr_x_racl_en = cross<uint32_t, bool>(this, "addr_x_racl_en", &addr_cp, &racl_en_cp);
  cross<uint32_t, bool, bool> addr_x_rw_x_racl_err = cross<uint32_t, bool, bool>(this, "addr_x_rw_x_racl_err", &addr_cp, &rw_cp, &racl_err_cp);
};

#endif // ENABLE_FC4SC



// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0




#pragma once

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <cstdint>
#include <array>

namespace opentitan {
namespace uart {

// ----- Types mirrored from uart_reg_pkg.sv -----


class uart_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<uart_reg_top> tlm_target;

  SC_HAS_PROCESS(uart_reg_top);
  explicit uart_reg_top(sc_core::sc_module_name name);

  // Register reset helper
  void reset();

  // Soft-access helpers for DV: bypass TLM socket to read/predict mirror
  void sw_predict_write(uint32_t addr, uint32_t data);
  uint32_t sw_read(uint32_t addr);

  // RACL controls (optional; default disabled and all allowed)
  void set_racl_enable(bool enable) { enable_racl_ = enable; }
  void set_racl_error_response(bool enable) { racl_error_rsp_ = enable; }
  void set_role_mask(uint32_t role_mask) { role_mask_ = role_mask; }
  void set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write);

  // Expose total register bytes for DV convenience
  static constexpr std::size_t kRegBytes = 52;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_default_e : uint32_t {
    INTR_STATE = 0,
    INTR_ENABLE = 4,
    INTR_TEST = 8,
    ALERT_TEST = 12,
    CTRL = 16,
    STATUS = 20,
    RDATA = 24,
    WDATA = 28,
    FIFO_CTRL = 32,
    FIFO_STATUS = 36,
    OVRD = 40,
    VAL = 44,
    TIMEOUT_CTRL = 48,
  };

  // Aggregate structs mirroring uart_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct uart_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } tx_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_parity_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_break_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_frame_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_done;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_watermark;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_watermark;
  };

  struct uart_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } tx_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_parity_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_break_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_frame_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_done;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_watermark;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_watermark;
  };

  struct uart_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } tx_empty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_parity_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_timeout;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_break_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_frame_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_overflow;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } tx_done;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_watermark;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } tx_watermark;
  };

  struct uart_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct uart_reg2hw_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
    } nco;
    struct {
      sc_dt::sc_bv<2> q;
    } rxblvl;
    struct {
      sc_dt::sc_bv<1> q;
    } parity_odd;
    struct {
      sc_dt::sc_bv<1> q;
    } parity_en;
    struct {
      sc_dt::sc_bv<1> q;
    } llpbk;
    struct {
      sc_dt::sc_bv<1> q;
    } slpbk;
    struct {
      sc_dt::sc_bv<1> q;
    } nf;
    struct {
      sc_dt::sc_bv<1> q;
    } rx;
    struct {
      sc_dt::sc_bv<1> q;
    } tx;
  };

  struct uart_reg2hw_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } rxempty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } rxidle;
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } txidle;
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } txempty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } rxfull;
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } txfull;
  };

  struct uart_reg2hw_rdata_reg_t {
    sc_dt::sc_bv<8> q;
    bool re;
  };

  struct uart_reg2hw_wdata_reg_t {
    sc_dt::sc_bv<8> q;
    bool qe;
  };

  struct uart_reg2hw_fifo_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<3> q;
      bool        qe;
    } txilvl;
    struct {
      sc_dt::sc_bv<3> q;
      bool        qe;
    } rxilvl;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } txrst;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rxrst;
  };

  struct uart_reg2hw_ovrd_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } txval;
    struct {
      sc_dt::sc_bv<1> q;
    } txen;
  };

  struct uart_reg2hw_timeout_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } en;
    struct {
      sc_dt::sc_bv<24> q;
    } val;
  };

  struct uart_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tx_empty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_parity_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_timeout;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_break_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_frame_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_overflow;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tx_done;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_watermark;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tx_watermark;
  };

  struct uart_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } rxempty;
    struct {
      sc_dt::sc_bv<1> d;
    } rxidle;
    struct {
      sc_dt::sc_bv<1> d;
    } txidle;
    struct {
      sc_dt::sc_bv<1> d;
    } txempty;
    struct {
      sc_dt::sc_bv<1> d;
    } rxfull;
    struct {
      sc_dt::sc_bv<1> d;
    } txfull;
  };

  struct uart_hw2reg_rdata_reg_t {
    sc_dt::sc_bv<8> d;
  };

  struct uart_hw2reg_fifo_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } txilvl;
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } rxilvl;
  };

  struct uart_hw2reg_fifo_status_reg_t {
    struct {
      sc_dt::sc_bv<8> d;
    } rxlvl;
    struct {
      sc_dt::sc_bv<8> d;
    } txlvl;
  };

  struct uart_hw2reg_val_reg_t {
    sc_dt::sc_bv<16> d;
  };

  // Register -> HW type
  struct uart_reg2hw_t {
    uart_reg2hw_intr_state_reg_t intr_state;
    uart_reg2hw_intr_enable_reg_t intr_enable;
    uart_reg2hw_intr_test_reg_t intr_test;
    uart_reg2hw_alert_test_reg_t alert_test;
    uart_reg2hw_ctrl_reg_t ctrl;
    uart_reg2hw_status_reg_t status;
    uart_reg2hw_rdata_reg_t rdata;
    uart_reg2hw_wdata_reg_t wdata;
    uart_reg2hw_fifo_ctrl_reg_t fifo_ctrl;
    uart_reg2hw_ovrd_reg_t ovrd;
    uart_reg2hw_timeout_ctrl_reg_t timeout_ctrl;
  };

  // HW -> register type
  struct uart_hw2reg_t {
    uart_hw2reg_intr_state_reg_t intr_state;
    uart_hw2reg_status_reg_t status;
    uart_hw2reg_rdata_reg_t rdata;
    uart_hw2reg_fifo_ctrl_reg_t fifo_ctrl;
    uart_hw2reg_fifo_status_reg_t fifo_status;
    uart_hw2reg_val_reg_t val;
  };

  // Flat register storage
  std::array<uint32_t, 13> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 13> shadow_stage_default_{};
  std::array<uint8_t,  13> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 13> racl_allow_rd_default_{};
  std::array<bool, 13> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata, uint32_t be_mask);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  uart_reg2hw_t reg2hw;
  uart_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace uart
}  // namespace opentitan



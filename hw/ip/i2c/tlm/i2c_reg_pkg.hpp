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
namespace i2c {

// ----- Types mirrored from i2c_reg_pkg.sv -----


class i2c_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<i2c_reg_top> tlm_target;

  SC_HAS_PROCESS(i2c_reg_top);
  explicit i2c_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 128;

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
    FDATA = 28,
    FIFO_CTRL = 32,
    HOST_FIFO_CONFIG = 36,
    TARGET_FIFO_CONFIG = 40,
    HOST_FIFO_STATUS = 44,
    TARGET_FIFO_STATUS = 48,
    OVRD = 52,
    VAL = 56,
    TIMING0 = 60,
    TIMING1 = 64,
    TIMING2 = 68,
    TIMING3 = 72,
    TIMING4 = 76,
    TIMEOUT_CTRL = 80,
    TARGET_ID = 84,
    ACQDATA = 88,
    TXDATA = 92,
    HOST_TIMEOUT_CTRL = 96,
    TARGET_TIMEOUT_CTRL = 100,
    TARGET_NACK_COUNT = 104,
    TARGET_ACK_CTRL = 108,
    ACQ_FIFO_NEXT_DATA = 112,
    HOST_NACK_HANDLER_TIMEOUT = 116,
    CONTROLLER_EVENTS = 120,
    TARGET_EVENTS = 124,
  };

  // Aggregate structs mirroring i2c_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct i2c_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } host_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } unexp_stop;
    struct {
      sc_dt::sc_bv<1> q;
    } acq_stretch;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_threshold;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_stretch;
    struct {
      sc_dt::sc_bv<1> q;
    } cmd_complete;
    struct {
      sc_dt::sc_bv<1> q;
    } sda_unstable;
    struct {
      sc_dt::sc_bv<1> q;
    } stretch_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } sda_interference;
    struct {
      sc_dt::sc_bv<1> q;
    } scl_interference;
    struct {
      sc_dt::sc_bv<1> q;
    } controller_halt;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } acq_threshold;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_threshold;
    struct {
      sc_dt::sc_bv<1> q;
    } fmt_threshold;
  };

  struct i2c_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } host_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } unexp_stop;
    struct {
      sc_dt::sc_bv<1> q;
    } acq_stretch;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_threshold;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_stretch;
    struct {
      sc_dt::sc_bv<1> q;
    } cmd_complete;
    struct {
      sc_dt::sc_bv<1> q;
    } sda_unstable;
    struct {
      sc_dt::sc_bv<1> q;
    } stretch_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } sda_interference;
    struct {
      sc_dt::sc_bv<1> q;
    } scl_interference;
    struct {
      sc_dt::sc_bv<1> q;
    } controller_halt;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } acq_threshold;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_threshold;
    struct {
      sc_dt::sc_bv<1> q;
    } fmt_threshold;
  };

  struct i2c_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } host_timeout;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } unexp_stop;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } acq_stretch;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } tx_threshold;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } tx_stretch;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } cmd_complete;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } sda_unstable;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } stretch_timeout;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } sda_interference;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } scl_interference;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } controller_halt;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_overflow;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } acq_threshold;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_threshold;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fmt_threshold;
  };

  struct i2c_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct i2c_reg2hw_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } tx_stretch_ctrl_en;
    struct {
      sc_dt::sc_bv<1> q;
    } multi_controller_monitor_en;
    struct {
      sc_dt::sc_bv<1> q;
    } ack_ctrl_en;
    struct {
      sc_dt::sc_bv<1> q;
    } nack_addr_after_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } llpbk;
    struct {
      sc_dt::sc_bv<1> q;
    } enabletarget;
    struct {
      sc_dt::sc_bv<1> q;
    } enablehost;
  };

  struct i2c_reg2hw_rdata_reg_t {
    sc_dt::sc_bv<8> q;
    bool re;
  };

  struct i2c_reg2hw_fdata_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } nakok;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rcont;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } readb;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } stop;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } start;
    struct {
      sc_dt::sc_bv<8> q;
      bool        qe;
    } fbyte;
  };

  struct i2c_reg2hw_fifo_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } txrst;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } acqrst;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fmtrst;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rxrst;
  };

  struct i2c_reg2hw_host_fifo_config_reg_t {
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } fmt_thresh;
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } rx_thresh;
  };

  struct i2c_reg2hw_target_fifo_config_reg_t {
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } acq_thresh;
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } tx_thresh;
  };

  struct i2c_reg2hw_ovrd_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } sdaval;
    struct {
      sc_dt::sc_bv<1> q;
    } sclval;
    struct {
      sc_dt::sc_bv<1> q;
    } txovrden;
  };

  struct i2c_reg2hw_timing0_reg_t {
    struct {
      sc_dt::sc_bv<13> q;
    } tlow;
    struct {
      sc_dt::sc_bv<13> q;
    } thigh;
  };

  struct i2c_reg2hw_timing1_reg_t {
    struct {
      sc_dt::sc_bv<9> q;
    } t_f;
    struct {
      sc_dt::sc_bv<10> q;
    } t_r;
  };

  struct i2c_reg2hw_timing2_reg_t {
    struct {
      sc_dt::sc_bv<13> q;
    } thd_sta;
    struct {
      sc_dt::sc_bv<13> q;
    } tsu_sta;
  };

  struct i2c_reg2hw_timing3_reg_t {
    struct {
      sc_dt::sc_bv<13> q;
    } thd_dat;
    struct {
      sc_dt::sc_bv<9> q;
    } tsu_dat;
  };

  struct i2c_reg2hw_timing4_reg_t {
    struct {
      sc_dt::sc_bv<13> q;
    } t_buf;
    struct {
      sc_dt::sc_bv<13> q;
    } tsu_sto;
  };

  struct i2c_reg2hw_timeout_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } en;
    struct {
      sc_dt::sc_bv<1> q;
    } mode;
    struct {
      sc_dt::sc_bv<30> q;
    } val;
  };

  struct i2c_reg2hw_target_id_reg_t {
    struct {
      sc_dt::sc_bv<7> q;
    } mask1;
    struct {
      sc_dt::sc_bv<7> q;
    } address1;
    struct {
      sc_dt::sc_bv<7> q;
    } mask0;
    struct {
      sc_dt::sc_bv<7> q;
    } address0;
  };

  struct i2c_reg2hw_acqdata_reg_t {
    struct {
      sc_dt::sc_bv<3> q;
      bool        re;
    } signal;
    struct {
      sc_dt::sc_bv<8> q;
      bool        re;
    } abyte;
  };

  struct i2c_reg2hw_txdata_reg_t {
    sc_dt::sc_bv<8> q;
    bool qe;
  };

  struct i2c_reg2hw_host_timeout_ctrl_reg_t {
    sc_dt::sc_bv<20> q;
  };

  struct i2c_reg2hw_target_timeout_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } en;
    struct {
      sc_dt::sc_bv<31> q;
    } val;
  };

  struct i2c_reg2hw_target_nack_count_reg_t {
    sc_dt::sc_bv<8> q;
  };

  struct i2c_reg2hw_target_ack_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } nack;
    struct {
      sc_dt::sc_bv<9> q;
      bool        qe;
    } nbytes;
  };

  struct i2c_reg2hw_host_nack_handler_timeout_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } en;
    struct {
      sc_dt::sc_bv<31> q;
    } val;
  };

  struct i2c_reg2hw_controller_events_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } arbitration_lost;
    struct {
      sc_dt::sc_bv<1> q;
    } bus_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } unhandled_nack_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } nack;
  };

  struct i2c_reg2hw_target_events_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } arbitration_lost;
    struct {
      sc_dt::sc_bv<1> q;
    } bus_timeout;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_pending;
  };

  struct i2c_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } host_timeout;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } unexp_stop;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } acq_stretch;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tx_threshold;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tx_stretch;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_complete;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sda_unstable;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } stretch_timeout;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sda_interference;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } scl_interference;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } controller_halt;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_overflow;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } acq_threshold;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_threshold;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fmt_threshold;
  };

  struct i2c_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } ack_ctrl_stretch;
    struct {
      sc_dt::sc_bv<1> d;
    } acqempty;
    struct {
      sc_dt::sc_bv<1> d;
    } txempty;
    struct {
      sc_dt::sc_bv<1> d;
    } acqfull;
    struct {
      sc_dt::sc_bv<1> d;
    } txfull;
    struct {
      sc_dt::sc_bv<1> d;
    } rxempty;
    struct {
      sc_dt::sc_bv<1> d;
    } targetidle;
    struct {
      sc_dt::sc_bv<1> d;
    } hostidle;
    struct {
      sc_dt::sc_bv<1> d;
    } fmtempty;
    struct {
      sc_dt::sc_bv<1> d;
    } rxfull;
    struct {
      sc_dt::sc_bv<1> d;
    } fmtfull;
  };

  struct i2c_hw2reg_rdata_reg_t {
    sc_dt::sc_bv<8> d;
  };

  struct i2c_hw2reg_host_fifo_status_reg_t {
    struct {
      sc_dt::sc_bv<12> d;
    } rxlvl;
    struct {
      sc_dt::sc_bv<12> d;
    } fmtlvl;
  };

  struct i2c_hw2reg_target_fifo_status_reg_t {
    struct {
      sc_dt::sc_bv<12> d;
    } acqlvl;
    struct {
      sc_dt::sc_bv<12> d;
    } txlvl;
  };

  struct i2c_hw2reg_val_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } sda_rx;
    struct {
      sc_dt::sc_bv<16> d;
    } scl_rx;
  };

  struct i2c_hw2reg_acqdata_reg_t {
    struct {
      sc_dt::sc_bv<3> d;
    } signal;
    struct {
      sc_dt::sc_bv<8> d;
    } abyte;
  };

  struct i2c_hw2reg_target_nack_count_reg_t {
    sc_dt::sc_bv<8> d;
    bool        de;
  };

  struct i2c_hw2reg_target_ack_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<9> d;
    } nbytes;
  };

  struct i2c_hw2reg_acq_fifo_next_data_reg_t {
    sc_dt::sc_bv<8> d;
  };

  struct i2c_hw2reg_controller_events_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } arbitration_lost;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } bus_timeout;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } unhandled_nack_timeout;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } nack;
  };

  struct i2c_hw2reg_target_events_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } arbitration_lost;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } bus_timeout;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tx_pending;
  };

  // Register -> HW type
  struct i2c_reg2hw_t {
    i2c_reg2hw_intr_state_reg_t intr_state;
    i2c_reg2hw_intr_enable_reg_t intr_enable;
    i2c_reg2hw_intr_test_reg_t intr_test;
    i2c_reg2hw_alert_test_reg_t alert_test;
    i2c_reg2hw_ctrl_reg_t ctrl;
    i2c_reg2hw_rdata_reg_t rdata;
    i2c_reg2hw_fdata_reg_t fdata;
    i2c_reg2hw_fifo_ctrl_reg_t fifo_ctrl;
    i2c_reg2hw_host_fifo_config_reg_t host_fifo_config;
    i2c_reg2hw_target_fifo_config_reg_t target_fifo_config;
    i2c_reg2hw_ovrd_reg_t ovrd;
    i2c_reg2hw_timing0_reg_t timing0;
    i2c_reg2hw_timing1_reg_t timing1;
    i2c_reg2hw_timing2_reg_t timing2;
    i2c_reg2hw_timing3_reg_t timing3;
    i2c_reg2hw_timing4_reg_t timing4;
    i2c_reg2hw_timeout_ctrl_reg_t timeout_ctrl;
    i2c_reg2hw_target_id_reg_t target_id;
    i2c_reg2hw_acqdata_reg_t acqdata;
    i2c_reg2hw_txdata_reg_t txdata;
    i2c_reg2hw_host_timeout_ctrl_reg_t host_timeout_ctrl;
    i2c_reg2hw_target_timeout_ctrl_reg_t target_timeout_ctrl;
    i2c_reg2hw_target_nack_count_reg_t target_nack_count;
    i2c_reg2hw_target_ack_ctrl_reg_t target_ack_ctrl;
    i2c_reg2hw_host_nack_handler_timeout_reg_t host_nack_handler_timeout;
    i2c_reg2hw_controller_events_reg_t controller_events;
    i2c_reg2hw_target_events_reg_t target_events;
  };

  // HW -> register type
  struct i2c_hw2reg_t {
    i2c_hw2reg_intr_state_reg_t intr_state;
    i2c_hw2reg_status_reg_t status;
    i2c_hw2reg_rdata_reg_t rdata;
    i2c_hw2reg_host_fifo_status_reg_t host_fifo_status;
    i2c_hw2reg_target_fifo_status_reg_t target_fifo_status;
    i2c_hw2reg_val_reg_t val;
    i2c_hw2reg_acqdata_reg_t acqdata;
    i2c_hw2reg_target_nack_count_reg_t target_nack_count;
    i2c_hw2reg_target_ack_ctrl_reg_t target_ack_ctrl;
    i2c_hw2reg_acq_fifo_next_data_reg_t acq_fifo_next_data;
    i2c_hw2reg_controller_events_reg_t controller_events;
    i2c_hw2reg_target_events_reg_t target_events;
  };

  // Flat register storage
  std::array<uint32_t, 32> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 32> shadow_stage_default_{};
  std::array<uint8_t,  32> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 32> racl_allow_rd_default_{};
  std::array<bool, 32> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata, uint32_t be_mask);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  i2c_reg2hw_t reg2hw;
  i2c_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace i2c
}  // namespace opentitan



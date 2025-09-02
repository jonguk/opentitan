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
namespace entropy_src {

// ----- Types mirrored from entropy_src_reg_pkg.sv -----


class entropy_src_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<entropy_src_reg_top> tlm_target;

  SC_HAS_PROCESS(entropy_src_reg_top);
  explicit entropy_src_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 224;

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
    ME_REGWEN = 16,
    SW_REGUPD = 20,
    REGWEN = 24,
    MODULE_ENABLE = 28,
    CONF = 32,
    ENTROPY_CONTROL = 36,
    ENTROPY_DATA = 40,
    HEALTH_TEST_WINDOWS = 44,
    REPCNT_THRESHOLDS = 48,
    REPCNTS_THRESHOLDS = 52,
    ADAPTP_HI_THRESHOLDS = 56,
    ADAPTP_LO_THRESHOLDS = 60,
    BUCKET_THRESHOLDS = 64,
    MARKOV_HI_THRESHOLDS = 68,
    MARKOV_LO_THRESHOLDS = 72,
    EXTHT_HI_THRESHOLDS = 76,
    EXTHT_LO_THRESHOLDS = 80,
    REPCNT_HI_WATERMARKS = 84,
    REPCNTS_HI_WATERMARKS = 88,
    ADAPTP_HI_WATERMARKS = 92,
    ADAPTP_LO_WATERMARKS = 96,
    EXTHT_HI_WATERMARKS = 100,
    EXTHT_LO_WATERMARKS = 104,
    BUCKET_HI_WATERMARKS = 108,
    MARKOV_HI_WATERMARKS = 112,
    MARKOV_LO_WATERMARKS = 116,
    REPCNT_TOTAL_FAILS = 120,
    REPCNTS_TOTAL_FAILS = 124,
    ADAPTP_HI_TOTAL_FAILS = 128,
    ADAPTP_LO_TOTAL_FAILS = 132,
    BUCKET_TOTAL_FAILS = 136,
    MARKOV_HI_TOTAL_FAILS = 140,
    MARKOV_LO_TOTAL_FAILS = 144,
    EXTHT_HI_TOTAL_FAILS = 148,
    EXTHT_LO_TOTAL_FAILS = 152,
    ALERT_THRESHOLD = 156,
    ALERT_SUMMARY_FAIL_COUNTS = 160,
    ALERT_FAIL_COUNTS = 164,
    EXTHT_FAIL_COUNTS = 168,
    FW_OV_CONTROL = 172,
    FW_OV_SHA3_START = 176,
    FW_OV_WR_FIFO_FULL = 180,
    FW_OV_RD_FIFO_OVERFLOW = 184,
    FW_OV_RD_DATA = 188,
    FW_OV_WR_DATA = 192,
    OBSERVE_FIFO_THRESH = 196,
    OBSERVE_FIFO_DEPTH = 200,
    DEBUG_STATUS = 204,
    RECOV_ALERT_STS = 208,
    ERR_CODE = 212,
    ERR_CODE_TEST = 216,
    MAIN_SM_STATE = 220,
  };

  // Aggregate structs mirroring entropy_src_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct entropy_src_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } es_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
    } es_observe_fifo_ready;
    struct {
      sc_dt::sc_bv<1> q;
    } es_health_test_failed;
    struct {
      sc_dt::sc_bv<1> q;
    } es_entropy_valid;
  };

  struct entropy_src_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } es_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
    } es_observe_fifo_ready;
    struct {
      sc_dt::sc_bv<1> q;
    } es_health_test_failed;
    struct {
      sc_dt::sc_bv<1> q;
    } es_entropy_valid;
  };

  struct entropy_src_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } es_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } es_observe_fifo_ready;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } es_health_test_failed;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } es_entropy_valid;
  };

  struct entropy_src_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_alert;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_alert;
  };

  struct entropy_src_reg2hw_sw_regupd_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct entropy_src_reg2hw_module_enable_reg_t {
    sc_dt::sc_bv<4> q;
  };

  struct entropy_src_reg2hw_conf_reg_t {
    struct {
      sc_dt::sc_bv<8> q;
    } rng_bit_sel;
    struct {
      sc_dt::sc_bv<4> q;
    } entropy_data_reg_enable;
    struct {
      sc_dt::sc_bv<4> q;
    } threshold_scope;
    struct {
      sc_dt::sc_bv<4> q;
    } rng_bit_enable;
    struct {
      sc_dt::sc_bv<4> q;
    } rng_fips;
    struct {
      sc_dt::sc_bv<4> q;
    } fips_flag;
    struct {
      sc_dt::sc_bv<4> q;
    } fips_enable;
  };

  struct entropy_src_reg2hw_entropy_control_reg_t {
    struct {
      sc_dt::sc_bv<4> q;
    } es_type;
    struct {
      sc_dt::sc_bv<4> q;
    } es_route;
  };

  struct entropy_src_reg2hw_entropy_data_reg_t {
    sc_dt::sc_bv<32> q;
    bool re;
  };

  struct entropy_src_reg2hw_health_test_windows_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
    } bypass_window;
    struct {
      sc_dt::sc_bv<16> q;
    } fips_window;
  };

  struct entropy_src_reg2hw_repcnt_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_repcnts_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_adaptp_hi_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_adaptp_lo_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_bucket_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_markov_hi_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_markov_lo_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_extht_hi_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_extht_lo_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> q;
      bool        qe;
    } fips_thresh;
  };

  struct entropy_src_reg2hw_alert_threshold_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
    } alert_threshold_inv;
    struct {
      sc_dt::sc_bv<16> q;
    } alert_threshold;
  };

  struct entropy_src_reg2hw_fw_ov_control_reg_t {
    struct {
      sc_dt::sc_bv<4> q;
    } fw_ov_entropy_insert;
    struct {
      sc_dt::sc_bv<4> q;
    } fw_ov_mode;
  };

  struct entropy_src_reg2hw_fw_ov_sha3_start_reg_t {
    sc_dt::sc_bv<4> q;
  };

  struct entropy_src_reg2hw_fw_ov_rd_data_reg_t {
    sc_dt::sc_bv<32> q;
    bool re;
  };

  struct entropy_src_reg2hw_fw_ov_wr_data_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct entropy_src_reg2hw_observe_fifo_thresh_reg_t {
    sc_dt::sc_bv<6> q;
  };

  struct entropy_src_reg2hw_err_code_test_reg_t {
    sc_dt::sc_bv<5> q;
    bool qe;
  };

  struct entropy_src_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_fatal_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_observe_fifo_ready;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_health_test_failed;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_entropy_valid;
  };

  struct entropy_src_hw2reg_regwen_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct entropy_src_hw2reg_entropy_data_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_repcnt_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_repcnts_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_adaptp_hi_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_adaptp_lo_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_bucket_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_markov_hi_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_markov_lo_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_extht_hi_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_extht_lo_thresholds_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_thresh;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_thresh;
  };

  struct entropy_src_hw2reg_repcnt_hi_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_repcnts_hi_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_adaptp_hi_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_adaptp_lo_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_extht_hi_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_extht_lo_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_bucket_hi_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_markov_hi_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_markov_lo_watermarks_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } bypass_watermark;
    struct {
      sc_dt::sc_bv<16> d;
    } fips_watermark;
  };

  struct entropy_src_hw2reg_repcnt_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_repcnts_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_adaptp_hi_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_adaptp_lo_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_bucket_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_markov_hi_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_markov_lo_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_extht_hi_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_extht_lo_total_fails_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_alert_summary_fail_counts_reg_t {
    sc_dt::sc_bv<16> d;
  };

  struct entropy_src_hw2reg_alert_fail_counts_reg_t {
    struct {
      sc_dt::sc_bv<4> d;
    } repcnts_fail_count;
    struct {
      sc_dt::sc_bv<4> d;
    } markov_lo_fail_count;
    struct {
      sc_dt::sc_bv<4> d;
    } markov_hi_fail_count;
    struct {
      sc_dt::sc_bv<4> d;
    } bucket_fail_count;
    struct {
      sc_dt::sc_bv<4> d;
    } adaptp_lo_fail_count;
    struct {
      sc_dt::sc_bv<4> d;
    } adaptp_hi_fail_count;
    struct {
      sc_dt::sc_bv<4> d;
    } repcnt_fail_count;
  };

  struct entropy_src_hw2reg_extht_fail_counts_reg_t {
    struct {
      sc_dt::sc_bv<4> d;
    } extht_lo_fail_count;
    struct {
      sc_dt::sc_bv<4> d;
    } extht_hi_fail_count;
  };

  struct entropy_src_hw2reg_fw_ov_wr_fifo_full_reg_t {
    sc_dt::sc_bv<1> d;
  };

  struct entropy_src_hw2reg_fw_ov_rd_fifo_overflow_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct entropy_src_hw2reg_fw_ov_rd_data_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct entropy_src_hw2reg_observe_fifo_depth_reg_t {
    sc_dt::sc_bv<6> d;
  };

  struct entropy_src_hw2reg_debug_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } main_sm_boot_done;
    struct {
      sc_dt::sc_bv<1> d;
    } main_sm_idle;
    struct {
      sc_dt::sc_bv<1> d;
    } sha3_err;
    struct {
      sc_dt::sc_bv<1> d;
    } sha3_absorbed;
    struct {
      sc_dt::sc_bv<1> d;
    } sha3_squeezing;
    struct {
      sc_dt::sc_bv<1> d;
    } sha3_block_pr;
    struct {
      sc_dt::sc_bv<3> d;
    } sha3_fsm;
    struct {
      sc_dt::sc_bv<2> d;
    } entropy_fifo_depth;
  };

  struct entropy_src_hw2reg_recov_alert_sts_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } postht_entropy_drop_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rng_fips_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fips_flag_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_fw_ov_disable_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_fw_ov_wr_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_thresh_cfg_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_bus_cmp_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_main_sm_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_type_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_route_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fw_ov_entropy_insert_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fw_ov_mode_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fw_ov_sha3_start_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rng_bit_enable_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } threshold_scope_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } module_enable_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } entropy_data_reg_en_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fips_enable_field_alert;
  };

  struct entropy_src_hw2reg_err_code_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fifo_state_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fifo_read_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fifo_write_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sha3_rst_storage_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sha3_state_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_cntr_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_main_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } es_ack_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_esfinal_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_observe_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_distr_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_esrng_err;
  };

  struct entropy_src_hw2reg_main_sm_state_reg_t {
    sc_dt::sc_bv<9> d;
    bool        de;
  };

  // Register -> HW type
  struct entropy_src_reg2hw_t {
    entropy_src_reg2hw_intr_state_reg_t intr_state;
    entropy_src_reg2hw_intr_enable_reg_t intr_enable;
    entropy_src_reg2hw_intr_test_reg_t intr_test;
    entropy_src_reg2hw_alert_test_reg_t alert_test;
    entropy_src_reg2hw_sw_regupd_reg_t sw_regupd;
    entropy_src_reg2hw_module_enable_reg_t module_enable;
    entropy_src_reg2hw_conf_reg_t conf;
    entropy_src_reg2hw_entropy_control_reg_t entropy_control;
    entropy_src_reg2hw_entropy_data_reg_t entropy_data;
    entropy_src_reg2hw_health_test_windows_reg_t health_test_windows;
    entropy_src_reg2hw_repcnt_thresholds_reg_t repcnt_thresholds;
    entropy_src_reg2hw_repcnts_thresholds_reg_t repcnts_thresholds;
    entropy_src_reg2hw_adaptp_hi_thresholds_reg_t adaptp_hi_thresholds;
    entropy_src_reg2hw_adaptp_lo_thresholds_reg_t adaptp_lo_thresholds;
    entropy_src_reg2hw_bucket_thresholds_reg_t bucket_thresholds;
    entropy_src_reg2hw_markov_hi_thresholds_reg_t markov_hi_thresholds;
    entropy_src_reg2hw_markov_lo_thresholds_reg_t markov_lo_thresholds;
    entropy_src_reg2hw_extht_hi_thresholds_reg_t extht_hi_thresholds;
    entropy_src_reg2hw_extht_lo_thresholds_reg_t extht_lo_thresholds;
    entropy_src_reg2hw_alert_threshold_reg_t alert_threshold;
    entropy_src_reg2hw_fw_ov_control_reg_t fw_ov_control;
    entropy_src_reg2hw_fw_ov_sha3_start_reg_t fw_ov_sha3_start;
    entropy_src_reg2hw_fw_ov_rd_data_reg_t fw_ov_rd_data;
    entropy_src_reg2hw_fw_ov_wr_data_reg_t fw_ov_wr_data;
    entropy_src_reg2hw_observe_fifo_thresh_reg_t observe_fifo_thresh;
    entropy_src_reg2hw_err_code_test_reg_t err_code_test;
  };

  // HW -> register type
  struct entropy_src_hw2reg_t {
    entropy_src_hw2reg_intr_state_reg_t intr_state;
    entropy_src_hw2reg_regwen_reg_t regwen;
    entropy_src_hw2reg_entropy_data_reg_t entropy_data;
    entropy_src_hw2reg_repcnt_thresholds_reg_t repcnt_thresholds;
    entropy_src_hw2reg_repcnts_thresholds_reg_t repcnts_thresholds;
    entropy_src_hw2reg_adaptp_hi_thresholds_reg_t adaptp_hi_thresholds;
    entropy_src_hw2reg_adaptp_lo_thresholds_reg_t adaptp_lo_thresholds;
    entropy_src_hw2reg_bucket_thresholds_reg_t bucket_thresholds;
    entropy_src_hw2reg_markov_hi_thresholds_reg_t markov_hi_thresholds;
    entropy_src_hw2reg_markov_lo_thresholds_reg_t markov_lo_thresholds;
    entropy_src_hw2reg_extht_hi_thresholds_reg_t extht_hi_thresholds;
    entropy_src_hw2reg_extht_lo_thresholds_reg_t extht_lo_thresholds;
    entropy_src_hw2reg_repcnt_hi_watermarks_reg_t repcnt_hi_watermarks;
    entropy_src_hw2reg_repcnts_hi_watermarks_reg_t repcnts_hi_watermarks;
    entropy_src_hw2reg_adaptp_hi_watermarks_reg_t adaptp_hi_watermarks;
    entropy_src_hw2reg_adaptp_lo_watermarks_reg_t adaptp_lo_watermarks;
    entropy_src_hw2reg_extht_hi_watermarks_reg_t extht_hi_watermarks;
    entropy_src_hw2reg_extht_lo_watermarks_reg_t extht_lo_watermarks;
    entropy_src_hw2reg_bucket_hi_watermarks_reg_t bucket_hi_watermarks;
    entropy_src_hw2reg_markov_hi_watermarks_reg_t markov_hi_watermarks;
    entropy_src_hw2reg_markov_lo_watermarks_reg_t markov_lo_watermarks;
    entropy_src_hw2reg_repcnt_total_fails_reg_t repcnt_total_fails;
    entropy_src_hw2reg_repcnts_total_fails_reg_t repcnts_total_fails;
    entropy_src_hw2reg_adaptp_hi_total_fails_reg_t adaptp_hi_total_fails;
    entropy_src_hw2reg_adaptp_lo_total_fails_reg_t adaptp_lo_total_fails;
    entropy_src_hw2reg_bucket_total_fails_reg_t bucket_total_fails;
    entropy_src_hw2reg_markov_hi_total_fails_reg_t markov_hi_total_fails;
    entropy_src_hw2reg_markov_lo_total_fails_reg_t markov_lo_total_fails;
    entropy_src_hw2reg_extht_hi_total_fails_reg_t extht_hi_total_fails;
    entropy_src_hw2reg_extht_lo_total_fails_reg_t extht_lo_total_fails;
    entropy_src_hw2reg_alert_summary_fail_counts_reg_t alert_summary_fail_counts;
    entropy_src_hw2reg_alert_fail_counts_reg_t alert_fail_counts;
    entropy_src_hw2reg_extht_fail_counts_reg_t extht_fail_counts;
    entropy_src_hw2reg_fw_ov_wr_fifo_full_reg_t fw_ov_wr_fifo_full;
    entropy_src_hw2reg_fw_ov_rd_fifo_overflow_reg_t fw_ov_rd_fifo_overflow;
    entropy_src_hw2reg_fw_ov_rd_data_reg_t fw_ov_rd_data;
    entropy_src_hw2reg_observe_fifo_depth_reg_t observe_fifo_depth;
    entropy_src_hw2reg_debug_status_reg_t debug_status;
    entropy_src_hw2reg_recov_alert_sts_reg_t recov_alert_sts;
    entropy_src_hw2reg_err_code_reg_t err_code;
    entropy_src_hw2reg_main_sm_state_reg_t main_sm_state;
  };

  // Flat register storage
  std::array<uint32_t, 56> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 56> shadow_stage_default_{};
  std::array<uint8_t,  56> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 56> racl_allow_rd_default_{};
  std::array<bool, 56> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  entropy_src_reg2hw_t reg2hw;
  entropy_src_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace entropy_src
}  // namespace opentitan



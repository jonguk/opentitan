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
namespace kmac {

// ----- Types mirrored from kmac_reg_pkg.sv -----


class kmac_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<kmac_reg_top> tlm_target;

  SC_HAS_PROCESS(kmac_reg_top);
  explicit kmac_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 228;

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
    CFG_REGWEN = 16,
    CFG_SHADOWED = 20,
    CMD = 24,
    STATUS = 28,
    ENTROPY_PERIOD = 32,
    ENTROPY_REFRESH_HASH_CNT = 36,
    ENTROPY_REFRESH_THRESHOLD_SHADOWED = 40,
    ENTROPY_SEED = 44,
    KEY_SHARE0_0 = 48,
    KEY_SHARE0_1 = 52,
    KEY_SHARE0_2 = 56,
    KEY_SHARE0_3 = 60,
    KEY_SHARE0_4 = 64,
    KEY_SHARE0_5 = 68,
    KEY_SHARE0_6 = 72,
    KEY_SHARE0_7 = 76,
    KEY_SHARE0_8 = 80,
    KEY_SHARE0_9 = 84,
    KEY_SHARE0_10 = 88,
    KEY_SHARE0_11 = 92,
    KEY_SHARE0_12 = 96,
    KEY_SHARE0_13 = 100,
    KEY_SHARE0_14 = 104,
    KEY_SHARE0_15 = 108,
    KEY_SHARE1_0 = 112,
    KEY_SHARE1_1 = 116,
    KEY_SHARE1_2 = 120,
    KEY_SHARE1_3 = 124,
    KEY_SHARE1_4 = 128,
    KEY_SHARE1_5 = 132,
    KEY_SHARE1_6 = 136,
    KEY_SHARE1_7 = 140,
    KEY_SHARE1_8 = 144,
    KEY_SHARE1_9 = 148,
    KEY_SHARE1_10 = 152,
    KEY_SHARE1_11 = 156,
    KEY_SHARE1_12 = 160,
    KEY_SHARE1_13 = 164,
    KEY_SHARE1_14 = 168,
    KEY_SHARE1_15 = 172,
    KEY_LEN = 176,
    PREFIX_0 = 180,
    PREFIX_1 = 184,
    PREFIX_2 = 188,
    PREFIX_3 = 192,
    PREFIX_4 = 196,
    PREFIX_5 = 200,
    PREFIX_6 = 204,
    PREFIX_7 = 208,
    PREFIX_8 = 212,
    PREFIX_9 = 216,
    PREFIX_10 = 220,
    ERR_CODE = 224,
  };

  // Aggregate structs mirroring kmac_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct kmac_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } kmac_err;
    struct {
      sc_dt::sc_bv<1> q;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } kmac_done;
  };

  struct kmac_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } kmac_err;
    struct {
      sc_dt::sc_bv<1> q;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } kmac_done;
  };

  struct kmac_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } kmac_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } kmac_done;
  };

  struct kmac_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_fault_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_operation_err;
  };

  struct kmac_reg2hw_cfg_shadowed_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } en_unsupported_modestrength;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } entropy_ready;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } msg_mask;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } entropy_fast_process;
    struct {
      sc_dt::sc_bv<2> q;
      bool        qe;
    } entropy_mode;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } sideload;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } state_endianness;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } msg_endianness;
    struct {
      sc_dt::sc_bv<2> q;
      bool        qe;
    } mode;
    struct {
      sc_dt::sc_bv<3> q;
      bool        qe;
    } kstrength;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } kmac_en;
  };

  struct kmac_reg2hw_cmd_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } err_processed;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } hash_cnt_clr;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } entropy_req;
    struct {
      sc_dt::sc_bv<6> q;
      bool        qe;
    } cmd;
  };

  struct kmac_reg2hw_entropy_period_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
    } wait_timer;
    struct {
      sc_dt::sc_bv<10> q;
    } prescaler;
  };

  struct kmac_reg2hw_entropy_refresh_threshold_shadowed_reg_t {
    sc_dt::sc_bv<10> q;
  };

  struct kmac_reg2hw_entropy_seed_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct kmac_reg2hw_key_share0_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct kmac_reg2hw_key_share1_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct kmac_reg2hw_key_len_reg_t {
    sc_dt::sc_bv<3> q;
  };

  struct kmac_reg2hw_prefix_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct kmac_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } kmac_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } kmac_done;
  };

  struct kmac_hw2reg_cfg_regwen_reg_t {
    sc_dt::sc_bv<1> d;
  };

  struct kmac_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } alert_recov_ctrl_update_err;
    struct {
      sc_dt::sc_bv<1> d;
    } alert_fatal_fault;
    struct {
      sc_dt::sc_bv<1> d;
    } fifo_full;
    struct {
      sc_dt::sc_bv<1> d;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<5> d;
    } fifo_depth;
    struct {
      sc_dt::sc_bv<1> d;
    } sha3_squeeze;
    struct {
      sc_dt::sc_bv<1> d;
    } sha3_absorb;
    struct {
      sc_dt::sc_bv<1> d;
    } sha3_idle;
  };

  struct kmac_hw2reg_entropy_refresh_hash_cnt_reg_t {
    sc_dt::sc_bv<10> d;
    bool        de;
  };

  struct kmac_hw2reg_err_code_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  // Register -> HW type
  struct kmac_reg2hw_t {
    kmac_reg2hw_intr_state_reg_t intr_state;
    kmac_reg2hw_intr_enable_reg_t intr_enable;
    kmac_reg2hw_intr_test_reg_t intr_test;
    kmac_reg2hw_alert_test_reg_t alert_test;
    kmac_reg2hw_cfg_shadowed_reg_t cfg_shadowed;
    kmac_reg2hw_cmd_reg_t cmd;
    kmac_reg2hw_entropy_period_reg_t entropy_period;
    kmac_reg2hw_entropy_refresh_threshold_shadowed_reg_t entropy_refresh_threshold_shadowed;
    kmac_reg2hw_entropy_seed_reg_t entropy_seed;
    std::array<kmac_reg2hw_key_share0_mreg_t, 16> key_share0;
    std::array<kmac_reg2hw_key_share1_mreg_t, 16> key_share1;
    kmac_reg2hw_key_len_reg_t key_len;
    std::array<kmac_reg2hw_prefix_mreg_t, 11> prefix;
  };

  // HW -> register type
  struct kmac_hw2reg_t {
    kmac_hw2reg_intr_state_reg_t intr_state;
    kmac_hw2reg_cfg_regwen_reg_t cfg_regwen;
    kmac_hw2reg_status_reg_t status;
    kmac_hw2reg_entropy_refresh_hash_cnt_reg_t entropy_refresh_hash_cnt;
    kmac_hw2reg_err_code_reg_t err_code;
  };

  // Flat register storage
  std::array<uint32_t, 57> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 57> shadow_stage_default_{};
  std::array<uint8_t,  57> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 57> racl_allow_rd_default_{};
  std::array<bool, 57> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  kmac_reg2hw_t reg2hw;
  kmac_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace kmac
}  // namespace opentitan



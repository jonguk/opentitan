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
namespace hmac {

// ----- Types mirrored from hmac_reg_pkg.sv -----


class hmac_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<hmac_reg_top> tlm_target;

  SC_HAS_PROCESS(hmac_reg_top);
  explicit hmac_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 236;

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
    CFG = 16,
    CMD = 20,
    STATUS = 24,
    ERR_CODE = 28,
    WIPE_SECRET = 32,
    KEY_0 = 36,
    KEY_1 = 40,
    KEY_2 = 44,
    KEY_3 = 48,
    KEY_4 = 52,
    KEY_5 = 56,
    KEY_6 = 60,
    KEY_7 = 64,
    KEY_8 = 68,
    KEY_9 = 72,
    KEY_10 = 76,
    KEY_11 = 80,
    KEY_12 = 84,
    KEY_13 = 88,
    KEY_14 = 92,
    KEY_15 = 96,
    KEY_16 = 100,
    KEY_17 = 104,
    KEY_18 = 108,
    KEY_19 = 112,
    KEY_20 = 116,
    KEY_21 = 120,
    KEY_22 = 124,
    KEY_23 = 128,
    KEY_24 = 132,
    KEY_25 = 136,
    KEY_26 = 140,
    KEY_27 = 144,
    KEY_28 = 148,
    KEY_29 = 152,
    KEY_30 = 156,
    KEY_31 = 160,
    DIGEST_0 = 164,
    DIGEST_1 = 168,
    DIGEST_2 = 172,
    DIGEST_3 = 176,
    DIGEST_4 = 180,
    DIGEST_5 = 184,
    DIGEST_6 = 188,
    DIGEST_7 = 192,
    DIGEST_8 = 196,
    DIGEST_9 = 200,
    DIGEST_10 = 204,
    DIGEST_11 = 208,
    DIGEST_12 = 212,
    DIGEST_13 = 216,
    DIGEST_14 = 220,
    DIGEST_15 = 224,
    MSG_LENGTH_LOWER = 228,
    MSG_LENGTH_UPPER = 232,
  };

  // Aggregate structs mirroring hmac_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct hmac_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } hmac_err;
    struct {
      sc_dt::sc_bv<1> q;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } hmac_done;
  };

  struct hmac_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } hmac_err;
    struct {
      sc_dt::sc_bv<1> q;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } hmac_done;
  };

  struct hmac_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } hmac_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } hmac_done;
  };

  struct hmac_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct hmac_reg2hw_cfg_reg_t {
    struct {
      sc_dt::sc_bv<6> q;
      bool        qe;
    } key_length;
    struct {
      sc_dt::sc_bv<4> q;
      bool        qe;
    } digest_size;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } key_swap;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } digest_swap;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } endian_swap;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } sha_en;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } hmac_en;
  };

  struct hmac_reg2hw_cmd_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } hash_continue;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } hash_stop;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } hash_process;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } hash_start;
  };

  struct hmac_reg2hw_wipe_secret_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct hmac_reg2hw_key_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct hmac_reg2hw_digest_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct hmac_reg2hw_msg_length_lower_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct hmac_reg2hw_msg_length_upper_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct hmac_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } hmac_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } hmac_done;
  };

  struct hmac_hw2reg_cfg_reg_t {
    struct {
      sc_dt::sc_bv<6> d;
    } key_length;
    struct {
      sc_dt::sc_bv<4> d;
    } digest_size;
    struct {
      sc_dt::sc_bv<1> d;
    } key_swap;
    struct {
      sc_dt::sc_bv<1> d;
    } digest_swap;
    struct {
      sc_dt::sc_bv<1> d;
    } endian_swap;
    struct {
      sc_dt::sc_bv<1> d;
    } sha_en;
    struct {
      sc_dt::sc_bv<1> d;
    } hmac_en;
  };

  struct hmac_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<6> d;
    } fifo_depth;
    struct {
      sc_dt::sc_bv<1> d;
    } fifo_full;
    struct {
      sc_dt::sc_bv<1> d;
    } fifo_empty;
    struct {
      sc_dt::sc_bv<1> d;
    } hmac_idle;
  };

  struct hmac_hw2reg_err_code_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct hmac_hw2reg_key_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct hmac_hw2reg_digest_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct hmac_hw2reg_msg_length_lower_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct hmac_hw2reg_msg_length_upper_reg_t {
    sc_dt::sc_bv<32> d;
  };

  // Register -> HW type
  struct hmac_reg2hw_t {
    hmac_reg2hw_intr_state_reg_t intr_state;
    hmac_reg2hw_intr_enable_reg_t intr_enable;
    hmac_reg2hw_intr_test_reg_t intr_test;
    hmac_reg2hw_alert_test_reg_t alert_test;
    hmac_reg2hw_cfg_reg_t cfg;
    hmac_reg2hw_cmd_reg_t cmd;
    hmac_reg2hw_wipe_secret_reg_t wipe_secret;
    std::array<hmac_reg2hw_key_mreg_t, 32> key;
    std::array<hmac_reg2hw_digest_mreg_t, 16> digest;
    hmac_reg2hw_msg_length_lower_reg_t msg_length_lower;
    hmac_reg2hw_msg_length_upper_reg_t msg_length_upper;
  };

  // HW -> register type
  struct hmac_hw2reg_t {
    hmac_hw2reg_intr_state_reg_t intr_state;
    hmac_hw2reg_cfg_reg_t cfg;
    hmac_hw2reg_status_reg_t status;
    hmac_hw2reg_err_code_reg_t err_code;
    std::array<hmac_hw2reg_key_mreg_t, 32> key;
    std::array<hmac_hw2reg_digest_mreg_t, 16> digest;
    hmac_hw2reg_msg_length_lower_reg_t msg_length_lower;
    hmac_hw2reg_msg_length_upper_reg_t msg_length_upper;
  };

  // Flat register storage
  std::array<uint32_t, 59> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 59> shadow_stage_default_{};
  std::array<uint8_t,  59> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 59> racl_allow_rd_default_{};
  std::array<bool, 59> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  hmac_reg2hw_t reg2hw;
  hmac_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace hmac
}  // namespace opentitan



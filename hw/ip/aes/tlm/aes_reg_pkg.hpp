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
namespace aes {

// ----- Types mirrored from aes_reg_pkg.sv -----


class aes_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<aes_reg_top> tlm_target;

  SC_HAS_PROCESS(aes_reg_top);
  explicit aes_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 136;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_default_e : uint32_t {
    ALERT_TEST = 0,
    KEY_SHARE0_0 = 4,
    KEY_SHARE0_1 = 8,
    KEY_SHARE0_2 = 12,
    KEY_SHARE0_3 = 16,
    KEY_SHARE0_4 = 20,
    KEY_SHARE0_5 = 24,
    KEY_SHARE0_6 = 28,
    KEY_SHARE0_7 = 32,
    KEY_SHARE1_0 = 36,
    KEY_SHARE1_1 = 40,
    KEY_SHARE1_2 = 44,
    KEY_SHARE1_3 = 48,
    KEY_SHARE1_4 = 52,
    KEY_SHARE1_5 = 56,
    KEY_SHARE1_6 = 60,
    KEY_SHARE1_7 = 64,
    IV_0 = 68,
    IV_1 = 72,
    IV_2 = 76,
    IV_3 = 80,
    DATA_IN_0 = 84,
    DATA_IN_1 = 88,
    DATA_IN_2 = 92,
    DATA_IN_3 = 96,
    DATA_OUT_0 = 100,
    DATA_OUT_1 = 104,
    DATA_OUT_2 = 108,
    DATA_OUT_3 = 112,
    CTRL_SHADOWED = 116,
    CTRL_AUX_SHADOWED = 120,
    CTRL_AUX_REGWEN = 124,
    TRIGGER = 128,
    STATUS = 132,
  };

  // Aggregate structs mirroring aes_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct aes_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_fault;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_ctrl_update_err;
  };

  struct aes_reg2hw_key_share0_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct aes_reg2hw_key_share1_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct aes_reg2hw_iv_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct aes_reg2hw_data_in_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct aes_reg2hw_data_out_mreg_t {
    sc_dt::sc_bv<32> q;
    bool re;
  };

  struct aes_reg2hw_ctrl_shadowed_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
      bool        re;
    } manual_operation;
    struct {
      sc_dt::sc_bv<3> q;
      bool        qe;
      bool        re;
    } prng_reseed_rate;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
      bool        re;
    } sideload;
    struct {
      sc_dt::sc_bv<3> q;
      bool        qe;
      bool        re;
    } key_len;
    struct {
      sc_dt::sc_bv<6> q;
      bool        qe;
      bool        re;
    } mode;
    struct {
      sc_dt::sc_bv<2> q;
      bool        qe;
      bool        re;
    } operation;
  };

  struct aes_reg2hw_ctrl_aux_shadowed_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } force_masks;
    struct {
      sc_dt::sc_bv<1> q;
    } key_touch_forces_reseed;
  };

  struct aes_reg2hw_trigger_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } prng_reseed;
    struct {
      sc_dt::sc_bv<1> q;
    } data_out_clear;
    struct {
      sc_dt::sc_bv<1> q;
    } key_iv_data_in_clear;
    struct {
      sc_dt::sc_bv<1> q;
    } start;
  };

  struct aes_reg2hw_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } output_lost;
    struct {
      sc_dt::sc_bv<1> q;
    } idle;
  };

  struct aes_hw2reg_key_share0_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct aes_hw2reg_key_share1_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct aes_hw2reg_iv_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct aes_hw2reg_data_in_mreg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct aes_hw2reg_data_out_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct aes_hw2reg_ctrl_shadowed_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } manual_operation;
    struct {
      sc_dt::sc_bv<3> d;
    } prng_reseed_rate;
    struct {
      sc_dt::sc_bv<1> d;
    } sideload;
    struct {
      sc_dt::sc_bv<3> d;
    } key_len;
    struct {
      sc_dt::sc_bv<6> d;
    } mode;
    struct {
      sc_dt::sc_bv<2> d;
    } operation;
  };

  struct aes_hw2reg_trigger_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } prng_reseed;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } data_out_clear;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key_iv_data_in_clear;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } start;
  };

  struct aes_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } alert_fatal_fault;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } alert_recov_ctrl_update_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } input_ready;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } output_valid;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } output_lost;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } stall;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } idle;
  };

  // Register -> HW type
  struct aes_reg2hw_t {
    aes_reg2hw_alert_test_reg_t alert_test;
    std::array<aes_reg2hw_key_share0_mreg_t, 8> key_share0;
    std::array<aes_reg2hw_key_share1_mreg_t, 8> key_share1;
    std::array<aes_reg2hw_iv_mreg_t, 4> iv;
    std::array<aes_reg2hw_data_in_mreg_t, 4> data_in;
    std::array<aes_reg2hw_data_out_mreg_t, 4> data_out;
    aes_reg2hw_ctrl_shadowed_reg_t ctrl_shadowed;
    aes_reg2hw_ctrl_aux_shadowed_reg_t ctrl_aux_shadowed;
    aes_reg2hw_trigger_reg_t trigger;
    aes_reg2hw_status_reg_t status;
  };

  // HW -> register type
  struct aes_hw2reg_t {
    std::array<aes_hw2reg_key_share0_mreg_t, 8> key_share0;
    std::array<aes_hw2reg_key_share1_mreg_t, 8> key_share1;
    std::array<aes_hw2reg_iv_mreg_t, 4> iv;
    std::array<aes_hw2reg_data_in_mreg_t, 4> data_in;
    std::array<aes_hw2reg_data_out_mreg_t, 4> data_out;
    aes_hw2reg_ctrl_shadowed_reg_t ctrl_shadowed;
    aes_hw2reg_trigger_reg_t trigger;
    aes_hw2reg_status_reg_t status;
  };

  // Flat register storage
  std::array<uint32_t, 34> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 34> shadow_stage_default_{};
  std::array<uint8_t,  34> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 34> racl_allow_rd_default_{};
  std::array<bool, 34> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  aes_reg2hw_t reg2hw;
  aes_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace aes
}  // namespace opentitan



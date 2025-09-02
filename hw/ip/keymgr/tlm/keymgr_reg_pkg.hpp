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
namespace keymgr {

// ----- Types mirrored from keymgr_reg_pkg.sv -----


class keymgr_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<keymgr_reg_top> tlm_target;

  SC_HAS_PROCESS(keymgr_reg_top);
  explicit keymgr_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 252;

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
    START = 20,
    CONTROL_SHADOWED = 24,
    SIDELOAD_CLEAR = 28,
    RESEED_INTERVAL_REGWEN = 32,
    RESEED_INTERVAL_SHADOWED = 36,
    SW_BINDING_REGWEN = 40,
    SEALING_SW_BINDING_0 = 44,
    SEALING_SW_BINDING_1 = 48,
    SEALING_SW_BINDING_2 = 52,
    SEALING_SW_BINDING_3 = 56,
    SEALING_SW_BINDING_4 = 60,
    SEALING_SW_BINDING_5 = 64,
    SEALING_SW_BINDING_6 = 68,
    SEALING_SW_BINDING_7 = 72,
    ATTEST_SW_BINDING_0 = 76,
    ATTEST_SW_BINDING_1 = 80,
    ATTEST_SW_BINDING_2 = 84,
    ATTEST_SW_BINDING_3 = 88,
    ATTEST_SW_BINDING_4 = 92,
    ATTEST_SW_BINDING_5 = 96,
    ATTEST_SW_BINDING_6 = 100,
    ATTEST_SW_BINDING_7 = 104,
    SALT_0 = 108,
    SALT_1 = 112,
    SALT_2 = 116,
    SALT_3 = 120,
    SALT_4 = 124,
    SALT_5 = 128,
    SALT_6 = 132,
    SALT_7 = 136,
    KEY_VERSION = 140,
    MAX_CREATOR_KEY_VER_REGWEN = 144,
    MAX_CREATOR_KEY_VER_SHADOWED = 148,
    MAX_OWNER_INT_KEY_VER_REGWEN = 152,
    MAX_OWNER_INT_KEY_VER_SHADOWED = 156,
    MAX_OWNER_KEY_VER_REGWEN = 160,
    MAX_OWNER_KEY_VER_SHADOWED = 164,
    SW_SHARE0_OUTPUT_0 = 168,
    SW_SHARE0_OUTPUT_1 = 172,
    SW_SHARE0_OUTPUT_2 = 176,
    SW_SHARE0_OUTPUT_3 = 180,
    SW_SHARE0_OUTPUT_4 = 184,
    SW_SHARE0_OUTPUT_5 = 188,
    SW_SHARE0_OUTPUT_6 = 192,
    SW_SHARE0_OUTPUT_7 = 196,
    SW_SHARE1_OUTPUT_0 = 200,
    SW_SHARE1_OUTPUT_1 = 204,
    SW_SHARE1_OUTPUT_2 = 208,
    SW_SHARE1_OUTPUT_3 = 212,
    SW_SHARE1_OUTPUT_4 = 216,
    SW_SHARE1_OUTPUT_5 = 220,
    SW_SHARE1_OUTPUT_6 = 224,
    SW_SHARE1_OUTPUT_7 = 228,
    WORKING_STATE = 232,
    OP_STATUS = 236,
    ERR_CODE = 240,
    FAULT_STATUS = 244,
    DEBUG = 248,
  };

  // Aggregate structs mirroring keymgr_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct keymgr_reg2hw_intr_state_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct keymgr_reg2hw_intr_enable_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct keymgr_reg2hw_intr_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct keymgr_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_fault_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_operation_err;
  };

  struct keymgr_reg2hw_start_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct keymgr_reg2hw_control_shadowed_reg_t {
    struct {
      sc_dt::sc_bv<2> q;
    } dest_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } cdi_sel;
    struct {
      sc_dt::sc_bv<3> q;
    } operation;
  };

  struct keymgr_reg2hw_sideload_clear_reg_t {
    sc_dt::sc_bv<3> q;
  };

  struct keymgr_reg2hw_reseed_interval_shadowed_reg_t {
    sc_dt::sc_bv<16> q;
  };

  struct keymgr_reg2hw_sw_binding_regwen_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct keymgr_reg2hw_sealing_sw_binding_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct keymgr_reg2hw_attest_sw_binding_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct keymgr_reg2hw_salt_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct keymgr_reg2hw_key_version_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct keymgr_reg2hw_max_creator_key_ver_shadowed_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct keymgr_reg2hw_max_owner_int_key_ver_shadowed_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct keymgr_reg2hw_max_owner_key_ver_shadowed_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct keymgr_reg2hw_fault_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } key_ecc;
    struct {
      sc_dt::sc_bv<1> q;
    } side_ctrl_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } side_ctrl_fsm;
    struct {
      sc_dt::sc_bv<1> q;
    } reseed_cnt;
    struct {
      sc_dt::sc_bv<1> q;
    } ctrl_fsm_cnt;
    struct {
      sc_dt::sc_bv<1> q;
    } ctrl_fsm_chk;
    struct {
      sc_dt::sc_bv<1> q;
    } ctrl_fsm_intg;
    struct {
      sc_dt::sc_bv<1> q;
    } shadow;
    struct {
      sc_dt::sc_bv<1> q;
    } regfile_intg;
    struct {
      sc_dt::sc_bv<1> q;
    } kmac_out;
    struct {
      sc_dt::sc_bv<1> q;
    } kmac_op;
    struct {
      sc_dt::sc_bv<1> q;
    } kmac_done;
    struct {
      sc_dt::sc_bv<1> q;
    } kmac_fsm;
    struct {
      sc_dt::sc_bv<1> q;
    } cmd;
  };

  struct keymgr_hw2reg_intr_state_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct keymgr_hw2reg_cfg_regwen_reg_t {
    sc_dt::sc_bv<1> d;
  };

  struct keymgr_hw2reg_start_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct keymgr_hw2reg_sw_binding_regwen_reg_t {
    sc_dt::sc_bv<1> d;
  };

  struct keymgr_hw2reg_sw_share0_output_mreg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct keymgr_hw2reg_sw_share1_output_mreg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct keymgr_hw2reg_working_state_reg_t {
    sc_dt::sc_bv<3> d;
    bool        de;
  };

  struct keymgr_hw2reg_op_status_reg_t {
    sc_dt::sc_bv<2> d;
    bool        de;
  };

  struct keymgr_hw2reg_err_code_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_shadow_update;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_kmac_input;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_op;
  };

  struct keymgr_hw2reg_fault_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key_ecc;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } side_ctrl_sel;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } side_ctrl_fsm;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } reseed_cnt;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ctrl_fsm_cnt;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ctrl_fsm_chk;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ctrl_fsm_intg;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } shadow;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } regfile_intg;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } kmac_out;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } kmac_op;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } kmac_done;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } kmac_fsm;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd;
  };

  struct keymgr_hw2reg_debug_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_digest;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_key;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_key_version;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_health_state;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_dev_id;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_owner_seed;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } invalid_creator_seed;
  };

  // Register -> HW type
  struct keymgr_reg2hw_t {
    keymgr_reg2hw_intr_state_reg_t intr_state;
    keymgr_reg2hw_intr_enable_reg_t intr_enable;
    keymgr_reg2hw_intr_test_reg_t intr_test;
    keymgr_reg2hw_alert_test_reg_t alert_test;
    keymgr_reg2hw_start_reg_t start;
    keymgr_reg2hw_control_shadowed_reg_t control_shadowed;
    keymgr_reg2hw_sideload_clear_reg_t sideload_clear;
    keymgr_reg2hw_reseed_interval_shadowed_reg_t reseed_interval_shadowed;
    keymgr_reg2hw_sw_binding_regwen_reg_t sw_binding_regwen;
    std::array<keymgr_reg2hw_sealing_sw_binding_mreg_t, 8> sealing_sw_binding;
    std::array<keymgr_reg2hw_attest_sw_binding_mreg_t, 8> attest_sw_binding;
    std::array<keymgr_reg2hw_salt_mreg_t, 8> salt;
    std::array<keymgr_reg2hw_key_version_mreg_t, 1> key_version;
    keymgr_reg2hw_max_creator_key_ver_shadowed_reg_t max_creator_key_ver_shadowed;
    keymgr_reg2hw_max_owner_int_key_ver_shadowed_reg_t max_owner_int_key_ver_shadowed;
    keymgr_reg2hw_max_owner_key_ver_shadowed_reg_t max_owner_key_ver_shadowed;
    keymgr_reg2hw_fault_status_reg_t fault_status;
  };

  // HW -> register type
  struct keymgr_hw2reg_t {
    keymgr_hw2reg_intr_state_reg_t intr_state;
    keymgr_hw2reg_cfg_regwen_reg_t cfg_regwen;
    keymgr_hw2reg_start_reg_t start;
    keymgr_hw2reg_sw_binding_regwen_reg_t sw_binding_regwen;
    std::array<keymgr_hw2reg_sw_share0_output_mreg_t, 8> sw_share0_output;
    std::array<keymgr_hw2reg_sw_share1_output_mreg_t, 8> sw_share1_output;
    keymgr_hw2reg_working_state_reg_t working_state;
    keymgr_hw2reg_op_status_reg_t op_status;
    keymgr_hw2reg_err_code_reg_t err_code;
    keymgr_hw2reg_fault_status_reg_t fault_status;
    keymgr_hw2reg_debug_reg_t debug;
  };

  // Flat register storage
  std::array<uint32_t, 63> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 63> shadow_stage_default_{};
  std::array<uint8_t,  63> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 63> racl_allow_rd_default_{};
  std::array<bool, 63> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata, uint32_t be_mask);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  keymgr_reg2hw_t reg2hw;
  keymgr_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace keymgr
}  // namespace opentitan



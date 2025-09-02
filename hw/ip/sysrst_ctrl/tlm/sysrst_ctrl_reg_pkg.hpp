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
namespace sysrst_ctrl {

// ----- Types mirrored from sysrst_ctrl_reg_pkg.sv -----


class sysrst_ctrl_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<sysrst_ctrl_reg_top> tlm_target;

  SC_HAS_PROCESS(sysrst_ctrl_reg_top);
  explicit sysrst_ctrl_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 172;

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
    REGWEN = 16,
    EC_RST_CTL = 20,
    ULP_AC_DEBOUNCE_CTL = 24,
    ULP_LID_DEBOUNCE_CTL = 28,
    ULP_PWRB_DEBOUNCE_CTL = 32,
    ULP_CTL = 36,
    ULP_STATUS = 40,
    WKUP_STATUS = 44,
    KEY_INVERT_CTL = 48,
    PIN_ALLOWED_CTL = 52,
    PIN_OUT_CTL = 56,
    PIN_OUT_VALUE = 60,
    PIN_IN_VALUE = 64,
    KEY_INTR_CTL = 68,
    KEY_INTR_DEBOUNCE_CTL = 72,
    AUTO_BLOCK_DEBOUNCE_CTL = 76,
    AUTO_BLOCK_OUT_CTL = 80,
    COM_PRE_SEL_CTL_0 = 84,
    COM_PRE_SEL_CTL_1 = 88,
    COM_PRE_SEL_CTL_2 = 92,
    COM_PRE_SEL_CTL_3 = 96,
    COM_PRE_DET_CTL_0 = 100,
    COM_PRE_DET_CTL_1 = 104,
    COM_PRE_DET_CTL_2 = 108,
    COM_PRE_DET_CTL_3 = 112,
    COM_SEL_CTL_0 = 116,
    COM_SEL_CTL_1 = 120,
    COM_SEL_CTL_2 = 124,
    COM_SEL_CTL_3 = 128,
    COM_DET_CTL_0 = 132,
    COM_DET_CTL_1 = 136,
    COM_DET_CTL_2 = 140,
    COM_DET_CTL_3 = 144,
    COM_OUT_CTL_0 = 148,
    COM_OUT_CTL_1 = 152,
    COM_OUT_CTL_2 = 156,
    COM_OUT_CTL_3 = 160,
    COMBO_INTR_STATUS = 164,
    KEY_INTR_STATUS = 168,
  };

  // Aggregate structs mirroring sysrst_ctrl_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct sysrst_ctrl_reg2hw_intr_state_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct sysrst_ctrl_reg2hw_intr_enable_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct sysrst_ctrl_reg2hw_intr_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct sysrst_ctrl_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct sysrst_ctrl_reg2hw_ec_rst_ctl_reg_t {
    sc_dt::sc_bv<16> q;
  };

  struct sysrst_ctrl_reg2hw_ulp_ac_debounce_ctl_reg_t {
    sc_dt::sc_bv<16> q;
  };

  struct sysrst_ctrl_reg2hw_ulp_lid_debounce_ctl_reg_t {
    sc_dt::sc_bv<16> q;
  };

  struct sysrst_ctrl_reg2hw_ulp_pwrb_debounce_ctl_reg_t {
    sc_dt::sc_bv<16> q;
  };

  struct sysrst_ctrl_reg2hw_ulp_ctl_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct sysrst_ctrl_reg2hw_ulp_status_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct sysrst_ctrl_reg2hw_wkup_status_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct sysrst_ctrl_reg2hw_key_invert_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } z3_wakeup;
    struct {
      sc_dt::sc_bv<1> q;
    } lid_open;
    struct {
      sc_dt::sc_bv<1> q;
    } bat_disable;
    struct {
      sc_dt::sc_bv<1> q;
    } ac_present;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_out;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_in;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_out;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_in;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_out;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_in;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_out;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_in;
  };

  struct sysrst_ctrl_reg2hw_pin_allowed_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } flash_wp_l_1;
    struct {
      sc_dt::sc_bv<1> q;
    } z3_wakeup_1;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_out_1;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_out_1;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_out_1;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_out_1;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst_l_1;
    struct {
      sc_dt::sc_bv<1> q;
    } bat_disable_1;
    struct {
      sc_dt::sc_bv<1> q;
    } flash_wp_l_0;
    struct {
      sc_dt::sc_bv<1> q;
    } z3_wakeup_0;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_out_0;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_out_0;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_out_0;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_out_0;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst_l_0;
    struct {
      sc_dt::sc_bv<1> q;
    } bat_disable_0;
  };

  struct sysrst_ctrl_reg2hw_pin_out_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } flash_wp_l;
    struct {
      sc_dt::sc_bv<1> q;
    } z3_wakeup;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_out;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_out;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_out;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_out;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst_l;
    struct {
      sc_dt::sc_bv<1> q;
    } bat_disable;
  };

  struct sysrst_ctrl_reg2hw_pin_out_value_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } flash_wp_l;
    struct {
      sc_dt::sc_bv<1> q;
    } z3_wakeup;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_out;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_out;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_out;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_out;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst_l;
    struct {
      sc_dt::sc_bv<1> q;
    } bat_disable;
  };

  struct sysrst_ctrl_reg2hw_key_intr_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } flash_wp_l_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst_l_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } ac_present_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_in_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_in_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_in_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_in_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } flash_wp_l_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst_l_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } ac_present_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_in_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_in_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_in_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_in_h2l;
  };

  struct sysrst_ctrl_reg2hw_key_intr_debounce_ctl_reg_t {
    sc_dt::sc_bv<16> q;
  };

  struct sysrst_ctrl_reg2hw_auto_block_debounce_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } auto_block_enable;
    struct {
      sc_dt::sc_bv<16> q;
    } debounce_timer;
  };

  struct sysrst_ctrl_reg2hw_auto_block_out_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } key2_out_value;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_out_value;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_out_value;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_out_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_out_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_out_sel;
  };

  struct sysrst_ctrl_reg2hw_com_pre_sel_ctl_mreg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } ac_present_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_in_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_in_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_in_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_in_sel;
  };

  struct sysrst_ctrl_reg2hw_com_pre_det_ctl_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct sysrst_ctrl_reg2hw_com_sel_ctl_mreg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } ac_present_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_in_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_in_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_in_sel;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_in_sel;
  };

  struct sysrst_ctrl_reg2hw_com_det_ctl_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct sysrst_ctrl_reg2hw_com_out_ctl_mreg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } rst_req;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst;
    struct {
      sc_dt::sc_bv<1> q;
    } interrupt;
    struct {
      sc_dt::sc_bv<1> q;
    } bat_disable;
  };

  struct sysrst_ctrl_reg2hw_combo_intr_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } combo3_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } combo2_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } combo1_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } combo0_h2l;
  };

  struct sysrst_ctrl_reg2hw_key_intr_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } flash_wp_l_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst_l_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } ac_present_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_in_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_in_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_in_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_l2h;
    struct {
      sc_dt::sc_bv<1> q;
    } flash_wp_l_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } ec_rst_l_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } ac_present_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } key2_in_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } key1_in_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } key0_in_h2l;
    struct {
      sc_dt::sc_bv<1> q;
    } pwrb_h2l;
  };

  struct sysrst_ctrl_hw2reg_intr_state_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct sysrst_ctrl_hw2reg_ulp_status_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct sysrst_ctrl_hw2reg_wkup_status_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct sysrst_ctrl_hw2reg_pin_in_value_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } flash_wp_l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ec_rst_l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ac_present;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } lid_open;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key2_in;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key1_in;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key0_in;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } pwrb_in;
  };

  struct sysrst_ctrl_hw2reg_combo_intr_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } combo3_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } combo2_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } combo1_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } combo0_h2l;
  };

  struct sysrst_ctrl_hw2reg_key_intr_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } flash_wp_l_l2h;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ec_rst_l_l2h;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ac_present_l2h;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key2_in_l2h;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key1_in_l2h;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key0_in_l2h;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } pwrb_l2h;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } flash_wp_l_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ec_rst_l_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ac_present_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key2_in_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key1_in_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } key0_in_h2l;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } pwrb_h2l;
  };

  // Register -> HW type
  struct sysrst_ctrl_reg2hw_t {
    sysrst_ctrl_reg2hw_intr_state_reg_t intr_state;
    sysrst_ctrl_reg2hw_intr_enable_reg_t intr_enable;
    sysrst_ctrl_reg2hw_intr_test_reg_t intr_test;
    sysrst_ctrl_reg2hw_alert_test_reg_t alert_test;
    sysrst_ctrl_reg2hw_ec_rst_ctl_reg_t ec_rst_ctl;
    sysrst_ctrl_reg2hw_ulp_ac_debounce_ctl_reg_t ulp_ac_debounce_ctl;
    sysrst_ctrl_reg2hw_ulp_lid_debounce_ctl_reg_t ulp_lid_debounce_ctl;
    sysrst_ctrl_reg2hw_ulp_pwrb_debounce_ctl_reg_t ulp_pwrb_debounce_ctl;
    sysrst_ctrl_reg2hw_ulp_ctl_reg_t ulp_ctl;
    sysrst_ctrl_reg2hw_ulp_status_reg_t ulp_status;
    sysrst_ctrl_reg2hw_wkup_status_reg_t wkup_status;
    sysrst_ctrl_reg2hw_key_invert_ctl_reg_t key_invert_ctl;
    sysrst_ctrl_reg2hw_pin_allowed_ctl_reg_t pin_allowed_ctl;
    sysrst_ctrl_reg2hw_pin_out_ctl_reg_t pin_out_ctl;
    sysrst_ctrl_reg2hw_pin_out_value_reg_t pin_out_value;
    sysrst_ctrl_reg2hw_key_intr_ctl_reg_t key_intr_ctl;
    sysrst_ctrl_reg2hw_key_intr_debounce_ctl_reg_t key_intr_debounce_ctl;
    sysrst_ctrl_reg2hw_auto_block_debounce_ctl_reg_t auto_block_debounce_ctl;
    sysrst_ctrl_reg2hw_auto_block_out_ctl_reg_t auto_block_out_ctl;
    std::array<sysrst_ctrl_reg2hw_com_pre_sel_ctl_mreg_t, 4> com_pre_sel_ctl;
    std::array<sysrst_ctrl_reg2hw_com_pre_det_ctl_mreg_t, 4> com_pre_det_ctl;
    std::array<sysrst_ctrl_reg2hw_com_sel_ctl_mreg_t, 4> com_sel_ctl;
    std::array<sysrst_ctrl_reg2hw_com_det_ctl_mreg_t, 4> com_det_ctl;
    std::array<sysrst_ctrl_reg2hw_com_out_ctl_mreg_t, 4> com_out_ctl;
    sysrst_ctrl_reg2hw_combo_intr_status_reg_t combo_intr_status;
    sysrst_ctrl_reg2hw_key_intr_status_reg_t key_intr_status;
  };

  // HW -> register type
  struct sysrst_ctrl_hw2reg_t {
    sysrst_ctrl_hw2reg_intr_state_reg_t intr_state;
    sysrst_ctrl_hw2reg_ulp_status_reg_t ulp_status;
    sysrst_ctrl_hw2reg_wkup_status_reg_t wkup_status;
    sysrst_ctrl_hw2reg_pin_in_value_reg_t pin_in_value;
    sysrst_ctrl_hw2reg_combo_intr_status_reg_t combo_intr_status;
    sysrst_ctrl_hw2reg_key_intr_status_reg_t key_intr_status;
  };

  // Flat register storage
  std::array<uint32_t, 43> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 43> shadow_stage_default_{};
  std::array<uint8_t,  43> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 43> racl_allow_rd_default_{};
  std::array<bool, 43> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  sysrst_ctrl_reg2hw_t reg2hw;
  sysrst_ctrl_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace sysrst_ctrl
}  // namespace opentitan



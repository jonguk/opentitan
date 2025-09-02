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
namespace adc_ctrl {

// ----- Types mirrored from adc_ctrl_reg_pkg.sv -----


class adc_ctrl_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<adc_ctrl_reg_top> tlm_target;

  SC_HAS_PROCESS(adc_ctrl_reg_top);
  explicit adc_ctrl_reg_top(sc_core::sc_module_name name);

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
    ADC_EN_CTL = 16,
    ADC_PD_CTL = 20,
    ADC_LP_SAMPLE_CTL = 24,
    ADC_SAMPLE_CTL = 28,
    ADC_FSM_RST = 32,
    ADC_CHN0_FILTER_CTL_0 = 36,
    ADC_CHN0_FILTER_CTL_1 = 40,
    ADC_CHN0_FILTER_CTL_2 = 44,
    ADC_CHN0_FILTER_CTL_3 = 48,
    ADC_CHN0_FILTER_CTL_4 = 52,
    ADC_CHN0_FILTER_CTL_5 = 56,
    ADC_CHN0_FILTER_CTL_6 = 60,
    ADC_CHN0_FILTER_CTL_7 = 64,
    ADC_CHN1_FILTER_CTL_0 = 68,
    ADC_CHN1_FILTER_CTL_1 = 72,
    ADC_CHN1_FILTER_CTL_2 = 76,
    ADC_CHN1_FILTER_CTL_3 = 80,
    ADC_CHN1_FILTER_CTL_4 = 84,
    ADC_CHN1_FILTER_CTL_5 = 88,
    ADC_CHN1_FILTER_CTL_6 = 92,
    ADC_CHN1_FILTER_CTL_7 = 96,
    ADC_CHN_VAL_0 = 100,
    ADC_CHN_VAL_1 = 104,
    ADC_WAKEUP_CTL = 108,
    FILTER_STATUS = 112,
    ADC_INTR_CTL = 116,
    ADC_INTR_STATUS = 120,
    ADC_FSM_STATE = 124,
  };

  // Aggregate structs mirroring adc_ctrl_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct adc_ctrl_reg2hw_intr_state_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct adc_ctrl_reg2hw_intr_enable_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct adc_ctrl_reg2hw_intr_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct adc_ctrl_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct adc_ctrl_reg2hw_adc_en_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } oneshot_mode;
    struct {
      sc_dt::sc_bv<1> q;
    } adc_enable;
  };

  struct adc_ctrl_reg2hw_adc_pd_ctl_reg_t {
    struct {
      sc_dt::sc_bv<24> q;
    } wakeup_time;
    struct {
      sc_dt::sc_bv<4> q;
    } pwrup_time;
    struct {
      sc_dt::sc_bv<1> q;
    } lp_mode;
  };

  struct adc_ctrl_reg2hw_adc_lp_sample_ctl_reg_t {
    sc_dt::sc_bv<8> q;
  };

  struct adc_ctrl_reg2hw_adc_sample_ctl_reg_t {
    sc_dt::sc_bv<16> q;
  };

  struct adc_ctrl_reg2hw_adc_fsm_rst_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct adc_ctrl_reg2hw_adc_chn0_filter_ctl_mreg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } en;
    struct {
      sc_dt::sc_bv<10> q;
    } max_v;
    struct {
      sc_dt::sc_bv<1> q;
    } cond;
    struct {
      sc_dt::sc_bv<10> q;
    } min_v;
  };

  struct adc_ctrl_reg2hw_adc_chn1_filter_ctl_mreg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } en;
    struct {
      sc_dt::sc_bv<10> q;
    } max_v;
    struct {
      sc_dt::sc_bv<1> q;
    } cond;
    struct {
      sc_dt::sc_bv<10> q;
    } min_v;
  };

  struct adc_ctrl_reg2hw_adc_wakeup_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } trans_en;
    struct {
      sc_dt::sc_bv<8> q;
    } match_en;
  };

  struct adc_ctrl_reg2hw_filter_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } trans;
    struct {
      sc_dt::sc_bv<8> q;
    } match;
  };

  struct adc_ctrl_reg2hw_adc_intr_ctl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } oneshot_en;
    struct {
      sc_dt::sc_bv<1> q;
    } trans_en;
    struct {
      sc_dt::sc_bv<8> q;
    } match_en;
  };

  struct adc_ctrl_reg2hw_adc_intr_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } oneshot;
    struct {
      sc_dt::sc_bv<1> q;
    } trans;
    struct {
      sc_dt::sc_bv<8> q;
    } match;
  };

  struct adc_ctrl_hw2reg_intr_state_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct adc_ctrl_hw2reg_adc_chn_val_mreg_t {
    struct {
      sc_dt::sc_bv<10> d;
      bool        de;
    } adc_chn_value_intr;
    struct {
      sc_dt::sc_bv<2> d;
      bool        de;
    } adc_chn_value_intr_ext;
    struct {
      sc_dt::sc_bv<10> d;
      bool        de;
    } adc_chn_value;
    struct {
      sc_dt::sc_bv<2> d;
      bool        de;
    } adc_chn_value_ext;
  };

  struct adc_ctrl_hw2reg_filter_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } trans;
    struct {
      sc_dt::sc_bv<8> d;
      bool        de;
    } match;
  };

  struct adc_ctrl_hw2reg_adc_intr_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } oneshot;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } trans;
    struct {
      sc_dt::sc_bv<8> d;
      bool        de;
    } match;
  };

  struct adc_ctrl_hw2reg_adc_fsm_state_reg_t {
    sc_dt::sc_bv<5> d;
  };

  // Register -> HW type
  struct adc_ctrl_reg2hw_t {
    adc_ctrl_reg2hw_intr_state_reg_t intr_state;
    adc_ctrl_reg2hw_intr_enable_reg_t intr_enable;
    adc_ctrl_reg2hw_intr_test_reg_t intr_test;
    adc_ctrl_reg2hw_alert_test_reg_t alert_test;
    adc_ctrl_reg2hw_adc_en_ctl_reg_t adc_en_ctl;
    adc_ctrl_reg2hw_adc_pd_ctl_reg_t adc_pd_ctl;
    adc_ctrl_reg2hw_adc_lp_sample_ctl_reg_t adc_lp_sample_ctl;
    adc_ctrl_reg2hw_adc_sample_ctl_reg_t adc_sample_ctl;
    adc_ctrl_reg2hw_adc_fsm_rst_reg_t adc_fsm_rst;
    std::array<adc_ctrl_reg2hw_adc_chn0_filter_ctl_mreg_t, 8> adc_chn0_filter_ctl;
    std::array<adc_ctrl_reg2hw_adc_chn1_filter_ctl_mreg_t, 8> adc_chn1_filter_ctl;
    adc_ctrl_reg2hw_adc_wakeup_ctl_reg_t adc_wakeup_ctl;
    adc_ctrl_reg2hw_filter_status_reg_t filter_status;
    adc_ctrl_reg2hw_adc_intr_ctl_reg_t adc_intr_ctl;
    adc_ctrl_reg2hw_adc_intr_status_reg_t adc_intr_status;
  };

  // HW -> register type
  struct adc_ctrl_hw2reg_t {
    adc_ctrl_hw2reg_intr_state_reg_t intr_state;
    std::array<adc_ctrl_hw2reg_adc_chn_val_mreg_t, 2> adc_chn_val;
    adc_ctrl_hw2reg_filter_status_reg_t filter_status;
    adc_ctrl_hw2reg_adc_intr_status_reg_t adc_intr_status;
    adc_ctrl_hw2reg_adc_fsm_state_reg_t adc_fsm_state;
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
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  adc_ctrl_reg2hw_t reg2hw;
  adc_ctrl_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace adc_ctrl
}  // namespace opentitan



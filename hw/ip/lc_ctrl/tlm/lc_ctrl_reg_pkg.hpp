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
namespace lc_ctrl {

// ----- Types mirrored from lc_ctrl_reg_pkg.sv -----


class lc_ctrl_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<lc_ctrl_reg_top> tlm_target;

  SC_HAS_PROCESS(lc_ctrl_reg_top);
  explicit lc_ctrl_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 140;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_regs_e : uint32_t {
    ALERT_TEST = 0,
    STATUS = 4,
    CLAIM_TRANSITION_IF_REGWEN = 8,
    CLAIM_TRANSITION_IF = 12,
    TRANSITION_REGWEN = 16,
    TRANSITION_CMD = 20,
    TRANSITION_CTRL = 24,
    TRANSITION_TOKEN_0 = 28,
    TRANSITION_TOKEN_1 = 32,
    TRANSITION_TOKEN_2 = 36,
    TRANSITION_TOKEN_3 = 40,
    TRANSITION_TARGET = 44,
    OTP_VENDOR_TEST_CTRL = 48,
    OTP_VENDOR_TEST_STATUS = 52,
    LC_STATE = 56,
    LC_TRANSITION_CNT = 60,
    LC_ID_STATE = 64,
    HW_REVISION0 = 68,
    HW_REVISION1 = 72,
    DEVICE_ID_0 = 76,
    DEVICE_ID_1 = 80,
    DEVICE_ID_2 = 84,
    DEVICE_ID_3 = 88,
    DEVICE_ID_4 = 92,
    DEVICE_ID_5 = 96,
    DEVICE_ID_6 = 100,
    DEVICE_ID_7 = 104,
    MANUF_STATE_0 = 108,
    MANUF_STATE_1 = 112,
    MANUF_STATE_2 = 116,
    MANUF_STATE_3 = 120,
    MANUF_STATE_4 = 124,
    MANUF_STATE_5 = 128,
    MANUF_STATE_6 = 132,
    MANUF_STATE_7 = 136,
  };
  enum class reg_dmi_e : uint32_t {
  };

  // Aggregate structs mirroring lc_ctrl_reg_pkg

//   ///////////////////////////////////////////////
  // Typedefs for registers for regs interface //
  ///////////////////////////////////////////////

  struct lc_ctrl_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_bus_integ_error;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_state_error;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_prog_error;
  };

  struct lc_ctrl_reg2hw_claim_transition_if_reg_t {
    sc_dt::sc_bv<8> q;
    bool qe;
  };

  struct lc_ctrl_reg2hw_transition_cmd_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct lc_ctrl_reg2hw_transition_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } volatile_raw_unlock;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } ext_clock_en;
  };

  struct lc_ctrl_reg2hw_transition_token_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct lc_ctrl_reg2hw_transition_target_reg_t {
    sc_dt::sc_bv<30> q;
    bool qe;
  };

  struct lc_ctrl_reg2hw_otp_vendor_test_ctrl_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct lc_ctrl_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } otp_partition_error;
    struct {
      sc_dt::sc_bv<1> d;
    } bus_integ_error;
    struct {
      sc_dt::sc_bv<1> d;
    } state_error;
    struct {
      sc_dt::sc_bv<1> d;
    } otp_error;
    struct {
      sc_dt::sc_bv<1> d;
    } flash_rma_error;
    struct {
      sc_dt::sc_bv<1> d;
    } token_error;
    struct {
      sc_dt::sc_bv<1> d;
    } transition_error;
    struct {
      sc_dt::sc_bv<1> d;
    } transition_count_error;
    struct {
      sc_dt::sc_bv<1> d;
    } transition_successful;
    struct {
      sc_dt::sc_bv<1> d;
    } ext_clock_switched;
    struct {
      sc_dt::sc_bv<1> d;
    } ready;
    struct {
      sc_dt::sc_bv<1> d;
    } initialized;
  };

  struct lc_ctrl_hw2reg_claim_transition_if_reg_t {
    sc_dt::sc_bv<8> d;
  };

  struct lc_ctrl_hw2reg_transition_regwen_reg_t {
    sc_dt::sc_bv<1> d;
  };

  struct lc_ctrl_hw2reg_transition_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } volatile_raw_unlock;
    struct {
      sc_dt::sc_bv<1> d;
    } ext_clock_en;
  };

  struct lc_ctrl_hw2reg_transition_token_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct lc_ctrl_hw2reg_transition_target_reg_t {
    sc_dt::sc_bv<30> d;
  };

  struct lc_ctrl_hw2reg_otp_vendor_test_ctrl_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct lc_ctrl_hw2reg_otp_vendor_test_status_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct lc_ctrl_hw2reg_lc_state_reg_t {
    sc_dt::sc_bv<30> d;
  };

  struct lc_ctrl_hw2reg_lc_transition_cnt_reg_t {
    sc_dt::sc_bv<5> d;
  };

  struct lc_ctrl_hw2reg_lc_id_state_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct lc_ctrl_hw2reg_hw_revision0_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
    } silicon_creator_id;
    struct {
      sc_dt::sc_bv<16> d;
    } product_id;
  };

  struct lc_ctrl_hw2reg_hw_revision1_reg_t {
    struct {
      sc_dt::sc_bv<24> d;
    } reserved;
    struct {
      sc_dt::sc_bv<8> d;
    } revision_id;
  };

  struct lc_ctrl_hw2reg_device_id_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct lc_ctrl_hw2reg_manuf_state_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  // Register -> HW type for regs interface
  struct lc_ctrl_regs_reg2hw_t {
    lc_ctrl_reg2hw_alert_test_reg_t alert_test;
    lc_ctrl_reg2hw_claim_transition_if_reg_t claim_transition_if;
    lc_ctrl_reg2hw_transition_cmd_reg_t transition_cmd;
    lc_ctrl_reg2hw_transition_ctrl_reg_t transition_ctrl;
    std::array<lc_ctrl_reg2hw_transition_token_mreg_t, 4> transition_token;
    lc_ctrl_reg2hw_transition_target_reg_t transition_target;
    lc_ctrl_reg2hw_otp_vendor_test_ctrl_reg_t otp_vendor_test_ctrl;
  };

  // HW -> register type for regs interface
  struct lc_ctrl_regs_hw2reg_t {
    lc_ctrl_hw2reg_status_reg_t status;
    lc_ctrl_hw2reg_claim_transition_if_reg_t claim_transition_if;
    lc_ctrl_hw2reg_transition_regwen_reg_t transition_regwen;
    lc_ctrl_hw2reg_transition_ctrl_reg_t transition_ctrl;
    std::array<lc_ctrl_hw2reg_transition_token_mreg_t, 4> transition_token;
    lc_ctrl_hw2reg_transition_target_reg_t transition_target;
    lc_ctrl_hw2reg_otp_vendor_test_ctrl_reg_t otp_vendor_test_ctrl;
    lc_ctrl_hw2reg_otp_vendor_test_status_reg_t otp_vendor_test_status;
    lc_ctrl_hw2reg_lc_state_reg_t lc_state;
    lc_ctrl_hw2reg_lc_transition_cnt_reg_t lc_transition_cnt;
    lc_ctrl_hw2reg_lc_id_state_reg_t lc_id_state;
    lc_ctrl_hw2reg_hw_revision0_reg_t hw_revision0;
    lc_ctrl_hw2reg_hw_revision1_reg_t hw_revision1;
    std::array<lc_ctrl_hw2reg_device_id_mreg_t, 8> device_id;
    std::array<lc_ctrl_hw2reg_manuf_state_mreg_t, 8> manuf_state;
  };

  // Flat register storage
  std::array<uint32_t, 35> regs_regs_;
  std::array<uint32_t, 0> regs_dmi_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 35> shadow_stage_regs_{};
  std::array<uint8_t,  35> shadow_phase_regs_{}; // 0:first,1:second
  std::array<uint32_t, 0> shadow_stage_dmi_{};
  std::array<uint8_t,  0> shadow_phase_dmi_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 35> racl_allow_rd_regs_{};
  std::array<bool, 35> racl_allow_wr_regs_{};
  std::array<bool, 0> racl_allow_rd_dmi_{};
  std::array<bool, 0> racl_allow_wr_dmi_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  lc_ctrl_regs_reg2hw_t reg2hw_regs;
  lc_ctrl_regs_hw2reg_t  hw2reg_regs;
  lc_ctrl_dmi_reg2hw_t reg2hw_dmi;
  lc_ctrl_dmi_hw2reg_t  hw2reg_dmi;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace lc_ctrl
}  // namespace opentitan



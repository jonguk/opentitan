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
namespace rv_timer {

// ----- Types mirrored from rv_timer_reg_pkg.sv -----


class rv_timer_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<rv_timer_reg_top> tlm_target;

  SC_HAS_PROCESS(rv_timer_reg_top);
  explicit rv_timer_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 40;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_default_e : uint32_t {
    ALERT_TEST = 0,
    CTRL = 4,
    INTR_ENABLE0 = 256,
    INTR_STATE0 = 260,
    INTR_TEST0 = 264,
    CFG0 = 268,
    TIMER_V_LOWER0 = 272,
    TIMER_V_UPPER0 = 276,
    COMPARE_LOWER0_0 = 280,
    COMPARE_UPPER0_0 = 284,
  };

  // Aggregate structs mirroring rv_timer_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct rv_timer_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct rv_timer_reg2hw_ctrl_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct rv_timer_reg2hw_intr_enable0_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct rv_timer_reg2hw_intr_state0_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct rv_timer_reg2hw_intr_test0_mreg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct rv_timer_reg2hw_cfg0_reg_t {
    struct {
      sc_dt::sc_bv<8> q;
    } step;
    struct {
      sc_dt::sc_bv<12> q;
    } prescale;
  };

  struct rv_timer_reg2hw_timer_v_lower0_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rv_timer_reg2hw_timer_v_upper0_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rv_timer_reg2hw_compare_lower0_0_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct rv_timer_reg2hw_compare_upper0_0_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct rv_timer_hw2reg_intr_state0_mreg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct rv_timer_hw2reg_timer_v_lower0_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct rv_timer_hw2reg_timer_v_upper0_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  // Register -> HW type
  struct rv_timer_reg2hw_t {
    rv_timer_reg2hw_alert_test_reg_t alert_test;
    std::array<rv_timer_reg2hw_ctrl_mreg_t, 1> ctrl;
    std::array<rv_timer_reg2hw_intr_enable0_mreg_t, 1> intr_enable0;
    std::array<rv_timer_reg2hw_intr_state0_mreg_t, 1> intr_state0;
    std::array<rv_timer_reg2hw_intr_test0_mreg_t, 1> intr_test0;
    rv_timer_reg2hw_cfg0_reg_t cfg0;
    rv_timer_reg2hw_timer_v_lower0_reg_t timer_v_lower0;
    rv_timer_reg2hw_timer_v_upper0_reg_t timer_v_upper0;
    rv_timer_reg2hw_compare_lower0_0_reg_t compare_lower0_0;
    rv_timer_reg2hw_compare_upper0_0_reg_t compare_upper0_0;
  };

  // HW -> register type
  struct rv_timer_hw2reg_t {
    std::array<rv_timer_hw2reg_intr_state0_mreg_t, 1> intr_state0;
    rv_timer_hw2reg_timer_v_lower0_reg_t timer_v_lower0;
    rv_timer_hw2reg_timer_v_upper0_reg_t timer_v_upper0;
  };

  // Flat register storage
  std::array<uint32_t, 10> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 10> shadow_stage_default_{};
  std::array<uint8_t,  10> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 10> racl_allow_rd_default_{};
  std::array<bool, 10> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  rv_timer_reg2hw_t reg2hw;
  rv_timer_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace rv_timer
}  // namespace opentitan



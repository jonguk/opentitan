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
namespace aon_timer {

// ----- Types mirrored from aon_timer_reg_pkg.sv -----


class aon_timer_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<aon_timer_reg_top> tlm_target;

  SC_HAS_PROCESS(aon_timer_reg_top);
  explicit aon_timer_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 56;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_default_e : uint32_t {
    ALERT_TEST = 0,
    WKUP_CTRL = 4,
    WKUP_THOLD_HI = 8,
    WKUP_THOLD_LO = 12,
    WKUP_COUNT_HI = 16,
    WKUP_COUNT_LO = 20,
    WDOG_REGWEN = 24,
    WDOG_CTRL = 28,
    WDOG_BARK_THOLD = 32,
    WDOG_BITE_THOLD = 36,
    WDOG_COUNT = 40,
    INTR_STATE = 44,
    INTR_TEST = 48,
    WKUP_CAUSE = 52,
  };

  // Aggregate structs mirroring aon_timer_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct aon_timer_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct aon_timer_reg2hw_wkup_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } prescaler;
    struct {
      sc_dt::sc_bv<1> q;
    } enable;
  };

  struct aon_timer_reg2hw_wkup_thold_hi_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct aon_timer_reg2hw_wkup_thold_lo_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct aon_timer_reg2hw_wkup_count_hi_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct aon_timer_reg2hw_wkup_count_lo_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct aon_timer_reg2hw_wdog_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } pause_in_sleep;
    struct {
      sc_dt::sc_bv<1> q;
    } enable;
  };

  struct aon_timer_reg2hw_wdog_bark_thold_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct aon_timer_reg2hw_wdog_bite_thold_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct aon_timer_reg2hw_wdog_count_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct aon_timer_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } wdog_timer_bark;
    struct {
      sc_dt::sc_bv<1> q;
    } wkup_timer_expired;
  };

  struct aon_timer_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } wdog_timer_bark;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } wkup_timer_expired;
  };

  struct aon_timer_reg2hw_wkup_cause_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct aon_timer_hw2reg_wkup_count_hi_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct aon_timer_hw2reg_wkup_count_lo_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct aon_timer_hw2reg_wdog_count_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct aon_timer_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } wdog_timer_bark;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } wkup_timer_expired;
  };

  struct aon_timer_hw2reg_wkup_cause_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  // Register -> HW type
  struct aon_timer_reg2hw_t {
    aon_timer_reg2hw_alert_test_reg_t alert_test;
    aon_timer_reg2hw_wkup_ctrl_reg_t wkup_ctrl;
    aon_timer_reg2hw_wkup_thold_hi_reg_t wkup_thold_hi;
    aon_timer_reg2hw_wkup_thold_lo_reg_t wkup_thold_lo;
    aon_timer_reg2hw_wkup_count_hi_reg_t wkup_count_hi;
    aon_timer_reg2hw_wkup_count_lo_reg_t wkup_count_lo;
    aon_timer_reg2hw_wdog_ctrl_reg_t wdog_ctrl;
    aon_timer_reg2hw_wdog_bark_thold_reg_t wdog_bark_thold;
    aon_timer_reg2hw_wdog_bite_thold_reg_t wdog_bite_thold;
    aon_timer_reg2hw_wdog_count_reg_t wdog_count;
    aon_timer_reg2hw_intr_state_reg_t intr_state;
    aon_timer_reg2hw_intr_test_reg_t intr_test;
    aon_timer_reg2hw_wkup_cause_reg_t wkup_cause;
  };

  // HW -> register type
  struct aon_timer_hw2reg_t {
    aon_timer_hw2reg_wkup_count_hi_reg_t wkup_count_hi;
    aon_timer_hw2reg_wkup_count_lo_reg_t wkup_count_lo;
    aon_timer_hw2reg_wdog_count_reg_t wdog_count;
    aon_timer_hw2reg_intr_state_reg_t intr_state;
    aon_timer_hw2reg_wkup_cause_reg_t wkup_cause;
  };

  // Flat register storage
  std::array<uint32_t, 14> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 14> shadow_stage_default_{};
  std::array<uint8_t,  14> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 14> racl_allow_rd_default_{};
  std::array<bool, 14> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  aon_timer_reg2hw_t reg2hw;
  aon_timer_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace aon_timer
}  // namespace opentitan



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
namespace soc_dbg_ctrl {

// ----- Types mirrored from soc_dbg_ctrl_reg_pkg.sv -----


class soc_dbg_ctrl_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<soc_dbg_ctrl_reg_top> tlm_target;

  SC_HAS_PROCESS(soc_dbg_ctrl_reg_top);
  explicit soc_dbg_ctrl_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 52;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_core_e : uint32_t {
    ALERT_TEST = 0,
    DEBUG_POLICY_VALID_SHADOWED = 4,
    DEBUG_POLICY_CATEGORY_SHADOWED = 8,
    DEBUG_POLICY_RELOCKED = 12,
    TRACE_DEBUG_POLICY_CATEGORY = 16,
    TRACE_DEBUG_POLICY_VALID_RELOCKED = 20,
    STATUS = 24,
  };
  enum class reg_jtag_e : uint32_t {
    JTAG_TRACE_DEBUG_POLICY_CATEGORY = 0,
    JTAG_TRACE_DEBUG_POLICY_VALID_RELOCKED = 4,
    JTAG_CONTROL = 8,
    JTAG_STATUS = 12,
    JTAG_BOOT_STATUS = 16,
    JTAG_TRACE_SOC_DBG_STATE = 20,
  };

  // Aggregate structs mirroring soc_dbg_ctrl_reg_pkg

//   ///////////////////////////////////////////////
  // Typedefs for registers for core interface //
  ///////////////////////////////////////////////

  struct soc_dbg_ctrl_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_ctrl_update_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_fault;
  };

  struct soc_dbg_ctrl_reg2hw_debug_policy_valid_shadowed_reg_t {
    sc_dt::sc_bv<4> q;
  };

  struct soc_dbg_ctrl_reg2hw_debug_policy_category_shadowed_reg_t {
    sc_dt::sc_bv<7> q;
    bool qe;
    bool re;
  };

  struct soc_dbg_ctrl_reg2hw_debug_policy_relocked_reg_t {
    sc_dt::sc_bv<4> q;
  };

  struct soc_dbg_ctrl_reg2hw_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } auth_unlock_failed;
    struct {
      sc_dt::sc_bv<1> q;
    } auth_unlock_success;
    struct {
      sc_dt::sc_bv<1> q;
    } auth_window_closed;
    struct {
      sc_dt::sc_bv<1> q;
    } auth_window_open;
    struct {
      sc_dt::sc_bv<1> q;
    } auth_debug_intent_set;
  };

  struct soc_dbg_ctrl_hw2reg_debug_policy_category_shadowed_reg_t {
    sc_dt::sc_bv<7> d;
  };

  struct soc_dbg_ctrl_hw2reg_trace_debug_policy_category_reg_t {
    sc_dt::sc_bv<7> d;
    bool        de;
  };

  struct soc_dbg_ctrl_hw2reg_trace_debug_policy_valid_relocked_reg_t {
    struct {
      sc_dt::sc_bv<4> d;
      bool        de;
    } relocked;
    struct {
      sc_dt::sc_bv<4> d;
      bool        de;
    } valid;
  };

  // Register -> HW type for core interface
  struct soc_dbg_ctrl_core_reg2hw_t {
    soc_dbg_ctrl_reg2hw_alert_test_reg_t alert_test;
    soc_dbg_ctrl_reg2hw_debug_policy_valid_shadowed_reg_t debug_policy_valid_shadowed;
    soc_dbg_ctrl_reg2hw_debug_policy_category_shadowed_reg_t debug_policy_category_shadowed;
    soc_dbg_ctrl_reg2hw_debug_policy_relocked_reg_t debug_policy_relocked;
    soc_dbg_ctrl_reg2hw_status_reg_t status;
  };

  // HW -> register type for core interface
  struct soc_dbg_ctrl_core_hw2reg_t {
    soc_dbg_ctrl_hw2reg_debug_policy_category_shadowed_reg_t debug_policy_category_shadowed;
    soc_dbg_ctrl_hw2reg_trace_debug_policy_category_reg_t trace_debug_policy_category;
    soc_dbg_ctrl_hw2reg_trace_debug_policy_valid_relocked_reg_t trace_debug_policy_valid_relocked;
  };

//   ///////////////////////////////////////////////
  // Typedefs for registers for jtag interface //
  ///////////////////////////////////////////////

  struct soc_dbg_ctrl_reg2hw_jtag_control_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct soc_dbg_ctrl_hw2reg_jtag_trace_debug_policy_category_reg_t {
    sc_dt::sc_bv<7> d;
    bool        de;
  };

  struct soc_dbg_ctrl_hw2reg_jtag_trace_debug_policy_valid_relocked_reg_t {
    struct {
      sc_dt::sc_bv<4> d;
      bool        de;
    } relocked;
    struct {
      sc_dt::sc_bv<4> d;
      bool        de;
    } valid;
  };

  struct soc_dbg_ctrl_hw2reg_jtag_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } auth_unlock_failed;
    struct {
      sc_dt::sc_bv<1> d;
    } auth_unlock_success;
    struct {
      sc_dt::sc_bv<1> d;
    } auth_window_closed;
    struct {
      sc_dt::sc_bv<1> d;
    } auth_window_open;
    struct {
      sc_dt::sc_bv<1> d;
    } auth_debug_intent_set;
  };

  struct soc_dbg_ctrl_hw2reg_jtag_boot_status_reg_t {
    struct {
      sc_dt::sc_bv<3> d;
    } boot_greenlight_good;
    struct {
      sc_dt::sc_bv<3> d;
    } boot_greenlight_done;
    struct {
      sc_dt::sc_bv<6> d;
    } halt_fsm_state;
    struct {
      sc_dt::sc_bv<1> d;
    } cpu_fetch_en;
    struct {
      sc_dt::sc_bv<1> d;
    } lc_done;
    struct {
      sc_dt::sc_bv<1> d;
    } otp_done;
    struct {
      sc_dt::sc_bv<1> d;
    } io_clk_status;
    struct {
      sc_dt::sc_bv<1> d;
    } main_clk_status;
  };

  struct soc_dbg_ctrl_hw2reg_jtag_trace_soc_dbg_state_reg_t {
    sc_dt::sc_bv<32> d;
  };

  // Register -> HW type for jtag interface
  struct soc_dbg_ctrl_jtag_reg2hw_t {
    soc_dbg_ctrl_reg2hw_jtag_control_reg_t jtag_control;
  };

  // HW -> register type for jtag interface
  struct soc_dbg_ctrl_jtag_hw2reg_t {
    soc_dbg_ctrl_hw2reg_jtag_trace_debug_policy_category_reg_t jtag_trace_debug_policy_category;
    soc_dbg_ctrl_hw2reg_jtag_trace_debug_policy_valid_relocked_reg_t jtag_trace_debug_policy_valid_relocked;
    soc_dbg_ctrl_hw2reg_jtag_status_reg_t jtag_status;
    soc_dbg_ctrl_hw2reg_jtag_boot_status_reg_t jtag_boot_status;
    soc_dbg_ctrl_hw2reg_jtag_trace_soc_dbg_state_reg_t jtag_trace_soc_dbg_state;
  };

  // Flat register storage
  std::array<uint32_t, 7> regs_core_;
  std::array<uint32_t, 6> regs_jtag_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 7> shadow_stage_core_{};
  std::array<uint8_t,  7> shadow_phase_core_{}; // 0:first,1:second
  std::array<uint32_t, 6> shadow_stage_jtag_{};
  std::array<uint8_t,  6> shadow_phase_jtag_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 7> racl_allow_rd_core_{};
  std::array<bool, 7> racl_allow_wr_core_{};
  std::array<bool, 6> racl_allow_rd_jtag_{};
  std::array<bool, 6> racl_allow_wr_jtag_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  soc_dbg_ctrl_core_reg2hw_t reg2hw_core;
  soc_dbg_ctrl_core_hw2reg_t  hw2reg_core;
  soc_dbg_ctrl_jtag_reg2hw_t reg2hw_jtag;
  soc_dbg_ctrl_jtag_hw2reg_t  hw2reg_jtag;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace soc_dbg_ctrl
}  // namespace opentitan



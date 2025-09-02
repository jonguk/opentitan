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
namespace rom_ctrl {

// ----- Types mirrored from rom_ctrl_reg_pkg.sv -----


class rom_ctrl_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<rom_ctrl_reg_top> tlm_target;

  SC_HAS_PROCESS(rom_ctrl_reg_top);
  explicit rom_ctrl_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 72;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_regs_e : uint32_t {
    ALERT_TEST = 0,
    FATAL_ALERT_CAUSE = 4,
    DIGEST_0 = 8,
    DIGEST_1 = 12,
    DIGEST_2 = 16,
    DIGEST_3 = 20,
    DIGEST_4 = 24,
    DIGEST_5 = 28,
    DIGEST_6 = 32,
    DIGEST_7 = 36,
    EXP_DIGEST_0 = 40,
    EXP_DIGEST_1 = 44,
    EXP_DIGEST_2 = 48,
    EXP_DIGEST_3 = 52,
    EXP_DIGEST_4 = 56,
    EXP_DIGEST_5 = 60,
    EXP_DIGEST_6 = 64,
    EXP_DIGEST_7 = 68,
  };
  enum class reg_rom_e : uint32_t {
  };

  // Aggregate structs mirroring rom_ctrl_reg_pkg

//   ///////////////////////////////////////////////
  // Typedefs for registers for regs interface //
  ///////////////////////////////////////////////

  struct rom_ctrl_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct rom_ctrl_reg2hw_digest_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rom_ctrl_reg2hw_exp_digest_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rom_ctrl_hw2reg_fatal_alert_cause_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } integrity_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } checker_error;
  };

  struct rom_ctrl_hw2reg_digest_mreg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct rom_ctrl_hw2reg_exp_digest_mreg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  // Register -> HW type for regs interface
  struct rom_ctrl_regs_reg2hw_t {
    rom_ctrl_reg2hw_alert_test_reg_t alert_test;
    std::array<rom_ctrl_reg2hw_digest_mreg_t, 8> digest;
    std::array<rom_ctrl_reg2hw_exp_digest_mreg_t, 8> exp_digest;
  };

  // HW -> register type for regs interface
  struct rom_ctrl_regs_hw2reg_t {
    rom_ctrl_hw2reg_fatal_alert_cause_reg_t fatal_alert_cause;
    std::array<rom_ctrl_hw2reg_digest_mreg_t, 8> digest;
    std::array<rom_ctrl_hw2reg_exp_digest_mreg_t, 8> exp_digest;
  };

  // Flat register storage
  std::array<uint32_t, 18> regs_regs_;
  std::array<uint32_t, 0> regs_rom_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 18> shadow_stage_regs_{};
  std::array<uint8_t,  18> shadow_phase_regs_{}; // 0:first,1:second
  std::array<uint32_t, 0> shadow_stage_rom_{};
  std::array<uint8_t,  0> shadow_phase_rom_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 18> racl_allow_rd_regs_{};
  std::array<bool, 18> racl_allow_wr_regs_{};
  std::array<bool, 0> racl_allow_rd_rom_{};
  std::array<bool, 0> racl_allow_wr_rom_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  rom_ctrl_regs_reg2hw_t reg2hw_regs;
  rom_ctrl_regs_hw2reg_t  hw2reg_regs;
  rom_ctrl_rom_reg2hw_t reg2hw_rom;
  rom_ctrl_rom_hw2reg_t  hw2reg_rom;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace rom_ctrl
}  // namespace opentitan



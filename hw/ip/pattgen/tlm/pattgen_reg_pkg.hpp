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
namespace pattgen {

// ----- Types mirrored from pattgen_reg_pkg.sv -----


class pattgen_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<pattgen_reg_top> tlm_target;

  SC_HAS_PROCESS(pattgen_reg_top);
  explicit pattgen_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 48;

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
    CTRL = 16,
    PREDIV_CH0 = 20,
    PREDIV_CH1 = 24,
    DATA_CH0_0 = 28,
    DATA_CH0_1 = 32,
    DATA_CH1_0 = 36,
    DATA_CH1_1 = 40,
    SIZE = 44,
  };

  // Aggregate structs mirroring pattgen_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct pattgen_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } done_ch1;
    struct {
      sc_dt::sc_bv<1> q;
    } done_ch0;
  };

  struct pattgen_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } done_ch1;
    struct {
      sc_dt::sc_bv<1> q;
    } done_ch0;
  };

  struct pattgen_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } done_ch1;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } done_ch0;
  };

  struct pattgen_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct pattgen_reg2hw_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } inactive_level_pda_ch1;
    struct {
      sc_dt::sc_bv<1> q;
    } inactive_level_pcl_ch1;
    struct {
      sc_dt::sc_bv<1> q;
    } inactive_level_pda_ch0;
    struct {
      sc_dt::sc_bv<1> q;
    } inactive_level_pcl_ch0;
    struct {
      sc_dt::sc_bv<1> q;
    } polarity_ch1;
    struct {
      sc_dt::sc_bv<1> q;
    } polarity_ch0;
    struct {
      sc_dt::sc_bv<1> q;
    } enable_ch1;
    struct {
      sc_dt::sc_bv<1> q;
    } enable_ch0;
  };

  struct pattgen_reg2hw_prediv_ch0_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct pattgen_reg2hw_prediv_ch1_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct pattgen_reg2hw_data_ch0_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct pattgen_reg2hw_data_ch1_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct pattgen_reg2hw_size_reg_t {
    struct {
      sc_dt::sc_bv<10> q;
    } reps_ch1;
    struct {
      sc_dt::sc_bv<6> q;
    } len_ch1;
    struct {
      sc_dt::sc_bv<10> q;
    } reps_ch0;
    struct {
      sc_dt::sc_bv<6> q;
    } len_ch0;
  };

  struct pattgen_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } done_ch1;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } done_ch0;
  };

  // Register -> HW type
  struct pattgen_reg2hw_t {
    pattgen_reg2hw_intr_state_reg_t intr_state;
    pattgen_reg2hw_intr_enable_reg_t intr_enable;
    pattgen_reg2hw_intr_test_reg_t intr_test;
    pattgen_reg2hw_alert_test_reg_t alert_test;
    pattgen_reg2hw_ctrl_reg_t ctrl;
    pattgen_reg2hw_prediv_ch0_reg_t prediv_ch0;
    pattgen_reg2hw_prediv_ch1_reg_t prediv_ch1;
    std::array<pattgen_reg2hw_data_ch0_mreg_t, 2> data_ch0;
    std::array<pattgen_reg2hw_data_ch1_mreg_t, 2> data_ch1;
    pattgen_reg2hw_size_reg_t size;
  };

  // HW -> register type
  struct pattgen_hw2reg_t {
    pattgen_hw2reg_intr_state_reg_t intr_state;
  };

  // Flat register storage
  std::array<uint32_t, 12> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 12> shadow_stage_default_{};
  std::array<uint8_t,  12> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 12> racl_allow_rd_default_{};
  std::array<bool, 12> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  pattgen_reg2hw_t reg2hw;
  pattgen_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace pattgen
}  // namespace opentitan



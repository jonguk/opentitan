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
namespace otbn {

// ----- Types mirrored from otbn_reg_pkg.sv -----


class otbn_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<otbn_reg_top> tlm_target;

  SC_HAS_PROCESS(otbn_reg_top);
  explicit otbn_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 44;

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
    CMD = 16,
    CTRL = 20,
    STATUS = 24,
    ERR_BITS = 28,
    FATAL_ALERT_CAUSE = 32,
    INSN_CNT = 36,
    LOAD_CHECKSUM = 40,
  };

  // Aggregate structs mirroring otbn_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct otbn_reg2hw_intr_state_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct otbn_reg2hw_intr_enable_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct otbn_reg2hw_intr_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct otbn_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal;
  };

  struct otbn_reg2hw_cmd_reg_t {
    sc_dt::sc_bv<8> q;
    bool qe;
  };

  struct otbn_reg2hw_ctrl_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct otbn_reg2hw_err_bits_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_software;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } lifecycle_escalation;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } illegal_bus_access;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } bad_internal_state;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } bus_intg_violation;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } reg_intg_violation;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } dmem_intg_violation;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } imem_intg_violation;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rnd_fips_chk_fail;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rnd_rep_chk_fail;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } key_invalid;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } loop;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } illegal_insn;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } call_stack;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } bad_insn_addr;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } bad_data_addr;
  };

  struct otbn_reg2hw_insn_cnt_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct otbn_reg2hw_load_checksum_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct otbn_hw2reg_intr_state_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct otbn_hw2reg_ctrl_reg_t {
    sc_dt::sc_bv<1> d;
  };

  struct otbn_hw2reg_status_reg_t {
    sc_dt::sc_bv<8> d;
    bool        de;
  };

  struct otbn_hw2reg_err_bits_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } fatal_software;
    struct {
      sc_dt::sc_bv<1> d;
    } lifecycle_escalation;
    struct {
      sc_dt::sc_bv<1> d;
    } illegal_bus_access;
    struct {
      sc_dt::sc_bv<1> d;
    } bad_internal_state;
    struct {
      sc_dt::sc_bv<1> d;
    } bus_intg_violation;
    struct {
      sc_dt::sc_bv<1> d;
    } reg_intg_violation;
    struct {
      sc_dt::sc_bv<1> d;
    } dmem_intg_violation;
    struct {
      sc_dt::sc_bv<1> d;
    } imem_intg_violation;
    struct {
      sc_dt::sc_bv<1> d;
    } rnd_fips_chk_fail;
    struct {
      sc_dt::sc_bv<1> d;
    } rnd_rep_chk_fail;
    struct {
      sc_dt::sc_bv<1> d;
    } key_invalid;
    struct {
      sc_dt::sc_bv<1> d;
    } loop;
    struct {
      sc_dt::sc_bv<1> d;
    } illegal_insn;
    struct {
      sc_dt::sc_bv<1> d;
    } call_stack;
    struct {
      sc_dt::sc_bv<1> d;
    } bad_insn_addr;
    struct {
      sc_dt::sc_bv<1> d;
    } bad_data_addr;
  };

  struct otbn_hw2reg_fatal_alert_cause_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fatal_software;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } lifecycle_escalation;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } illegal_bus_access;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } bad_internal_state;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } bus_intg_violation;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } reg_intg_violation;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } dmem_intg_violation;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } imem_intg_violation;
  };

  struct otbn_hw2reg_insn_cnt_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct otbn_hw2reg_load_checksum_reg_t {
    sc_dt::sc_bv<32> d;
  };

  // Register -> HW type
  struct otbn_reg2hw_t {
    otbn_reg2hw_intr_state_reg_t intr_state;
    otbn_reg2hw_intr_enable_reg_t intr_enable;
    otbn_reg2hw_intr_test_reg_t intr_test;
    otbn_reg2hw_alert_test_reg_t alert_test;
    otbn_reg2hw_cmd_reg_t cmd;
    otbn_reg2hw_ctrl_reg_t ctrl;
    otbn_reg2hw_err_bits_reg_t err_bits;
    otbn_reg2hw_insn_cnt_reg_t insn_cnt;
    otbn_reg2hw_load_checksum_reg_t load_checksum;
  };

  // HW -> register type
  struct otbn_hw2reg_t {
    otbn_hw2reg_intr_state_reg_t intr_state;
    otbn_hw2reg_ctrl_reg_t ctrl;
    otbn_hw2reg_status_reg_t status;
    otbn_hw2reg_err_bits_reg_t err_bits;
    otbn_hw2reg_fatal_alert_cause_reg_t fatal_alert_cause;
    otbn_hw2reg_insn_cnt_reg_t insn_cnt;
    otbn_hw2reg_load_checksum_reg_t load_checksum;
  };

  // Flat register storage
  std::array<uint32_t, 11> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 11> shadow_stage_default_{};
  std::array<uint8_t,  11> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 11> racl_allow_rd_default_{};
  std::array<bool, 11> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  otbn_reg2hw_t reg2hw;
  otbn_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace otbn
}  // namespace opentitan



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
namespace ascon {

// ----- Types mirrored from ascon_reg_pkg.sv -----


class ascon_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<ascon_reg_top> tlm_target;

  SC_HAS_PROCESS(ascon_reg_top);
  explicit ascon_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 188;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_default_e : uint32_t {
    ALERT_TEST = 0,
    KEY_SHARE0_0 = 4,
    KEY_SHARE0_1 = 8,
    KEY_SHARE0_2 = 12,
    KEY_SHARE0_3 = 16,
    KEY_SHARE1_0 = 20,
    KEY_SHARE1_1 = 24,
    KEY_SHARE1_2 = 28,
    KEY_SHARE1_3 = 32,
    NONCE_SHARE0_0 = 36,
    NONCE_SHARE0_1 = 40,
    NONCE_SHARE0_2 = 44,
    NONCE_SHARE0_3 = 48,
    NONCE_SHARE1_0 = 52,
    NONCE_SHARE1_1 = 56,
    NONCE_SHARE1_2 = 60,
    NONCE_SHARE1_3 = 64,
    DATA_IN_SHARE0_0 = 68,
    DATA_IN_SHARE0_1 = 72,
    DATA_IN_SHARE0_2 = 76,
    DATA_IN_SHARE0_3 = 80,
    DATA_IN_SHARE1_0 = 84,
    DATA_IN_SHARE1_1 = 88,
    DATA_IN_SHARE1_2 = 92,
    DATA_IN_SHARE1_3 = 96,
    TAG_IN_0 = 100,
    TAG_IN_1 = 104,
    TAG_IN_2 = 108,
    TAG_IN_3 = 112,
    MSG_OUT_0 = 116,
    MSG_OUT_1 = 120,
    MSG_OUT_2 = 124,
    MSG_OUT_3 = 128,
    TAG_OUT_0 = 132,
    TAG_OUT_1 = 136,
    TAG_OUT_2 = 140,
    TAG_OUT_3 = 144,
    CTRL_SHADOWED = 148,
    CTRL_AUX_SHADOWED = 152,
    CTRL_AUX_REGWEN = 156,
    BLOCK_CTRL_SHADOWED = 160,
    TRIGGER = 164,
    STATUS = 168,
    OUTPUT_VALID = 172,
    FSM_STATE = 176,
    FSM_STATE_REGREN = 180,
    ERROR = 184,
  };

  // Aggregate structs mirroring ascon_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct ascon_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_fault;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_ctrl_update_err;
  };

  struct ascon_reg2hw_key_share0_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct ascon_reg2hw_key_share1_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct ascon_reg2hw_nonce_share0_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct ascon_reg2hw_nonce_share1_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct ascon_reg2hw_data_in_share0_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct ascon_reg2hw_data_in_share1_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct ascon_reg2hw_tag_in_mreg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct ascon_reg2hw_msg_out_mreg_t {
    sc_dt::sc_bv<32> q;
    bool re;
  };

  struct ascon_reg2hw_tag_out_mreg_t {
    sc_dt::sc_bv<32> q;
    bool re;
  };

  struct ascon_reg2hw_ctrl_shadowed_reg_t {
    struct {
      sc_dt::sc_bv<4> q;
    } no_ad;
    struct {
      sc_dt::sc_bv<4> q;
    } no_msg;
    struct {
      sc_dt::sc_bv<1> q;
    } masked_msg_input;
    struct {
      sc_dt::sc_bv<1> q;
    } masked_ad_input;
    struct {
      sc_dt::sc_bv<1> q;
    } sideload_key;
    struct {
      sc_dt::sc_bv<2> q;
    } ascon_variant;
    struct {
      sc_dt::sc_bv<3> q;
    } operation;
  };

  struct ascon_reg2hw_ctrl_aux_shadowed_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } force_data_overwrite;
    struct {
      sc_dt::sc_bv<1> q;
    } manual_start_trigger;
  };

  struct ascon_reg2hw_block_ctrl_shadowed_reg_t {
    struct {
      sc_dt::sc_bv<5> q;
    } valid_bytes;
    struct {
      sc_dt::sc_bv<12> q;
    } data_type_last;
    struct {
      sc_dt::sc_bv<12> q;
    } data_type_start;
  };

  struct ascon_reg2hw_trigger_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } wipe;
    struct {
      sc_dt::sc_bv<1> q;
    } start;
  };

  struct ascon_reg2hw_fsm_state_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct ascon_hw2reg_key_share0_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_key_share1_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_nonce_share0_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_nonce_share1_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_data_in_share0_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_data_in_share1_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_msg_out_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_tag_out_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_trigger_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } wipe;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } start;
  };

  struct ascon_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } alert_fatal_fault;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } alert_recov_ctrl_update_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ascon_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } wait_edn;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } stall;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } idle;
  };

  struct ascon_hw2reg_output_valid_reg_t {
    struct {
      sc_dt::sc_bv<2> d;
      bool        de;
    } tag_comparison_valid;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tag_valid;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } msg_valid;
  };

  struct ascon_hw2reg_fsm_state_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct ascon_hw2reg_error_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } flag_input_missmatch;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } wrong_order;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } no_nonce;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } no_key;
  };

  // Register -> HW type
  struct ascon_reg2hw_t {
    ascon_reg2hw_alert_test_reg_t alert_test;
    std::array<ascon_reg2hw_key_share0_mreg_t, 4> key_share0;
    std::array<ascon_reg2hw_key_share1_mreg_t, 4> key_share1;
    std::array<ascon_reg2hw_nonce_share0_mreg_t, 4> nonce_share0;
    std::array<ascon_reg2hw_nonce_share1_mreg_t, 4> nonce_share1;
    std::array<ascon_reg2hw_data_in_share0_mreg_t, 4> data_in_share0;
    std::array<ascon_reg2hw_data_in_share1_mreg_t, 4> data_in_share1;
    std::array<ascon_reg2hw_tag_in_mreg_t, 4> tag_in;
    std::array<ascon_reg2hw_msg_out_mreg_t, 4> msg_out;
    std::array<ascon_reg2hw_tag_out_mreg_t, 4> tag_out;
    ascon_reg2hw_ctrl_shadowed_reg_t ctrl_shadowed;
    ascon_reg2hw_ctrl_aux_shadowed_reg_t ctrl_aux_shadowed;
    ascon_reg2hw_block_ctrl_shadowed_reg_t block_ctrl_shadowed;
    ascon_reg2hw_trigger_reg_t trigger;
    ascon_reg2hw_fsm_state_reg_t fsm_state;
  };

  // HW -> register type
  struct ascon_hw2reg_t {
    std::array<ascon_hw2reg_key_share0_mreg_t, 4> key_share0;
    std::array<ascon_hw2reg_key_share1_mreg_t, 4> key_share1;
    std::array<ascon_hw2reg_nonce_share0_mreg_t, 4> nonce_share0;
    std::array<ascon_hw2reg_nonce_share1_mreg_t, 4> nonce_share1;
    std::array<ascon_hw2reg_data_in_share0_mreg_t, 4> data_in_share0;
    std::array<ascon_hw2reg_data_in_share1_mreg_t, 4> data_in_share1;
    std::array<ascon_hw2reg_msg_out_mreg_t, 4> msg_out;
    std::array<ascon_hw2reg_tag_out_mreg_t, 4> tag_out;
    ascon_hw2reg_trigger_reg_t trigger;
    ascon_hw2reg_status_reg_t status;
    ascon_hw2reg_output_valid_reg_t output_valid;
    ascon_hw2reg_fsm_state_reg_t fsm_state;
    ascon_hw2reg_error_reg_t error;
  };

  // Flat register storage
  std::array<uint32_t, 47> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 47> shadow_stage_default_{};
  std::array<uint8_t,  47> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 47> racl_allow_rd_default_{};
  std::array<bool, 47> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  ascon_reg2hw_t reg2hw;
  ascon_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace ascon
}  // namespace opentitan



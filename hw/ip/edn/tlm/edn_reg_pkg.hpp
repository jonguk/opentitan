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
namespace edn {

// ----- Types mirrored from edn_reg_pkg.sv -----


class edn_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<edn_reg_top> tlm_target;

  SC_HAS_PROCESS(edn_reg_top);
  explicit edn_reg_top(sc_core::sc_module_name name);

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
  enum class reg_default_e : uint32_t {
    INTR_STATE = 0,
    INTR_ENABLE = 4,
    INTR_TEST = 8,
    ALERT_TEST = 12,
    REGWEN = 16,
    CTRL = 20,
    BOOT_INS_CMD = 24,
    BOOT_GEN_CMD = 28,
    SW_CMD_REQ = 32,
    SW_CMD_STS = 36,
    HW_CMD_STS = 40,
    RESEED_CMD = 44,
    GENERATE_CMD = 48,
    MAX_NUM_REQS_BETWEEN_RESEEDS = 52,
    RECOV_ALERT_STS = 56,
    ERR_CODE = 60,
    ERR_CODE_TEST = 64,
    MAIN_SM_STATE = 68,
  };

  // Aggregate structs mirroring edn_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct edn_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } edn_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
    } edn_cmd_req_done;
  };

  struct edn_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } edn_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
    } edn_cmd_req_done;
  };

  struct edn_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } edn_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } edn_cmd_req_done;
  };

  struct edn_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_alert;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_alert;
  };

  struct edn_reg2hw_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<4> q;
    } cmd_fifo_rst;
    struct {
      sc_dt::sc_bv<4> q;
    } auto_req_mode;
    struct {
      sc_dt::sc_bv<4> q;
    } boot_req_mode;
    struct {
      sc_dt::sc_bv<4> q;
    } edn_enable;
  };

  struct edn_reg2hw_boot_ins_cmd_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct edn_reg2hw_boot_gen_cmd_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct edn_reg2hw_sw_cmd_req_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct edn_reg2hw_reseed_cmd_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct edn_reg2hw_generate_cmd_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct edn_reg2hw_max_num_reqs_between_reseeds_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct edn_reg2hw_err_code_test_reg_t {
    sc_dt::sc_bv<5> q;
    bool qe;
  };

  struct edn_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } edn_fatal_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } edn_cmd_req_done;
  };

  struct edn_hw2reg_sw_cmd_sts_reg_t {
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } cmd_sts;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_ack;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_rdy;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_reg_rdy;
  };

  struct edn_hw2reg_hw_cmd_sts_reg_t {
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } cmd_sts;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_ack;
    struct {
      sc_dt::sc_bv<4> d;
      bool        de;
    } cmd_type;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } auto_mode;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } boot_mode;
  };

  struct edn_hw2reg_recov_alert_sts_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } csrng_ack_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } edn_bus_cmp_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_fifo_rst_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } auto_req_mode_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } boot_req_mode_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } edn_enable_field_alert;
  };

  struct edn_hw2reg_err_code_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fifo_state_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fifo_read_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fifo_write_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } edn_cntr_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } edn_main_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } edn_ack_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_gencmd_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_rescmd_err;
  };

  struct edn_hw2reg_main_sm_state_reg_t {
    sc_dt::sc_bv<9> d;
    bool        de;
  };

  // Register -> HW type
  struct edn_reg2hw_t {
    edn_reg2hw_intr_state_reg_t intr_state;
    edn_reg2hw_intr_enable_reg_t intr_enable;
    edn_reg2hw_intr_test_reg_t intr_test;
    edn_reg2hw_alert_test_reg_t alert_test;
    edn_reg2hw_ctrl_reg_t ctrl;
    edn_reg2hw_boot_ins_cmd_reg_t boot_ins_cmd;
    edn_reg2hw_boot_gen_cmd_reg_t boot_gen_cmd;
    edn_reg2hw_sw_cmd_req_reg_t sw_cmd_req;
    edn_reg2hw_reseed_cmd_reg_t reseed_cmd;
    edn_reg2hw_generate_cmd_reg_t generate_cmd;
    edn_reg2hw_max_num_reqs_between_reseeds_reg_t max_num_reqs_between_reseeds;
    edn_reg2hw_err_code_test_reg_t err_code_test;
  };

  // HW -> register type
  struct edn_hw2reg_t {
    edn_hw2reg_intr_state_reg_t intr_state;
    edn_hw2reg_sw_cmd_sts_reg_t sw_cmd_sts;
    edn_hw2reg_hw_cmd_sts_reg_t hw_cmd_sts;
    edn_hw2reg_recov_alert_sts_reg_t recov_alert_sts;
    edn_hw2reg_err_code_reg_t err_code;
    edn_hw2reg_main_sm_state_reg_t main_sm_state;
  };

  // Flat register storage
  std::array<uint32_t, 18> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 18> shadow_stage_default_{};
  std::array<uint8_t,  18> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 18> racl_allow_rd_default_{};
  std::array<bool, 18> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  edn_reg2hw_t reg2hw;
  edn_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace edn
}  // namespace opentitan



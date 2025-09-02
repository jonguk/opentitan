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
namespace csrng {

// ----- Types mirrored from csrng_reg_pkg.sv -----


class csrng_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<csrng_reg_top> tlm_target;

  SC_HAS_PROCESS(csrng_reg_top);
  explicit csrng_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 96;

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
    CMD_REQ = 24,
    RESEED_INTERVAL = 28,
    RESEED_COUNTER_0 = 32,
    RESEED_COUNTER_1 = 36,
    RESEED_COUNTER_2 = 40,
    SW_CMD_STS = 44,
    GENBITS_VLD = 48,
    GENBITS = 52,
    INT_STATE_READ_ENABLE = 56,
    INT_STATE_READ_ENABLE_REGWEN = 60,
    INT_STATE_NUM = 64,
    INT_STATE_VAL = 68,
    FIPS_FORCE = 72,
    HW_EXC_STS = 76,
    RECOV_ALERT_STS = 80,
    ERR_CODE = 84,
    ERR_CODE_TEST = 88,
    MAIN_SM_STATE = 92,
  };

  // Aggregate structs mirroring csrng_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct csrng_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } cs_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
    } cs_hw_inst_exc;
    struct {
      sc_dt::sc_bv<1> q;
    } cs_entropy_req;
    struct {
      sc_dt::sc_bv<1> q;
    } cs_cmd_req_done;
  };

  struct csrng_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } cs_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
    } cs_hw_inst_exc;
    struct {
      sc_dt::sc_bv<1> q;
    } cs_entropy_req;
    struct {
      sc_dt::sc_bv<1> q;
    } cs_cmd_req_done;
  };

  struct csrng_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } cs_fatal_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } cs_hw_inst_exc;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } cs_entropy_req;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } cs_cmd_req_done;
  };

  struct csrng_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_alert;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_alert;
  };

  struct csrng_reg2hw_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<4> q;
    } fips_force_enable;
    struct {
      sc_dt::sc_bv<4> q;
    } read_int_state;
    struct {
      sc_dt::sc_bv<4> q;
    } sw_app_enable;
    struct {
      sc_dt::sc_bv<4> q;
    } enable;
  };

  struct csrng_reg2hw_cmd_req_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct csrng_reg2hw_reseed_interval_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct csrng_reg2hw_genbits_reg_t {
    sc_dt::sc_bv<32> q;
    bool re;
  };

  struct csrng_reg2hw_int_state_read_enable_reg_t {
    sc_dt::sc_bv<3> q;
  };

  struct csrng_reg2hw_int_state_num_reg_t {
    sc_dt::sc_bv<4> q;
    bool qe;
  };

  struct csrng_reg2hw_int_state_val_reg_t {
    sc_dt::sc_bv<32> q;
    bool re;
  };

  struct csrng_reg2hw_fips_force_reg_t {
    sc_dt::sc_bv<3> q;
  };

  struct csrng_reg2hw_err_code_test_reg_t {
    sc_dt::sc_bv<5> q;
    bool qe;
  };

  struct csrng_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cs_fatal_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cs_hw_inst_exc;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cs_entropy_req;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cs_cmd_req_done;
  };

  struct csrng_hw2reg_reseed_counter_mreg_t {
    sc_dt::sc_bv<32> d;
  };

  struct csrng_hw2reg_sw_cmd_sts_reg_t {
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
  };

  struct csrng_hw2reg_genbits_vld_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } genbits_fips;
    struct {
      sc_dt::sc_bv<1> d;
    } genbits_vld;
  };

  struct csrng_hw2reg_genbits_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct csrng_hw2reg_int_state_val_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct csrng_hw2reg_hw_exc_sts_reg_t {
    sc_dt::sc_bv<16> d;
    bool        de;
  };

  struct csrng_hw2reg_recov_alert_sts_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_stage_reseed_cnt_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_stage_invalid_cmd_seq_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_stage_invalid_acmd_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cs_bus_cmp_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } acmd_flag0_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } fips_force_enable_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } read_int_state_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sw_app_enable_field_alert;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } enable_field_alert;
  };

  struct csrng_hw2reg_err_code_reg_t {
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
    } cmd_gen_cnt_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } aes_cipher_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } drbg_updob_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } drbg_updbe_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } drbg_gen_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } main_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmd_stage_sm_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_blkenc_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_ggenbits_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_gadstage_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_ggenreq_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_grcstage_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_gbencack_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_final_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_pdata_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_bencack_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_bencreq_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_updreq_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_keyvrc_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_rcstage_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_cmdreq_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_genbits_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sfifo_cmd_err;
  };

  struct csrng_hw2reg_main_sm_state_reg_t {
    sc_dt::sc_bv<8> d;
    bool        de;
  };

  // Register -> HW type
  struct csrng_reg2hw_t {
    csrng_reg2hw_intr_state_reg_t intr_state;
    csrng_reg2hw_intr_enable_reg_t intr_enable;
    csrng_reg2hw_intr_test_reg_t intr_test;
    csrng_reg2hw_alert_test_reg_t alert_test;
    csrng_reg2hw_ctrl_reg_t ctrl;
    csrng_reg2hw_cmd_req_reg_t cmd_req;
    csrng_reg2hw_reseed_interval_reg_t reseed_interval;
    csrng_reg2hw_genbits_reg_t genbits;
    csrng_reg2hw_int_state_read_enable_reg_t int_state_read_enable;
    csrng_reg2hw_int_state_num_reg_t int_state_num;
    csrng_reg2hw_int_state_val_reg_t int_state_val;
    csrng_reg2hw_fips_force_reg_t fips_force;
    csrng_reg2hw_err_code_test_reg_t err_code_test;
  };

  // HW -> register type
  struct csrng_hw2reg_t {
    csrng_hw2reg_intr_state_reg_t intr_state;
    std::array<csrng_hw2reg_reseed_counter_mreg_t, 3> reseed_counter;
    csrng_hw2reg_sw_cmd_sts_reg_t sw_cmd_sts;
    csrng_hw2reg_genbits_vld_reg_t genbits_vld;
    csrng_hw2reg_genbits_reg_t genbits;
    csrng_hw2reg_int_state_val_reg_t int_state_val;
    csrng_hw2reg_hw_exc_sts_reg_t hw_exc_sts;
    csrng_hw2reg_recov_alert_sts_reg_t recov_alert_sts;
    csrng_hw2reg_err_code_reg_t err_code;
    csrng_hw2reg_main_sm_state_reg_t main_sm_state;
  };

  // Flat register storage
  std::array<uint32_t, 24> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 24> shadow_stage_default_{};
  std::array<uint8_t,  24> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 24> racl_allow_rd_default_{};
  std::array<bool, 24> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  csrng_reg2hw_t reg2hw;
  csrng_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace csrng
}  // namespace opentitan



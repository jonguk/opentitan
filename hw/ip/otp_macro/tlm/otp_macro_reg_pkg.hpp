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
namespace otp_macro {

// ----- Types mirrored from otp_macro_reg_pkg.sv -----


class otp_macro_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<otp_macro_reg_top> tlm_target;

  SC_HAS_PROCESS(otp_macro_reg_top);
  explicit otp_macro_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 32;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_prim_e : uint32_t {
    CSR0 = 0,
    CSR1 = 4,
    CSR2 = 8,
    CSR3 = 12,
    CSR4 = 16,
    CSR5 = 20,
    CSR6 = 24,
    CSR7 = 28,
  };

  // Aggregate structs mirroring otp_macro_reg_pkg

//   ///////////////////////////////////////////////
  // Typedefs for registers for prim interface //
  ///////////////////////////////////////////////

  struct otp_macro_reg2hw_csr0_reg_t {
    struct {
      sc_dt::sc_bv<11> q;
    } field4;
    struct {
      sc_dt::sc_bv<10> q;
    } field3;
    struct {
      sc_dt::sc_bv<1> q;
    } field2;
    struct {
      sc_dt::sc_bv<1> q;
    } field1;
    struct {
      sc_dt::sc_bv<1> q;
    } field0;
  };

  struct otp_macro_reg2hw_csr1_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
    } field4;
    struct {
      sc_dt::sc_bv<1> q;
    } field3;
    struct {
      sc_dt::sc_bv<7> q;
    } field2;
    struct {
      sc_dt::sc_bv<1> q;
    } field1;
    struct {
      sc_dt::sc_bv<7> q;
    } field0;
  };

  struct otp_macro_reg2hw_csr2_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct otp_macro_reg2hw_csr3_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } field8;
    struct {
      sc_dt::sc_bv<1> q;
    } field7;
    struct {
      sc_dt::sc_bv<1> q;
    } field6;
    struct {
      sc_dt::sc_bv<1> q;
    } field5;
    struct {
      sc_dt::sc_bv<1> q;
    } field4;
    struct {
      sc_dt::sc_bv<1> q;
    } field3;
    struct {
      sc_dt::sc_bv<1> q;
    } field2;
    struct {
      sc_dt::sc_bv<10> q;
    } field1;
    struct {
      sc_dt::sc_bv<3> q;
    } field0;
  };

  struct otp_macro_reg2hw_csr4_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } field3;
    struct {
      sc_dt::sc_bv<1> q;
    } field2;
    struct {
      sc_dt::sc_bv<1> q;
    } field1;
    struct {
      sc_dt::sc_bv<10> q;
    } field0;
  };

  struct otp_macro_reg2hw_csr5_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
    } field6;
    struct {
      sc_dt::sc_bv<1> q;
    } field5;
    struct {
      sc_dt::sc_bv<1> q;
    } field4;
    struct {
      sc_dt::sc_bv<3> q;
    } field3;
    struct {
      sc_dt::sc_bv<1> q;
    } field2;
    struct {
      sc_dt::sc_bv<2> q;
    } field1;
    struct {
      sc_dt::sc_bv<6> q;
    } field0;
  };

  struct otp_macro_reg2hw_csr6_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
    } field3;
    struct {
      sc_dt::sc_bv<1> q;
    } field2;
    struct {
      sc_dt::sc_bv<1> q;
    } field1;
    struct {
      sc_dt::sc_bv<10> q;
    } field0;
  };

  struct otp_macro_reg2hw_csr7_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } field3;
    struct {
      sc_dt::sc_bv<1> q;
    } field2;
    struct {
      sc_dt::sc_bv<3> q;
    } field1;
    struct {
      sc_dt::sc_bv<6> q;
    } field0;
  };

  struct otp_macro_hw2reg_csr3_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field8;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field7;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field6;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field5;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field4;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field3;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field2;
    struct {
      sc_dt::sc_bv<10> d;
      bool        de;
    } field1;
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } field0;
  };

  struct otp_macro_hw2reg_csr5_reg_t {
    struct {
      sc_dt::sc_bv<16> d;
      bool        de;
    } field6;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field5;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field4;
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } field3;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field2;
    struct {
      sc_dt::sc_bv<2> d;
      bool        de;
    } field1;
    struct {
      sc_dt::sc_bv<6> d;
      bool        de;
    } field0;
  };

  struct otp_macro_hw2reg_csr7_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field3;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } field2;
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } field1;
    struct {
      sc_dt::sc_bv<6> d;
      bool        de;
    } field0;
  };

  // Register -> HW type for prim interface
  struct otp_macro_prim_reg2hw_t {
    otp_macro_reg2hw_csr0_reg_t csr0;
    otp_macro_reg2hw_csr1_reg_t csr1;
    otp_macro_reg2hw_csr2_reg_t csr2;
    otp_macro_reg2hw_csr3_reg_t csr3;
    otp_macro_reg2hw_csr4_reg_t csr4;
    otp_macro_reg2hw_csr5_reg_t csr5;
    otp_macro_reg2hw_csr6_reg_t csr6;
    otp_macro_reg2hw_csr7_reg_t csr7;
  };

  // HW -> register type for prim interface
  struct otp_macro_prim_hw2reg_t {
    otp_macro_hw2reg_csr3_reg_t csr3;
    otp_macro_hw2reg_csr5_reg_t csr5;
    otp_macro_hw2reg_csr7_reg_t csr7;
  };

  // Flat register storage
  std::array<uint32_t, 8> regs_prim_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 8> shadow_stage_prim_{};
  std::array<uint8_t,  8> shadow_phase_prim_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 8> racl_allow_rd_prim_{};
  std::array<bool, 8> racl_allow_wr_prim_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  otp_macro_prim_reg2hw_t reg2hw_prim;
  otp_macro_prim_hw2reg_t  hw2reg_prim;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace otp_macro
}  // namespace opentitan



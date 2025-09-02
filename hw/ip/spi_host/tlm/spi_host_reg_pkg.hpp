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
namespace spi_host {

// ----- Types mirrored from spi_host_reg_pkg.sv -----


class spi_host_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<spi_host_reg_top> tlm_target;

  SC_HAS_PROCESS(spi_host_reg_top);
  explicit spi_host_reg_top(sc_core::sc_module_name name);

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
    CONTROL = 16,
    STATUS = 20,
    CONFIGOPTS = 24,
    CSID = 28,
    COMMAND = 32,
    ERROR_ENABLE = 44,
    ERROR_STATUS = 48,
    EVENT_ENABLE = 52,
  };

  // Aggregate structs mirroring spi_host_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct spi_host_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } spi_event;
    struct {
      sc_dt::sc_bv<1> q;
    } error;
  };

  struct spi_host_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } spi_event;
    struct {
      sc_dt::sc_bv<1> q;
    } error;
  };

  struct spi_host_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } spi_event;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } error;
  };

  struct spi_host_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct spi_host_reg2hw_control_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } spien;
    struct {
      sc_dt::sc_bv<1> q;
    } sw_rst;
    struct {
      sc_dt::sc_bv<1> q;
    } output_en;
    struct {
      sc_dt::sc_bv<8> q;
    } tx_watermark;
    struct {
      sc_dt::sc_bv<8> q;
    } rx_watermark;
  };

  struct spi_host_reg2hw_configopts_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } cpol;
    struct {
      sc_dt::sc_bv<1> q;
    } cpha;
    struct {
      sc_dt::sc_bv<1> q;
    } fullcyc;
    struct {
      sc_dt::sc_bv<4> q;
    } csnlead;
    struct {
      sc_dt::sc_bv<4> q;
    } csntrail;
    struct {
      sc_dt::sc_bv<4> q;
    } csnidle;
    struct {
      sc_dt::sc_bv<16> q;
    } clkdiv;
  };

  struct spi_host_reg2hw_csid_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_host_reg2hw_command_reg_t {
    struct {
      sc_dt::sc_bv<20> q;
      bool        qe;
    } len;
    struct {
      sc_dt::sc_bv<2> q;
      bool        qe;
    } direction;
    struct {
      sc_dt::sc_bv<2> q;
      bool        qe;
    } speed;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } csaat;
  };

  struct spi_host_reg2hw_error_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } csidinval;
    struct {
      sc_dt::sc_bv<1> q;
    } cmdinval;
    struct {
      sc_dt::sc_bv<1> q;
    } underflow;
    struct {
      sc_dt::sc_bv<1> q;
    } overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } cmdbusy;
  };

  struct spi_host_reg2hw_error_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } accessinval;
    struct {
      sc_dt::sc_bv<1> q;
    } csidinval;
    struct {
      sc_dt::sc_bv<1> q;
    } cmdinval;
    struct {
      sc_dt::sc_bv<1> q;
    } underflow;
    struct {
      sc_dt::sc_bv<1> q;
    } overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } cmdbusy;
  };

  struct spi_host_reg2hw_event_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } idle;
    struct {
      sc_dt::sc_bv<1> q;
    } ready;
    struct {
      sc_dt::sc_bv<1> q;
    } txwm;
    struct {
      sc_dt::sc_bv<1> q;
    } rxwm;
    struct {
      sc_dt::sc_bv<1> q;
    } txempty;
    struct {
      sc_dt::sc_bv<1> q;
    } rxfull;
  };

  struct spi_host_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } spi_event;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } error;
  };

  struct spi_host_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ready;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } active;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } txfull;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } txempty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } txstall;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } txwm;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rxfull;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rxempty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rxstall;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } byteorder;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rxwm;
    struct {
      sc_dt::sc_bv<4> d;
      bool        de;
    } cmdqd;
    struct {
      sc_dt::sc_bv<8> d;
      bool        de;
    } rxqd;
    struct {
      sc_dt::sc_bv<8> d;
      bool        de;
    } txqd;
  };

  struct spi_host_hw2reg_error_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } accessinval;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } csidinval;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmdinval;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } underflow;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } overflow;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmdbusy;
  };

  // Register -> HW type
  struct spi_host_reg2hw_t {
    spi_host_reg2hw_intr_state_reg_t intr_state;
    spi_host_reg2hw_intr_enable_reg_t intr_enable;
    spi_host_reg2hw_intr_test_reg_t intr_test;
    spi_host_reg2hw_alert_test_reg_t alert_test;
    spi_host_reg2hw_control_reg_t control;
    spi_host_reg2hw_configopts_reg_t configopts;
    spi_host_reg2hw_csid_reg_t csid;
    spi_host_reg2hw_command_reg_t command;
    spi_host_reg2hw_error_enable_reg_t error_enable;
    spi_host_reg2hw_error_status_reg_t error_status;
    spi_host_reg2hw_event_enable_reg_t event_enable;
  };

  // HW -> register type
  struct spi_host_hw2reg_t {
    spi_host_hw2reg_intr_state_reg_t intr_state;
    spi_host_hw2reg_status_reg_t status;
    spi_host_hw2reg_error_status_reg_t error_status;
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
  spi_host_reg2hw_t reg2hw;
  spi_host_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace spi_host
}  // namespace opentitan



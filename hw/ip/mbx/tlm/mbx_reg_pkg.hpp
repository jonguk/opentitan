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
namespace mbx {

// ----- Types mirrored from mbx_reg_pkg.sv -----


class mbx_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<mbx_reg_top> tlm_target;

  SC_HAS_PROCESS(mbx_reg_top);
  explicit mbx_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 84;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_core_e : uint32_t {
    INTR_STATE = 0,
    INTR_ENABLE = 4,
    INTR_TEST = 8,
    ALERT_TEST = 12,
    CONTROL = 16,
    STATUS = 20,
    ADDRESS_RANGE_REGWEN = 24,
    ADDRESS_RANGE_VALID = 28,
    INBOUND_BASE_ADDRESS = 32,
    INBOUND_LIMIT_ADDRESS = 36,
    INBOUND_WRITE_PTR = 40,
    OUTBOUND_BASE_ADDRESS = 44,
    OUTBOUND_LIMIT_ADDRESS = 48,
    OUTBOUND_READ_PTR = 52,
    OUTBOUND_OBJECT_SIZE = 56,
    DOE_INTR_MSG_ADDR = 60,
    DOE_INTR_MSG_DATA = 64,
  };
  enum class reg_soc_e : uint32_t {
    SOC_CONTROL = 8,
    SOC_STATUS = 12,
    SOC_DOE_INTR_MSG_ADDR = 24,
    SOC_DOE_INTR_MSG_DATA = 28,
  };

  // Aggregate structs mirroring mbx_reg_pkg

//   ///////////////////////////////////////////////
  // Typedefs for registers for core interface //
  ///////////////////////////////////////////////

  struct mbx_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } mbx_error;
    struct {
      sc_dt::sc_bv<1> q;
    } mbx_abort;
    struct {
      sc_dt::sc_bv<1> q;
    } mbx_ready;
  };

  struct mbx_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } mbx_error;
    struct {
      sc_dt::sc_bv<1> q;
    } mbx_abort;
    struct {
      sc_dt::sc_bv<1> q;
    } mbx_ready;
  };

  struct mbx_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } mbx_error;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } mbx_abort;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } mbx_ready;
  };

  struct mbx_reg2hw_alert_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } recov_fault;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } fatal_fault;
  };

  struct mbx_reg2hw_control_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } sys_async_msg;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } error;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } abort;
  };

  struct mbx_reg2hw_address_range_valid_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct mbx_reg2hw_inbound_base_address_reg_t {
    sc_dt::sc_bv<30> q;
  };

  struct mbx_reg2hw_inbound_limit_address_reg_t {
    sc_dt::sc_bv<30> q;
  };

  struct mbx_reg2hw_outbound_base_address_reg_t {
    sc_dt::sc_bv<30> q;
  };

  struct mbx_reg2hw_outbound_limit_address_reg_t {
    sc_dt::sc_bv<30> q;
  };

  struct mbx_reg2hw_outbound_object_size_reg_t {
    sc_dt::sc_bv<11> q;
    bool qe;
  };

  struct mbx_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } mbx_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } mbx_abort;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } mbx_ready;
  };

  struct mbx_hw2reg_control_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } error;
    struct {
      sc_dt::sc_bv<1> d;
    } abort;
  };

  struct mbx_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } sys_async_enable;
    struct {
      sc_dt::sc_bv<1> d;
    } sys_intr_enable;
    struct {
      sc_dt::sc_bv<1> d;
    } sys_intr_state;
    struct {
      sc_dt::sc_bv<1> d;
    } busy;
  };

  struct mbx_hw2reg_inbound_write_ptr_reg_t {
    sc_dt::sc_bv<30> d;
  };

  struct mbx_hw2reg_outbound_read_ptr_reg_t {
    sc_dt::sc_bv<30> d;
  };

  struct mbx_hw2reg_outbound_object_size_reg_t {
    sc_dt::sc_bv<11> d;
    bool        de;
  };

  struct mbx_hw2reg_doe_intr_msg_addr_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct mbx_hw2reg_doe_intr_msg_data_reg_t {
    sc_dt::sc_bv<32> d;
  };

  // Register -> HW type for core interface
  struct mbx_core_reg2hw_t {
    mbx_reg2hw_intr_state_reg_t intr_state;
    mbx_reg2hw_intr_enable_reg_t intr_enable;
    mbx_reg2hw_intr_test_reg_t intr_test;
    mbx_reg2hw_alert_test_reg_t alert_test;
    mbx_reg2hw_control_reg_t control;
    mbx_reg2hw_address_range_valid_reg_t address_range_valid;
    mbx_reg2hw_inbound_base_address_reg_t inbound_base_address;
    mbx_reg2hw_inbound_limit_address_reg_t inbound_limit_address;
    mbx_reg2hw_outbound_base_address_reg_t outbound_base_address;
    mbx_reg2hw_outbound_limit_address_reg_t outbound_limit_address;
    mbx_reg2hw_outbound_object_size_reg_t outbound_object_size;
  };

  // HW -> register type for core interface
  struct mbx_core_hw2reg_t {
    mbx_hw2reg_intr_state_reg_t intr_state;
    mbx_hw2reg_control_reg_t control;
    mbx_hw2reg_status_reg_t status;
    mbx_hw2reg_inbound_write_ptr_reg_t inbound_write_ptr;
    mbx_hw2reg_outbound_read_ptr_reg_t outbound_read_ptr;
    mbx_hw2reg_outbound_object_size_reg_t outbound_object_size;
    mbx_hw2reg_doe_intr_msg_addr_reg_t doe_intr_msg_addr;
    mbx_hw2reg_doe_intr_msg_data_reg_t doe_intr_msg_data;
  };

//   //////////////////////////////////////////////
  // Typedefs for registers for soc interface //
  //////////////////////////////////////////////

  struct mbx_reg2hw_soc_control_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } go;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } doe_async_msg_en;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } doe_intr_en;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } abort;
  };

  struct mbx_reg2hw_soc_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } ready;
    struct {
      sc_dt::sc_bv<1> q;
    } error;
    struct {
      sc_dt::sc_bv<1> q;
    } doe_intr_status;
    struct {
      sc_dt::sc_bv<1> q;
    } busy;
  };

  struct mbx_reg2hw_soc_doe_intr_msg_addr_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct mbx_reg2hw_soc_doe_intr_msg_data_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct mbx_hw2reg_soc_control_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } go;
    struct {
      sc_dt::sc_bv<1> d;
    } doe_async_msg_en;
    struct {
      sc_dt::sc_bv<1> d;
    } doe_intr_en;
    struct {
      sc_dt::sc_bv<1> d;
    } abort;
  };

  struct mbx_hw2reg_soc_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } ready;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } doe_async_msg_status;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } doe_intr_status;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } busy;
  };

  // Register -> HW type for soc interface
  struct mbx_soc_reg2hw_t {
    mbx_reg2hw_soc_control_reg_t soc_control;
    mbx_reg2hw_soc_status_reg_t soc_status;
    mbx_reg2hw_soc_doe_intr_msg_addr_reg_t soc_doe_intr_msg_addr;
    mbx_reg2hw_soc_doe_intr_msg_data_reg_t soc_doe_intr_msg_data;
  };

  // HW -> register type for soc interface
  struct mbx_soc_hw2reg_t {
    mbx_hw2reg_soc_control_reg_t soc_control;
    mbx_hw2reg_soc_status_reg_t soc_status;
  };

  // Flat register storage
  std::array<uint32_t, 17> regs_core_;
  std::array<uint32_t, 4> regs_soc_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 17> shadow_stage_core_{};
  std::array<uint8_t,  17> shadow_phase_core_{}; // 0:first,1:second
  std::array<uint32_t, 4> shadow_stage_soc_{};
  std::array<uint8_t,  4> shadow_phase_soc_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 17> racl_allow_rd_core_{};
  std::array<bool, 17> racl_allow_wr_core_{};
  std::array<bool, 4> racl_allow_rd_soc_{};
  std::array<bool, 4> racl_allow_wr_soc_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  mbx_core_reg2hw_t reg2hw_core;
  mbx_core_hw2reg_t  hw2reg_core;
  mbx_soc_reg2hw_t reg2hw_soc;
  mbx_soc_hw2reg_t  hw2reg_soc;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace mbx
}  // namespace opentitan



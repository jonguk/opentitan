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
namespace dma {

// ----- Types mirrored from dma_reg_pkg.sv -----


class dma_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<dma_reg_top> tlm_target;

  SC_HAS_PROCESS(dma_reg_top);
  explicit dma_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 252;

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
    SRC_ADDR_LO = 16,
    SRC_ADDR_HI = 20,
    DST_ADDR_LO = 24,
    DST_ADDR_HI = 28,
    ADDR_SPACE_ID = 32,
    ENABLED_MEMORY_RANGE_BASE = 36,
    ENABLED_MEMORY_RANGE_LIMIT = 40,
    RANGE_VALID = 44,
    RANGE_REGWEN = 48,
    CFG_REGWEN = 52,
    TOTAL_DATA_SIZE = 56,
    CHUNK_DATA_SIZE = 60,
    TRANSFER_WIDTH = 64,
    CONTROL = 68,
    SRC_CONFIG = 72,
    DST_CONFIG = 76,
    STATUS = 80,
    ERROR_CODE = 84,
    SHA2_DIGEST_0 = 88,
    SHA2_DIGEST_1 = 92,
    SHA2_DIGEST_2 = 96,
    SHA2_DIGEST_3 = 100,
    SHA2_DIGEST_4 = 104,
    SHA2_DIGEST_5 = 108,
    SHA2_DIGEST_6 = 112,
    SHA2_DIGEST_7 = 116,
    SHA2_DIGEST_8 = 120,
    SHA2_DIGEST_9 = 124,
    SHA2_DIGEST_10 = 128,
    SHA2_DIGEST_11 = 132,
    SHA2_DIGEST_12 = 136,
    SHA2_DIGEST_13 = 140,
    SHA2_DIGEST_14 = 144,
    SHA2_DIGEST_15 = 148,
    HANDSHAKE_INTR_ENABLE = 152,
    CLEAR_INTR_SRC = 156,
    CLEAR_INTR_BUS = 160,
    INTR_SRC_ADDR_0 = 164,
    INTR_SRC_ADDR_1 = 168,
    INTR_SRC_ADDR_2 = 172,
    INTR_SRC_ADDR_3 = 176,
    INTR_SRC_ADDR_4 = 180,
    INTR_SRC_ADDR_5 = 184,
    INTR_SRC_ADDR_6 = 188,
    INTR_SRC_ADDR_7 = 192,
    INTR_SRC_ADDR_8 = 196,
    INTR_SRC_ADDR_9 = 200,
    INTR_SRC_ADDR_10 = 204,
    INTR_SRC_WR_VAL_0 = 292,
    INTR_SRC_WR_VAL_1 = 296,
    INTR_SRC_WR_VAL_2 = 300,
    INTR_SRC_WR_VAL_3 = 304,
    INTR_SRC_WR_VAL_4 = 308,
    INTR_SRC_WR_VAL_5 = 312,
    INTR_SRC_WR_VAL_6 = 316,
    INTR_SRC_WR_VAL_7 = 320,
    INTR_SRC_WR_VAL_8 = 324,
    INTR_SRC_WR_VAL_9 = 328,
    INTR_SRC_WR_VAL_10 = 332,
  };

  // Aggregate structs mirroring dma_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct dma_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } dma_error;
    struct {
      sc_dt::sc_bv<1> q;
    } dma_chunk_done;
    struct {
      sc_dt::sc_bv<1> q;
    } dma_done;
  };

  struct dma_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } dma_error;
    struct {
      sc_dt::sc_bv<1> q;
    } dma_chunk_done;
    struct {
      sc_dt::sc_bv<1> q;
    } dma_done;
  };

  struct dma_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } dma_error;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } dma_chunk_done;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } dma_done;
  };

  struct dma_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct dma_reg2hw_src_addr_lo_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct dma_reg2hw_src_addr_hi_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct dma_reg2hw_dst_addr_lo_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct dma_reg2hw_dst_addr_hi_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct dma_reg2hw_addr_space_id_reg_t {
    struct {
      sc_dt::sc_bv<4> q;
    } dst_asid;
    struct {
      sc_dt::sc_bv<4> q;
    } src_asid;
  };

  struct dma_reg2hw_enabled_memory_range_base_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct dma_reg2hw_enabled_memory_range_limit_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct dma_reg2hw_range_valid_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct dma_reg2hw_range_regwen_reg_t {
    sc_dt::sc_bv<4> q;
  };

  struct dma_reg2hw_total_data_size_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct dma_reg2hw_chunk_data_size_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct dma_reg2hw_transfer_width_reg_t {
    sc_dt::sc_bv<2> q;
  };

  struct dma_reg2hw_control_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } go;
    struct {
      sc_dt::sc_bv<1> q;
    } abort;
    struct {
      sc_dt::sc_bv<1> q;
    } initial_transfer;
    struct {
      sc_dt::sc_bv<1> q;
    } digest_swap;
    struct {
      sc_dt::sc_bv<1> q;
    } hardware_handshake_enable;
    struct {
      sc_dt::sc_bv<4> q;
    } opcode;
  };

  struct dma_reg2hw_src_config_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } wrap;
    struct {
      sc_dt::sc_bv<1> q;
    } increment;
  };

  struct dma_reg2hw_dst_config_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } wrap;
    struct {
      sc_dt::sc_bv<1> q;
    } increment;
  };

  struct dma_reg2hw_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } chunk_done;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } sha2_digest_valid;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } error;
    struct {
      sc_dt::sc_bv<1> q;
    } done;
    struct {
      sc_dt::sc_bv<1> q;
    } busy;
  };

  struct dma_reg2hw_handshake_intr_enable_reg_t {
    sc_dt::sc_bv<11> q;
  };

  struct dma_reg2hw_clear_intr_src_reg_t {
    sc_dt::sc_bv<11> q;
  };

  struct dma_reg2hw_clear_intr_bus_reg_t {
    sc_dt::sc_bv<11> q;
  };

  struct dma_reg2hw_intr_src_addr_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct dma_reg2hw_intr_src_wr_val_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct dma_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } dma_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } dma_chunk_done;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } dma_done;
  };

  struct dma_hw2reg_src_addr_lo_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct dma_hw2reg_src_addr_hi_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct dma_hw2reg_dst_addr_lo_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct dma_hw2reg_dst_addr_hi_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  struct dma_hw2reg_cfg_regwen_reg_t {
    sc_dt::sc_bv<4> d;
  };

  struct dma_hw2reg_control_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } go;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } abort;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } initial_transfer;
  };

  struct dma_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } chunk_done;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sha2_digest_valid;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } aborted;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } done;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } busy;
  };

  struct dma_hw2reg_error_code_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } asid_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } range_valid_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } base_limit_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } bus_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } size_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } opcode_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } dst_addr_error;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } src_addr_error;
  };

  struct dma_hw2reg_sha2_digest_mreg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  // Register -> HW type
  struct dma_reg2hw_t {
    dma_reg2hw_intr_state_reg_t intr_state;
    dma_reg2hw_intr_enable_reg_t intr_enable;
    dma_reg2hw_intr_test_reg_t intr_test;
    dma_reg2hw_alert_test_reg_t alert_test;
    dma_reg2hw_src_addr_lo_reg_t src_addr_lo;
    dma_reg2hw_src_addr_hi_reg_t src_addr_hi;
    dma_reg2hw_dst_addr_lo_reg_t dst_addr_lo;
    dma_reg2hw_dst_addr_hi_reg_t dst_addr_hi;
    dma_reg2hw_addr_space_id_reg_t addr_space_id;
    dma_reg2hw_enabled_memory_range_base_reg_t enabled_memory_range_base;
    dma_reg2hw_enabled_memory_range_limit_reg_t enabled_memory_range_limit;
    dma_reg2hw_range_valid_reg_t range_valid;
    dma_reg2hw_range_regwen_reg_t range_regwen;
    dma_reg2hw_total_data_size_reg_t total_data_size;
    dma_reg2hw_chunk_data_size_reg_t chunk_data_size;
    dma_reg2hw_transfer_width_reg_t transfer_width;
    dma_reg2hw_control_reg_t control;
    dma_reg2hw_src_config_reg_t src_config;
    dma_reg2hw_dst_config_reg_t dst_config;
    dma_reg2hw_status_reg_t status;
    dma_reg2hw_handshake_intr_enable_reg_t handshake_intr_enable;
    dma_reg2hw_clear_intr_src_reg_t clear_intr_src;
    dma_reg2hw_clear_intr_bus_reg_t clear_intr_bus;
    std::array<dma_reg2hw_intr_src_addr_mreg_t, 11> intr_src_addr;
    std::array<dma_reg2hw_intr_src_wr_val_mreg_t, 11> intr_src_wr_val;
  };

  // HW -> register type
  struct dma_hw2reg_t {
    dma_hw2reg_intr_state_reg_t intr_state;
    dma_hw2reg_src_addr_lo_reg_t src_addr_lo;
    dma_hw2reg_src_addr_hi_reg_t src_addr_hi;
    dma_hw2reg_dst_addr_lo_reg_t dst_addr_lo;
    dma_hw2reg_dst_addr_hi_reg_t dst_addr_hi;
    dma_hw2reg_cfg_regwen_reg_t cfg_regwen;
    dma_hw2reg_control_reg_t control;
    dma_hw2reg_status_reg_t status;
    dma_hw2reg_error_code_reg_t error_code;
    std::array<dma_hw2reg_sha2_digest_mreg_t, 16> sha2_digest;
  };

  // Flat register storage
  std::array<uint32_t, 63> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 63> shadow_stage_default_{};
  std::array<uint8_t,  63> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 63> racl_allow_rd_default_{};
  std::array<bool, 63> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  dma_reg2hw_t reg2hw;
  dma_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace dma
}  // namespace opentitan



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
namespace spi_device {

// ----- Types mirrored from spi_device_reg_pkg.sv -----


class spi_device_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<spi_device_reg_top> tlm_target;

  SC_HAS_PROCESS(spi_device_reg_top);
  explicit spi_device_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 292;

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
    CFG = 20,
    STATUS = 24,
    INTERCEPT_EN = 28,
    ADDR_MODE = 32,
    LAST_READ_ADDR = 36,
    FLASH_STATUS = 40,
    JEDEC_CC = 44,
    JEDEC_ID = 48,
    READ_THRESHOLD = 52,
    MAILBOX_ADDR = 56,
    UPLOAD_STATUS = 60,
    UPLOAD_STATUS2 = 64,
    UPLOAD_CMDFIFO = 68,
    UPLOAD_ADDRFIFO = 72,
    CMD_FILTER_0 = 76,
    CMD_FILTER_1 = 80,
    CMD_FILTER_2 = 84,
    CMD_FILTER_3 = 88,
    CMD_FILTER_4 = 92,
    CMD_FILTER_5 = 96,
    CMD_FILTER_6 = 100,
    CMD_FILTER_7 = 104,
    ADDR_SWAP_MASK = 108,
    ADDR_SWAP_DATA = 112,
    PAYLOAD_SWAP_MASK = 116,
    PAYLOAD_SWAP_DATA = 120,
    CMD_INFO_0 = 124,
    CMD_INFO_1 = 128,
    CMD_INFO_2 = 132,
    CMD_INFO_3 = 136,
    CMD_INFO_4 = 140,
    CMD_INFO_5 = 144,
    CMD_INFO_6 = 148,
    CMD_INFO_7 = 152,
    CMD_INFO_8 = 156,
    CMD_INFO_9 = 160,
    CMD_INFO_10 = 164,
    CMD_INFO_11 = 168,
    CMD_INFO_12 = 172,
    CMD_INFO_13 = 176,
    CMD_INFO_14 = 180,
    CMD_INFO_15 = 184,
    CMD_INFO_16 = 188,
    CMD_INFO_17 = 192,
    CMD_INFO_18 = 196,
    CMD_INFO_19 = 200,
    CMD_INFO_20 = 204,
    CMD_INFO_21 = 208,
    CMD_INFO_22 = 212,
    CMD_INFO_23 = 216,
    CMD_INFO_EN4B = 220,
    CMD_INFO_EX4B = 224,
    CMD_INFO_WREN = 228,
    CMD_INFO_WRDI = 232,
    TPM_CAP = 2048,
    TPM_CFG = 2052,
    TPM_STATUS = 2056,
    TPM_ACCESS_0 = 2060,
    TPM_ACCESS_1 = 2064,
    TPM_STS = 2068,
    TPM_INTF_CAPABILITY = 2072,
    TPM_INT_ENABLE = 2076,
    TPM_INT_VECTOR = 2080,
    TPM_INT_STATUS = 2084,
    TPM_DID_VID = 2088,
    TPM_RID = 2092,
    TPM_CMD_ADDR = 2096,
    TPM_READ_FIFO = 2100,
  };

  // Aggregate structs mirroring spi_device_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct spi_device_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } tpm_rdfifo_drop;
    struct {
      sc_dt::sc_bv<1> q;
    } tpm_rdfifo_cmd_end;
    struct {
      sc_dt::sc_bv<1> q;
    } tpm_header_not_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } readbuf_flip;
    struct {
      sc_dt::sc_bv<1> q;
    } readbuf_watermark;
    struct {
      sc_dt::sc_bv<1> q;
    } upload_payload_overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } upload_payload_not_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } upload_cmdfifo_not_empty;
  };

  struct spi_device_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } tpm_rdfifo_drop;
    struct {
      sc_dt::sc_bv<1> q;
    } tpm_rdfifo_cmd_end;
    struct {
      sc_dt::sc_bv<1> q;
    } tpm_header_not_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } readbuf_flip;
    struct {
      sc_dt::sc_bv<1> q;
    } readbuf_watermark;
    struct {
      sc_dt::sc_bv<1> q;
    } upload_payload_overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } upload_payload_not_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } upload_cmdfifo_not_empty;
  };

  struct spi_device_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } tpm_rdfifo_drop;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } tpm_rdfifo_cmd_end;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } tpm_header_not_empty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } readbuf_flip;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } readbuf_watermark;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } upload_payload_overflow;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } upload_payload_not_empty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } upload_cmdfifo_not_empty;
  };

  struct spi_device_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct spi_device_reg2hw_control_reg_t {
    struct {
      sc_dt::sc_bv<2> q;
    } mode;
    struct {
      sc_dt::sc_bv<1> q;
    } flash_read_buffer_clr;
    struct {
      sc_dt::sc_bv<1> q;
    } flash_status_fifo_clr;
  };

  struct spi_device_reg2hw_cfg_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } mailbox_en;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_order;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_order;
  };

  struct spi_device_reg2hw_intercept_en_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } mbx;
    struct {
      sc_dt::sc_bv<1> q;
    } sfdp;
    struct {
      sc_dt::sc_bv<1> q;
    } jedec;
    struct {
      sc_dt::sc_bv<1> q;
    } status;
  };

  struct spi_device_reg2hw_addr_mode_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } addr_4b_en;
  };

  struct spi_device_reg2hw_flash_status_reg_t {
    struct {
      sc_dt::sc_bv<22> q;
      bool        qe;
    } status;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } wel;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } busy;
  };

  struct spi_device_reg2hw_jedec_cc_reg_t {
    struct {
      sc_dt::sc_bv<8> q;
    } num_cc;
    struct {
      sc_dt::sc_bv<8> q;
    } cc;
  };

  struct spi_device_reg2hw_jedec_id_reg_t {
    struct {
      sc_dt::sc_bv<8> q;
    } mf;
    struct {
      sc_dt::sc_bv<16> q;
    } id;
  };

  struct spi_device_reg2hw_read_threshold_reg_t {
    sc_dt::sc_bv<10> q;
  };

  struct spi_device_reg2hw_mailbox_addr_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_upload_cmdfifo_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } addr4b_mode;
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } wel;
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } busy;
    struct {
      sc_dt::sc_bv<8> q;
      bool        re;
    } data;
  };

  struct spi_device_reg2hw_upload_addrfifo_reg_t {
    sc_dt::sc_bv<32> q;
    bool re;
  };

  struct spi_device_reg2hw_cmd_filter_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct spi_device_reg2hw_addr_swap_mask_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_addr_swap_data_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_payload_swap_mask_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_payload_swap_data_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_cmd_info_mreg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } valid;
    struct {
      sc_dt::sc_bv<1> q;
    } busy;
    struct {
      sc_dt::sc_bv<1> q;
    } upload;
    struct {
      sc_dt::sc_bv<2> q;
    } read_pipeline_mode;
    struct {
      sc_dt::sc_bv<1> q;
    } payload_swap_en;
    struct {
      sc_dt::sc_bv<1> q;
    } payload_dir;
    struct {
      sc_dt::sc_bv<4> q;
    } payload_en;
    struct {
      sc_dt::sc_bv<1> q;
    } dummy_en;
    struct {
      sc_dt::sc_bv<3> q;
    } dummy_size;
    struct {
      sc_dt::sc_bv<1> q;
    } mbyte_en;
    struct {
      sc_dt::sc_bv<1> q;
    } addr_swap_en;
    struct {
      sc_dt::sc_bv<2> q;
    } addr_mode;
    struct {
      sc_dt::sc_bv<8> q;
    } opcode;
  };

  struct spi_device_reg2hw_cmd_info_en4b_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } valid;
    struct {
      sc_dt::sc_bv<8> q;
    } opcode;
  };

  struct spi_device_reg2hw_cmd_info_ex4b_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } valid;
    struct {
      sc_dt::sc_bv<8> q;
    } opcode;
  };

  struct spi_device_reg2hw_cmd_info_wren_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } valid;
    struct {
      sc_dt::sc_bv<8> q;
    } opcode;
  };

  struct spi_device_reg2hw_cmd_info_wrdi_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } valid;
    struct {
      sc_dt::sc_bv<8> q;
    } opcode;
  };

  struct spi_device_reg2hw_tpm_cfg_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } invalid_locality;
    struct {
      sc_dt::sc_bv<1> q;
    } tpm_reg_chk_dis;
    struct {
      sc_dt::sc_bv<1> q;
    } hw_reg_dis;
    struct {
      sc_dt::sc_bv<1> q;
    } tpm_mode;
    struct {
      sc_dt::sc_bv<1> q;
    } en;
  };

  struct spi_device_reg2hw_tpm_status_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } wrfifo_pending;
  };

  struct spi_device_reg2hw_tpm_access_mreg_t {
    sc_dt::sc_bv<8> q;
  };

  struct spi_device_reg2hw_tpm_sts_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_tpm_intf_capability_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_tpm_int_enable_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_tpm_int_vector_reg_t {
    sc_dt::sc_bv<8> q;
  };

  struct spi_device_reg2hw_tpm_int_status_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct spi_device_reg2hw_tpm_did_vid_reg_t {
    struct {
      sc_dt::sc_bv<16> q;
    } did;
    struct {
      sc_dt::sc_bv<16> q;
    } vid;
  };

  struct spi_device_reg2hw_tpm_rid_reg_t {
    sc_dt::sc_bv<8> q;
  };

  struct spi_device_reg2hw_tpm_cmd_addr_reg_t {
    struct {
      sc_dt::sc_bv<8> q;
      bool        qe;
      bool        re;
    } cmd;
    struct {
      sc_dt::sc_bv<24> q;
      bool        qe;
      bool        re;
    } addr;
  };

  struct spi_device_reg2hw_tpm_read_fifo_reg_t {
    sc_dt::sc_bv<32> q;
    bool qe;
  };

  struct spi_device_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tpm_rdfifo_drop;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tpm_rdfifo_cmd_end;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } tpm_header_not_empty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } readbuf_flip;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } readbuf_watermark;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } upload_payload_overflow;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } upload_payload_not_empty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } upload_cmdfifo_not_empty;
  };

  struct spi_device_hw2reg_control_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } flash_read_buffer_clr;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } flash_status_fifo_clr;
  };

  struct spi_device_hw2reg_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } tpm_csb;
    struct {
      sc_dt::sc_bv<1> d;
    } csb;
  };

  struct spi_device_hw2reg_addr_mode_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } pending;
    struct {
      sc_dt::sc_bv<1> d;
    } addr_4b_en;
  };

  struct spi_device_hw2reg_last_read_addr_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct spi_device_hw2reg_flash_status_reg_t {
    struct {
      sc_dt::sc_bv<22> d;
    } status;
    struct {
      sc_dt::sc_bv<1> d;
    } wel;
    struct {
      sc_dt::sc_bv<1> d;
    } busy;
  };

  struct spi_device_hw2reg_upload_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } addrfifo_notempty;
    struct {
      sc_dt::sc_bv<5> d;
      bool        de;
    } addrfifo_depth;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } cmdfifo_notempty;
    struct {
      sc_dt::sc_bv<5> d;
      bool        de;
    } cmdfifo_depth;
  };

  struct spi_device_hw2reg_upload_status2_reg_t {
    struct {
      sc_dt::sc_bv<8> d;
      bool        de;
    } payload_start_idx;
    struct {
      sc_dt::sc_bv<9> d;
      bool        de;
    } payload_depth;
  };

  struct spi_device_hw2reg_upload_cmdfifo_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } addr4b_mode;
    struct {
      sc_dt::sc_bv<1> d;
    } wel;
    struct {
      sc_dt::sc_bv<1> d;
    } busy;
    struct {
      sc_dt::sc_bv<8> d;
    } data;
  };

  struct spi_device_hw2reg_upload_addrfifo_reg_t {
    sc_dt::sc_bv<32> d;
  };

  struct spi_device_hw2reg_tpm_cap_reg_t {
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } max_rd_size;
    struct {
      sc_dt::sc_bv<3> d;
      bool        de;
    } max_wr_size;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } locality;
    struct {
      sc_dt::sc_bv<8> d;
      bool        de;
    } rev;
  };

  struct spi_device_hw2reg_tpm_status_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } rdfifo_aborted;
    struct {
      sc_dt::sc_bv<1> d;
    } wrfifo_pending;
    struct {
      sc_dt::sc_bv<1> d;
    } cmdaddr_notempty;
  };

  struct spi_device_hw2reg_tpm_cmd_addr_reg_t {
    struct {
      sc_dt::sc_bv<8> d;
    } cmd;
    struct {
      sc_dt::sc_bv<24> d;
    } addr;
  };

  // Register -> HW type
  struct spi_device_reg2hw_t {
    spi_device_reg2hw_intr_state_reg_t intr_state;
    spi_device_reg2hw_intr_enable_reg_t intr_enable;
    spi_device_reg2hw_intr_test_reg_t intr_test;
    spi_device_reg2hw_alert_test_reg_t alert_test;
    spi_device_reg2hw_control_reg_t control;
    spi_device_reg2hw_cfg_reg_t cfg;
    spi_device_reg2hw_intercept_en_reg_t intercept_en;
    spi_device_reg2hw_addr_mode_reg_t addr_mode;
    spi_device_reg2hw_flash_status_reg_t flash_status;
    spi_device_reg2hw_jedec_cc_reg_t jedec_cc;
    spi_device_reg2hw_jedec_id_reg_t jedec_id;
    spi_device_reg2hw_read_threshold_reg_t read_threshold;
    spi_device_reg2hw_mailbox_addr_reg_t mailbox_addr;
    spi_device_reg2hw_upload_cmdfifo_reg_t upload_cmdfifo;
    spi_device_reg2hw_upload_addrfifo_reg_t upload_addrfifo;
    std::array<spi_device_reg2hw_cmd_filter_mreg_t, 256> cmd_filter;
    spi_device_reg2hw_addr_swap_mask_reg_t addr_swap_mask;
    spi_device_reg2hw_addr_swap_data_reg_t addr_swap_data;
    spi_device_reg2hw_payload_swap_mask_reg_t payload_swap_mask;
    spi_device_reg2hw_payload_swap_data_reg_t payload_swap_data;
    std::array<spi_device_reg2hw_cmd_info_mreg_t, 24> cmd_info;
    spi_device_reg2hw_cmd_info_en4b_reg_t cmd_info_en4b;
    spi_device_reg2hw_cmd_info_ex4b_reg_t cmd_info_ex4b;
    spi_device_reg2hw_cmd_info_wren_reg_t cmd_info_wren;
    spi_device_reg2hw_cmd_info_wrdi_reg_t cmd_info_wrdi;
    spi_device_reg2hw_tpm_cfg_reg_t tpm_cfg;
    spi_device_reg2hw_tpm_status_reg_t tpm_status;
    std::array<spi_device_reg2hw_tpm_access_mreg_t, 5> tpm_access;
    spi_device_reg2hw_tpm_sts_reg_t tpm_sts;
    spi_device_reg2hw_tpm_intf_capability_reg_t tpm_intf_capability;
    spi_device_reg2hw_tpm_int_enable_reg_t tpm_int_enable;
    spi_device_reg2hw_tpm_int_vector_reg_t tpm_int_vector;
    spi_device_reg2hw_tpm_int_status_reg_t tpm_int_status;
    spi_device_reg2hw_tpm_did_vid_reg_t tpm_did_vid;
    spi_device_reg2hw_tpm_rid_reg_t tpm_rid;
    spi_device_reg2hw_tpm_cmd_addr_reg_t tpm_cmd_addr;
    spi_device_reg2hw_tpm_read_fifo_reg_t tpm_read_fifo;
  };

  // HW -> register type
  struct spi_device_hw2reg_t {
    spi_device_hw2reg_intr_state_reg_t intr_state;
    spi_device_hw2reg_control_reg_t control;
    spi_device_hw2reg_status_reg_t status;
    spi_device_hw2reg_addr_mode_reg_t addr_mode;
    spi_device_hw2reg_last_read_addr_reg_t last_read_addr;
    spi_device_hw2reg_flash_status_reg_t flash_status;
    spi_device_hw2reg_upload_status_reg_t upload_status;
    spi_device_hw2reg_upload_status2_reg_t upload_status2;
    spi_device_hw2reg_upload_cmdfifo_reg_t upload_cmdfifo;
    spi_device_hw2reg_upload_addrfifo_reg_t upload_addrfifo;
    spi_device_hw2reg_tpm_cap_reg_t tpm_cap;
    spi_device_hw2reg_tpm_status_reg_t tpm_status;
    spi_device_hw2reg_tpm_cmd_addr_reg_t tpm_cmd_addr;
  };

  // Flat register storage
  std::array<uint32_t, 73> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 73> shadow_stage_default_{};
  std::array<uint8_t,  73> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 73> racl_allow_rd_default_{};
  std::array<bool, 73> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  spi_device_reg2hw_t reg2hw;
  spi_device_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace spi_device
}  // namespace opentitan



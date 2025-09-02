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
namespace usbdev {

// ----- Types mirrored from usbdev_reg_pkg.sv -----


class usbdev_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<usbdev_reg_top> tlm_target;

  SC_HAS_PROCESS(usbdev_reg_top);
  explicit usbdev_reg_top(sc_core::sc_module_name name);

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
  static constexpr std::size_t kRegBytes = 172;

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
    USBCTRL = 16,
    EP_OUT_ENABLE = 20,
    EP_IN_ENABLE = 24,
    USBSTAT = 28,
    AVOUTBUFFER = 32,
    AVSETUPBUFFER = 36,
    RXFIFO = 40,
    RXENABLE_SETUP = 44,
    RXENABLE_OUT = 48,
    SET_NAK_OUT = 52,
    IN_SENT = 56,
    OUT_STALL = 60,
    IN_STALL = 64,
    CONFIGIN_0 = 68,
    CONFIGIN_1 = 72,
    CONFIGIN_2 = 76,
    CONFIGIN_3 = 80,
    CONFIGIN_4 = 84,
    CONFIGIN_5 = 88,
    CONFIGIN_6 = 92,
    CONFIGIN_7 = 96,
    CONFIGIN_8 = 100,
    CONFIGIN_9 = 104,
    CONFIGIN_10 = 108,
    CONFIGIN_11 = 112,
    OUT_ISO = 116,
    IN_ISO = 120,
    OUT_DATA_TOGGLE = 124,
    IN_DATA_TOGGLE = 128,
    PHY_PINS_SENSE = 132,
    PHY_PINS_DRIVE = 136,
    PHY_CONFIG = 140,
    WAKE_CONTROL = 144,
    WAKE_EVENTS = 148,
    FIFO_CTRL = 152,
    COUNT_OUT = 156,
    COUNT_IN = 160,
    COUNT_NODATA_IN = 164,
    COUNT_ERRORS = 168,
  };

  // Aggregate structs mirroring usbdev_reg_pkg

//   ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////

  struct usbdev_reg2hw_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } av_setup_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } link_out_err;
    struct {
      sc_dt::sc_bv<1> q;
    } powered;
    struct {
      sc_dt::sc_bv<1> q;
    } frame;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_bitstuff_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_pid_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_crc_err;
    struct {
      sc_dt::sc_bv<1> q;
    } link_in_err;
    struct {
      sc_dt::sc_bv<1> q;
    } av_overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_full;
    struct {
      sc_dt::sc_bv<1> q;
    } av_out_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } link_resume;
    struct {
      sc_dt::sc_bv<1> q;
    } link_suspend;
    struct {
      sc_dt::sc_bv<1> q;
    } link_reset;
    struct {
      sc_dt::sc_bv<1> q;
    } host_lost;
    struct {
      sc_dt::sc_bv<1> q;
    } disconnected;
    struct {
      sc_dt::sc_bv<1> q;
    } pkt_sent;
    struct {
      sc_dt::sc_bv<1> q;
    } pkt_received;
  };

  struct usbdev_reg2hw_intr_enable_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } av_setup_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } link_out_err;
    struct {
      sc_dt::sc_bv<1> q;
    } powered;
    struct {
      sc_dt::sc_bv<1> q;
    } frame;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_bitstuff_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_pid_err;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_crc_err;
    struct {
      sc_dt::sc_bv<1> q;
    } link_in_err;
    struct {
      sc_dt::sc_bv<1> q;
    } av_overflow;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_full;
    struct {
      sc_dt::sc_bv<1> q;
    } av_out_empty;
    struct {
      sc_dt::sc_bv<1> q;
    } link_resume;
    struct {
      sc_dt::sc_bv<1> q;
    } link_suspend;
    struct {
      sc_dt::sc_bv<1> q;
    } link_reset;
    struct {
      sc_dt::sc_bv<1> q;
    } host_lost;
    struct {
      sc_dt::sc_bv<1> q;
    } disconnected;
    struct {
      sc_dt::sc_bv<1> q;
    } pkt_sent;
    struct {
      sc_dt::sc_bv<1> q;
    } pkt_received;
  };

  struct usbdev_reg2hw_intr_test_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } av_setup_empty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } link_out_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } powered;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } frame;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_bitstuff_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_pid_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_crc_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } link_in_err;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } av_overflow;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_full;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } av_out_empty;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } link_resume;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } link_suspend;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } link_reset;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } host_lost;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } disconnected;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } pkt_sent;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } pkt_received;
  };

  struct usbdev_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct usbdev_reg2hw_usbctrl_reg_t {
    struct {
      sc_dt::sc_bv<7> q;
    } device_address;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } resume_link_active;
    struct {
      sc_dt::sc_bv<1> q;
    } enable;
  };

  struct usbdev_reg2hw_ep_out_enable_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_ep_in_enable_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_avoutbuffer_reg_t {
    sc_dt::sc_bv<5> q;
    bool qe;
  };

  struct usbdev_reg2hw_avsetupbuffer_reg_t {
    sc_dt::sc_bv<5> q;
    bool qe;
  };

  struct usbdev_reg2hw_rxfifo_reg_t {
    struct {
      sc_dt::sc_bv<4> q;
      bool        re;
    } ep;
    struct {
      sc_dt::sc_bv<1> q;
      bool        re;
    } setup;
    struct {
      sc_dt::sc_bv<7> q;
      bool        re;
    } size;
    struct {
      sc_dt::sc_bv<5> q;
      bool        re;
    } buffer;
  };

  struct usbdev_reg2hw_rxenable_setup_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_rxenable_out_reg_t {
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } preserve;
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } out;
  };

  struct usbdev_reg2hw_set_nak_out_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_in_sent_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_out_stall_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_in_stall_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_configin_mreg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } rdy;
    struct {
      sc_dt::sc_bv<1> q;
    } pend;
    struct {
      sc_dt::sc_bv<7> q;
    } size;
    struct {
      sc_dt::sc_bv<5> q;
    } buffer;
  };

  struct usbdev_reg2hw_out_iso_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_in_iso_mreg_t {
    sc_dt::sc_bv<1> q;
  };

  struct usbdev_reg2hw_out_data_toggle_reg_t {
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } mask;
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } status;
  };

  struct usbdev_reg2hw_in_data_toggle_reg_t {
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } mask;
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } status;
  };

  struct usbdev_reg2hw_phy_pins_drive_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } en;
    struct {
      sc_dt::sc_bv<1> q;
    } dn_pullup_en_o;
    struct {
      sc_dt::sc_bv<1> q;
    } dp_pullup_en_o;
    struct {
      sc_dt::sc_bv<1> q;
    } rx_enable_o;
    struct {
      sc_dt::sc_bv<1> q;
    } oe_o;
    struct {
      sc_dt::sc_bv<1> q;
    } se0_o;
    struct {
      sc_dt::sc_bv<1> q;
    } d_o;
    struct {
      sc_dt::sc_bv<1> q;
    } dn_o;
    struct {
      sc_dt::sc_bv<1> q;
    } dp_o;
  };

  struct usbdev_reg2hw_phy_config_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
    } tx_osc_test_mode;
    struct {
      sc_dt::sc_bv<1> q;
    } usb_ref_disable;
    struct {
      sc_dt::sc_bv<1> q;
    } pinflip;
    struct {
      sc_dt::sc_bv<1> q;
    } eop_single_bit;
    struct {
      sc_dt::sc_bv<1> q;
    } tx_use_d_se0;
    struct {
      sc_dt::sc_bv<1> q;
    } use_diff_rcvr;
  };

  struct usbdev_reg2hw_wake_control_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } wake_ack;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } suspend_req;
  };

  struct usbdev_reg2hw_fifo_ctrl_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rx_rst;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } avsetup_rst;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } avout_rst;
  };

  struct usbdev_reg2hw_count_out_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rst;
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } endpoints;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } ign_avsetup;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } drop_avout;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } drop_rx;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } datatog_out;
  };

  struct usbdev_reg2hw_count_in_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rst;
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } endpoints;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } timeout;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } nak;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } nodata;
  };

  struct usbdev_reg2hw_count_nodata_in_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rst;
    struct {
      sc_dt::sc_bv<12> q;
      bool        qe;
    } endpoints;
  };

  struct usbdev_reg2hw_count_errors_reg_t {
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } rst;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } crc5;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } crc16;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } bitstuff;
    struct {
      sc_dt::sc_bv<1> q;
      bool        qe;
    } pid_invalid;
  };

  struct usbdev_hw2reg_intr_state_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } av_setup_empty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } link_out_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } powered;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } frame;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_bitstuff_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_pid_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_crc_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } link_in_err;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } av_overflow;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rx_full;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } av_out_empty;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } link_resume;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } link_suspend;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } link_reset;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } host_lost;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } disconnected;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } pkt_sent;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } pkt_received;
  };

  struct usbdev_hw2reg_usbctrl_reg_t {
    struct {
      sc_dt::sc_bv<7> d;
      bool        de;
    } device_address;
  };

  struct usbdev_hw2reg_usbstat_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } rx_empty;
    struct {
      sc_dt::sc_bv<1> d;
    } av_setup_full;
    struct {
      sc_dt::sc_bv<4> d;
    } rx_depth;
    struct {
      sc_dt::sc_bv<1> d;
    } av_out_full;
    struct {
      sc_dt::sc_bv<3> d;
    } av_setup_depth;
    struct {
      sc_dt::sc_bv<4> d;
    } av_out_depth;
    struct {
      sc_dt::sc_bv<1> d;
    } sense;
    struct {
      sc_dt::sc_bv<3> d;
    } link_state;
    struct {
      sc_dt::sc_bv<1> d;
    } host_lost;
    struct {
      sc_dt::sc_bv<11> d;
    } frame;
  };

  struct usbdev_hw2reg_rxfifo_reg_t {
    struct {
      sc_dt::sc_bv<4> d;
    } ep;
    struct {
      sc_dt::sc_bv<1> d;
    } setup;
    struct {
      sc_dt::sc_bv<7> d;
    } size;
    struct {
      sc_dt::sc_bv<5> d;
    } buffer;
  };

  struct usbdev_hw2reg_rxenable_out_reg_t {
    struct {
      sc_dt::sc_bv<12> d;
    } out;
  };

  struct usbdev_hw2reg_in_sent_mreg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct usbdev_hw2reg_out_stall_mreg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct usbdev_hw2reg_in_stall_mreg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct usbdev_hw2reg_configin_mreg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } rdy;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } pend;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } sending;
  };

  struct usbdev_hw2reg_out_data_toggle_reg_t {
    struct {
      sc_dt::sc_bv<12> d;
    } mask;
    struct {
      sc_dt::sc_bv<12> d;
    } status;
  };

  struct usbdev_hw2reg_in_data_toggle_reg_t {
    struct {
      sc_dt::sc_bv<12> d;
    } mask;
    struct {
      sc_dt::sc_bv<12> d;
    } status;
  };

  struct usbdev_hw2reg_phy_pins_sense_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } pwr_sense;
    struct {
      sc_dt::sc_bv<1> d;
    } tx_oe_o;
    struct {
      sc_dt::sc_bv<1> d;
    } tx_se0_o;
    struct {
      sc_dt::sc_bv<1> d;
    } tx_d_o;
    struct {
      sc_dt::sc_bv<1> d;
    } tx_dn_o;
    struct {
      sc_dt::sc_bv<1> d;
    } tx_dp_o;
    struct {
      sc_dt::sc_bv<1> d;
    } rx_d_i;
    struct {
      sc_dt::sc_bv<1> d;
    } rx_dn_i;
    struct {
      sc_dt::sc_bv<1> d;
    } rx_dp_i;
  };

  struct usbdev_hw2reg_wake_events_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } bus_not_idle;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } bus_reset;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } disconnected;
    struct {
      sc_dt::sc_bv<1> d;
      bool        de;
    } module_active;
  };

  struct usbdev_hw2reg_count_out_reg_t {
    struct {
      sc_dt::sc_bv<12> d;
    } endpoints;
    struct {
      sc_dt::sc_bv<1> d;
    } ign_avsetup;
    struct {
      sc_dt::sc_bv<1> d;
    } drop_avout;
    struct {
      sc_dt::sc_bv<1> d;
    } drop_rx;
    struct {
      sc_dt::sc_bv<1> d;
    } datatog_out;
    struct {
      sc_dt::sc_bv<8> d;
    } count;
  };

  struct usbdev_hw2reg_count_in_reg_t {
    struct {
      sc_dt::sc_bv<12> d;
    } endpoints;
    struct {
      sc_dt::sc_bv<1> d;
    } timeout;
    struct {
      sc_dt::sc_bv<1> d;
    } nak;
    struct {
      sc_dt::sc_bv<1> d;
    } nodata;
    struct {
      sc_dt::sc_bv<8> d;
    } count;
  };

  struct usbdev_hw2reg_count_nodata_in_reg_t {
    struct {
      sc_dt::sc_bv<12> d;
    } endpoints;
    struct {
      sc_dt::sc_bv<8> d;
    } count;
  };

  struct usbdev_hw2reg_count_errors_reg_t {
    struct {
      sc_dt::sc_bv<1> d;
    } crc5;
    struct {
      sc_dt::sc_bv<1> d;
    } crc16;
    struct {
      sc_dt::sc_bv<1> d;
    } bitstuff;
    struct {
      sc_dt::sc_bv<1> d;
    } pid_invalid;
    struct {
      sc_dt::sc_bv<8> d;
    } count;
  };

  // Register -> HW type
  struct usbdev_reg2hw_t {
    usbdev_reg2hw_intr_state_reg_t intr_state;
    usbdev_reg2hw_intr_enable_reg_t intr_enable;
    usbdev_reg2hw_intr_test_reg_t intr_test;
    usbdev_reg2hw_alert_test_reg_t alert_test;
    usbdev_reg2hw_usbctrl_reg_t usbctrl;
    std::array<usbdev_reg2hw_ep_out_enable_mreg_t, 12> ep_out_enable;
    std::array<usbdev_reg2hw_ep_in_enable_mreg_t, 12> ep_in_enable;
    usbdev_reg2hw_avoutbuffer_reg_t avoutbuffer;
    usbdev_reg2hw_avsetupbuffer_reg_t avsetupbuffer;
    usbdev_reg2hw_rxfifo_reg_t rxfifo;
    std::array<usbdev_reg2hw_rxenable_setup_mreg_t, 12> rxenable_setup;
    usbdev_reg2hw_rxenable_out_reg_t rxenable_out;
    std::array<usbdev_reg2hw_set_nak_out_mreg_t, 12> set_nak_out;
    std::array<usbdev_reg2hw_in_sent_mreg_t, 12> in_sent;
    std::array<usbdev_reg2hw_out_stall_mreg_t, 12> out_stall;
    std::array<usbdev_reg2hw_in_stall_mreg_t, 12> in_stall;
    std::array<usbdev_reg2hw_configin_mreg_t, 12> configin;
    std::array<usbdev_reg2hw_out_iso_mreg_t, 12> out_iso;
    std::array<usbdev_reg2hw_in_iso_mreg_t, 12> in_iso;
    usbdev_reg2hw_out_data_toggle_reg_t out_data_toggle;
    usbdev_reg2hw_in_data_toggle_reg_t in_data_toggle;
    usbdev_reg2hw_phy_pins_drive_reg_t phy_pins_drive;
    usbdev_reg2hw_phy_config_reg_t phy_config;
    usbdev_reg2hw_wake_control_reg_t wake_control;
    usbdev_reg2hw_fifo_ctrl_reg_t fifo_ctrl;
    usbdev_reg2hw_count_out_reg_t count_out;
    usbdev_reg2hw_count_in_reg_t count_in;
    usbdev_reg2hw_count_nodata_in_reg_t count_nodata_in;
    usbdev_reg2hw_count_errors_reg_t count_errors;
  };

  // HW -> register type
  struct usbdev_hw2reg_t {
    usbdev_hw2reg_intr_state_reg_t intr_state;
    usbdev_hw2reg_usbctrl_reg_t usbctrl;
    usbdev_hw2reg_usbstat_reg_t usbstat;
    usbdev_hw2reg_rxfifo_reg_t rxfifo;
    usbdev_hw2reg_rxenable_out_reg_t rxenable_out;
    std::array<usbdev_hw2reg_in_sent_mreg_t, 12> in_sent;
    std::array<usbdev_hw2reg_out_stall_mreg_t, 12> out_stall;
    std::array<usbdev_hw2reg_in_stall_mreg_t, 12> in_stall;
    std::array<usbdev_hw2reg_configin_mreg_t, 12> configin;
    usbdev_hw2reg_out_data_toggle_reg_t out_data_toggle;
    usbdev_hw2reg_in_data_toggle_reg_t in_data_toggle;
    usbdev_hw2reg_phy_pins_sense_reg_t phy_pins_sense;
    usbdev_hw2reg_wake_events_reg_t wake_events;
    usbdev_hw2reg_count_out_reg_t count_out;
    usbdev_hw2reg_count_in_reg_t count_in;
    usbdev_hw2reg_count_nodata_in_reg_t count_nodata_in;
    usbdev_hw2reg_count_errors_reg_t count_errors;
  };

  // Flat register storage
  std::array<uint32_t, 43> regs_default_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 43> shadow_stage_default_{};
  std::array<uint8_t,  43> shadow_phase_default_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 43> racl_allow_rd_default_{};
  std::array<bool, 43> racl_allow_wr_default_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  usbdev_reg2hw_t reg2hw;
  usbdev_hw2reg_t  hw2reg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace usbdev
}  // namespace opentitan



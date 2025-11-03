// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
//
// util/topgen.py -t hw/top_assam/data/top_assam.hjson \
//                -o hw/top_assam/

module top_assam #(
  // Manually defined parameters

  // Auto-inferred parameters
  // parameters for uart0
  // parameters for spi_device
  parameter spi_device_pkg::sram_type_e SpiDeviceSramType = spi_device_pkg::DefaultSramType,
  // parameters for rv_timer
  // parameters for pwrmgr_aon
  // parameters for rstmgr_aon
  parameter bit SecRstmgrAonCheck = 0,
  parameter int SecRstmgrAonMaxSyncDelay = 2,
  // parameters for clkmgr_aon
  // parameters for pinmux_aon
  parameter bit SecPinmuxAonVolatileRawUnlockEn = 1'b0,
  parameter pinmux_pkg::target_cfg_t PinmuxAonTargetCfg = pinmux_pkg::DefaultTargetCfg,
  // parameters for rv_plic
  // parameters for aes
  parameter bit SecAesMasking = 1,
  parameter aes_pkg::sbox_impl_e SecAesSBoxImpl = aes_pkg::SBoxImplDom,
  parameter int unsigned SecAesStartTriggerDelay = 0,
  parameter bit SecAesAllowForcingMasks = 1'b0,
  parameter bit SecAesSkipPRNGReseeding = 1'b0,
  // parameters for sram_ctrl_main
  parameter int SramCtrlMainInstSize = 4096,
  parameter int SramCtrlMainNumRamInst = 1,
  parameter bit SramCtrlMainInstrExec = 1,
  parameter int SramCtrlMainNumPrinceRoundsHalf = 3,
  parameter bit SramCtrlMainEccCorrection = 0,
  // parameters for rom_ctrl
  parameter RomCtrlBootRomInitFile = "",
  parameter bit SecRomCtrlDisableScrambling = 1'b1,
  // parameters for rv_core_ibex
  parameter bit RvCoreIbexPMPEnable = 0,
  parameter int unsigned RvCoreIbexPMPGranularity = 0,
  parameter int unsigned RvCoreIbexPMPNumRegions = 16,
  parameter int unsigned RvCoreIbexMHPMCounterNum = 0,
  parameter int unsigned RvCoreIbexMHPMCounterWidth = 32,
  parameter ibex_pkg::pmp_cfg_t RvCoreIbexPMPRstCfg[16] = ibex_pkg::PmpCfgRst,
  parameter logic [33:0] RvCoreIbexPMPRstAddr[16] = ibex_pkg::PmpAddrRst,
  parameter ibex_pkg::pmp_mseccfg_t RvCoreIbexPMPRstMsecCfg = ibex_pkg::PmpMseccfgRst,
  parameter bit RvCoreIbexRV32E = 0,
  parameter ibex_pkg::rv32m_e RvCoreIbexRV32M = ibex_pkg::RV32MSingleCycle,
  parameter ibex_pkg::rv32b_e RvCoreIbexRV32B = ibex_pkg::RV32BNone,
  parameter ibex_pkg::regfile_e RvCoreIbexRegFile = ibex_pkg::RegFileFF,
  parameter bit RvCoreIbexBranchTargetALU = 1,
  parameter bit RvCoreIbexWritebackStage = 1,
  parameter bit RvCoreIbexICache = 0,
  parameter bit RvCoreIbexICacheECC = 0,
  parameter bit RvCoreIbexICacheScramble = 0,
  parameter int unsigned RvCoreIbexICacheNWays = 2,
  parameter bit RvCoreIbexBranchPredictor = 0,
  parameter bit RvCoreIbexDbgTriggerEn = 1,
  parameter int RvCoreIbexDbgHwBreakNum = 1,
  parameter bit RvCoreIbexSecureIbex = 0,
  parameter int unsigned RvCoreIbexDmBaseAddr = 437321728,
  parameter int unsigned RvCoreIbexDmAddrMask = 4095,
  parameter int unsigned RvCoreIbexDmHaltAddr = 0,
  parameter int unsigned RvCoreIbexDmExceptionAddr = 0,
  parameter bit RvCoreIbexPipeLine = 0,
  parameter logic [tlul_pkg::RsvdWidth-1:0] RvCoreIbexTlulHostUserRsvdBits = '0,
  parameter logic [31:0] RvCoreIbexCsrMvendorId = '0,
  parameter logic [31:0] RvCoreIbexCsrMimpId = '0
) (
  // Multiplexed I/O
  input        [3:0] mio_in_i,
  output logic [3:0] mio_out_o,
  output logic [3:0] mio_oe_o,
  // Dedicated I/O
  input        [5:0] dio_in_i,
  output logic [5:0] dio_out_o,
  output logic [5:0] dio_oe_o,

  // pad attributes to padring
  output prim_pad_wrapper_pkg::pad_attr_t [pinmux_reg_pkg::NMioPads-1:0] mio_attr_o,
  output prim_pad_wrapper_pkg::pad_attr_t [pinmux_reg_pkg::NDioPads-1:0] dio_attr_o,


  // Inter-module Signal External type
  output prim_mubi_pkg::mubi4_t       clk_main_jitter_en_o,
  output prim_mubi_pkg::mubi4_t       hi_speed_sel_o,
  input  prim_mubi_pkg::mubi4_t       div_step_down_req_i,
  output prim_mubi_pkg::mubi4_t       all_clk_byp_req_o,
  input  prim_mubi_pkg::mubi4_t       all_clk_byp_ack_i,
  output prim_mubi_pkg::mubi4_t       io_clk_byp_req_o,
  input  prim_mubi_pkg::mubi4_t       io_clk_byp_ack_i,
  output pinmux_pkg::dft_strap_test_req_t       dft_strap_test_o,
  input  logic       dft_hold_tap_sel_i,
  output pwrmgr_pkg::pwr_ast_req_t       pwrmgr_ast_req_o,
  input  pwrmgr_pkg::pwr_ast_rsp_t       pwrmgr_ast_rsp_i,
  input  logic [1:0] por_n_i,
  input  logic [31:0] fpga_info_i,
  output logic       sck_monitor_o,


  // All externally supplied clocks
  input clk_main_i,
  input clk_io_i,
  input clk_aon_i,

  // Outgoing alerts for group assam
  output prim_alert_pkg::alert_tx_t [top_assam_pkg::NOutgoingAlertsAssam-1:0] outgoing_alert_assam_tx_o,
  input  prim_alert_pkg::alert_rx_t [top_assam_pkg::NOutgoingAlertsAssam-1:0] outgoing_alert_assam_rx_i,
  output prim_mubi_pkg::mubi4_t     [top_assam_pkg::NOutgoingLpgsAssam-1:0]   outgoing_lpg_cg_en_assam_o,
  output prim_mubi_pkg::mubi4_t     [top_assam_pkg::NOutgoingLpgsAssam-1:0]   outgoing_lpg_rst_en_assam_o,

  // All clocks forwarded to ast
  output clkmgr_pkg::clkmgr_out_t clks_ast_o,
  output rstmgr_pkg::rstmgr_out_t rsts_ast_o,

  input                      scan_rst_ni, // reset used for test mode
  input                      scan_en_i,
  input prim_mubi_pkg::mubi4_t scanmode_i   // lc_ctrl_pkg::On for Scan
);

  import tlul_pkg::*;
  import top_pkg::*;
  import tl_main_pkg::*;
  import top_assam_pkg::*;
  // Compile-time random constants
  import top_assam_rnd_cnst_pkg::*;

  // Local Parameters
  // local parameters for sram_ctrl_main
  localparam int SramCtrlMainOutstanding = 2;
  // local parameters for rom_ctrl
  localparam bit RomCtrlFlopToKmac = 1'b0;
  // local parameters for rv_core_ibex
  localparam bit RvCoreIbexInstructionPipeline = 1'b0;

  // Signals
  logic [0:0] mio_p2d;
  logic [0:0] mio_d2p;
  logic [0:0] mio_en_d2p;
  logic [5:0] dio_p2d;
  logic [5:0] dio_d2p;
  logic [5:0] dio_en_d2p;
  // uart0
  logic       cio_uart0_rx_p2d;
  logic       cio_uart0_tx_d2p;
  logic       cio_uart0_tx_en_d2p;
  // spi_device
  logic       cio_spi_device_sck_p2d;
  logic       cio_spi_device_csb_p2d;
  logic       cio_spi_device_tpm_csb_p2d;
  logic [3:0] cio_spi_device_sd_p2d;
  logic [3:0] cio_spi_device_sd_d2p;
  logic [3:0] cio_spi_device_sd_en_d2p;
  // rv_timer
  // pwrmgr_aon
  // rstmgr_aon
  // clkmgr_aon
  // pinmux_aon
  // rv_plic
  // aes
  // sram_ctrl_main
  // rom_ctrl
  // rv_core_ibex


  logic [18:0]  intr_vector;
  // Interrupt source list
  logic intr_uart0_tx_watermark;
  logic intr_uart0_rx_watermark;
  logic intr_uart0_tx_done;
  logic intr_uart0_rx_overflow;
  logic intr_uart0_rx_frame_err;
  logic intr_uart0_rx_break_err;
  logic intr_uart0_rx_timeout;
  logic intr_uart0_rx_parity_err;
  logic intr_uart0_tx_empty;
  logic intr_spi_device_upload_cmdfifo_not_empty;
  logic intr_spi_device_upload_payload_not_empty;
  logic intr_spi_device_upload_payload_overflow;
  logic intr_spi_device_readbuf_watermark;
  logic intr_spi_device_readbuf_flip;
  logic intr_spi_device_tpm_header_not_empty;
  logic intr_spi_device_tpm_rdfifo_cmd_end;
  logic intr_spi_device_tpm_rdfifo_drop;
  logic intr_rv_timer_timer_expired_hart0_timer0;
  logic intr_pwrmgr_aon_wakeup;

  // Alert list


  // define inter-module signals
  pwrmgr_pkg::pwr_rst_req_t       pwrmgr_aon_pwr_rst_req;
  pwrmgr_pkg::pwr_rst_rsp_t       pwrmgr_aon_pwr_rst_rsp;
  pwrmgr_pkg::pwr_clk_req_t       pwrmgr_aon_pwr_clk_req;
  pwrmgr_pkg::pwr_clk_rsp_t       pwrmgr_aon_pwr_clk_rsp;
  logic       pwrmgr_aon_strap;
  logic       pwrmgr_aon_low_power;
  lc_ctrl_pkg::lc_tx_t       pwrmgr_aon_fetch_en;
  prim_mubi_pkg::mubi4_t       clkmgr_aon_idle;
  logic       rv_plic_msip;
  logic       rv_plic_irq;
  rv_core_ibex_pkg::cpu_crash_dump_t       rv_core_ibex_crash_dump;
  rv_core_ibex_pkg::cpu_pwrmgr_t       rv_core_ibex_pwrmgr;
  prim_mubi_pkg::mubi4_t       rstmgr_aon_sw_rst_req;
  logic       pwrmgr_aon_wakeups;
  logic       pwrmgr_aon_rstreqs;
  tlul_pkg::tl_h2d_t       main_tl_rv_core_ibex__corei_req;
  tlul_pkg::tl_d2h_t       main_tl_rv_core_ibex__corei_rsp;
  tlul_pkg::tl_h2d_t       main_tl_rv_core_ibex__cored_req;
  tlul_pkg::tl_d2h_t       main_tl_rv_core_ibex__cored_rsp;
  tlul_pkg::tl_h2d_t       rom_ctrl_rom_tl_req;
  tlul_pkg::tl_d2h_t       rom_ctrl_rom_tl_rsp;
  tlul_pkg::tl_h2d_t       rom_ctrl_regs_tl_req;
  tlul_pkg::tl_d2h_t       rom_ctrl_regs_tl_rsp;
  tlul_pkg::tl_h2d_t       main_tl_peri_req;
  tlul_pkg::tl_d2h_t       main_tl_peri_rsp;
  tlul_pkg::tl_h2d_t       aes_tl_req;
  tlul_pkg::tl_d2h_t       aes_tl_rsp;
  tlul_pkg::tl_h2d_t       rv_plic_tl_req;
  tlul_pkg::tl_d2h_t       rv_plic_tl_rsp;
  tlul_pkg::tl_h2d_t       rv_core_ibex_cfg_tl_d_req;
  tlul_pkg::tl_d2h_t       rv_core_ibex_cfg_tl_d_rsp;
  tlul_pkg::tl_h2d_t       sram_ctrl_main_regs_tl_req;
  tlul_pkg::tl_d2h_t       sram_ctrl_main_regs_tl_rsp;
  tlul_pkg::tl_h2d_t       sram_ctrl_main_ram_tl_req;
  tlul_pkg::tl_d2h_t       sram_ctrl_main_ram_tl_rsp;
  tlul_pkg::tl_h2d_t       uart0_tl_req;
  tlul_pkg::tl_d2h_t       uart0_tl_rsp;
  tlul_pkg::tl_h2d_t       spi_device_tl_req;
  tlul_pkg::tl_d2h_t       spi_device_tl_rsp;
  tlul_pkg::tl_h2d_t       rv_timer_tl_req;
  tlul_pkg::tl_d2h_t       rv_timer_tl_rsp;
  tlul_pkg::tl_h2d_t       pwrmgr_aon_tl_req;
  tlul_pkg::tl_d2h_t       pwrmgr_aon_tl_rsp;
  tlul_pkg::tl_h2d_t       rstmgr_aon_tl_req;
  tlul_pkg::tl_d2h_t       rstmgr_aon_tl_rsp;
  tlul_pkg::tl_h2d_t       clkmgr_aon_tl_req;
  tlul_pkg::tl_d2h_t       clkmgr_aon_tl_rsp;
  tlul_pkg::tl_h2d_t       pinmux_aon_tl_req;
  tlul_pkg::tl_d2h_t       pinmux_aon_tl_rsp;
  clkmgr_pkg::clkmgr_out_t       clkmgr_aon_clocks;
  clkmgr_pkg::clkmgr_cg_en_t       clkmgr_aon_cg_en;
  rstmgr_pkg::rstmgr_out_t       rstmgr_aon_resets;
  rstmgr_pkg::rstmgr_rst_en_t       rstmgr_aon_rst_en;
  logic       rv_core_ibex_irq_timer;
  logic [31:0] rv_core_ibex_hart_id;
  logic [31:0] rv_core_ibex_boot_addr;
  jtag_pkg::jtag_req_t       pinmux_aon_dft_jtag_req;
  jtag_pkg::jtag_rsp_t       pinmux_aon_dft_jtag_rsp;
  prim_mubi_pkg::mubi8_t       sram_ctrl_main_otp_en_sram_ifetch;

  // define mixed connection to port

  // define partial inter-module tie-off

  // assign partial inter-module tie-off




  // See #7978 This below is a hack.
  // This is because ast is a comportable-like module that sits outside
  // of top_assam's boundary.
  assign clks_ast_o = clkmgr_aon_clocks;
  assign rsts_ast_o = rstmgr_aon_resets;

  // ibex specific assignments
  // TODO: This should be further automated in the future.
  assign rv_core_ibex_irq_timer = intr_rv_timer_timer_expired_hart0_timer0;
  assign rv_core_ibex_hart_id = '0;

  assign rv_core_ibex_boot_addr = ADDR_SPACE_ROM_CTRL__ROM;


  // Struct breakout module tool-inserted DFT TAP signals
  pinmux_jtag_breakout u_dft_tap_breakout (
    .req_i    (pinmux_aon_dft_jtag_req),
    .rsp_o    (pinmux_aon_dft_jtag_rsp),
    .tck_o    (),
    .trst_no  (),
    .tms_o    (),
    .tdi_o    (),
    .tdo_i    (1'b0),
    .tdo_oe_i (1'b0)
  );

  // Wire up alert handler LPGs
  prim_mubi_pkg::mubi4_t [alert_handler_pkg::NLpg-1:0] lpg_cg_en;
  prim_mubi_pkg::mubi4_t [alert_handler_pkg::NLpg-1:0] lpg_rst_en;


  // secure_lc_io_div4_0
  assign lpg_cg_en[0] = clkmgr_aon_cg_en.io_div4_secure;
  assign lpg_rst_en[0] = rstmgr_aon_rst_en.lc_io_div4[rstmgr_pkg::Domain0Sel];

  // Outgoing LPGs for alert group assam
  // peri_lc_io_div4_0
  assign outgoing_lpg_cg_en_assam_o[0] = clkmgr_aon_cg_en.io_div4_peri;
  assign outgoing_lpg_rst_en_assam_o[0] = rstmgr_aon_rst_en.lc_io_div4[rstmgr_pkg::Domain0Sel];
  // peri_spi_device_0
  assign outgoing_lpg_cg_en_assam_o[1] = clkmgr_aon_cg_en.io_div4_peri;
  assign outgoing_lpg_rst_en_assam_o[1] = rstmgr_aon_rst_en.spi_device[rstmgr_pkg::Domain0Sel];
  // timers_sys_io_div4_0
  assign outgoing_lpg_cg_en_assam_o[2] = clkmgr_aon_cg_en.io_div4_timers;
  assign outgoing_lpg_rst_en_assam_o[2] = rstmgr_aon_rst_en.sys_io_div4[rstmgr_pkg::Domain0Sel];
  // powerup_por_io_div4_Aon
  assign outgoing_lpg_cg_en_assam_o[3] = clkmgr_aon_cg_en.io_div4_powerup;
  assign outgoing_lpg_rst_en_assam_o[3] = rstmgr_aon_rst_en.por_io_div4[rstmgr_pkg::DomainAonSel];
  // powerup_lc_io_div4_Aon
  assign outgoing_lpg_cg_en_assam_o[4] = clkmgr_aon_cg_en.io_div4_powerup;
  assign outgoing_lpg_rst_en_assam_o[4] = rstmgr_aon_rst_en.lc_io_div4[rstmgr_pkg::DomainAonSel];
  // secure_sys_0
  assign outgoing_lpg_cg_en_assam_o[5] = clkmgr_aon_cg_en.main_secure;
  assign outgoing_lpg_rst_en_assam_o[5] = rstmgr_aon_rst_en.sys[rstmgr_pkg::Domain0Sel];
  // aes_trans_sys_0
  assign outgoing_lpg_cg_en_assam_o[6] = clkmgr_aon_cg_en.main_aes;
  assign outgoing_lpg_rst_en_assam_o[6] = rstmgr_aon_rst_en.sys[rstmgr_pkg::Domain0Sel];
  // infra_sys_0
  assign outgoing_lpg_cg_en_assam_o[7] = clkmgr_aon_cg_en.main_infra;
  assign outgoing_lpg_rst_en_assam_o[7] = rstmgr_aon_rst_en.sys[rstmgr_pkg::Domain0Sel];

// tie-off unused connections
//VCS coverage off
// pragma coverage off
    prim_mubi_pkg::mubi4_t unused_cg_en_0;
    assign unused_cg_en_0 = clkmgr_aon_cg_en.aon_powerup;
    prim_mubi_pkg::mubi4_t unused_cg_en_1;
    assign unused_cg_en_1 = clkmgr_aon_cg_en.main_powerup;
    prim_mubi_pkg::mubi4_t unused_cg_en_2;
    assign unused_cg_en_2 = clkmgr_aon_cg_en.io_powerup;
    prim_mubi_pkg::mubi4_t unused_cg_en_3;
    assign unused_cg_en_3 = clkmgr_aon_cg_en.io_div2_powerup;
    prim_mubi_pkg::mubi4_t unused_cg_en_4;
    assign unused_cg_en_4 = clkmgr_aon_cg_en.aon_secure;
    prim_mubi_pkg::mubi4_t unused_cg_en_5;
    assign unused_cg_en_5 = clkmgr_aon_cg_en.io_div4_infra;
    prim_mubi_pkg::mubi4_t unused_cg_en_6;
    assign unused_cg_en_6 = clkmgr_aon_cg_en.io_div2_peri;
    prim_mubi_pkg::mubi4_t unused_rst_en_0;
    assign unused_rst_en_0 = rstmgr_aon_rst_en.por_aon[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_1;
    assign unused_rst_en_1 = rstmgr_aon_rst_en.por_aon[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_2;
    assign unused_rst_en_2 = rstmgr_aon_rst_en.por[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_3;
    assign unused_rst_en_3 = rstmgr_aon_rst_en.por[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_4;
    assign unused_rst_en_4 = rstmgr_aon_rst_en.por_io[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_5;
    assign unused_rst_en_5 = rstmgr_aon_rst_en.por_io[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_6;
    assign unused_rst_en_6 = rstmgr_aon_rst_en.por_io_div2[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_7;
    assign unused_rst_en_7 = rstmgr_aon_rst_en.por_io_div2[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_8;
    assign unused_rst_en_8 = rstmgr_aon_rst_en.por_io_div4_shadowed[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_9;
    assign unused_rst_en_9 = rstmgr_aon_rst_en.por_io_div4_shadowed[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_10;
    assign unused_rst_en_10 = rstmgr_aon_rst_en.por_io_div4[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_11;
    assign unused_rst_en_11 = rstmgr_aon_rst_en.lc[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_12;
    assign unused_rst_en_12 = rstmgr_aon_rst_en.lc[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_13;
    assign unused_rst_en_13 = rstmgr_aon_rst_en.sys_shadowed[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_14;
    assign unused_rst_en_14 = rstmgr_aon_rst_en.sys[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_15;
    assign unused_rst_en_15 = rstmgr_aon_rst_en.sys_shadowed[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_16;
    assign unused_rst_en_16 = rstmgr_aon_rst_en.sys_io_div4[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_17;
    assign unused_rst_en_17 = rstmgr_aon_rst_en.sys_aon[rstmgr_pkg::DomainAonSel];
    prim_mubi_pkg::mubi4_t unused_rst_en_18;
    assign unused_rst_en_18 = rstmgr_aon_rst_en.sys_aon[rstmgr_pkg::Domain0Sel];
    prim_mubi_pkg::mubi4_t unused_rst_en_19;
    assign unused_rst_en_19 = rstmgr_aon_rst_en.spi_device[rstmgr_pkg::DomainAonSel];
//VCS coverage on
// pragma coverage on

  // Peripheral Instantiation


  uart #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[0:0]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles)
  ) u_uart0 (

      // Input
      .cio_rx_i    (cio_uart0_rx_p2d),

      // Output
      .cio_tx_o    (cio_uart0_tx_d2p),
      .cio_tx_en_o (cio_uart0_tx_en_d2p),

      // Interrupt
      .intr_tx_watermark_o  (intr_uart0_tx_watermark),
      .intr_rx_watermark_o  (intr_uart0_rx_watermark),
      .intr_tx_done_o       (intr_uart0_tx_done),
      .intr_rx_overflow_o   (intr_uart0_rx_overflow),
      .intr_rx_frame_err_o  (intr_uart0_rx_frame_err),
      .intr_rx_break_err_o  (intr_uart0_rx_break_err),
      .intr_rx_timeout_o    (intr_uart0_rx_timeout),
      .intr_rx_parity_err_o (intr_uart0_rx_parity_err),
      .intr_tx_empty_o      (intr_uart0_tx_empty),
      // External alert group "assam" [0]: fatal_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[0:0] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[0:0] ),

      // Inter-module signals
      .lsio_trigger_o(),
      .racl_policies_i(top_racl_pkg::RACL_POLICY_VEC_DEFAULT),
      .racl_error_o(),
      .tl_i(uart0_tl_req),
      .tl_o(uart0_tl_rsp),

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_io_div4_peri),
      .rst_ni (rstmgr_aon_resets.rst_lc_io_div4_n[rstmgr_pkg::Domain0Sel])
  );
  spi_device #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[1:1]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles),
    .SramType(SpiDeviceSramType)
  ) u_spi_device (

      // Input
      .cio_sck_i        (cio_spi_device_sck_p2d),
      .cio_csb_i        (cio_spi_device_csb_p2d),
      .cio_tpm_csb_i    (cio_spi_device_tpm_csb_p2d),
      .cio_sd_i         (cio_spi_device_sd_p2d),

      // Output
      .cio_sd_o         (cio_spi_device_sd_d2p),
      .cio_sd_en_o      (cio_spi_device_sd_en_d2p),

      // Interrupt
      .intr_upload_cmdfifo_not_empty_o (intr_spi_device_upload_cmdfifo_not_empty),
      .intr_upload_payload_not_empty_o (intr_spi_device_upload_payload_not_empty),
      .intr_upload_payload_overflow_o  (intr_spi_device_upload_payload_overflow),
      .intr_readbuf_watermark_o        (intr_spi_device_readbuf_watermark),
      .intr_readbuf_flip_o             (intr_spi_device_readbuf_flip),
      .intr_tpm_header_not_empty_o     (intr_spi_device_tpm_header_not_empty),
      .intr_tpm_rdfifo_cmd_end_o       (intr_spi_device_tpm_rdfifo_cmd_end),
      .intr_tpm_rdfifo_drop_o          (intr_spi_device_tpm_rdfifo_drop),
      // External alert group "assam" [1]: fatal_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[1:1] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[1:1] ),

      // Inter-module signals
      .ram_cfg_sys2spi_i(prim_ram_2p_pkg::RAM_2P_CFG_DEFAULT),
      .ram_cfg_rsp_sys2spi_o(),
      .ram_cfg_spi2sys_i(prim_ram_2p_pkg::RAM_2P_CFG_DEFAULT),
      .ram_cfg_rsp_spi2sys_o(),
      .passthrough_o(),
      .passthrough_i(spi_device_pkg::PASSTHROUGH_RSP_DEFAULT),
      .mbist_en_i('0),
      .sck_monitor_o(sck_monitor_o),
      .racl_policies_i(top_racl_pkg::RACL_POLICY_VEC_DEFAULT),
      .racl_error_o(),
      .tl_i(spi_device_tl_req),
      .tl_o(spi_device_tl_rsp),
      .scanmode_i,
      .scan_rst_ni,

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_io_div4_peri),
      .scan_clk_i (clkmgr_aon_clocks.clk_io_div2_peri),
      .rst_ni (rstmgr_aon_resets.rst_spi_device_n[rstmgr_pkg::Domain0Sel])
  );
  rv_timer #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[2:2]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles)
  ) u_rv_timer (

      // Interrupt
      .intr_timer_expired_hart0_timer0_o (intr_rv_timer_timer_expired_hart0_timer0),
      // External alert group "assam" [2]: fatal_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[2:2] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[2:2] ),

      // Inter-module signals
      .racl_policies_i(top_racl_pkg::RACL_POLICY_VEC_DEFAULT),
      .racl_error_o(),
      .tl_i(rv_timer_tl_req),
      .tl_o(rv_timer_tl_rsp),

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_io_div4_timers),
      .rst_ni (rstmgr_aon_resets.rst_sys_io_div4_n[rstmgr_pkg::Domain0Sel])
  );
  pwrmgr #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[3:3]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles),
    .EscNumSeverities(4),
    .EscPingCountWidth(16)
  ) u_pwrmgr_aon (

      // Interrupt
      .intr_wakeup_o (intr_pwrmgr_aon_wakeup),
      // External alert group "assam" [3]: fatal_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[3:3] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[3:3] ),

      // Inter-module signals
      .pwr_ast_o(pwrmgr_ast_req_o),
      .pwr_ast_i(pwrmgr_ast_rsp_i),
      .pwr_rst_o(pwrmgr_aon_pwr_rst_req),
      .pwr_rst_i(pwrmgr_aon_pwr_rst_rsp),
      .pwr_clk_o(pwrmgr_aon_pwr_clk_req),
      .pwr_clk_i(pwrmgr_aon_pwr_clk_rsp),
      .pwr_otp_o(),
      .pwr_otp_i(pwrmgr_pkg::PWR_OTP_RSP_DEFAULT),
      .pwr_lc_o(),
      .pwr_lc_i(lc_ctrl_pkg::PWR_LC_RSP_DEFAULT),
      .pwr_flash_i(pwrmgr_pkg::PWR_FLASH_DEFAULT),
      .esc_rst_tx_i(prim_esc_pkg::ESC_TX_DEFAULT),
      .esc_rst_rx_o(),
      .pwr_cpu_i(rv_core_ibex_pwrmgr),
      .wakeups_i(pwrmgr_aon_wakeups),
      .rstreqs_i(pwrmgr_aon_rstreqs),
      .ndmreset_req_i('0),
      .strap_o(pwrmgr_aon_strap),
      .low_power_o(pwrmgr_aon_low_power),
      .rom_ctrl_i(rom_ctrl_pkg::PWRMGR_DATA_DEFAULT),
      .fetch_en_o(pwrmgr_aon_fetch_en),
      .lc_dft_en_i(lc_ctrl_pkg::LC_TX_DEFAULT),
      .lc_hw_debug_en_i(lc_ctrl_pkg::LC_TX_DEFAULT),
      .sw_rst_req_i(rstmgr_aon_sw_rst_req),
      .tl_i(pwrmgr_aon_tl_req),
      .tl_o(pwrmgr_aon_tl_rsp),

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_io_div4_powerup),
      .clk_slow_i (clkmgr_aon_clocks.clk_aon_powerup),
      .clk_lc_i (clkmgr_aon_clocks.clk_io_div4_powerup),
      .clk_esc_i (clkmgr_aon_clocks.clk_io_div4_powerup),
      .rst_ni (rstmgr_aon_resets.rst_por_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_lc_ni (rstmgr_aon_resets.rst_lc_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_esc_ni (rstmgr_aon_resets.rst_lc_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_main_ni (rstmgr_aon_resets.rst_por_aon_n[rstmgr_pkg::DomainAonSel]),
      .rst_slow_ni (rstmgr_aon_resets.rst_por_aon_n[rstmgr_pkg::DomainAonSel])
  );
  rstmgr #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[5:4]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles),
    .SecCheck(SecRstmgrAonCheck),
    .SecMaxSyncDelay(SecRstmgrAonMaxSyncDelay)
  ) u_rstmgr_aon (
      // External alert group "assam" [4]: fatal_fault
      // External alert group "assam" [5]: fatal_cnsty_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[5:4] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[5:4] ),

      // Inter-module signals
      .por_n_i(por_n_i),
      .pwr_i(pwrmgr_aon_pwr_rst_req),
      .pwr_o(pwrmgr_aon_pwr_rst_rsp),
      .resets_o(rstmgr_aon_resets),
      .rst_en_o(rstmgr_aon_rst_en),
      .alert_dump_i(alert_handler_pkg::ALERT_CRASHDUMP_DEFAULT),
      .cpu_dump_i(rv_core_ibex_crash_dump),
      .sw_rst_req_o(rstmgr_aon_sw_rst_req),
      .tl_i(rstmgr_aon_tl_req),
      .tl_o(rstmgr_aon_tl_rsp),
      .scanmode_i,
      .scan_rst_ni,

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_io_div4_powerup),
      .clk_por_i (clkmgr_aon_clocks.clk_io_div4_powerup),
      .clk_aon_i (clkmgr_aon_clocks.clk_aon_powerup),
      .clk_main_i (clkmgr_aon_clocks.clk_main_powerup),
      .clk_io_i (clkmgr_aon_clocks.clk_io_powerup),
      .clk_io_div2_i (clkmgr_aon_clocks.clk_io_div2_powerup),
      .clk_io_div4_i (clkmgr_aon_clocks.clk_io_div4_powerup),
      .rst_ni (rstmgr_aon_resets.rst_lc_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_por_ni (rstmgr_aon_resets.rst_por_io_div4_n[rstmgr_pkg::DomainAonSel])
  );
  clkmgr #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[7:6]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles)
  ) u_clkmgr_aon (
      // External alert group "assam" [6]: recov_fault
      // External alert group "assam" [7]: fatal_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[7:6] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[7:6] ),

      // Inter-module signals
      .clocks_o(clkmgr_aon_clocks),
      .cg_en_o(clkmgr_aon_cg_en),
      .lc_hw_debug_en_i(lc_ctrl_pkg::LC_TX_DEFAULT),
      .io_clk_byp_req_o(io_clk_byp_req_o),
      .io_clk_byp_ack_i(io_clk_byp_ack_i),
      .all_clk_byp_req_o(all_clk_byp_req_o),
      .all_clk_byp_ack_i(all_clk_byp_ack_i),
      .hi_speed_sel_o(hi_speed_sel_o),
      .div_step_down_req_i(div_step_down_req_i),
      .lc_clk_byp_req_i(lc_ctrl_pkg::LC_TX_DEFAULT),
      .lc_clk_byp_ack_o(),
      .jitter_en_o(clk_main_jitter_en_o),
      .pwr_i(pwrmgr_aon_pwr_clk_req),
      .pwr_o(pwrmgr_aon_pwr_clk_rsp),
      .idle_i(clkmgr_aon_idle),
      .calib_rdy_i(prim_mubi_pkg::MuBi4True),
      .tl_i(clkmgr_aon_tl_req),
      .tl_o(clkmgr_aon_tl_rsp),
      .scanmode_i,

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_io_div4_powerup),
      .clk_main_i (clk_main_i),
      .clk_io_i (clk_io_i),
      .clk_aon_i (clk_aon_i),
      .rst_shadowed_ni (rstmgr_aon_resets.rst_por_io_div4_shadowed_n[rstmgr_pkg::DomainAonSel]),
      .rst_ni (rstmgr_aon_resets.rst_por_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_aon_ni (rstmgr_aon_resets.rst_por_aon_n[rstmgr_pkg::DomainAonSel]),
      .rst_io_ni (rstmgr_aon_resets.rst_por_io_n[rstmgr_pkg::DomainAonSel]),
      .rst_io_div2_ni (rstmgr_aon_resets.rst_por_io_div2_n[rstmgr_pkg::DomainAonSel]),
      .rst_io_div4_ni (rstmgr_aon_resets.rst_por_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_main_ni (rstmgr_aon_resets.rst_por_n[rstmgr_pkg::DomainAonSel]),
      .rst_root_ni (rstmgr_aon_resets.rst_por_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_root_io_ni (rstmgr_aon_resets.rst_por_io_n[rstmgr_pkg::DomainAonSel]),
      .rst_root_io_div2_ni (rstmgr_aon_resets.rst_por_io_div2_n[rstmgr_pkg::DomainAonSel]),
      .rst_root_io_div4_ni (rstmgr_aon_resets.rst_por_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_root_main_ni (rstmgr_aon_resets.rst_por_n[rstmgr_pkg::DomainAonSel])
  );
  pinmux #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[8:8]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles),
    .SecVolatileRawUnlockEn(SecPinmuxAonVolatileRawUnlockEn),
    .TargetCfg(PinmuxAonTargetCfg)
  ) u_pinmux_aon (
      // External alert group "assam" [8]: fatal_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[8:8] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[8:8] ),

      // Inter-module signals
      .lc_hw_debug_clr_i(lc_ctrl_pkg::Off),
      .lc_hw_debug_en_i(lc_ctrl_pkg::Off),
      .lc_dft_en_i(lc_ctrl_pkg::Off),
      .lc_escalate_en_i(lc_ctrl_pkg::Off),
      .lc_check_byp_en_i(lc_ctrl_pkg::Off),
      .pinmux_hw_debug_en_o(),
      .lc_jtag_o(),
      .lc_jtag_i(jtag_pkg::JTAG_RSP_DEFAULT),
      .rv_jtag_o(),
      .rv_jtag_i(jtag_pkg::JTAG_RSP_DEFAULT),
      .dft_jtag_o(pinmux_aon_dft_jtag_req),
      .dft_jtag_i(pinmux_aon_dft_jtag_rsp),
      .dft_strap_test_o(dft_strap_test_o),
      .dft_hold_tap_sel_i(dft_hold_tap_sel_i),
      .sleep_en_i(pwrmgr_aon_low_power),
      .strap_en_i(pwrmgr_aon_strap),
      .strap_en_override_i(1'b0),
      .pin_wkup_req_o(pwrmgr_aon_wakeups),
      .tl_i(pinmux_aon_tl_req),
      .tl_o(pinmux_aon_tl_rsp),

      .periph_to_mio_i      (mio_d2p    ),
      .periph_to_mio_oe_i   (mio_en_d2p ),
      .mio_to_periph_o      (mio_p2d    ),

      .mio_attr_o,
      .mio_out_o,
      .mio_oe_o,
      .mio_in_i,

      .periph_to_dio_i      (dio_d2p    ),
      .periph_to_dio_oe_i   (dio_en_d2p ),
      .dio_to_periph_o      (dio_p2d    ),

      .dio_attr_o,
      .dio_out_o,
      .dio_oe_o,
      .dio_in_i,

      .scanmode_i,

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_io_div4_powerup),
      .clk_aon_i (clkmgr_aon_clocks.clk_aon_powerup),
      .rst_ni (rstmgr_aon_resets.rst_lc_io_div4_n[rstmgr_pkg::DomainAonSel]),
      .rst_aon_ni (rstmgr_aon_resets.rst_sys_aon_n[rstmgr_pkg::DomainAonSel]),
      .rst_sys_ni (rstmgr_aon_resets.rst_sys_io_div4_n[rstmgr_pkg::DomainAonSel])
  );
  rv_plic #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[9:9]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles)
  ) u_rv_plic (
      // External alert group "assam" [9]: fatal_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[9:9] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[9:9] ),

      // Inter-module signals
      .irq_o(rv_plic_irq),
      .irq_id_o(),
      .msip_o(rv_plic_msip),
      .tl_i(rv_plic_tl_req),
      .tl_o(rv_plic_tl_rsp),
      .intr_src_i (intr_vector),

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_main_secure),
      .rst_ni (rstmgr_aon_resets.rst_sys_n[rstmgr_pkg::Domain0Sel])
  );
  aes #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[11:10]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles),
    .AES192Enable(1'b1),
    .SecMasking(SecAesMasking),
    .SecSBoxImpl(SecAesSBoxImpl),
    .SecStartTriggerDelay(SecAesStartTriggerDelay),
    .SecAllowForcingMasks(SecAesAllowForcingMasks),
    .SecSkipPRNGReseeding(SecAesSkipPRNGReseeding),
    .RndCnstClearingLfsrSeed(RndCnstAesClearingLfsrSeed),
    .RndCnstClearingLfsrPerm(RndCnstAesClearingLfsrPerm),
    .RndCnstClearingSharePerm(RndCnstAesClearingSharePerm),
    .RndCnstMaskingLfsrSeed(RndCnstAesMaskingLfsrSeed),
    .RndCnstMaskingLfsrPerm(RndCnstAesMaskingLfsrPerm)
  ) u_aes (
      // External alert group "assam" [10]: recov_ctrl_update_err
      // External alert group "assam" [11]: fatal_fault
      .alert_tx_o  ( outgoing_alert_assam_tx_o[11:10] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[11:10] ),

      // Inter-module signals
      .idle_o(clkmgr_aon_idle),
      .lc_escalate_en_i(lc_ctrl_pkg::Off),
      .edn_o(),
      .edn_i(edn_pkg::EDN_RSP_DEFAULT),
      .keymgr_key_i(keymgr_pkg::HW_KEY_REQ_DEFAULT),
      .tl_i(aes_tl_req),
      .tl_o(aes_tl_rsp),

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_main_aes),
      .clk_edn_i (clkmgr_aon_clocks.clk_main_aes),
      .rst_shadowed_ni (rstmgr_aon_resets.rst_sys_shadowed_n[rstmgr_pkg::Domain0Sel]),
      .rst_ni (rstmgr_aon_resets.rst_sys_n[rstmgr_pkg::Domain0Sel]),
      .rst_edn_ni (rstmgr_aon_resets.rst_sys_n[rstmgr_pkg::Domain0Sel])
  );
  sram_ctrl #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[12:12]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles),
    .RndCnstSramKey(RndCnstSramCtrlMainSramKey),
    .RndCnstSramNonce(RndCnstSramCtrlMainSramNonce),
    .RndCnstLfsrSeed(RndCnstSramCtrlMainLfsrSeed),
    .RndCnstLfsrPerm(RndCnstSramCtrlMainLfsrPerm),
    .MemSizeRam(131072),
    .InstSize(SramCtrlMainInstSize),
    .NumRamInst(SramCtrlMainNumRamInst),
    .InstrExec(SramCtrlMainInstrExec),
    .NumPrinceRoundsHalf(SramCtrlMainNumPrinceRoundsHalf),
    .Outstanding(SramCtrlMainOutstanding),
    .EccCorrection(SramCtrlMainEccCorrection)
  ) u_sram_ctrl_main (
      // External alert group "assam" [12]: fatal_error
      .alert_tx_o  ( outgoing_alert_assam_tx_o[12:12] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[12:12] ),
      .racl_policy_sel_ranges_ram_i('{top_racl_pkg::RACL_RANGE_T_DEFAULT}),

      // Inter-module signals
      .sram_otp_key_o(),
      .sram_otp_key_i(otp_ctrl_pkg::SRAM_OTP_KEY_RSP_DEFAULT),
      .cfg_i('0),
      .cfg_rsp_o(),
      .lc_escalate_en_i(lc_ctrl_pkg::Off),
      .lc_hw_debug_en_i(lc_ctrl_pkg::Off),
      .otp_en_sram_ifetch_i(sram_ctrl_main_otp_en_sram_ifetch),
      .racl_policies_i(top_racl_pkg::RACL_POLICY_VEC_DEFAULT),
      .racl_error_o(),
      .sram_rerror_o(),
      .regs_tl_i(sram_ctrl_main_regs_tl_req),
      .regs_tl_o(sram_ctrl_main_regs_tl_rsp),
      .ram_tl_i(sram_ctrl_main_ram_tl_req),
      .ram_tl_o(sram_ctrl_main_ram_tl_rsp),

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_main_secure),
      .clk_otp_i (clkmgr_aon_clocks.clk_io_div4_secure),
      .rst_ni (rstmgr_aon_resets.rst_sys_n[rstmgr_pkg::Domain0Sel]),
      .rst_otp_ni (rstmgr_aon_resets.rst_lc_io_div4_n[rstmgr_pkg::Domain0Sel])
  );
  rom_ctrl #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[13:13]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles),
    .BootRomInitFile(RomCtrlBootRomInitFile),
    .FlopToKmac(RomCtrlFlopToKmac),
    .RndCnstScrNonce(RndCnstRomCtrlScrNonce),
    .RndCnstScrKey(RndCnstRomCtrlScrKey),
    .SecDisableScrambling(SecRomCtrlDisableScrambling),
    .MemSizeRom(32768)
  ) u_rom_ctrl (
      // External alert group "assam" [13]: fatal
      .alert_tx_o  ( outgoing_alert_assam_tx_o[13:13] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[13:13] ),

      // Inter-module signals
      .rom_cfg_i(prim_rom_pkg::ROM_CFG_DEFAULT),
      .pwrmgr_data_o(),
      .keymgr_data_o(),
      .kmac_data_o(),
      .kmac_data_i(kmac_pkg::APP_RSP_DEFAULT),
      .regs_tl_i(rom_ctrl_regs_tl_req),
      .regs_tl_o(rom_ctrl_regs_tl_rsp),
      .rom_tl_i(rom_ctrl_rom_tl_req),
      .rom_tl_o(rom_ctrl_rom_tl_rsp),

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_main_infra),
      .rst_ni (rstmgr_aon_resets.rst_sys_n[rstmgr_pkg::Domain0Sel])
  );
  rv_core_ibex #(
    .AlertAsyncOn(AsyncOnOutgoingAlertAssam[17:14]),
    .AlertSkewCycles(top_pkg::AlertSkewCycles),
    .RndCnstLfsrSeed(RndCnstRvCoreIbexLfsrSeed),
    .RndCnstLfsrPerm(RndCnstRvCoreIbexLfsrPerm),
    .RndCnstIbexKeyDefault(RndCnstRvCoreIbexIbexKeyDefault),
    .RndCnstIbexNonceDefault(RndCnstRvCoreIbexIbexNonceDefault),
    .NEscalationSeverities(4),
    .WidthPingCounter(16),
    .PMPEnable(RvCoreIbexPMPEnable),
    .PMPGranularity(RvCoreIbexPMPGranularity),
    .PMPNumRegions(RvCoreIbexPMPNumRegions),
    .MHPMCounterNum(RvCoreIbexMHPMCounterNum),
    .MHPMCounterWidth(RvCoreIbexMHPMCounterWidth),
    .PMPRstCfg(RvCoreIbexPMPRstCfg),
    .PMPRstAddr(RvCoreIbexPMPRstAddr),
    .PMPRstMsecCfg(RvCoreIbexPMPRstMsecCfg),
    .RV32E(RvCoreIbexRV32E),
    .RV32M(RvCoreIbexRV32M),
    .RV32B(RvCoreIbexRV32B),
    .RegFile(RvCoreIbexRegFile),
    .BranchTargetALU(RvCoreIbexBranchTargetALU),
    .WritebackStage(RvCoreIbexWritebackStage),
    .ICache(RvCoreIbexICache),
    .ICacheECC(RvCoreIbexICacheECC),
    .ICacheScramble(RvCoreIbexICacheScramble),
    .ICacheNWays(RvCoreIbexICacheNWays),
    .BranchPredictor(RvCoreIbexBranchPredictor),
    .DbgTriggerEn(RvCoreIbexDbgTriggerEn),
    .DbgHwBreakNum(RvCoreIbexDbgHwBreakNum),
    .SecureIbex(RvCoreIbexSecureIbex),
    .DmBaseAddr(RvCoreIbexDmBaseAddr),
    .DmAddrMask(RvCoreIbexDmAddrMask),
    .DmHaltAddr(RvCoreIbexDmHaltAddr),
    .DmExceptionAddr(RvCoreIbexDmExceptionAddr),
    .PipeLine(RvCoreIbexPipeLine),
    .TlulHostUserRsvdBits(RvCoreIbexTlulHostUserRsvdBits),
    .CsrMvendorId(RvCoreIbexCsrMvendorId),
    .CsrMimpId(RvCoreIbexCsrMimpId),
    .InstructionPipeline(RvCoreIbexInstructionPipeline)
  ) u_rv_core_ibex (
      // External alert group "assam" [14]: fatal_sw_err
      // External alert group "assam" [15]: recov_sw_err
      // External alert group "assam" [16]: fatal_hw_err
      // External alert group "assam" [17]: recov_hw_err
      .alert_tx_o  ( outgoing_alert_assam_tx_o[17:14] ),
      .alert_rx_i  ( outgoing_alert_assam_rx_i[17:14] ),

      // Inter-module signals
      .rst_cpu_n_o(),
      .ram_cfg_icache_tag_i(prim_ram_1p_pkg::RAM_1P_CFG_DEFAULT),
      .ram_cfg_rsp_icache_tag_o(),
      .ram_cfg_icache_data_i(prim_ram_1p_pkg::RAM_1P_CFG_DEFAULT),
      .ram_cfg_rsp_icache_data_o(),
      .hart_id_i(rv_core_ibex_hart_id),
      .boot_addr_i(rv_core_ibex_boot_addr),
      .irq_software_i(rv_plic_msip),
      .irq_timer_i(rv_core_ibex_irq_timer),
      .irq_external_i(rv_plic_irq),
      .esc_tx_i(prim_esc_pkg::ESC_TX_DEFAULT),
      .esc_rx_o(),
      .debug_req_i('0),
      .crash_dump_o(rv_core_ibex_crash_dump),
      .lc_cpu_en_i(lc_ctrl_pkg::On),
      .pwrmgr_cpu_en_i(pwrmgr_aon_fetch_en),
      .pwrmgr_o(rv_core_ibex_pwrmgr),
      .nmi_wdog_i('0),
      .edn_o(),
      .edn_i(edn_pkg::EDN_RSP_DEFAULT),
      .icache_otp_key_o(),
      .icache_otp_key_i(otp_ctrl_pkg::SRAM_OTP_KEY_RSP_DEFAULT),
      .fpga_info_i(fpga_info_i),
      .corei_tl_h_o(main_tl_rv_core_ibex__corei_req),
      .corei_tl_h_i(main_tl_rv_core_ibex__corei_rsp),
      .cored_tl_h_o(main_tl_rv_core_ibex__cored_req),
      .cored_tl_h_i(main_tl_rv_core_ibex__cored_rsp),
      .cfg_tl_d_i(rv_core_ibex_cfg_tl_d_req),
      .cfg_tl_d_o(rv_core_ibex_cfg_tl_d_rsp),
      .scanmode_i,
      .scan_rst_ni,

      // Clock and reset connections
      .clk_i (clkmgr_aon_clocks.clk_main_infra),
      .clk_edn_i (clkmgr_aon_clocks.clk_main_infra),
      .clk_esc_i (clkmgr_aon_clocks.clk_io_div4_infra),
      .clk_otp_i (clkmgr_aon_clocks.clk_io_div4_infra),
      .rst_ni (rstmgr_aon_resets.rst_sys_n[rstmgr_pkg::Domain0Sel]),
      .rst_edn_ni (rstmgr_aon_resets.rst_sys_n[rstmgr_pkg::Domain0Sel]),
      .rst_esc_ni (rstmgr_aon_resets.rst_lc_io_div4_n[rstmgr_pkg::Domain0Sel]),
      .rst_otp_ni (rstmgr_aon_resets.rst_lc_io_div4_n[rstmgr_pkg::Domain0Sel])
  );


  // interrupt assignments
  assign intr_vector = {
      intr_pwrmgr_aon_wakeup, // IDs [18 +: 1]
      intr_spi_device_tpm_rdfifo_drop, // IDs [17 +: 1]
      intr_spi_device_tpm_rdfifo_cmd_end, // IDs [16 +: 1]
      intr_spi_device_tpm_header_not_empty, // IDs [15 +: 1]
      intr_spi_device_readbuf_flip, // IDs [14 +: 1]
      intr_spi_device_readbuf_watermark, // IDs [13 +: 1]
      intr_spi_device_upload_payload_overflow, // IDs [12 +: 1]
      intr_spi_device_upload_payload_not_empty, // IDs [11 +: 1]
      intr_spi_device_upload_cmdfifo_not_empty, // IDs [10 +: 1]
      intr_uart0_tx_empty, // IDs [9 +: 1]
      intr_uart0_rx_parity_err, // IDs [8 +: 1]
      intr_uart0_rx_timeout, // IDs [7 +: 1]
      intr_uart0_rx_break_err, // IDs [6 +: 1]
      intr_uart0_rx_frame_err, // IDs [5 +: 1]
      intr_uart0_rx_overflow, // IDs [4 +: 1]
      intr_uart0_tx_done, // IDs [3 +: 1]
      intr_uart0_rx_watermark, // IDs [2 +: 1]
      intr_uart0_tx_watermark, // IDs [1 +: 1]
      1'b 0 // ID [0 +: 1] is a special case and tied to zero.
  };

  // TL-UL Crossbar
  xbar_main u_xbar_main (
    .clk_main_i (clkmgr_aon_clocks.clk_main_infra),
    .clk_fixed_i (clkmgr_aon_clocks.clk_io_div4_infra),
    .rst_main_ni (rstmgr_aon_resets.rst_sys_n[rstmgr_pkg::Domain0Sel]),
    .rst_fixed_ni (rstmgr_aon_resets.rst_sys_io_div4_n[rstmgr_pkg::Domain0Sel]),

    // port: tl_rv_core_ibex__corei
    .tl_rv_core_ibex__corei_i(main_tl_rv_core_ibex__corei_req),
    .tl_rv_core_ibex__corei_o(main_tl_rv_core_ibex__corei_rsp),

    // port: tl_rv_core_ibex__cored
    .tl_rv_core_ibex__cored_i(main_tl_rv_core_ibex__cored_req),
    .tl_rv_core_ibex__cored_o(main_tl_rv_core_ibex__cored_rsp),

    // port: tl_rom_ctrl__rom
    .tl_rom_ctrl__rom_o(rom_ctrl_rom_tl_req),
    .tl_rom_ctrl__rom_i(rom_ctrl_rom_tl_rsp),

    // port: tl_rom_ctrl__regs
    .tl_rom_ctrl__regs_o(rom_ctrl_regs_tl_req),
    .tl_rom_ctrl__regs_i(rom_ctrl_regs_tl_rsp),

    // port: tl_peri
    .tl_peri_o(main_tl_peri_req),
    .tl_peri_i(main_tl_peri_rsp),

    // port: tl_aes
    .tl_aes_o(aes_tl_req),
    .tl_aes_i(aes_tl_rsp),

    // port: tl_rv_plic
    .tl_rv_plic_o(rv_plic_tl_req),
    .tl_rv_plic_i(rv_plic_tl_rsp),

    // port: tl_rv_core_ibex__cfg
    .tl_rv_core_ibex__cfg_o(rv_core_ibex_cfg_tl_d_req),
    .tl_rv_core_ibex__cfg_i(rv_core_ibex_cfg_tl_d_rsp),

    // port: tl_sram_ctrl_main__regs
    .tl_sram_ctrl_main__regs_o(sram_ctrl_main_regs_tl_req),
    .tl_sram_ctrl_main__regs_i(sram_ctrl_main_regs_tl_rsp),

    // port: tl_sram_ctrl_main__ram
    .tl_sram_ctrl_main__ram_o(sram_ctrl_main_ram_tl_req),
    .tl_sram_ctrl_main__ram_i(sram_ctrl_main_ram_tl_rsp),


    .scanmode_i
  );
  xbar_peri u_xbar_peri (
    .clk_peri_i (clkmgr_aon_clocks.clk_io_div4_infra),
    .rst_peri_ni (rstmgr_aon_resets.rst_sys_io_div4_n[rstmgr_pkg::Domain0Sel]),

    // port: tl_main
    .tl_main_i(main_tl_peri_req),
    .tl_main_o(main_tl_peri_rsp),

    // port: tl_uart0
    .tl_uart0_o(uart0_tl_req),
    .tl_uart0_i(uart0_tl_rsp),

    // port: tl_spi_device
    .tl_spi_device_o(spi_device_tl_req),
    .tl_spi_device_i(spi_device_tl_rsp),

    // port: tl_rv_timer
    .tl_rv_timer_o(rv_timer_tl_req),
    .tl_rv_timer_i(rv_timer_tl_rsp),

    // port: tl_pwrmgr_aon
    .tl_pwrmgr_aon_o(pwrmgr_aon_tl_req),
    .tl_pwrmgr_aon_i(pwrmgr_aon_tl_rsp),

    // port: tl_rstmgr_aon
    .tl_rstmgr_aon_o(rstmgr_aon_tl_req),
    .tl_rstmgr_aon_i(rstmgr_aon_tl_rsp),

    // port: tl_clkmgr_aon
    .tl_clkmgr_aon_o(clkmgr_aon_tl_req),
    .tl_clkmgr_aon_i(clkmgr_aon_tl_rsp),

    // port: tl_pinmux_aon
    .tl_pinmux_aon_o(pinmux_aon_tl_req),
    .tl_pinmux_aon_i(pinmux_aon_tl_rsp),


    .scanmode_i
  );

  // Pinmux connections
  // All muxed inputs
  assign cio_uart0_rx_p2d = mio_p2d[MioInUart0Rx];

  // All muxed outputs
  assign mio_d2p[MioOutUart0Tx] = cio_uart0_tx_d2p;

  // All muxed output enables
  assign mio_en_d2p[MioOutUart0Tx] = cio_uart0_tx_en_d2p;

  // All dedicated inputs
  logic [5:0] unused_dio_p2d;
  assign unused_dio_p2d = dio_p2d;
  assign cio_spi_device_sd_p2d[0] = dio_p2d[DioSpiDeviceSd0];
  assign cio_spi_device_sd_p2d[1] = dio_p2d[DioSpiDeviceSd1];
  assign cio_spi_device_sd_p2d[2] = dio_p2d[DioSpiDeviceSd2];
  assign cio_spi_device_sd_p2d[3] = dio_p2d[DioSpiDeviceSd3];
  assign cio_spi_device_sck_p2d = dio_p2d[DioSpiDeviceSck];
  assign cio_spi_device_csb_p2d = dio_p2d[DioSpiDeviceCsb];

    // All dedicated outputs
  assign dio_d2p[DioSpiDeviceSd0] = cio_spi_device_sd_d2p[0];
  assign dio_d2p[DioSpiDeviceSd1] = cio_spi_device_sd_d2p[1];
  assign dio_d2p[DioSpiDeviceSd2] = cio_spi_device_sd_d2p[2];
  assign dio_d2p[DioSpiDeviceSd3] = cio_spi_device_sd_d2p[3];
  assign dio_d2p[DioSpiDeviceSck] = 1'b0;
  assign dio_d2p[DioSpiDeviceCsb] = 1'b0;

  // All dedicated output enables
  assign dio_en_d2p[DioSpiDeviceSd0] = cio_spi_device_sd_en_d2p[0];
  assign dio_en_d2p[DioSpiDeviceSd1] = cio_spi_device_sd_en_d2p[1];
  assign dio_en_d2p[DioSpiDeviceSd2] = cio_spi_device_sd_en_d2p[2];
  assign dio_en_d2p[DioSpiDeviceSd3] = cio_spi_device_sd_en_d2p[3];
  assign dio_en_d2p[DioSpiDeviceSck] = 1'b0;
  assign dio_en_d2p[DioSpiDeviceCsb] = 1'b0;


  // make sure scanmode_i is never X (including during reset)
  `ASSERT_KNOWN(scanmodeKnown, scanmode_i, clk_main_i, 0)

endmodule

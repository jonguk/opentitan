// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
//
// util/topgen.py -t hw/top_assam/data/top_assam.hjson \
//                -o hw/top_assam/

package top_assam_pkg;
  /**
   * Peripheral base address for uart0 in top assam.
   */
  parameter int unsigned TOP_ASSAM_UART0_BASE_ADDR = 32'h40000000;

  /**
   * Peripheral size in bytes for uart0 in top assam.
   */
  parameter int unsigned TOP_ASSAM_UART0_SIZE_BYTES = 32'h40;

  /**
   * Peripheral base address for spi_device in top assam.
   */
  parameter int unsigned TOP_ASSAM_SPI_DEVICE_BASE_ADDR = 32'h40050000;

  /**
   * Peripheral size in bytes for spi_device in top assam.
   */
  parameter int unsigned TOP_ASSAM_SPI_DEVICE_SIZE_BYTES = 32'h2000;

  /**
   * Peripheral base address for rv_timer in top assam.
   */
  parameter int unsigned TOP_ASSAM_RV_TIMER_BASE_ADDR = 32'h40100000;

  /**
   * Peripheral size in bytes for rv_timer in top assam.
   */
  parameter int unsigned TOP_ASSAM_RV_TIMER_SIZE_BYTES = 32'h200;

  /**
   * Peripheral base address for pwrmgr_aon in top assam.
   */
  parameter int unsigned TOP_ASSAM_PWRMGR_AON_BASE_ADDR = 32'h40400000;

  /**
   * Peripheral size in bytes for pwrmgr_aon in top assam.
   */
  parameter int unsigned TOP_ASSAM_PWRMGR_AON_SIZE_BYTES = 32'h80;

  /**
   * Peripheral base address for rstmgr_aon in top assam.
   */
  parameter int unsigned TOP_ASSAM_RSTMGR_AON_BASE_ADDR = 32'h40410000;

  /**
   * Peripheral size in bytes for rstmgr_aon in top assam.
   */
  parameter int unsigned TOP_ASSAM_RSTMGR_AON_SIZE_BYTES = 32'h40;

  /**
   * Peripheral base address for clkmgr_aon in top assam.
   */
  parameter int unsigned TOP_ASSAM_CLKMGR_AON_BASE_ADDR = 32'h40420000;

  /**
   * Peripheral size in bytes for clkmgr_aon in top assam.
   */
  parameter int unsigned TOP_ASSAM_CLKMGR_AON_SIZE_BYTES = 32'h40;

  /**
   * Peripheral base address for pinmux_aon in top assam.
   */
  parameter int unsigned TOP_ASSAM_PINMUX_AON_BASE_ADDR = 32'h40460000;

  /**
   * Peripheral size in bytes for pinmux_aon in top assam.
   */
  parameter int unsigned TOP_ASSAM_PINMUX_AON_SIZE_BYTES = 32'h200;

  /**
   * Peripheral base address for rv_plic in top assam.
   */
  parameter int unsigned TOP_ASSAM_RV_PLIC_BASE_ADDR = 32'h48000000;

  /**
   * Peripheral size in bytes for rv_plic in top assam.
   */
  parameter int unsigned TOP_ASSAM_RV_PLIC_SIZE_BYTES = 32'h8000000;

  /**
   * Peripheral base address for aes in top assam.
   */
  parameter int unsigned TOP_ASSAM_AES_BASE_ADDR = 32'h41100000;

  /**
   * Peripheral size in bytes for aes in top assam.
   */
  parameter int unsigned TOP_ASSAM_AES_SIZE_BYTES = 32'h100;

  /**
   * Peripheral base address for ast in top assam.
   */
  parameter int unsigned TOP_ASSAM_AST_BASE_ADDR = 32'h40480000;

  /**
   * Peripheral size in bytes for ast in top assam.
   */
  parameter int unsigned TOP_ASSAM_AST_SIZE_BYTES = 32'h400;

  /**
   * Peripheral base address for regs device on sram_ctrl_main in top assam.
   */
  parameter int unsigned TOP_ASSAM_SRAM_CTRL_MAIN_REGS_BASE_ADDR = 32'h411C0000;

  /**
   * Peripheral size in bytes for regs device on sram_ctrl_main in top assam.
   */
  parameter int unsigned TOP_ASSAM_SRAM_CTRL_MAIN_REGS_SIZE_BYTES = 32'h40;

  /**
   * Peripheral base address for regs device on rom_ctrl in top assam.
   */
  parameter int unsigned TOP_ASSAM_ROM_CTRL_REGS_BASE_ADDR = 32'h411E0000;

  /**
   * Peripheral size in bytes for regs device on rom_ctrl in top assam.
   */
  parameter int unsigned TOP_ASSAM_ROM_CTRL_REGS_SIZE_BYTES = 32'h80;

  /**
   * Peripheral base address for cfg device on rv_core_ibex in top assam.
   */
  parameter int unsigned TOP_ASSAM_RV_CORE_IBEX_CFG_BASE_ADDR = 32'h411F0000;

  /**
   * Peripheral size in bytes for cfg device on rv_core_ibex in top assam.
   */
  parameter int unsigned TOP_ASSAM_RV_CORE_IBEX_CFG_SIZE_BYTES = 32'h100;

  /**
   * Memory base address for ram memory on sram_ctrl_main in top assam.
   */
  parameter int unsigned TOP_ASSAM_SRAM_CTRL_MAIN_RAM_BASE_ADDR = 32'h10000000;

  /**
   * Memory size for ram memory on sram_ctrl_main in top assam.
   */
  parameter int unsigned TOP_ASSAM_SRAM_CTRL_MAIN_RAM_SIZE_BYTES = 32'h20000;

  /**
   * Memory base address for rom memory on rom_ctrl in top assam.
   */
  parameter int unsigned TOP_ASSAM_ROM_CTRL_ROM_BASE_ADDR = 32'h8000;

  /**
   * Memory size for rom memory on rom_ctrl in top assam.
   */
  parameter int unsigned TOP_ASSAM_ROM_CTRL_ROM_SIZE_BYTES = 32'h8000;


  // Number of assam outgoing alerts
  parameter int unsigned NOutgoingAlertsAssam = 18;

  // Number of LPGs for outgoing alert group assam
  parameter int unsigned NOutgoingLpgsAssam = 8;

  // Enumeration of assam outgoing alert modules
  typedef enum int unsigned {
    TopAssamAlertPeripheralUart0 = 0,
    TopAssamAlertPeripheralSpiDevice = 1,
    TopAssamAlertPeripheralRvTimer = 2,
    TopAssamAlertPeripheralPwrmgrAon = 3,
    TopAssamAlertPeripheralRstmgrAon = 4,
    TopAssamAlertPeripheralClkmgrAon = 5,
    TopAssamAlertPeripheralPinmuxAon = 6,
    TopAssamAlertPeripheralRvPlic = 7,
    TopAssamAlertPeripheralAes = 8,
    TopAssamAlertPeripheralSramCtrlMain = 9,
    TopAssamAlertPeripheralRomCtrl = 10,
    TopAssamAlertPeripheralRvCoreIbex = 11,
    TopAssamOutgoingAlertAssamPeripheralCount
  } outgoing_alert_assam_peripheral_e;

  // Enumeration of assam outgoing alerts
  typedef enum int unsigned {
    TopAssamAlertIdUart0FatalFault = 0,
    TopAssamAlertIdSpiDeviceFatalFault = 1,
    TopAssamAlertIdRvTimerFatalFault = 2,
    TopAssamAlertIdPwrmgrAonFatalFault = 3,
    TopAssamAlertIdRstmgrAonFatalFault = 4,
    TopAssamAlertIdRstmgrAonFatalCnstyFault = 5,
    TopAssamAlertIdClkmgrAonRecovFault = 6,
    TopAssamAlertIdClkmgrAonFatalFault = 7,
    TopAssamAlertIdPinmuxAonFatalFault = 8,
    TopAssamAlertIdRvPlicFatalFault = 9,
    TopAssamAlertIdAesRecovCtrlUpdateErr = 10,
    TopAssamAlertIdAesFatalFault = 11,
    TopAssamAlertIdSramCtrlMainFatalError = 12,
    TopAssamAlertIdRomCtrlFatal = 13,
    TopAssamAlertIdRvCoreIbexFatalSwErr = 14,
    TopAssamAlertIdRvCoreIbexRecovSwErr = 15,
    TopAssamAlertIdRvCoreIbexFatalHwErr = 16,
    TopAssamAlertIdRvCoreIbexRecovHwErr = 17,
    TopAssamOutgoingAlertAssamIdCount
  } outgoing_alert_assam_id_e;

  // Enumeration of assam outgoing alerts AsyncOn configuration
  parameter logic [NOutgoingAlertsAssam-1:0] AsyncOnOutgoingAlertAssam = {
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1,
    1'b1
  };

  // Enumeration of interrupts
  typedef enum int unsigned {
    TopAssamPlicIrqIdNone = 0,
    TopAssamPlicIrqIdUart0TxWatermark = 1,
    TopAssamPlicIrqIdUart0RxWatermark = 2,
    TopAssamPlicIrqIdUart0TxDone = 3,
    TopAssamPlicIrqIdUart0RxOverflow = 4,
    TopAssamPlicIrqIdUart0RxFrameErr = 5,
    TopAssamPlicIrqIdUart0RxBreakErr = 6,
    TopAssamPlicIrqIdUart0RxTimeout = 7,
    TopAssamPlicIrqIdUart0RxParityErr = 8,
    TopAssamPlicIrqIdUart0TxEmpty = 9,
    TopAssamPlicIrqIdSpiDeviceUploadCmdfifoNotEmpty = 10,
    TopAssamPlicIrqIdSpiDeviceUploadPayloadNotEmpty = 11,
    TopAssamPlicIrqIdSpiDeviceUploadPayloadOverflow = 12,
    TopAssamPlicIrqIdSpiDeviceReadbufWatermark = 13,
    TopAssamPlicIrqIdSpiDeviceReadbufFlip = 14,
    TopAssamPlicIrqIdSpiDeviceTpmHeaderNotEmpty = 15,
    TopAssamPlicIrqIdSpiDeviceTpmRdfifoCmdEnd = 16,
    TopAssamPlicIrqIdSpiDeviceTpmRdfifoDrop = 17,
    TopAssamPlicIrqIdPwrmgrAonWakeup = 18,
    TopAssamPlicIrqIdCount
  } interrupt_rv_plic_id_e;


  // Enumeration of IO power domains.
  // Only used in ASIC target.
  typedef enum logic [0:0] {
    IoBankVcc = 0,
    IoBankCount = 1
  } pwr_dom_e;

  // Enumeration for MIO signals on the top-level.
  typedef enum int unsigned {
    MioInUart0Rx = 0,
    MioInCount = 1
  } mio_in_e;

  typedef enum {
    MioOutUart0Tx = 0,
    MioOutCount = 1
  } mio_out_e;

  // Enumeration for DIO signals, used on both the top and chip-levels.
  typedef enum int unsigned {
    DioSpiDeviceSd0 = 0,
    DioSpiDeviceSd1 = 1,
    DioSpiDeviceSd2 = 2,
    DioSpiDeviceSd3 = 3,
    DioSpiDeviceSck = 4,
    DioSpiDeviceCsb = 5,
    DioCount = 6
  } dio_e;

  // Enumeration for the types of pads.
  typedef enum {
    MioPad,
    DioPad
  } pad_type_e;

  // Raw MIO/DIO input array indices on chip-level.
  // TODO: Does not account for target specific stubbed/added pads.
  // Need to make a target-specific package for those.
  typedef enum int unsigned {
    MioPadIoa0 = 0,
    MioPadIoa1 = 1,
    MioPadIoa2 = 2,
    MioPadIoa3 = 3,
    MioPadCount
  } mio_pad_e;

  typedef enum int unsigned {
    DioPadPorN = 0,
    DioPadSpiDevD0 = 1,
    DioPadSpiDevD1 = 2,
    DioPadSpiDevD2 = 3,
    DioPadSpiDevD3 = 4,
    DioPadSpiDevClk = 5,
    DioPadSpiDevCsL = 6,
    DioPadCount
  } dio_pad_e;

  // List of peripheral instantiated in this chip.
  typedef enum {
    PeripheralAes,
    PeripheralAst,
    PeripheralClkmgrAon,
    PeripheralPinmuxAon,
    PeripheralPwrmgrAon,
    PeripheralRomCtrl,
    PeripheralRstmgrAon,
    PeripheralRvCoreIbex,
    PeripheralRvPlic,
    PeripheralRvTimer,
    PeripheralSpiDevice,
    PeripheralSramCtrlMain,
    PeripheralUart0,
    PeripheralCount
  } peripheral_e;

  // MMIO Region
  //
  parameter int unsigned TOP_ASSAM_MMIO_BASE_ADDR = 32'h40000000;
  parameter int unsigned TOP_ASSAM_MMIO_SIZE_BYTES = 32'h10000000;

  // TODO: Enumeration for PLIC Interrupt source peripheral.

// MACROs for AST analog simulation support
`ifdef ANALOGSIM
  `define INOUT_AI input ast_pkg::awire_t
  `define INOUT_AO output ast_pkg::awire_t
`else
  `define INOUT_AI inout
  `define INOUT_AO inout
`endif

endpackage

// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
// util/topgen.py -t hw/top_assam/data/top_assam.hjson \
//                -o hw/top_assam/

#ifndef OPENTITAN_HW_TOP_ASSAM_SW_AUTOGEN_TOP_ASSAM_H_
#define OPENTITAN_HW_TOP_ASSAM_SW_AUTOGEN_TOP_ASSAM_H_

/**
 * @file
 * @brief Top-specific Definitions
 *
 * This file contains preprocessor and type definitions for use within the
 * device C/C++ codebase.
 *
 * These definitions are for information that depends on the top-specific chip
 * configuration, which includes:
 * - Device Memory Information (for Peripherals and Memory)
 * - PLIC Interrupt ID Names and Source Mappings
 * - Pinmux Pin/Select Names
 * - Power Manager Wakeups
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Peripheral base address for uart0 in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_UART0_BASE_ADDR 0x40000000u

/**
 * Peripheral size for uart0 in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_UART0_BASE_ADDR and
 * `TOP_ASSAM_UART0_BASE_ADDR + TOP_ASSAM_UART0_SIZE_BYTES`.
 */
#define TOP_ASSAM_UART0_SIZE_BYTES 0x40u

/**
 * Peripheral base address for spi_device in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_SPI_DEVICE_BASE_ADDR 0x40050000u

/**
 * Peripheral size for spi_device in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_SPI_DEVICE_BASE_ADDR and
 * `TOP_ASSAM_SPI_DEVICE_BASE_ADDR + TOP_ASSAM_SPI_DEVICE_SIZE_BYTES`.
 */
#define TOP_ASSAM_SPI_DEVICE_SIZE_BYTES 0x2000u

/**
 * Peripheral base address for rv_timer in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_RV_TIMER_BASE_ADDR 0x40100000u

/**
 * Peripheral size for rv_timer in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_RV_TIMER_BASE_ADDR and
 * `TOP_ASSAM_RV_TIMER_BASE_ADDR + TOP_ASSAM_RV_TIMER_SIZE_BYTES`.
 */
#define TOP_ASSAM_RV_TIMER_SIZE_BYTES 0x200u

/**
 * Peripheral base address for pwrmgr_aon in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_PWRMGR_AON_BASE_ADDR 0x40400000u

/**
 * Peripheral size for pwrmgr_aon in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_PWRMGR_AON_BASE_ADDR and
 * `TOP_ASSAM_PWRMGR_AON_BASE_ADDR + TOP_ASSAM_PWRMGR_AON_SIZE_BYTES`.
 */
#define TOP_ASSAM_PWRMGR_AON_SIZE_BYTES 0x80u

/**
 * Peripheral base address for rstmgr_aon in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_RSTMGR_AON_BASE_ADDR 0x40410000u

/**
 * Peripheral size for rstmgr_aon in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_RSTMGR_AON_BASE_ADDR and
 * `TOP_ASSAM_RSTMGR_AON_BASE_ADDR + TOP_ASSAM_RSTMGR_AON_SIZE_BYTES`.
 */
#define TOP_ASSAM_RSTMGR_AON_SIZE_BYTES 0x40u

/**
 * Peripheral base address for clkmgr_aon in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_CLKMGR_AON_BASE_ADDR 0x40420000u

/**
 * Peripheral size for clkmgr_aon in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_CLKMGR_AON_BASE_ADDR and
 * `TOP_ASSAM_CLKMGR_AON_BASE_ADDR + TOP_ASSAM_CLKMGR_AON_SIZE_BYTES`.
 */
#define TOP_ASSAM_CLKMGR_AON_SIZE_BYTES 0x40u

/**
 * Peripheral base address for pinmux_aon in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_PINMUX_AON_BASE_ADDR 0x40460000u

/**
 * Peripheral size for pinmux_aon in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_PINMUX_AON_BASE_ADDR and
 * `TOP_ASSAM_PINMUX_AON_BASE_ADDR + TOP_ASSAM_PINMUX_AON_SIZE_BYTES`.
 */
#define TOP_ASSAM_PINMUX_AON_SIZE_BYTES 0x200u

/**
 * Peripheral base address for rv_plic in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_RV_PLIC_BASE_ADDR 0x48000000u

/**
 * Peripheral size for rv_plic in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_RV_PLIC_BASE_ADDR and
 * `TOP_ASSAM_RV_PLIC_BASE_ADDR + TOP_ASSAM_RV_PLIC_SIZE_BYTES`.
 */
#define TOP_ASSAM_RV_PLIC_SIZE_BYTES 0x8000000u

/**
 * Peripheral base address for aes in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_AES_BASE_ADDR 0x41100000u

/**
 * Peripheral size for aes in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_AES_BASE_ADDR and
 * `TOP_ASSAM_AES_BASE_ADDR + TOP_ASSAM_AES_SIZE_BYTES`.
 */
#define TOP_ASSAM_AES_SIZE_BYTES 0x100u

/**
 * Peripheral base address for ast in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_AST_BASE_ADDR 0x40480000u

/**
 * Peripheral size for ast in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_AST_BASE_ADDR and
 * `TOP_ASSAM_AST_BASE_ADDR + TOP_ASSAM_AST_SIZE_BYTES`.
 */
#define TOP_ASSAM_AST_SIZE_BYTES 0x400u

/**
 * Peripheral base address for regs device on sram_ctrl_main in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_SRAM_CTRL_MAIN_REGS_BASE_ADDR 0x411C0000u

/**
 * Peripheral size for regs device on sram_ctrl_main in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_SRAM_CTRL_MAIN_REGS_BASE_ADDR and
 * `TOP_ASSAM_SRAM_CTRL_MAIN_REGS_BASE_ADDR + TOP_ASSAM_SRAM_CTRL_MAIN_REGS_SIZE_BYTES`.
 */
#define TOP_ASSAM_SRAM_CTRL_MAIN_REGS_SIZE_BYTES 0x40u

/**
 * Peripheral base address for regs device on rom_ctrl in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_ROM_CTRL_REGS_BASE_ADDR 0x411E0000u

/**
 * Peripheral size for regs device on rom_ctrl in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_ROM_CTRL_REGS_BASE_ADDR and
 * `TOP_ASSAM_ROM_CTRL_REGS_BASE_ADDR + TOP_ASSAM_ROM_CTRL_REGS_SIZE_BYTES`.
 */
#define TOP_ASSAM_ROM_CTRL_REGS_SIZE_BYTES 0x80u

/**
 * Peripheral base address for cfg device on rv_core_ibex in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_RV_CORE_IBEX_CFG_BASE_ADDR 0x411F0000u

/**
 * Peripheral size for cfg device on rv_core_ibex in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_RV_CORE_IBEX_CFG_BASE_ADDR and
 * `TOP_ASSAM_RV_CORE_IBEX_CFG_BASE_ADDR + TOP_ASSAM_RV_CORE_IBEX_CFG_SIZE_BYTES`.
 */
#define TOP_ASSAM_RV_CORE_IBEX_CFG_SIZE_BYTES 0x100u


/**
 * Memory base address for ram memory on sram_ctrl_main in top assam.
 */
#define TOP_ASSAM_SRAM_CTRL_MAIN_RAM_BASE_ADDR 0x10000000u

/**
 * Memory size for ram memory on sram_ctrl_main in top assam.
 */
#define TOP_ASSAM_SRAM_CTRL_MAIN_RAM_SIZE_BYTES 0x20000u

/**
 * Memory base address for rom memory on rom_ctrl in top assam.
 */
#define TOP_ASSAM_ROM_CTRL_ROM_BASE_ADDR 0x8000u

/**
 * Memory size for rom memory on rom_ctrl in top assam.
 */
#define TOP_ASSAM_ROM_CTRL_ROM_SIZE_BYTES 0x8000u


/**
 * PLIC Interrupt Source Peripheral.
 *
 * Enumeration used to determine which peripheral asserted the corresponding
 * interrupt.
 */
typedef enum top_assam_plic_peripheral {
  kTopAssamPlicPeripheralUnknown = 0, /**< Unknown Peripheral */
  kTopAssamPlicPeripheralUart0 = 1, /**< uart0 */
  kTopAssamPlicPeripheralSpiDevice = 2, /**< spi_device */
  kTopAssamPlicPeripheralPwrmgrAon = 3, /**< pwrmgr_aon */
  kTopAssamPlicPeripheralLast = 3, /**< \internal Final PLIC peripheral */
} top_assam_plic_peripheral_t;

/**
 * PLIC Interrupt Source.
 *
 * Enumeration of all PLIC interrupt sources. The interrupt sources belonging to
 * the same peripheral are guaranteed to be consecutive.
 */
typedef enum top_assam_plic_irq_id {
  kTopAssamPlicIrqIdNone = 0, /**< No Interrupt */
  kTopAssamPlicIrqIdUart0TxWatermark = 1, /**< uart0_tx_watermark */
  kTopAssamPlicIrqIdUart0RxWatermark = 2, /**< uart0_rx_watermark */
  kTopAssamPlicIrqIdUart0TxDone = 3, /**< uart0_tx_done */
  kTopAssamPlicIrqIdUart0RxOverflow = 4, /**< uart0_rx_overflow */
  kTopAssamPlicIrqIdUart0RxFrameErr = 5, /**< uart0_rx_frame_err */
  kTopAssamPlicIrqIdUart0RxBreakErr = 6, /**< uart0_rx_break_err */
  kTopAssamPlicIrqIdUart0RxTimeout = 7, /**< uart0_rx_timeout */
  kTopAssamPlicIrqIdUart0RxParityErr = 8, /**< uart0_rx_parity_err */
  kTopAssamPlicIrqIdUart0TxEmpty = 9, /**< uart0_tx_empty */
  kTopAssamPlicIrqIdSpiDeviceUploadCmdfifoNotEmpty = 10, /**< spi_device_upload_cmdfifo_not_empty */
  kTopAssamPlicIrqIdSpiDeviceUploadPayloadNotEmpty = 11, /**< spi_device_upload_payload_not_empty */
  kTopAssamPlicIrqIdSpiDeviceUploadPayloadOverflow = 12, /**< spi_device_upload_payload_overflow */
  kTopAssamPlicIrqIdSpiDeviceReadbufWatermark = 13, /**< spi_device_readbuf_watermark */
  kTopAssamPlicIrqIdSpiDeviceReadbufFlip = 14, /**< spi_device_readbuf_flip */
  kTopAssamPlicIrqIdSpiDeviceTpmHeaderNotEmpty = 15, /**< spi_device_tpm_header_not_empty */
  kTopAssamPlicIrqIdSpiDeviceTpmRdfifoCmdEnd = 16, /**< spi_device_tpm_rdfifo_cmd_end */
  kTopAssamPlicIrqIdSpiDeviceTpmRdfifoDrop = 17, /**< spi_device_tpm_rdfifo_drop */
  kTopAssamPlicIrqIdPwrmgrAonWakeup = 18, /**< pwrmgr_aon_wakeup */
  kTopAssamPlicIrqIdLast = 18, /**< \internal The Last Valid Interrupt ID. */
} top_assam_plic_irq_id_t;

/**
 * PLIC Interrupt Source to Peripheral Map
 *
 * This array is a mapping from `top_assam_plic_irq_id_t` to
 * `top_assam_plic_peripheral_t`.
 */
extern const top_assam_plic_peripheral_t
    top_assam_plic_interrupt_for_peripheral[19];

/**
 * PLIC Interrupt Target.
 *
 * Enumeration used to determine which set of IE, CC, threshold registers to
 * access for a given interrupt target.
 */
typedef enum top_assam_plic_target {
  kTopAssamPlicTargetIbex0 = 0, /**< Ibex Core 0 */
  kTopAssamPlicTargetLast = 0, /**< \internal Final PLIC target */
} top_assam_plic_target_t;


#define PINMUX_MIO_PERIPH_INSEL_IDX_OFFSET 2

// PERIPH_INSEL ranges from 0 to NUM_MIO_PADS + 2 -1}
//  0 and 1 are tied to value 0 and 1
#define NUM_MIO_PADS 4
#define NUM_DIO_PADS 6

#define PINMUX_PERIPH_OUTSEL_IDX_OFFSET 3

/**
 * Pinmux Peripheral Input.
 */
typedef enum top_assam_pinmux_peripheral_in {
  kTopAssamPinmuxPeripheralInUart0Rx = 0, /**< Peripheral Input 0 */
  kTopAssamPinmuxPeripheralInLast = 0, /**< \internal Last valid peripheral input */
} top_assam_pinmux_peripheral_in_t;

/**
 * Pinmux MIO Input Selector.
 */
typedef enum top_assam_pinmux_insel {
  kTopAssamPinmuxInselConstantZero = 0, /**< Tie constantly to zero */
  kTopAssamPinmuxInselConstantOne = 1, /**< Tie constantly to one */
  kTopAssamPinmuxInselIoa0 = 2, /**< MIO Pad 0 */
  kTopAssamPinmuxInselIoa1 = 3, /**< MIO Pad 1 */
  kTopAssamPinmuxInselIoa2 = 4, /**< MIO Pad 2 */
  kTopAssamPinmuxInselIoa3 = 5, /**< MIO Pad 3 */
  kTopAssamPinmuxInselLast = 5, /**< \internal Last valid insel value */
} top_assam_pinmux_insel_t;

/**
 * Pinmux MIO Output.
 */
typedef enum top_assam_pinmux_mio_out {
  kTopAssamPinmuxMioOutIoa0 = 0, /**< MIO Pad 0 */
  kTopAssamPinmuxMioOutIoa1 = 1, /**< MIO Pad 1 */
  kTopAssamPinmuxMioOutIoa2 = 2, /**< MIO Pad 2 */
  kTopAssamPinmuxMioOutIoa3 = 3, /**< MIO Pad 3 */
  kTopAssamPinmuxMioOutLast = 3, /**< \internal Last valid mio output */
} top_assam_pinmux_mio_out_t;

/**
 * Pinmux Peripheral Output Selector.
 */
typedef enum top_assam_pinmux_outsel {
  kTopAssamPinmuxOutselConstantZero = 0, /**< Tie constantly to zero */
  kTopAssamPinmuxOutselConstantOne = 1, /**< Tie constantly to one */
  kTopAssamPinmuxOutselConstantHighZ = 2, /**< Tie constantly to high-Z */
  kTopAssamPinmuxOutselUart0Tx = 3, /**< Peripheral Output 0 */
  kTopAssamPinmuxOutselLast = 3, /**< \internal Last valid outsel value */
} top_assam_pinmux_outsel_t;

/**
 * Dedicated Pad Selects
 */
typedef enum top_assam_direct_pads {
  kTopAssamDirectPadsSpiDeviceSd0 = 0, /**<  */
  kTopAssamDirectPadsSpiDeviceSd1 = 1, /**<  */
  kTopAssamDirectPadsSpiDeviceSd2 = 2, /**<  */
  kTopAssamDirectPadsSpiDeviceSd3 = 3, /**<  */
  kTopAssamDirectPadsSpiDeviceSck = 4, /**<  */
  kTopAssamDirectPadsSpiDeviceCsb = 5, /**<  */
  kTopAssamDirectPadsLast = 5, /**< \internal Last valid direct pad */
} top_assam_direct_pads_t;

/**
 * Muxed Pad Selects
 */
typedef enum top_assam_muxed_pads {
  kTopAssamMuxedPadsIoa0 = 0, /**<  */
  kTopAssamMuxedPadsIoa1 = 1, /**<  */
  kTopAssamMuxedPadsIoa2 = 2, /**<  */
  kTopAssamMuxedPadsIoa3 = 3, /**<  */
  kTopAssamMuxedPadsLast = 3, /**< \internal Last valid muxed pad */
} top_assam_muxed_pads_t;

/**
 * Power Manager Wakeup Signals
 */
typedef enum top_assam_power_manager_wake_ups {
  kTopAssamPowerManagerWakeUpsPinmuxAonPinWkupReq = 0, /**<  */
  kTopAssamPowerManagerWakeUpsLast = 0, /**< \internal Last valid pwrmgr wakeup signal */
} top_assam_power_manager_wake_ups_t;

/**
 * Reset Manager Software Controlled Resets
 */
typedef enum top_assam_reset_manager_sw_resets {
  kTopAssamResetManagerSwResetsSpiDevice = 0, /**<  */
  kTopAssamResetManagerSwResetsLast = 0, /**< \internal Last valid rstmgr software reset request */
} top_assam_reset_manager_sw_resets_t;

/**
 * Power Manager Reset Request Signals
 */
typedef enum top_assam_power_manager_reset_requests {
  kTopAssamPowerManagerResetRequestsNone = 0,  // Assam has no reset requests
} top_assam_power_manager_reset_requests_t;

/**
 * Clock Manager Software-Controlled ("Gated") Clocks.
 *
 * The Software has full control over these clocks.
 */
typedef enum top_assam_gateable_clocks {
  kTopAssamGateableClocksIoDiv4Peri = 0, /**< Clock clk_io_div4_peri in group peri */
  kTopAssamGateableClocksIoDiv2Peri = 1, /**< Clock clk_io_div2_peri in group peri */
  kTopAssamGateableClocksLast = 1, /**< \internal Last Valid Gateable Clock */
} top_assam_gateable_clocks_t;

/**
 * Clock Manager Software-Hinted Clocks.
 *
 * The Software has partial control over these clocks. It can ask them to stop,
 * but the clock manager is in control of whether the clock actually is stopped.
 */
typedef enum top_assam_hintable_clocks {
  kTopAssamHintableClocksMainAes = 0, /**< Clock clk_main_aes in group trans */
  kTopAssamHintableClocksLast = 0, /**< \internal Last Valid Hintable Clock */
} top_assam_hintable_clocks_t;

/**
 * MMIO Region
 *
 * MMIO region excludes any memory that is separate from the module
 * configuration space, i.e. ROM, main SRAM are excluded.
 */
#define TOP_ASSAM_MMIO_BASE_ADDR 0x40000000u
#define TOP_ASSAM_MMIO_SIZE_BYTES 0x10000000u

// Header Extern Guard
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // OPENTITAN_HW_TOP_ASSAM_SW_AUTOGEN_TOP_ASSAM_H_

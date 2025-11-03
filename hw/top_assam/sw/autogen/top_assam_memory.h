// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
// util/topgen.py -t hw/top_assam/data/top_assam.hjson \
//                -o hw/top_assam/

#ifndef OPENTITAN_HW_TOP_ASSAM_SW_AUTOGEN_TOP_ASSAM_MEMORY_H_
#define OPENTITAN_HW_TOP_ASSAM_SW_AUTOGEN_TOP_ASSAM_MEMORY_H_

/**
 * @file
 * @brief Assembler-only Top-Specific Definitions.
 *
 * This file contains preprocessor definitions for use within assembly code.
 *
 * These are not shared with C/C++ code because these are only allowed to be
 * preprocessor definitions, no data or type declarations are allowed. The
 * assembler is also stricter about literals (not allowing suffixes for
 * signed/unsigned which are sensible to use for unsigned values in C/C++).
 */

// Include guard for assembler
#ifdef __ASSEMBLER__

/**
 * Memory base for ram memory on sram_ctrl_main in top assam.
 */
#define TOP_ASSAM_SRAM_CTRL_MAIN_RAM_BASE_ADDR 0x10000000

/**
 * Memory size for ram memory on sram_ctrl_main in top assam.
 */
#define TOP_ASSAM_SRAM_CTRL_MAIN_RAM_SIZE_BYTES 0x20000

/**
 * Memory base for rom memory on rom_ctrl in top assam.
 */
#define TOP_ASSAM_ROM_CTRL_ROM_BASE_ADDR 0x8000

/**
 * Memory size for rom memory on rom_ctrl in top assam.
 */
#define TOP_ASSAM_ROM_CTRL_ROM_SIZE_BYTES 0x8000


/**
 * Peripheral base address for uart0 in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_UART0_BASE_ADDR 0x40000000

/**
 * Peripheral size for uart0 in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_UART0_BASE_ADDR and
 * `TOP_ASSAM_UART0_BASE_ADDR + TOP_ASSAM_UART0_SIZE_BYTES`.
 */
#define TOP_ASSAM_UART0_SIZE_BYTES 0x40
/**
 * Peripheral base address for spi_device in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_SPI_DEVICE_BASE_ADDR 0x40050000

/**
 * Peripheral size for spi_device in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_SPI_DEVICE_BASE_ADDR and
 * `TOP_ASSAM_SPI_DEVICE_BASE_ADDR + TOP_ASSAM_SPI_DEVICE_SIZE_BYTES`.
 */
#define TOP_ASSAM_SPI_DEVICE_SIZE_BYTES 0x2000
/**
 * Peripheral base address for rv_timer in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_RV_TIMER_BASE_ADDR 0x40100000

/**
 * Peripheral size for rv_timer in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_RV_TIMER_BASE_ADDR and
 * `TOP_ASSAM_RV_TIMER_BASE_ADDR + TOP_ASSAM_RV_TIMER_SIZE_BYTES`.
 */
#define TOP_ASSAM_RV_TIMER_SIZE_BYTES 0x200
/**
 * Peripheral base address for pwrmgr_aon in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_PWRMGR_AON_BASE_ADDR 0x40400000

/**
 * Peripheral size for pwrmgr_aon in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_PWRMGR_AON_BASE_ADDR and
 * `TOP_ASSAM_PWRMGR_AON_BASE_ADDR + TOP_ASSAM_PWRMGR_AON_SIZE_BYTES`.
 */
#define TOP_ASSAM_PWRMGR_AON_SIZE_BYTES 0x80
/**
 * Peripheral base address for rstmgr_aon in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_RSTMGR_AON_BASE_ADDR 0x40410000

/**
 * Peripheral size for rstmgr_aon in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_RSTMGR_AON_BASE_ADDR and
 * `TOP_ASSAM_RSTMGR_AON_BASE_ADDR + TOP_ASSAM_RSTMGR_AON_SIZE_BYTES`.
 */
#define TOP_ASSAM_RSTMGR_AON_SIZE_BYTES 0x40
/**
 * Peripheral base address for clkmgr_aon in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_CLKMGR_AON_BASE_ADDR 0x40420000

/**
 * Peripheral size for clkmgr_aon in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_CLKMGR_AON_BASE_ADDR and
 * `TOP_ASSAM_CLKMGR_AON_BASE_ADDR + TOP_ASSAM_CLKMGR_AON_SIZE_BYTES`.
 */
#define TOP_ASSAM_CLKMGR_AON_SIZE_BYTES 0x40
/**
 * Peripheral base address for pinmux_aon in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_PINMUX_AON_BASE_ADDR 0x40460000

/**
 * Peripheral size for pinmux_aon in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_PINMUX_AON_BASE_ADDR and
 * `TOP_ASSAM_PINMUX_AON_BASE_ADDR + TOP_ASSAM_PINMUX_AON_SIZE_BYTES`.
 */
#define TOP_ASSAM_PINMUX_AON_SIZE_BYTES 0x200
/**
 * Peripheral base address for rv_plic in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_RV_PLIC_BASE_ADDR 0x48000000

/**
 * Peripheral size for rv_plic in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_RV_PLIC_BASE_ADDR and
 * `TOP_ASSAM_RV_PLIC_BASE_ADDR + TOP_ASSAM_RV_PLIC_SIZE_BYTES`.
 */
#define TOP_ASSAM_RV_PLIC_SIZE_BYTES 0x8000000
/**
 * Peripheral base address for aes in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_AES_BASE_ADDR 0x41100000

/**
 * Peripheral size for aes in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_AES_BASE_ADDR and
 * `TOP_ASSAM_AES_BASE_ADDR + TOP_ASSAM_AES_SIZE_BYTES`.
 */
#define TOP_ASSAM_AES_SIZE_BYTES 0x100
/**
 * Peripheral base address for ast in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_AST_BASE_ADDR 0x40480000

/**
 * Peripheral size for ast in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_AST_BASE_ADDR and
 * `TOP_ASSAM_AST_BASE_ADDR + TOP_ASSAM_AST_SIZE_BYTES`.
 */
#define TOP_ASSAM_AST_SIZE_BYTES 0x400
/**
 * Peripheral base address for regs device on sram_ctrl_main in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_SRAM_CTRL_MAIN_REGS_BASE_ADDR 0x411C0000

/**
 * Peripheral size for regs device on sram_ctrl_main in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_SRAM_CTRL_MAIN_REGS_BASE_ADDR and
 * `TOP_ASSAM_SRAM_CTRL_MAIN_REGS_BASE_ADDR + TOP_ASSAM_SRAM_CTRL_MAIN_REGS_SIZE_BYTES`.
 */
#define TOP_ASSAM_SRAM_CTRL_MAIN_REGS_SIZE_BYTES 0x40
/**
 * Peripheral base address for regs device on rom_ctrl in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_ROM_CTRL_REGS_BASE_ADDR 0x411E0000

/**
 * Peripheral size for regs device on rom_ctrl in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_ROM_CTRL_REGS_BASE_ADDR and
 * `TOP_ASSAM_ROM_CTRL_REGS_BASE_ADDR + TOP_ASSAM_ROM_CTRL_REGS_SIZE_BYTES`.
 */
#define TOP_ASSAM_ROM_CTRL_REGS_SIZE_BYTES 0x80
/**
 * Peripheral base address for cfg device on rv_core_ibex in top assam.
 *
 * This should be used with #mmio_region_from_addr to access the memory-mapped
 * registers associated with the peripheral (usually via a DIF).
 */
#define TOP_ASSAM_RV_CORE_IBEX_CFG_BASE_ADDR 0x411F0000

/**
 * Peripheral size for cfg device on rv_core_ibex in top assam.
 *
 * This is the size (in bytes) of the peripheral's reserved memory area. All
 * memory-mapped registers associated with this peripheral should have an
 * address between #TOP_ASSAM_RV_CORE_IBEX_CFG_BASE_ADDR and
 * `TOP_ASSAM_RV_CORE_IBEX_CFG_BASE_ADDR + TOP_ASSAM_RV_CORE_IBEX_CFG_SIZE_BYTES`.
 */
#define TOP_ASSAM_RV_CORE_IBEX_CFG_SIZE_BYTES 0x100

/**
 * MMIO Region
 *
 * MMIO region excludes any memory that is separate from the module
 * configuration space, i.e. ROM, main SRAM are excluded.
 */
#define TOP_ASSAM_MMIO_BASE_ADDR 0x40000000
#define TOP_ASSAM_MMIO_SIZE_BYTES 0x10000000

#endif  // __ASSEMBLER__

#endif  // OPENTITAN_HW_TOP_ASSAM_SW_AUTOGEN_TOP_ASSAM_MEMORY_H_

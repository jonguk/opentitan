/* Copyright lowRISC contributors (OpenTitan project). */
/* Licensed under the Apache License, Version 2.0, see LICENSE for details. */
/* SPDX-License-Identifier: Apache-2.0 */

/**
 * Linker script for Assam UART Hello World
 *
 * Assam runs from RAM (no flash), similar to EnglishBreakfast.
 */

OUTPUT_ARCH(riscv)

/**
 * Indicate that there are no dynamic libraries, whatsoever.
 */
__DYNAMIC = 0;

INCLUDE hw/top_assam/sw/autogen/top_assam_memory.ld

/**
 * The boot address, which indicates the location of the initial interrupt
 * vector.
 */
_boot_address = ORIGIN(ram_main);

/* DV log offset (not used for Assam, but define for compatibility) */
_dv_log_offset = 0x0;

/**
 * Physical Memory Protection (PMP) regions
 */
ASSERT((ORIGIN(ram_main) % 4) == 0, "RAM origin must be 4-byte aligned");

/**
 * The _stack_end symbol defines the initial stack pointer value.
 */
_stack_start = ORIGIN(ram_main) + LENGTH(ram_main);
_stack_end = _stack_start - 0x2000;  /* 8KB stack */
_stack_size = _stack_start - _stack_end;

SECTIONS {
  /**
   * Ibex interrupt vector. See irq_vector.S for more information.
   *
   * This has to be set up at the boot address, so that execution jumps to the
   * reset handler correctly.
   */
  .vectors _boot_address : ALIGN(256) {
    KEEP(*(.vectors))
  } > ram_main

  /**
   * C runtime (CRT) section, containing program initialization code.
   */
  .crt : ALIGN(4) {
    KEEP(*(.crt))
  } > ram_main

  /**
   * Standard text section, containing program code.
   */
  .text : ALIGN(4) {
    *(.text)
    *(.text.*)
  } > ram_main

  /**
   * Read-only data section, containing all large compile-time constants, like
   * strings.
   */
  .rodata : ALIGN(4) {
    /* Small read-only data comes before regular read-only data for the same
     * reasons as in the data section */
    *(.srodata)
    *(.srodata.*)
    *(.rodata)
    *(.rodata.*)
  } > ram_main

  /**
   * "Intitial data" section, the initial values of the mutable data section
   * initialized at runtime.
   */
  .idata : ALIGN(4) {
    _data_init_start = .;
  } > ram_main

  /**
   * Standard mutable data section at the end of RAM. This will be initialized
   * from the .idata section at runtime by the CRT.
   */
  .data : ALIGN(4) {
    _data_start = .;
    __global_pointer$ = . + 2048;

    /* Small data should come before larger data. This helps to ensure small
     * globals are within 2048 bytes of the value of `gp`, making their accesses
     * hopefully only take one instruction. */
    *(.sdata)
    *(.sdata.*)

    /* Other data will likely need multiple instructions to load, so we're less
     * concerned about address materialisation taking more than one instruction.
     */
    *(.data)
    *(.data.*)

    /* Ensure section end is word-aligned. */
    . = ALIGN(4);
    _data_end = .;
  } > ram_main

  /* Link-time constants for data section */
  _data_init_end = LOADADDR(.data) + SIZEOF(.data);

  /**
   * Standard BSS section. This will be zeroed at runtime by the CRT.
   */
  .bss : ALIGN(4) {
    _bss_start = .;

    /* Small BSS comes before regular BSS for the same reasons as in the data
     * section */
    *(.sbss)
    *(.sbss.*)
    *(.bss)
    *(.bss.*)

    /* Ensure section end is word-aligned. */
    . = ALIGN(4);
    _bss_end = .;
  } > ram_main

  INCLUDE sw/device/info_sections.ld
}

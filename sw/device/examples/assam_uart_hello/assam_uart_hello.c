// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/lib/dif/dif_uart.h"
#include "sw/device/lib/runtime/hart.h"
#include "sw/device/lib/runtime/print.h"

#include "hw/top_assam/sw/autogen/top_assam.h"

/**
 * Minimal UART Hello World for Assam SoC
 *
 * This is a bare-metal program that prints to UART without using
 * the OpenTitan Test Framework (OTTF) to avoid lifecycle controller
 * dependencies that Assam doesn't have.
 */

static dif_uart_t uart0;

static void uart_init(void) {
  dif_uart_config_t config = {
    .baudrate = 115200,
    .clk_freq_hz = 24000000,  // io_div4 clock
    .parity_enable = kDifToggleDisabled,
    .parity = kDifUartParityEven,
    .tx_enable = kDifToggleEnabled,
    .rx_enable = kDifToggleDisabled,
  };

  (void)dif_uart_init(mmio_region_from_addr(TOP_ASSAM_UART0_BASE_ADDR), &uart0);
  (void)dif_uart_configure(&uart0, config);
}

static void uart_send_str(const char *str) {
  for (const char *p = str; *p; p++) {
    (void)dif_uart_byte_send_polled(&uart0, *p);
  }
}

int main(void) {
  uart_init();

  uart_send_str("\r\n");
  uart_send_str("====================================\r\n");
  uart_send_str("   Assam SoC - UART Hello World\r\n");
  uart_send_str("====================================\r\n");
  uart_send_str("\r\n");
  uart_send_str("Hello from Assam!\r\n");
  uart_send_str("\r\n");
  uart_send_str("This is a minimal FPGA SoC with:\r\n");
  uart_send_str("  - RISC-V Ibex CPU\r\n");
  uart_send_str("  - UART0 (you are here!)\r\n");
  uart_send_str("  - SPI Device\r\n");
  uart_send_str("  - AES Accelerator\r\n");
  uart_send_str("  - Power/Reset/Clock Managers\r\n");
  uart_send_str("\r\n");
  uart_send_str("UART Test: SUCCESS\r\n");
  uart_send_str("\r\n");

  // Infinite loop - simulation will timeout
  while (1) {
    asm volatile("wfi");
  }

  return 0;
}

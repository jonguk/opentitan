// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/lib/dif/dif_uart.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/testing/test_framework/check.h"
#include "sw/device/lib/testing/test_framework/ottf_main.h"

#include "hw/top_assam/sw/autogen/top_assam.h"

OTTF_DEFINE_TEST_CONFIG();

/**
 * Simple UART test for Assam SoC.
 *
 * This test initializes UART and sends a test message to verify
 * basic UART TX functionality on the Assam SoC.
 */

static dif_uart_t uart0;

static void uart_init(void) {
    CHECK_DIF_OK(dif_uart_init(
        mmio_region_from_addr(TOP_ASSAM_UART0_BASE_ADDR), &uart0));

    // Configure UART: 115200 baud @ 24MHz clock
    // NCO = (115200 * 2^20) / 24000000 = 5033
    CHECK_DIF_OK(dif_uart_configure(
        &uart0,
        (dif_uart_config_t){
            .baudrate = 115200,
            .clk_freq_hz = 24000000,
            .parity_enable = kDifToggleDisabled,
            .parity = kDifUartParityEven,
            .tx_enable = kDifToggleEnabled,
            .rx_enable = kDifToggleDisabled,
        }));
}

static void uart_send_str(const char *str) {
    for (const char *p = str; *p; p++) {
        CHECK_DIF_OK(dif_uart_byte_send_polled(&uart0, *p));
    }
}

bool test_main(void) {
    uart_init();

    // Send test message
    uart_send_str("\r\n");
    uart_send_str("====================================\r\n");
    uart_send_str("Assam SoC UART Test\r\n");
    uart_send_str("====================================\r\n");
    uart_send_str("\r\n");
    uart_send_str("UART Initialization: OK\r\n");
    uart_send_str("UART Configuration:\r\n");
    uart_send_str("  Baud Rate: 115200\r\n");
    uart_send_str("  Clock Frequency: 24 MHz\r\n");
    uart_send_str("  Parity: Disabled\r\n");
    uart_send_str("\r\n");
    uart_send_str("Test Status: SUCCESS\r\n");
    uart_send_str("\r\n");

    return true;
}

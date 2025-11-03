// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
// clang-format off
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
// util/topgen.py -t hw/top_assam/data/top_assam.hjson \
//                -o hw/top_assam/
#include <limits.h>

// This test should avoid otp_ctrl interrupts in rom_ext, since the rom
// extension configures CSR accesses to OTP and AST to become illegal.
//
// This test is getting too big so we need to split it up. To do so,
// each peripheral is given an ID (according to their alphabetical order)
// and we define TEST_MIN_IRQ_PERIPHERAL and TEST_MAX_IRQ_PERIPHERAL to
// choose which ones are being tested.

#ifndef TEST_MIN_IRQ_PERIPHERAL
#define TEST_MIN_IRQ_PERIPHERAL 0
#endif

#ifndef TEST_MAX_IRQ_PERIPHERAL
#define TEST_MAX_IRQ_PERIPHERAL 3
#endif

#include "sw/device/lib/arch/boot_stage.h"
#include "sw/device/lib/base/csr.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/dif/autogen/dif_pwrmgr_autogen.h"
#include "sw/device/lib/dif/autogen/dif_rv_plic_autogen.h"
#include "sw/device/lib/dif/autogen/dif_spi_device_autogen.h"
#include "sw/device/lib/dif/autogen/dif_uart_autogen.h"
#include "sw/device/lib/runtime/ibex.h"
#include "sw/device/lib/runtime/irq.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/testing/rv_plic_testutils.h"
#include "sw/device/lib/testing/test_framework/check.h"
#include "sw/device/lib/testing/test_framework/ottf_main.h"
#include "sw/device/lib/testing/test_framework/status.h"

#include "hw/top_assam/sw/autogen/top_assam.h"

#if TEST_MIN_IRQ_PERIPHERAL <= 0 && 0 < TEST_MAX_IRQ_PERIPHERAL
static dif_pwrmgr_t pwrmgr_aon;
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 1 && 1 < TEST_MAX_IRQ_PERIPHERAL
static dif_spi_device_t spi_device;
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 2 && 2 < TEST_MAX_IRQ_PERIPHERAL
static dif_uart_t uart0;
#endif

static dif_rv_plic_t plic;
static const top_assam_plic_target_t kHart = kTopAssamPlicTargetIbex0;

/**
 * Flag indicating which peripheral is under test.
 *
 * Declared volatile because it is referenced in the main program flow as well
 * as the ISR.
 */
static volatile top_assam_plic_peripheral_t peripheral_expected;

/**
 * Flags indicating the IRQ expected to have triggered and serviced within the
 * peripheral.
 *
 * Declared volatile because it is referenced in the main program flow as well
 * as the ISR.
 */

#if TEST_MIN_IRQ_PERIPHERAL <= 0 && 0 < TEST_MAX_IRQ_PERIPHERAL
static volatile dif_pwrmgr_irq_t pwrmgr_irq_expected;
static volatile dif_pwrmgr_irq_t pwrmgr_irq_serviced;
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 1 && 1 < TEST_MAX_IRQ_PERIPHERAL
static volatile dif_spi_device_irq_t spi_device_irq_expected;
static volatile dif_spi_device_irq_t spi_device_irq_serviced;
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 2 && 2 < TEST_MAX_IRQ_PERIPHERAL
static volatile dif_uart_irq_t uart_irq_expected;
static volatile dif_uart_irq_t uart_irq_serviced;
#endif

/**
 * Provides external IRQ handling for this test.
 *
 * This function overrides the default OTTF external ISR.
 *
 * For each IRQ, it performs the following:
 * 1. Claims the IRQ fired (finds PLIC IRQ index).
 * 2. Checks that the index belongs to the expected peripheral.
 * 3. Checks that the correct and the only IRQ from the expected peripheral
 *    triggered.
 * 4. Clears the IRQ at the peripheral.
 * 5. Completes the IRQ service at PLIC.
 */
void ottf_external_isr(uint32_t *exc_info) {
  dif_rv_plic_irq_id_t plic_irq_id;
  CHECK_DIF_OK(dif_rv_plic_irq_claim(&plic, kHart, &plic_irq_id));

  top_assam_plic_peripheral_t peripheral = (top_assam_plic_peripheral_t)
      top_assam_plic_interrupt_for_peripheral[plic_irq_id];
  CHECK(peripheral == peripheral_expected,
        "Interrupt from incorrect peripheral: exp = %d, obs = %d",
        peripheral_expected, peripheral);

  switch (peripheral) {
#if TEST_MIN_IRQ_PERIPHERAL <= 0 && 0 < TEST_MAX_IRQ_PERIPHERAL
    case kTopAssamPlicPeripheralPwrmgrAon: {
      dif_pwrmgr_irq_t irq =
          (dif_pwrmgr_irq_t)(plic_irq_id -
                             (dif_rv_plic_irq_id_t)
                                 kTopAssamPlicIrqIdPwrmgrAonWakeup);
      CHECK(irq == pwrmgr_irq_expected,
            "Incorrect pwrmgr_aon IRQ triggered: exp = %d, obs = %d",
            pwrmgr_irq_expected, irq);
      pwrmgr_irq_serviced = irq;

      dif_pwrmgr_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_pwrmgr_irq_get_state(&pwrmgr_aon, &snapshot));
      CHECK(snapshot == (dif_pwrmgr_irq_state_snapshot_t)(1 << irq),
            "Only pwrmgr_aon IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      CHECK_DIF_OK(dif_pwrmgr_irq_acknowledge(&pwrmgr_aon, irq));
      break;
    }
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 1 && 1 < TEST_MAX_IRQ_PERIPHERAL
    case kTopAssamPlicPeripheralSpiDevice: {
      dif_spi_device_irq_t irq =
          (dif_spi_device_irq_t)(plic_irq_id -
                                 (dif_rv_plic_irq_id_t)
                                     kTopAssamPlicIrqIdSpiDeviceUploadCmdfifoNotEmpty);
      CHECK(irq == spi_device_irq_expected,
            "Incorrect spi_device IRQ triggered: exp = %d, obs = %d",
            spi_device_irq_expected, irq);
      spi_device_irq_serviced = irq;

      dif_spi_device_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_spi_device_irq_get_state(&spi_device, &snapshot));
      CHECK(snapshot == (dif_spi_device_irq_state_snapshot_t)(1 << irq),
            "Only spi_device IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      if (0x20 & (1 << irq)) {
        // We do not acknowledge status type interrupt at the IP side, but we
        // need to clear the test force register.
        CHECK_DIF_OK(dif_spi_device_irq_force(&spi_device, irq, false));
        // In case this status interrupt is asserted by default, we also
        // disable it at this point so that it does not interfere with the
        // rest of the test.
        if ((0x0 & (1 << irq))) {
          CHECK_DIF_OK(dif_spi_device_irq_set_enabled(&spi_device, irq, false));
        }
      } else {
        // We acknowledge event type interrupt.
        CHECK_DIF_OK(dif_spi_device_irq_acknowledge(&spi_device, irq));
      }
      break;
    }
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 2 && 2 < TEST_MAX_IRQ_PERIPHERAL
    case kTopAssamPlicPeripheralUart0: {
      dif_uart_irq_t irq =
          (dif_uart_irq_t)(plic_irq_id -
                           (dif_rv_plic_irq_id_t)
                               kTopAssamPlicIrqIdUart0TxWatermark);
      CHECK(irq == uart_irq_expected,
            "Incorrect uart0 IRQ triggered: exp = %d, obs = %d",
            uart_irq_expected, irq);
      uart_irq_serviced = irq;

      dif_uart_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_uart_irq_get_state(&uart0, &snapshot));
      CHECK(snapshot == (dif_uart_irq_state_snapshot_t)((1 << irq) | 0x101),
            "Expected uart0 interrupt status %x. Actual interrupt "
            "status = %x",
            (1 << irq) | 0x101, snapshot);

      if (0x103 & (1 << irq)) {
        // We do not acknowledge status type interrupt at the IP side, but we
        // need to clear the test force register.
        CHECK_DIF_OK(dif_uart_irq_force(&uart0, irq, false));
        // In case this status interrupt is asserted by default, we also
        // disable it at this point so that it does not interfere with the
        // rest of the test.
        if ((0x101 & (1 << irq))) {
          CHECK_DIF_OK(dif_uart_irq_set_enabled(&uart0, irq, false));
        }
      } else {
        // We acknowledge event type interrupt.
        CHECK_DIF_OK(dif_uart_irq_acknowledge(&uart0, irq));
      }
      break;
    }
#endif

    default:
      LOG_FATAL("ISR is not implemented!");
      test_status_set(kTestStatusFailed);
  }
  // Complete the IRQ at PLIC.
  CHECK_DIF_OK(dif_rv_plic_irq_complete(&plic, kHart, plic_irq_id));
}

/**
 * Initializes the handles to all peripherals.
 */
static void peripherals_init(void) {
  mmio_region_t base_addr;

#if TEST_MIN_IRQ_PERIPHERAL <= 0 && 0 < TEST_MAX_IRQ_PERIPHERAL
  base_addr = mmio_region_from_addr(TOP_ASSAM_PWRMGR_AON_BASE_ADDR);
  CHECK_DIF_OK(dif_pwrmgr_init(base_addr, &pwrmgr_aon));
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 1 && 1 < TEST_MAX_IRQ_PERIPHERAL
  base_addr = mmio_region_from_addr(TOP_ASSAM_SPI_DEVICE_BASE_ADDR);
  CHECK_DIF_OK(dif_spi_device_init(base_addr, &spi_device));
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 2 && 2 < TEST_MAX_IRQ_PERIPHERAL
  base_addr = mmio_region_from_addr(TOP_ASSAM_UART0_BASE_ADDR);
  CHECK_DIF_OK(dif_uart_init(base_addr, &uart0));
#endif

  base_addr = mmio_region_from_addr(TOP_ASSAM_RV_PLIC_BASE_ADDR);
  CHECK_DIF_OK(dif_rv_plic_init(base_addr, &plic));
}

/**
 * Clears pending IRQs in all peripherals.
 */
static void peripheral_irqs_clear(void) {
#if TEST_MIN_IRQ_PERIPHERAL <= 0 && 0 < TEST_MAX_IRQ_PERIPHERAL
  CHECK_DIF_OK(dif_pwrmgr_irq_acknowledge_all(&pwrmgr_aon));
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 1 && 1 < TEST_MAX_IRQ_PERIPHERAL
  CHECK_DIF_OK(dif_spi_device_irq_acknowledge_all(&spi_device));
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 2 && 2 < TEST_MAX_IRQ_PERIPHERAL
  CHECK_DIF_OK(dif_uart_irq_acknowledge_all(&uart0));
#endif
}

/**
 * Enables all IRQs in all peripherals.
 */
static void peripheral_irqs_enable(void) {
#if TEST_MIN_IRQ_PERIPHERAL <= 0 && 0 < TEST_MAX_IRQ_PERIPHERAL
  dif_pwrmgr_irq_state_snapshot_t pwrmgr_irqs =
      (dif_pwrmgr_irq_state_snapshot_t)0xffffffff;
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 1 && 1 < TEST_MAX_IRQ_PERIPHERAL
  dif_spi_device_irq_state_snapshot_t spi_device_irqs =
      (dif_spi_device_irq_state_snapshot_t)0xffffffff;
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 2 && 2 < TEST_MAX_IRQ_PERIPHERAL
  // Note: this peripheral contains status interrupts that are asserted by
  // default. Therefore, not all interrupts are enabled here, since that
  // would interfere with this test. Instead, these interrupts are enabled on
  // demand once they are being tested.
  dif_uart_irq_state_snapshot_t uart_irqs =
      (dif_uart_irq_state_snapshot_t)0xfffffefe;
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 0 && 0 < TEST_MAX_IRQ_PERIPHERAL
  CHECK_DIF_OK(dif_pwrmgr_irq_restore_all(&pwrmgr_aon, &pwrmgr_irqs));
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 1 && 1 < TEST_MAX_IRQ_PERIPHERAL
  CHECK_DIF_OK(dif_spi_device_irq_restore_all(&spi_device, &spi_device_irqs));
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 2 && 2 < TEST_MAX_IRQ_PERIPHERAL
  // lowrisc/opentitan#8656: Skip UART0 in non-DV setups due to interference
  // from the logging facility.
  if (kDeviceType == kDeviceSimDV) {
    CHECK_DIF_OK(dif_uart_irq_restore_all(&uart0, &uart_irqs));
  }
#endif
}

/**
 * Triggers all IRQs in all peripherals one by one.
 *
 * Walks through all instances of all peripherals and triggers an interrupt one
 * by one, by forcing with the `intr_test` CSR. On trigger, the CPU instantly
 * jumps into the ISR. The main flow of execution thus proceeds to check that
 * the correct IRQ was serviced immediately. The ISR, in turn checks if the
 * expected IRQ from the expected peripheral triggered.
 */
static void peripheral_irqs_trigger(void) {
  unsigned int status_default_mask;
  // Depending on the build configuration, this variable may show up as unused
  // in the clang linter. This statement waives that error.
  (void)status_default_mask;

#if TEST_MIN_IRQ_PERIPHERAL <= 0 && 0 < TEST_MAX_IRQ_PERIPHERAL
  peripheral_expected = kTopAssamPlicPeripheralPwrmgrAon;
  for (dif_pwrmgr_irq_t irq = kDifPwrmgrIrqWakeup; irq <= kDifPwrmgrIrqWakeup;
       ++irq) {
    pwrmgr_irq_expected = irq;
    LOG_INFO("Triggering pwrmgr_aon IRQ %d.", irq);
    CHECK_DIF_OK(dif_pwrmgr_irq_force(&pwrmgr_aon, irq, true));

    // This avoids a race where *irq_serviced is read before
    // entering the ISR.
    IBEX_SPIN_FOR(pwrmgr_irq_serviced == irq, 1);
    LOG_INFO("IRQ %d from pwrmgr_aon is serviced.", irq);
  }
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 1 && 1 < TEST_MAX_IRQ_PERIPHERAL
  peripheral_expected = kTopAssamPlicPeripheralSpiDevice;
  status_default_mask = 0x0;
  for (dif_spi_device_irq_t irq = kDifSpiDeviceIrqUploadCmdfifoNotEmpty; irq <= kDifSpiDeviceIrqTpmRdfifoDrop;
       ++irq) {
    spi_device_irq_expected = irq;
    LOG_INFO("Triggering spi_device IRQ %d.", irq);
    CHECK_DIF_OK(dif_spi_device_irq_force(&spi_device, irq, true));

    // In this case, the interrupt has not been enabled yet because that would
    // interfere with testing other interrupts. We enable it here and let the
    // interrupt handler disable it again.
    if ((status_default_mask & 0x1)) {
      CHECK_DIF_OK(dif_spi_device_irq_set_enabled(&spi_device, irq, true));
    }
    status_default_mask >>= 1;

    // This avoids a race where *irq_serviced is read before
    // entering the ISR.
    IBEX_SPIN_FOR(spi_device_irq_serviced == irq, 1);
    LOG_INFO("IRQ %d from spi_device is serviced.", irq);
  }
#endif

#if TEST_MIN_IRQ_PERIPHERAL <= 2 && 2 < TEST_MAX_IRQ_PERIPHERAL
  // lowrisc/opentitan#8656: Skip UART0 in non-DV setups due to interference
  // from the logging facility.
  // aon_timer may generate a NMI instead of a PLIC IRQ depending on the ROM.
  // Since there are other tests covering this already, we just skip this for
  // non-DV setups.
  if (kDeviceType == kDeviceSimDV) {
    peripheral_expected = kTopAssamPlicPeripheralUart0;
    status_default_mask = 0x101;
    for (dif_uart_irq_t irq = kDifUartIrqTxWatermark; irq <= kDifUartIrqTxEmpty;
         ++irq) {
      uart_irq_expected = irq;
      LOG_INFO("Triggering uart0 IRQ %d.", irq);
      CHECK_DIF_OK(dif_uart_irq_force(&uart0, irq, true));

      // In this case, the interrupt has not been enabled yet because that would
      // interfere with testing other interrupts. We enable it here and let the
      // interrupt handler disable it again.
      if ((status_default_mask & 0x1)) {
        CHECK_DIF_OK(dif_uart_irq_set_enabled(&uart0, irq, true));
      }
      status_default_mask >>= 1;

      // This avoids a race where *irq_serviced is read before
      // entering the ISR.
      IBEX_SPIN_FOR(uart_irq_serviced == irq, 1);
      LOG_INFO("IRQ %d from uart0 is serviced.", irq);
    }
  }
#endif
}

/**
 * Checks that the target ID corresponds to the ID of the hart on which
 * this test is executed on. This check is meant to be used in a
 * single-hart system only.
 */
static void check_hart_id(uint32_t exp_hart_id) {
  uint32_t act_hart_id;
  CSR_READ(CSR_REG_MHARTID, &act_hart_id);
  CHECK(act_hart_id == exp_hart_id, "Processor has unexpected HART ID.");
}

OTTF_DEFINE_TEST_CONFIG();

bool test_main(void) {
  irq_global_ctrl(true);
  irq_external_ctrl(true);
  peripherals_init();
  check_hart_id((uint32_t)kHart);
  rv_plic_testutils_irq_range_enable(
      &plic, kHart, kTopAssamPlicIrqIdNone + 1, kTopAssamPlicIrqIdLast);
  peripheral_irqs_clear();
  peripheral_irqs_enable();
  peripheral_irqs_trigger();
  return true;
}

// clang-format on

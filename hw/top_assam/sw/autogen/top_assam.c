// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
// util/topgen.py -t hw/top_assam/data/top_assam.hjson \
//                -o hw/top_assam/

#include "hw/top_assam/sw/autogen/top_assam.h"

/**
 * PLIC Interrupt Source to Peripheral Map
 *
 * This array is a mapping from `top_assam_plic_irq_id_t` to
 * `top_assam_plic_peripheral_t`.
 */
const top_assam_plic_peripheral_t
    top_assam_plic_interrupt_for_peripheral[19] = {
  [kTopAssamPlicIrqIdNone] = kTopAssamPlicPeripheralUnknown,
  [kTopAssamPlicIrqIdUart0TxWatermark] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdUart0RxWatermark] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdUart0TxDone] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdUart0RxOverflow] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdUart0RxFrameErr] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdUart0RxBreakErr] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdUart0RxTimeout] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdUart0RxParityErr] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdUart0TxEmpty] = kTopAssamPlicPeripheralUart0,
  [kTopAssamPlicIrqIdSpiDeviceUploadCmdfifoNotEmpty] = kTopAssamPlicPeripheralSpiDevice,
  [kTopAssamPlicIrqIdSpiDeviceUploadPayloadNotEmpty] = kTopAssamPlicPeripheralSpiDevice,
  [kTopAssamPlicIrqIdSpiDeviceUploadPayloadOverflow] = kTopAssamPlicPeripheralSpiDevice,
  [kTopAssamPlicIrqIdSpiDeviceReadbufWatermark] = kTopAssamPlicPeripheralSpiDevice,
  [kTopAssamPlicIrqIdSpiDeviceReadbufFlip] = kTopAssamPlicPeripheralSpiDevice,
  [kTopAssamPlicIrqIdSpiDeviceTpmHeaderNotEmpty] = kTopAssamPlicPeripheralSpiDevice,
  [kTopAssamPlicIrqIdSpiDeviceTpmRdfifoCmdEnd] = kTopAssamPlicPeripheralSpiDevice,
  [kTopAssamPlicIrqIdSpiDeviceTpmRdfifoDrop] = kTopAssamPlicPeripheralSpiDevice,
  [kTopAssamPlicIrqIdPwrmgrAonWakeup] = kTopAssamPlicPeripheralPwrmgrAon,
};

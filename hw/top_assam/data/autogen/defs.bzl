# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------#
# PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
# util/topgen.py -t hw/top_assam/data/top_assam.hjson \
#                -o hw/top_assam/

load("//hw/ip/aes:defs.bzl", "AES")
load("//hw/top_assam/ip/ast:defs.bzl", "AST")
load("//hw/top_assam/ip_autogen/clkmgr:defs.bzl", "CLKMGR")
load("//hw/top_assam/ip_autogen/pinmux:defs.bzl", "PINMUX")
load("//hw/top_assam/ip_autogen/pwrmgr:defs.bzl", "PWRMGR")
load("//hw/ip/rom_ctrl:defs.bzl", "ROM_CTRL")
load("//hw/top_assam/ip_autogen/rstmgr:defs.bzl", "RSTMGR")
load("//hw/top_assam/ip_autogen/rv_core_ibex:defs.bzl", "RV_CORE_IBEX")
load("//hw/top_assam/ip_autogen/rv_plic:defs.bzl", "RV_PLIC")
load("//hw/ip/rv_timer:defs.bzl", "RV_TIMER")
load("//hw/ip/spi_device:defs.bzl", "SPI_DEVICE")
load("//hw/ip/sram_ctrl:defs.bzl", "SRAM_CTRL")
load("//hw/ip/uart:defs.bzl", "UART")

ASSAM_IPS = [
    AES,
    AST,
    CLKMGR,
    PINMUX,
    PWRMGR,
    ROM_CTRL,
    RSTMGR,
    RV_CORE_IBEX,
    RV_PLIC,
    RV_TIMER,
    SPI_DEVICE,
    SRAM_CTRL,
    UART,
]

ASSAM_ALERTS = [
]

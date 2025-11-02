# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

load(
    "//rules/opentitan:defs.bzl",
    "opentitan_binary",
)
load("//rules:linker.bzl", "ld_library")

package(default_visibility = ["//visibility:public"])

ld_library(
    name = "linker_script",
    script = "assam_uart_hello.ld",
    deps = [
        "//hw/top_assam/sw/autogen:top_assam_memory",
        "//sw/device:info_sections",
    ],
)

opentitan_binary(
    name = "assam_uart_hello",
    srcs = ["assam_uart_hello.c"],
    exec_env = [
        "//hw/top_assam:sim_verilator_base",
    ],
    linker_script = ":linker_script",
    deps = [
        "//hw/top_assam/sw/autogen:top_assam",
        "//sw/device/lib/arch:device",
        "//sw/device/lib/crt",
        "//sw/device/lib/dif:uart",
        "//sw/device/lib/runtime:hart",
        "//sw/device/lib/runtime:print",
    ],
)

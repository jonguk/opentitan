# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

load(
    "//rules/opentitan:defs.bzl",
    "DEFAULT_TEST_FAILURE_MSG",
    "DEFAULT_TEST_SUCCESS_MSG",
    "sim_verilator",
)

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "rtl_files",
    srcs = glob(["**"], allow_empty = True) + [
        "//hw/top_assam/data:all_files",
        "//hw/top_assam/sw/autogen:all_files",
    ],
)

filegroup(
    name = "doc_files",
    srcs = glob(
        [
            "**/*.md",
            "**/*.svg",
            "**/*.png",
        ],
        allow_empty = True,
    ),
)

###########################################################################
# Verilator Simulation Environments
###########################################################################

# Base sim_verilator environment for Assam (non-testonly, minimal dependencies)
sim_verilator(
    name = "sim_verilator_base",
    design = "assam",
    exec_env = "sim_verilator",
    libs = [
        "//sw/device/lib/arch:boot_stage_rom_ext",
        "//sw/device/lib/arch:sim_verilator",
        "//hw/top_assam/sw/autogen:top_assam",
    ],
    linker_script = "//sw/device/lib/testing/test_framework:ottf_ld_silicon_creator_slot_a",
    otp = "//hw/top_assam/data/otp:img_rma",
    test_cmd = "testing-not-supported",
)

# Test only sim_verilator environment for Assam
# For build verification, use sim_verilator_base which doesn't require ROM
alias(
    name = "sim_verilator",
    actual = ":sim_verilator_base",
)

# Full sim_verilator with ROM (TODO: need Assam-specific test_rom without lc_ctrl dependency)
# sim_verilator(
#     name = "sim_verilator_with_rom",
#     testonly = True,
#     args = [
#         "--rcfile=",
#         "--logging=info",
#         "--interface=verilator",
#         "--verilator-bin=$(rootpath //hw:verilator)",
#         "--verilator-rom={rom}",
#         "--verilator-otp={otp}",
#         "--verilator-flash={firmware}",
#     ],
#     base = ":sim_verilator_base",
#     data = [
#         "//hw:fusesoc_ignore",
#         "//hw:verilator",
#     ],
#     exec_env = "sim_verilator",
#     param = {
#         "exit_success": DEFAULT_TEST_SUCCESS_MSG,
#         "exit_failure": DEFAULT_TEST_FAILURE_MSG,
#     },
#     rom = "//sw/device/lib/testing/test_rom:test_rom",
#     test_cmd = """
#         --exec="console --non-interactive --exit-success='{exit_success}' --exit-failure='{exit_failure}'"
#         no-op
#     """,
# )

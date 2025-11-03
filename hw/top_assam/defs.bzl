# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#

load("//rules/opentitan:hw.bzl", "opentitan_top")
load("//hw/top_assam/data/autogen:defs.bzl", "ASSAM_IPS")
# Borrow OTP configuration from EarlGrey for now
load("//hw/top_earlgrey/data/otp:defs.bzl", "EARLGREY_OTP_SIGVERIFY_FAKE_KEYS", "EARLGREY_STD_OTP_OVERLAYS")

ASSAM = opentitan_top(
    name = "assam",
    hjson = "//hw/top_assam/data/autogen:top_assam.gen.hjson",
    top_lib = "//hw/top_assam/sw/autogen:top_assam",
    top_ld = "//hw/top_assam/sw/autogen:top_assam_memory",
    otp_map = "//hw/top_earlgrey/data/otp:otp_ctrl_mmap.hjson",  # Borrow from EarlGrey
    std_otp_overlay = EARLGREY_STD_OTP_OVERLAYS,  # Borrow from EarlGrey
    otp_sigverify_fake_keys = EARLGREY_OTP_SIGVERIFY_FAKE_KEYS,  # Borrow from EarlGrey
    ips = ASSAM_IPS,
    secret_cfgs = {
        "testing": "//hw/top_assam/data/autogen:top_assam.secrets.testing.gen.hjson",
    },
)

ASSAM_SLOTS = {
    "rom_ext_slot_a": "0x0",
    "rom_ext_slot_b": "0x80000",
    "owner_slot_a": "0x10000",
    "owner_slot_b": "0x90000",
    "rom_ext_size": "0x10000",
}

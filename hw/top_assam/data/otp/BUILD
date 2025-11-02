# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

# Minimal OTP configuration for Assam simulation

load(
    "//rules:otp.bzl",
    "STD_OTP_OVERLAYS",
    "otp_image",
    "otp_json",
    "otp_partition",
)

package(default_visibility = ["//visibility:public"])

# Minimal RMA lifecycle state for simulation
otp_json(
    name = "otp_json_rma",
    partitions = [
        otp_partition(
            name = "LIFE_CYCLE",
            count = 8,
            state = "RMA",
        ),
    ],
)

# Minimal RMA OTP image for Assam simulation
otp_image(
    name = "img_rma",
    src = ":otp_json_rma",
    overlays = [],  # No overlays for minimal Assam OTP
)

# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
"""Generate SystemC/TLM2.0 register model skeletons from IpBlock object"""

import logging as log
import os
from mako import exceptions  # type: ignore
from mako.template import Template  # type: ignore
import importlib.resources

from reggen.ip_block import IpBlock


def gen_systemc(block: IpBlock, outdir: str) -> int:
    """Render SystemC header and source using Mako templates

    Outputs:
      - <block>_reg_pkg.hpp
      - <block>_reg_top.cpp
    """

    try:
        hpp_tpl = Template(
            filename=str(importlib.resources.files('reggen') / 'systemc' / 'reg_model.hpp.tpl'))
        cpp_tpl = Template(
            filename=str(importlib.resources.files('reggen') / 'systemc' / 'reg_model.cpp.tpl'))
    except Exception:
        log.error(exceptions.text_error_template().render())
        return 1

    base = block.name.lower()
    # Align naming with SV: *_reg_pkg.sv -> *_reg_pkg.hpp, *_reg_top.sv -> *_reg_top.cpp
    hpp_path = os.path.join(outdir, f"{base}_reg_pkg.hpp")
    cpp_path = os.path.join(outdir, f"{base}_reg_top.cpp")

    os.makedirs(outdir, exist_ok=True)

    try:
        with open(hpp_path, 'w', encoding='UTF-8') as fh:
            fh.write(hpp_tpl.render(block=block))
        with open(cpp_path, 'w', encoding='UTF-8') as fc:
            fc.write(cpp_tpl.render(block=block))
    except Exception:
        log.error(exceptions.text_error_template().render())
        return 1

    return 0



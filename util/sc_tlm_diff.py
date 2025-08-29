#!/usr/bin/env python3
# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

"""Regenerate SystemC/TLM for an IP and diff against checked-in tlm/.

Usage:
  util/sc_tlm_diff.py --ip uart [--hjson PATH] [--update]

This script runs regtool with --systemc to a temporary directory and then
diffs the generated sources against hw/ip/<ip>/tlm. If --update is passed,
it will copy differing files from the generated output into the repository
tree (overwriting).
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


def run_cmd(cmd: list[str]) -> tuple[int, str]:
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    out, _ = proc.communicate()
    return proc.returncode, out


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--ip', required=True, help='IP block name, e.g. uart')
    parser.add_argument('--hjson', help='Path to IP HJSON; defaults to hw/ip/<ip>/data/<ip>.hjson')
    parser.add_argument('--update', action='store_true', help='If set, update repo tlm files from generated output')
    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parents[1]
    ip = args.ip
    hjson = Path(args.hjson) if args.hjson else repo_root / 'hw' / 'ip' / ip / 'data' / f'{ip}.hjson'
    if not hjson.exists():
        print(f'ERROR: HJSON not found: {hjson}', file=sys.stderr)
        return 2
    tlm_dir = repo_root / 'hw' / 'ip' / ip / 'tlm'
    if not tlm_dir.exists():
        print(f'ERROR: TLM dir not found: {tlm_dir}', file=sys.stderr)
        return 2

    with tempfile.TemporaryDirectory(prefix=f'{ip}_tlm_gen_') as tmpdir:
        gen_dir = Path(tmpdir)
        rc, out = run_cmd([sys.executable, str(repo_root / 'util' / 'regtool.py'), '--systemc', '--outdir', str(gen_dir), str(hjson)])
        if rc != 0:
            print(out)
            print('ERROR: regtool generation failed', file=sys.stderr)
            return rc

        # Ignore whitespace-only diffs
        rc, diff_out = run_cmd(['diff', '-ruw', str(tlm_dir), str(gen_dir)])
        if rc == 0:
            print('OK: Generated TLM matches checked-in files.')
            return 0

        print('DIFF detected between checked-in and generated TLM:')
        print(diff_out)

        if args.update:
            # Copy over differing files from gen_dir to tlm_dir
            for root, _, files in os.walk(gen_dir):
                for f in files:
                    src = Path(root) / f
                    rel = src.relative_to(gen_dir)
                    dst = tlm_dir / rel
                    dst.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(src, dst)
                    print(f'Updated {dst}')
            print('Repository TLM files updated from generated output.')
            return 0

        return 1


if __name__ == '__main__':
    sys.exit(main())



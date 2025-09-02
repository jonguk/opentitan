#!/usr/bin/env python3
# Copyright lowRISC contributors.
# SPDX-License-Identifier: Apache-2.0

import argparse
import os
import subprocess
import sys
from pathlib import Path


def run(cmd: list[str], cwd: Path):
  print("+", " ".join(cmd))
  subprocess.check_call(cmd, cwd=str(cwd))


CORE_TMPL = """filesets:
  files_dv:
    files:
      - {ip}_ral_pkg.hpp
    file_type: user
name: lowrisc:dv:{ip}_ral_pkg
targets:
  default:
    filesets:
      - files_dv
"""

HPP_TMPL = """#pragma once
// SV-like naming for SystemC RAL package: {ip}_ral_pkg
// Re-exports the existing SystemC register model to keep names aligned

#include "../../tlm/{ip}_reg_pkg.hpp"

namespace {ip}_ral_pkg {{
  using ::opentitan::{ip}::{ip}_reg_top;
}}
"""


def ensure_dir(p: Path):
  p.mkdir(parents=True, exist_ok=True)


def _write_text(path: Path, content: str):
  path.parent.mkdir(parents=True, exist_ok=True)
  path.write_text(content)


def _maybe_diff(new_path: Path, existing_path: Path):
  try:
    old = existing_path.read_text().splitlines(keepends=True)
  except FileNotFoundError:
    old = []
  new = new_path.read_text().splitlines(keepends=True)
  import difflib
  diff = difflib.unified_diff(old, new,
                              fromfile=str(existing_path),
                              tofile=str(new_path),
                              n=3)
  diff_text = "".join(diff)
  if diff_text:
    print(diff_text)
  else:
    print(f"No differences: {existing_path}")


def gen_for_ip(repo_top: Path, ip: str, tmp_out, skip_regtool: bool = False, vendor: str = 'lowrisc'):
  hjson = repo_top / f"hw/ip/{ip}/data/{ip}.hjson"
  if not hjson.exists():
    raise FileNotFoundError(f"HJSON not found: {hjson}")

  regtool = repo_top / "util/regtool.py"
  if not skip_regtool and not regtool.exists():
    raise FileNotFoundError("util/regtool.py not found")

  # Resolve python executable: prefer project venv if present
  venv_py = repo_top / ".venv/bin/python"
  python_exe = str(venv_py) if venv_py.exists() else sys.executable

  if not skip_regtool:
    # 1) Generate SV RAL into hw/ip/<ip>/dv (or tmp path)
    dv_target = repo_top / f"hw/ip/{ip}/dv"
    dv_out = (tmp_out / f"hw/ip/{ip}/dv") if tmp_out else dv_target
    ensure_dir(dv_out)
    run([python_exe, str(regtool), "-s", "-t", str(dv_out), str(hjson)], cwd=repo_top / "util")
    if tmp_out:
      print(f"\n== Diff SV RAL ({ip}) ==")
      for p in dv_out.rglob("*"):
        if p.is_file():
          rel = p.relative_to(tmp_out)
          _maybe_diff(p, repo_top / rel)

  if not skip_regtool:
    # 2) Generate SystemC register model into hw/ip/<ip>/tlm (or tmp path)
    tlm_target = repo_top / f"hw/ip/{ip}/tlm"
    tlm_out = (tmp_out / f"hw/ip/{ip}/tlm") if tmp_out else tlm_target
    ensure_dir(tlm_out)
    run([python_exe, str(regtool), "--systemc", "-t", str(tlm_out), str(hjson)], cwd=repo_top / "util")
    if tmp_out:
      print(f"\n== Diff SystemC Reg Model ({ip}) ==")
      for p in tlm_out.rglob("*"):
        if p.is_file():
          rel = p.relative_to(tmp_out)
          _maybe_diff(p, repo_top / rel)

  # 3) Create scdv RAL wrapper (<ip>_ral_pkg.hpp/.core)
  scdv_target = repo_top / f"hw/ip/{ip}/scdv/ral"
  scdv_ral = (tmp_out / f"hw/ip/{ip}/scdv/ral") if tmp_out else scdv_target
  ensure_dir(scdv_ral)
  _write_text(scdv_ral / f"{ip}_ral_pkg.core", CORE_TMPL.format(ip=ip))
  _write_text(scdv_ral / f"{ip}_ral_pkg.hpp", HPP_TMPL.format(ip=ip))
  if tmp_out:
    print(f"\n== Diff SC RAL Wrapper ({ip}) ==")
    for name in (f"{ip}_ral_pkg.core", f"{ip}_ral_pkg.hpp"):
      _maybe_diff(scdv_ral / name, scdv_target / name)

  # 4) Generate/update SC DV scaffold (env/tests) using uvmdvgen templates
  try:
    sys.path.insert(0, str(repo_top / 'util'))
    from uvmdvgen.gen_sc import gen_sc as gen_sc_skel  # type: ignore
  except Exception as e:
    print(f"Warning: failed to import uvmdvgen.gen_sc: {e}")
    gen_sc_skel = None
  if gen_sc_skel is not None:
    env_outdir = str(repo_top / f"hw/ip/{ip}")
    # Emit into tmp if requested, then diff against repo
    if tmp_out:
      out_base = tmp_out / f"hw/ip/{ip}"
      out_base.mkdir(parents=True, exist_ok=True)
      gen_sc_skel(ip, str(out_base), vendor=vendor, is_cip=True, has_ral=True, has_interrupts=False, has_alerts=False, env_agents=[])
      print(f"\n== Diff SC DV Scaffold ({ip}) ==")
      for p in (out_base / 'scdv').rglob('*'):
        if p.is_file():
          rel = p.relative_to(tmp_out)
          _maybe_diff(p, repo_top / rel)
    else:
      gen_sc_skel(ip, env_outdir, vendor=vendor, is_cip=True, has_ral=True, has_interrupts=False, has_alerts=False, env_agents=[])


def main():
  ap = argparse.ArgumentParser(description="Generate SC/SV DV collateral for IPs (SV RAL, SystemC reg model, SC RAL wrapper)")
  ap.add_argument("ips", nargs="*", help="IP names (e.g. uart)")
  ap.add_argument("--all", action="store_true", help="Process all IPs that have data/<ip>.hjson")
  ap.add_argument("--tmp-out", action="store_true", help="Write outputs to a temporary directory and show diffs against current repo")
  ap.add_argument("--skip-regtool", action="store_true", help="Skip running regtool (generate only SC DV scaffold and RAL wrapper)")
  ap.add_argument("--vendor", default="lowrisc", help="VLNV vendor for generated cores (default: lowrisc)")
  args = ap.parse_args()

  repo_top = Path(__file__).resolve().parents[1]
  tmp_root = (repo_top / "build_scdv_tmp") if args.tmp_out else None
  if tmp_root:
    tmp_root.mkdir(parents=True, exist_ok=True)
    print(f"Temporary output: {tmp_root}")
  ip_list: list[str] = []
  if args.all:
    for p in (repo_top / 'hw' / 'ip').iterdir():
      if not p.is_dir():
        continue
      ip = p.name
      if (p / 'data' / f'{ip}.hjson').exists():
        ip_list.append(ip)
  else:
    ip_list = list(args.ips)
  if not ip_list:
    print("No IPs specified or found. Use --all or pass IP names.")
    return
  for ip in sorted(ip_list):
    print(f"\n=== Generating collateral for IP: {ip} ===")
    gen_for_ip(repo_top, ip, tmp_root, skip_regtool=args.skip_regtool, vendor=args.vendor)
  print("Done.")


if __name__ == "__main__":
  main()



#!/usr/bin/env bash
# Usage: source util/scdv_env.sh sc23|sc30

set -euo pipefail

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
  echo "Please source this script: source util/scdv_env.sh <sc23|sc30>" >&2
  exit 1
fi

if [[ $# -ne 1 ]]; then
  echo "Usage: source util/scdv_env.sh sc23|sc30" >&2
  return 2
fi

proj_root="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"

case "$1" in
  sc23)
    export SYSTEMC_HOME="${proj_root}/../install"
    export UVM_SYSTEMC_HOME="${proj_root}/../install_uvmsc_sc23"
    ;;
  sc30)
    export SYSTEMC_HOME="${proj_root}/../install_sc30"
    export UVM_SYSTEMC_HOME="${proj_root}/../install_uvmsc_sc30"
    ;;
  *)
    echo "Unknown target '$1'. Use sc23 or sc30." >&2
    return 2
    ;;
esac

# Update DYLD_LIBRARY_PATH so test binaries find the right libs at runtime
export DYLD_LIBRARY_PATH="${UVM_SYSTEMC_HOME}/lib:${SYSTEMC_HOME}/lib:${DYLD_LIBRARY_PATH:-}"

# Prefer project Python virtual environment (if present) for tooling like regtool.py
if [[ -d "${proj_root}/.venv" && -x "${proj_root}/.venv/bin/python" ]]; then
  export PATH="${proj_root}/.venv/bin:${PATH}"
  echo "  Using project venv: ${proj_root}/.venv"
fi

echo "[scdv_env] Selected $1"
echo "  SYSTEMC_HOME=${SYSTEMC_HOME}"
echo "  UVM_SYSTEMC_HOME=${UVM_SYSTEMC_HOME}"
echo "  DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}"

if [[ ! -d "${SYSTEMC_HOME}/include" ]] || [[ ! -d "${SYSTEMC_HOME}/lib" ]]; then
  echo "[scdv_env] Warning: SYSTEMC_HOME does not look installed (${SYSTEMC_HOME})" >&2
fi
if [[ ! -d "${UVM_SYSTEMC_HOME}/include" ]] || [[ ! -d "${UVM_SYSTEMC_HOME}/lib" ]]; then
  echo "[scdv_env] Warning: UVM_SYSTEMC_HOME does not look staged (${UVM_SYSTEMC_HOME})" >&2
fi



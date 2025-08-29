#!/usr/bin/env bash
set -euo pipefail

# Regenerate SystemC TLM for the given IPs and diff against checked-in tlm directories.
# Usage: util/ci_sc_tlm_check.sh uart [ip2 ip3 ...]

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
IP_LIST=("$@")
if [[ ${#IP_LIST[@]} -eq 0 ]]; then
  echo "ERROR: Provide at least one IP name (e.g. uart)." >&2
  exit 2
fi

RC=0
for ip in "${IP_LIST[@]}"; do
  echo "[sc_tlm_diff] Checking IP: ${ip}"
  if ! python3 "${ROOT_DIR}/util/sc_tlm_diff.py" --ip "${ip}"; then
    echo "[sc_tlm_diff] Mismatch detected for ${ip}" >&2
    RC=1
  fi
done

exit ${RC}



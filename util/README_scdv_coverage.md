## SCDV Coverage Usage

This guide explains how to generate, merge, and view SystemC DV coverage, and how N/A handling works.

### Quick Start
1) Run tests to emit FC4SC JSONs (example: UART):
```
make -C hw/ip/uart/scdv/tests -j1 uart_smoke_test
hw/ip/uart/scdv/tests/uart_smoke_test +ntb_random_seed=random
```
2) Build the HTML:
```
./.venv/bin/python util/scdv_cov_report.py --ip uart \
  --outdir hw/ip/uart/scdv/tests/coverage_html --systemc-home "$SYSTEMC_HOME" \
  --jobs 1 --repeat 1 --rich
```

### Targeted Runs
Use `--tests` for a subset and `--timeout` to avoid long waits:
```
./.venv/bin/python util/scdv_cov_report.py --ip i2c \
  --tests "i2c_csr_addr_sweep_test i2c_tlul_err_test" --timeout 600 \
  --outdir hw/ip/i2c/scdv/tests/coverage_html --systemc-home "$SYSTEMC_HOME"
```

### What is Reported
The HTML lists all covergroups/coverpoints/bins/crosses present in the JSON inputs. No IP-specific hardcoding is required.

### N/A Handling
- `gen_sc.py` writes `coverage_manifest.json` per IP recording existing categories and addr policies.
- The report marks bins in `category_cp` with `ignore=true` and `goal=0` when the category doesn't exist (e.g., no `shadow`, `regwen`, `rc`).

### Address Coverage
- `addr_cp` auto-generates one bin per CSR plus `others`.
- Illegal/unmapped reads are sampled to `0xFFFFFFFFu` to hit `others`.
- Ensure at least one of: `*_tlul_err_test` or `*_csr_addr_sweep_test` is run.

### TL Partial Writes
- `*_tl_partial_test` drives 1B/2B/4B PutPartialData with contiguous and non-contiguous masks to populate TL mask-size cross.

### Tips
- Always use the venv Python: `./.venv/bin/python`.
- Set `SYSTEMC_HOME` and `UVM_SYSTEMC_HOME` before running tests/report.
- HTML output is under `hw/ip/<ip>/scdv/tests/coverage_html/index.html`.




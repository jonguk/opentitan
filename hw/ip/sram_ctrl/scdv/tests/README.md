# sram_ctrl SystemC UVM Tests

Build and run:

1) Ensure env vars:
   - SYSTEMC_HOME: your SystemC install prefix
   - UVM_SYSTEMC_HOME: your UVM-SystemC install prefix

2) Build all tests:

   make -C hw/ip/sram_ctrl/scdv/tests all

3) Run a specific test (UVM style):

   cd hw/ip/sram_ctrl/scdv/tests
   ./$sram_ctrl_csr_rw_test +UVM_TESTNAME=$sram_ctrl_csr_rw_test +UVM_VERBOSITY=UVM_MEDIUM +ntb_random_seed=random

Supported flags:
 - +UVM_TESTNAME=<test_class>
 - +UVM_VERBOSITY=UVM_NONE|UVM_LOW|UVM_MEDIUM|UVM_HIGH|UVM_FULL
 - +UVM_TIMEOUT=<value>[ns|us|ms|s]
 - +ntb_random_seed=<N>|random

4) Generate coverage HTML (parallel, accumulate across repeats/seeds):

   util/scdv_cov_report.py --ip sram_ctrl --outdir hw/ip/sram_ctrl/scdv/tests/coverage_html      --systemc-home "$SYSTEMC_HOME" --jobs 4 --repeat 1 --extra-args "+UVM_VERBOSITY=UVM_LOW +ntb_random_seed=random" --accumulate --rich

CSR lookup policy (strict):
- The generator resolves CSR offsets strictly by exact names in HJSON.
- Required: CTRL, INTR_ENABLE. Optional: WDATA.
- Standard comportable IP CSRs (INTR_STATE @0, INTR_ENABLE @4, INTR_TEST @8, ALERT_TEST @12) are auto-available when interrupts/alerts exist, even if not listed in HJSON.
- If a required CSR is missing, scaffold generation fails fast with an error listing the first few available register names.

Template verification:
 - To verify generated TLM matches checked-in sources, use:

   util/sc_tlm_diff.py --ip sram_ctrl  # add --update to sync

Collateral generation:
 - Dry-run scaffold & diff:

   util/gen_scdv_collateral.py --tmp-out --skip-regtool sram_ctrl

 - Apply scaffold (writes to repo):

   util/gen_scdv_collateral.py sram_ctrl

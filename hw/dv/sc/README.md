SystemC/UVM-SC counterparts of hw/dv/sv components. Skeleton directories mirror sv layout.

Usage (per IP tests under hw/ip/<ip>/scdv/tests):
- Build: ensure SYSTEMC_HOME and UVM_SYSTEMC_HOME, then `make all`
- Run: `./<ip>_smoke_test +UVM_TESTNAME=<ip>_smoke_test +UVM_VERBOSITY=UVM_MEDIUM +ntb_random_seed=random`

Flags:
- +UVM_TESTNAME=<test_class>
- +UVM_VERBOSITY=UVM_NONE|UVM_LOW|UVM_MEDIUM|UVM_HIGH|UVM_FULL
- +UVM_TIMEOUT=<value>[ns|us|ms|s]
- +ntb_random_seed=<N>|random

Template verification:
- `util/sc_tlm_diff.py --ip <ip>` to regen TLM and diff vs checked-in.

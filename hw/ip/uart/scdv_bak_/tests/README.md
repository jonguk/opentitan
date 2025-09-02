# uart SystemC UVM Tests

Build and run:

1) Ensure env vars:
   - SYSTEMC_HOME: your SystemC install prefix
   - UVM_SYSTEMC_HOME: your UVM-SystemC install prefix

2) Build all tests:

   make -C hw/ip/uart/scdv/tests all

3) Run a specific test (UVM style):

   cd hw/ip/uart/scdv/tests
   ./$uart_csr_rw_test +UVM_TESTNAME=$uart_csr_rw_test +UVM_VERBOSITY=UVM_MEDIUM +ntb_random_seed=random

Supported flags:
 - +UVM_TESTNAME=<test_class>
 - +UVM_VERBOSITY=UVM_NONE|UVM_LOW|UVM_MEDIUM|UVM_HIGH|UVM_FULL
 - +UVM_TIMEOUT=<value>[ns|us|ms|s]
 - +ntb_random_seed=<N>|random

Template verification:
 - To verify generated TLM matches checked-in sources, use:

   util/sc_tlm_diff.py --ip uart  # add --update to sync

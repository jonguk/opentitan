#pragma once
#include <uvm>
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include "../../../dv/sc/coverage/uart_cov.hpp"
#define CSR_ADDR_BINS_DEF \
  COVERPOINT(uint32_t, addr_cp, addr_val_) { \
      bin<uint32_t>("alert_test", fc4sc::interval<uint32_t>(12u, 12u)), \
      bin<uint32_t>("ctrl", fc4sc::interval<uint32_t>(0u, 0u)), \
      bin<uint32_t>("fifo_ctrl", fc4sc::interval<uint32_t>(16u, 16u)), \
      bin<uint32_t>("fifo_status", fc4sc::interval<uint32_t>(20u, 20u)), \
      bin<uint32_t>("intr_enable", fc4sc::interval<uint32_t>(4u, 4u)), \
      bin<uint32_t>("intr_state", fc4sc::interval<uint32_t>(0u, 0u)), \
      bin<uint32_t>("intr_test", fc4sc::interval<uint32_t>(8u, 8u)), \
      bin<uint32_t>("ovrd", fc4sc::interval<uint32_t>(24u, 24u)), \
      bin<uint32_t>("rdata", fc4sc::interval<uint32_t>(8u, 8u)), \
      bin<uint32_t>("status", fc4sc::interval<uint32_t>(4u, 4u)), \
      bin<uint32_t>("timeout_ctrl", fc4sc::interval<uint32_t>(32u, 32u)), \
      bin<uint32_t>("val", fc4sc::interval<uint32_t>(28u, 28u)), \
      bin<uint32_t>("wdata", fc4sc::interval<uint32_t>(12u, 12u)), \
      bin<uint32_t>("others", fc4sc::interval<uint32_t>(0xFFFFFFFFu, 0xFFFFFFFFu)) \
  }; \
  cross<uint32_t,uint8_t> addr_x_rw = cross<uint32_t,uint8_t>(this, "addr_x_rw", &addr_cp, &is_write_cp);
#include "../../../dv/sc/coverage/csr_cov.hpp"
#endif

#include "../../../dv/sc/cip_lib/cip_base_env.hpp"
class uart_env : public cip_base_env {
 public:
  UVM_COMPONENT_UTILS(uart_env);
  explicit uart_env(uvm::uvm_component_name name) : cip_base_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    cip_base_env::build_phase(phase);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    if (mon) mon->set_coverage(&UartCov::get());
    // CSR category registration for coverage
    { auto &cc = CsrCov::get();
      cc.register_w1c(0); cc.register_intr(0);
      cc.register_intr(8);
      cc.register_alert(12);
      cc.register_w1c(0);
    }
#endif
  }
};

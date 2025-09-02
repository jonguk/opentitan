#pragma once
#include <uvm>
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#define CSR_ADDR_BINS_DEF \
  COVERPOINT(uint32_t, addr_cp, addr_val_) { \
      bin<uint32_t>("acqdata", fc4sc::interval<uint32_t>(72u, 72u)), \
      bin<uint32_t>("acq_fifo_next_data", fc4sc::interval<uint32_t>(96u, 96u)), \
      bin<uint32_t>("alert_test", fc4sc::interval<uint32_t>(12u, 12u)), \
      bin<uint32_t>("controller_events", fc4sc::interval<uint32_t>(104u, 104u)), \
      bin<uint32_t>("ctrl", fc4sc::interval<uint32_t>(0u, 0u)), \
      bin<uint32_t>("fdata", fc4sc::interval<uint32_t>(12u, 12u)), \
      bin<uint32_t>("fifo_ctrl", fc4sc::interval<uint32_t>(16u, 16u)), \
      bin<uint32_t>("host_fifo_config", fc4sc::interval<uint32_t>(20u, 20u)), \
      bin<uint32_t>("host_fifo_status", fc4sc::interval<uint32_t>(28u, 28u)), \
      bin<uint32_t>("host_nack_handler_timeout", fc4sc::interval<uint32_t>(100u, 100u)), \
      bin<uint32_t>("host_timeout_ctrl", fc4sc::interval<uint32_t>(80u, 80u)), \
      bin<uint32_t>("intr_enable", fc4sc::interval<uint32_t>(4u, 4u)), \
      bin<uint32_t>("intr_state", fc4sc::interval<uint32_t>(0u, 0u)), \
      bin<uint32_t>("intr_test", fc4sc::interval<uint32_t>(8u, 8u)), \
      bin<uint32_t>("ovrd", fc4sc::interval<uint32_t>(36u, 36u)), \
      bin<uint32_t>("rdata", fc4sc::interval<uint32_t>(8u, 8u)), \
      bin<uint32_t>("status", fc4sc::interval<uint32_t>(4u, 4u)), \
      bin<uint32_t>("target_ack_ctrl", fc4sc::interval<uint32_t>(92u, 92u)), \
      bin<uint32_t>("target_events", fc4sc::interval<uint32_t>(108u, 108u)), \
      bin<uint32_t>("target_fifo_config", fc4sc::interval<uint32_t>(24u, 24u)), \
      bin<uint32_t>("target_fifo_status", fc4sc::interval<uint32_t>(32u, 32u)), \
      bin<uint32_t>("target_id", fc4sc::interval<uint32_t>(68u, 68u)), \
      bin<uint32_t>("target_nack_count", fc4sc::interval<uint32_t>(88u, 88u)), \
      bin<uint32_t>("target_timeout_ctrl", fc4sc::interval<uint32_t>(84u, 84u)), \
      bin<uint32_t>("timeout_ctrl", fc4sc::interval<uint32_t>(64u, 64u)), \
      bin<uint32_t>("timing0", fc4sc::interval<uint32_t>(44u, 44u)), \
      bin<uint32_t>("timing1", fc4sc::interval<uint32_t>(48u, 48u)), \
      bin<uint32_t>("timing2", fc4sc::interval<uint32_t>(52u, 52u)), \
      bin<uint32_t>("timing3", fc4sc::interval<uint32_t>(56u, 56u)), \
      bin<uint32_t>("timing4", fc4sc::interval<uint32_t>(60u, 60u)), \
      bin<uint32_t>("txdata", fc4sc::interval<uint32_t>(76u, 76u)), \
      bin<uint32_t>("val", fc4sc::interval<uint32_t>(40u, 40u)), \
      bin<uint32_t>("others", fc4sc::interval<uint32_t>(0xFFFFFFFFu, 0xFFFFFFFFu)) \
  }; \
  cross<uint32_t,uint8_t> addr_x_rw = cross<uint32_t,uint8_t>(this, "addr_x_rw", &addr_cp, &is_write_cp);
#include "../../../dv/sc/coverage/csr_cov.hpp"
#endif

#include "../../../dv/sc/cip_lib/cip_base_env.hpp"
class i2c_env : public cip_base_env {
 public:
  UVM_COMPONENT_UTILS(i2c_env);
  explicit i2c_env(uvm::uvm_component_name name) : cip_base_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    cip_base_env::build_phase(phase);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    // CSR category registration for coverage
    { auto &cc = CsrCov::get();
      cc.register_w1c(0); cc.register_intr(0);
      cc.register_intr(8);
      cc.register_alert(12);
      cc.register_w1c(104);
      cc.register_w1c(108);
      cc.register_w1c(0);
      cc.register_rc(88);
    }
#endif
  }
};

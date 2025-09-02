#pragma once
#include <uvm>
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#define CSR_ADDR_BINS_DEF \
  COVERPOINT(uint32_t, addr_cp, addr_val_) { \
      bin<uint32_t>("alert_test", fc4sc::interval<uint32_t>(12u, 12u)), \
      bin<uint32_t>("attest_sw_binding[0]", fc4sc::interval<uint32_t>(32u, 32u)), \
      bin<uint32_t>("cfg_regwen", fc4sc::interval<uint32_t>(0u, 0u)), \
      bin<uint32_t>("control_shadowed", fc4sc::interval<uint32_t>(8u, 8u)), \
      bin<uint32_t>("debug", fc4sc::interval<uint32_t>(92u, 92u)), \
      bin<uint32_t>("err_code", fc4sc::interval<uint32_t>(84u, 84u)), \
      bin<uint32_t>("fault_status", fc4sc::interval<uint32_t>(88u, 88u)), \
      bin<uint32_t>("intr_enable", fc4sc::interval<uint32_t>(4u, 4u)), \
      bin<uint32_t>("intr_state", fc4sc::interval<uint32_t>(0u, 0u)), \
      bin<uint32_t>("intr_test", fc4sc::interval<uint32_t>(8u, 8u)), \
      bin<uint32_t>("key_version[0]", fc4sc::interval<uint32_t>(40u, 40u)), \
      bin<uint32_t>("max_creator_key_ver_regwen", fc4sc::interval<uint32_t>(44u, 44u)), \
      bin<uint32_t>("max_creator_key_ver_shadowed", fc4sc::interval<uint32_t>(48u, 48u)), \
      bin<uint32_t>("max_owner_int_key_ver_regwen", fc4sc::interval<uint32_t>(52u, 52u)), \
      bin<uint32_t>("max_owner_int_key_ver_shadowed", fc4sc::interval<uint32_t>(56u, 56u)), \
      bin<uint32_t>("max_owner_key_ver_regwen", fc4sc::interval<uint32_t>(60u, 60u)), \
      bin<uint32_t>("max_owner_key_ver_shadowed", fc4sc::interval<uint32_t>(64u, 64u)), \
      bin<uint32_t>("op_status", fc4sc::interval<uint32_t>(80u, 80u)), \
      bin<uint32_t>("reseed_interval_regwen", fc4sc::interval<uint32_t>(16u, 16u)), \
      bin<uint32_t>("reseed_interval_shadowed", fc4sc::interval<uint32_t>(20u, 20u)), \
      bin<uint32_t>("sealing_sw_binding[0]", fc4sc::interval<uint32_t>(28u, 28u)), \
      bin<uint32_t>("sideload_clear", fc4sc::interval<uint32_t>(12u, 12u)), \
      bin<uint32_t>("start", fc4sc::interval<uint32_t>(4u, 4u)), \
      bin<uint32_t>("sw_binding_regwen", fc4sc::interval<uint32_t>(24u, 24u)), \
      bin<uint32_t>("sw_share0_output[0]", fc4sc::interval<uint32_t>(68u, 68u)), \
      bin<uint32_t>("sw_share1_output[0]", fc4sc::interval<uint32_t>(72u, 72u)), \
      bin<uint32_t>("salt[0]", fc4sc::interval<uint32_t>(36u, 36u)), \
      bin<uint32_t>("working_state", fc4sc::interval<uint32_t>(76u, 76u)), \
      bin<uint32_t>("others", fc4sc::interval<uint32_t>(0xFFFFFFFFu, 0xFFFFFFFFu)) \
  }; \
  cross<uint32_t,uint8_t> addr_x_rw = cross<uint32_t,uint8_t>(this, "addr_x_rw", &addr_cp, &is_write_cp);
#include "../../../dv/sc/coverage/csr_cov.hpp"
#endif

#include "../../../dv/sc/cip_lib/cip_base_env.hpp"
class keymgr_env : public cip_base_env {
 public:
  UVM_COMPONENT_UTILS(keymgr_env);
  explicit keymgr_env(uvm::uvm_component_name name) : cip_base_env(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    cip_base_env::build_phase(phase);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    // CSR category registration for coverage
    { auto &cc = CsrCov::get();
      cc.register_w1c(0); cc.register_intr(0);
      cc.register_intr(8);
      cc.register_alert(12);
      cc.register_w1c(80);
      cc.register_w1c(84);
      cc.register_w1c(0);
      cc.register_shadow(8);
      cc.register_shadow(56);
      cc.register_shadow(20);
      cc.register_shadow(64);
      cc.register_shadow(48);
      cc.register_regwen(24);
      cc.register_regwen(12);
      cc.register_regwen(52);
      cc.register_regwen(8);
      cc.register_regwen(56);
      cc.register_regwen(0);
      cc.register_regwen(4);
      cc.register_regwen(60);
      cc.register_regwen(64);
      cc.register_regwen(48);
      cc.register_regwen(16);
      cc.register_regwen(20);
      cc.register_regwen(44);
    }
#endif
  }
};

#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
using namespace uvm;

class uart_window_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_window_test);
  uart_env* m_env {};
  explicit uart_window_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    if (tl_bind::reset) tl_bind::reset();
    const uint32_t RDATA = 24;       // RO
    const uint32_t WDATA = 28;       // WO
    const uint32_t FIFO_STATUS = 36; // RO
    const uint32_t STATUS = 20;      // RO

    // RO: write should have no effect
    uint32_t r0 = scdv::csr_rd(RDATA) & 0xFF;
    scdv::csr_wr(RDATA, 0xAA);
    uint32_t r1 = scdv::csr_rd(RDATA) & 0xFF;
    if (r1 != r0) {
      UVM_ERROR("WIN", "RDATA write should have no effect");
    }

    // WO: read should return 0
    scdv::csr_wr(WDATA, 0x5A);
    if ((scdv::csr_rd(WDATA) & 0xFF) != 0) {
      UVM_ERROR("WIN", "WDATA read should be 0 for WO");
    }

    // FIFO_STATUS (RO) write should have no effect
    uint32_t fs0 = scdv::csr_rd(FIFO_STATUS);
    scdv::csr_wr(FIFO_STATUS, 0xFFFFFFFF);
    uint32_t fs1 = scdv::csr_rd(FIFO_STATUS);
    if (fs1 != fs0) {
      UVM_ERROR("WIN", "FIFO_STATUS write should have no effect");
    }

    // STATUS (RO) write should have no effect
    uint32_t s0 = scdv::csr_rd(STATUS);
    scdv::csr_wr(STATUS, 0xFFFFFFFF);
    uint32_t s1 = scdv::csr_rd(STATUS);
    if (s1 != s0) {
      UVM_ERROR("WIN", "STATUS write should have no effect");
    }
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_window_test);



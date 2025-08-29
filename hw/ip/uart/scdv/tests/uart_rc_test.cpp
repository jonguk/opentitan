#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
using namespace uvm;

class uart_rc_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_rc_test);
  uart_env* m_env {};
  explicit uart_rc_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    if (tl_bind::reset) tl_bind::reset();
    // Use FIFO_CTRL (offset 32) where txrst/rxrst bits are clear-on-read in model
    const uint32_t FIFO_CTRL = 32;
    // Set both reset bits (assume bit positions 0 and 1)
    scdv::csr_wr(FIFO_CTRL, (1u << 0) | (1u << 1));
    // Read-clear: expect 0 after read
    if (m_env && m_env->scb) m_env->scb->push_expected(FIFO_CTRL, 0x0, 0x3);
    (void)scdv::csr_rd(FIFO_CTRL);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_rc_test);



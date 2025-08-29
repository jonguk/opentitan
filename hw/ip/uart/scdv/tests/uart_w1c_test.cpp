#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
using namespace uvm;

class uart_w1c_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_w1c_test);
  uart_env* m_env {};
  explicit uart_w1c_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    if (tl_bind::reset) tl_bind::reset();
    // INTR_STATE is W1C at offset 0. Set bits via intr_test (offset 8), then clear with W1C.
    const uint32_t INTR_STATE = 0;
    const uint32_t INTR_TEST  = 8;
    // Drive a couple of interrupt bits via intr_test
    scdv::csr_wr(INTR_TEST, (1u << 0) | (1u << 6)); // tx_empty, tx_done
    // Expect those bits to be 1 on read
    if (m_env && m_env->scb) m_env->scb->push_expected(INTR_STATE, (1u<<0) | (1u<<6));
    (void)scdv::csr_rd(INTR_STATE);
    // Clear one bit with W1C write (write 1 to clear that bit)
    scdv::csr_wr(INTR_STATE, (1u << 0));
    // Expect only tx_done to remain set
    if (m_env && m_env->scb) m_env->scb->push_expected(INTR_STATE, (1u<<6));
    (void)scdv::csr_rd(INTR_STATE);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_w1c_test);



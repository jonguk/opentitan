#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
using namespace uvm;

class uart_bit_bash_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_bit_bash_test);
  uart_env* m_env {};
  explicit uart_bit_bash_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    if (tl_bind::reset) tl_bind::reset();
    const uint32_t CTRL = 16; // rw
    // Sweep bits in CTRL: toggle TX/RX/PARITY_EN/PARITY_ODD
    uint32_t patterns[] = {0x0, 0x1, 0x2, 0x3, 0x40, 0x80, 0xC0, 0x1 | 0x2 | 0xC0};
    for (uint32_t v : patterns) {
      scdv::csr_wr(CTRL, v);
      if (m_env && m_env->scb) m_env->scb->push_expected(CTRL, v & 0x000700F3u, 0x000700F3u);
      (void)scdv::csr_rd(CTRL);
    }
    sc_core::sc_stop();
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_bit_bash_test);



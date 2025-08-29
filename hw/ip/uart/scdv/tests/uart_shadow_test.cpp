#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
using namespace uvm;

class uart_shadow_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_shadow_test);
  uart_env* m_env {};
  explicit uart_shadow_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    uvm::uvm_report_info("SKIP", "UART has no shadowed CSRs; skipping shadow test (SV behavior)", uvm::UVM_LOW);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_shadow_test);



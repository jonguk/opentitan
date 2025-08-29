#include <uvm>
#include "../env/uart_env.hpp"
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include <fc4sc/includes/fc4sc.hpp>
#include <fc4sc/includes/xml_printer.hpp>
#endif
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
#include "../../../dv/sc/scoreboard/scoreboard.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
using namespace uvm;

class uart_csr_rw_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_csr_rw_test);
  uart_env* m_env {};
  explicit uart_csr_rw_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    if (tl_bind::reset) tl_bind::reset();
    const uint32_t CTRL_ADDR = 16; // ctrl
    uint32_t ctrl = 0;
    ctrl |= (1u << 0); // tx
    ctrl |= (1u << 1); // rx
    ctrl |= (0x1234u << 16); // nco
    scdv::csr_wr(CTRL_ADDR, ctrl);
    // Expectation registered via monitor->scoreboard (SV style)
    if (m_env && m_env->scb) m_env->scb->push_expected(CTRL_ADDR, ctrl);
    const uint32_t INTR_ENABLE = 4;
    uint32_t ien = (1u<<0) | (1u<<3) | (1u<<7);
    scdv::csr_wr(INTR_ENABLE, ien);
    uint32_t ien_r = scdv::csr_rd(INTR_ENABLE);
    if ((ien_r & ien) != ien) {
      UVM_ERROR("CSR_RW", "INTR_ENABLE mismatch");
    }
    // WDATA is write-only; write should not read back
    const uint32_t WDATA = 28;
    scdv::csr_wr(WDATA, 0xAB);
    uint32_t wdata_r = scdv::csr_rd(WDATA);
    if ((wdata_r & 0xFF) != 0x00) {
      UVM_ERROR("CSR_RW", "WDATA should be WO and read as 0");
    }
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    std::cout << "[FC4SC] saving UCIS XML from uart_csr_rw_test..." << std::endl;
    xml_printer::coverage_save("/tmp/fc4sc_cov.ucis.xml", fc4sc::global::getter());
    std::cout << "[FC4SC] saved to /tmp/fc4sc_cov.ucis.xml" << std::endl;
#endif
    sc_core::sc_stop();
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_csr_rw_test);



#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
#include <tlm>
#include "../../../dv/sc/scoreboard/scoreboard.hpp"
using namespace uvm;

class uart_racl_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_racl_test);
  uart_env* m_env {};
  explicit uart_racl_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    // Enable RACL and configure policies
    if (tl_bind::set_racl_enable) tl_bind::set_racl_enable(true);
    if (tl_bind::set_racl_error_response) tl_bind::set_racl_error_response(true);
    // Index: INTR_ENABLE(1) allow; CTRL(4) disallow write; STATUS(5) allow read only
    if (tl_bind::set_reg_policy) {
      tl_bind::set_reg_policy(1, true, true);    // INTR_ENABLE allow all
      tl_bind::set_reg_policy(4, true, false);   // CTRL write disallowed
      tl_bind::set_reg_policy(5, true, false);   // STATUS write disallowed (RO-like)
      tl_bind::set_reg_policy(0, false, true);   // INTR_STATE read disallowed (for read-deny test)
    }

    // Monitor in env forwards to scoreboard (SV-style), no manual observer needed

    // Allowed write to INTR_ENABLE (offset 4)
    scdv::csr_wr(4, 0x1);
    if (m_env && m_env->scb) m_env->scb->push_expected(4, 0x1, 0x1);
    (void)scdv::csr_rd(4);

    // Disallowed write to CTRL (offset 16). Expect error response via direct TLM
    if (tl_bind::b_transport) {
      tlm::tlm_generic_payload t; sc_core::sc_time d = sc_core::SC_ZERO_TIME;
      uint32_t data = 0x3; unsigned char buf[4]; std::memcpy(buf, &data, 4);
      t.set_command(tlm::TLM_WRITE_COMMAND); t.set_address(16); t.set_data_length(4);
      t.set_streaming_width(4); t.set_byte_enable_ptr(nullptr); t.set_data_ptr(buf);
      tl_bind::b_transport(t, d);
      if (t.get_response_status() == tlm::TLM_OK_RESPONSE) {
        UVM_ERROR("RACL", "CTRL write should error (error mode)");
      } else {
        uvm::uvm_report_info("RACL", "CTRL write correctly returned error", uvm::UVM_LOW);
      }
    }
    // Verify masked (unchanged) value after denied write
    if (m_env && m_env->scb) m_env->scb->push_expected(16, 0x0, 0xFFFFFFFF);
    (void)scdv::csr_rd(16);

    // Disallowed read policy: INTR_STATE (offset 0) should error on read
    if (tl_bind::b_transport) {
      tlm::tlm_generic_payload t; sc_core::sc_time d = sc_core::SC_ZERO_TIME;
      unsigned char buf[4]{};
      t.set_command(tlm::TLM_READ_COMMAND); t.set_address(0); t.set_data_length(4);
      t.set_streaming_width(4); t.set_byte_enable_ptr(nullptr); t.set_data_ptr(buf);
      tl_bind::b_transport(t, d);
      if (t.get_response_status() == tlm::TLM_OK_RESPONSE) {
        UVM_ERROR("RACL", "INTR_STATE read should error (error mode)");
      } else {
        uvm::uvm_report_info("RACL", "INTR_STATE read correctly returned error", uvm::UVM_LOW);
      }
    }
    // Switch to mask (no-error) mode and re-try CTRL write: expect OK but value unchanged
    if (tl_bind::set_racl_error_response) tl_bind::set_racl_error_response(false);
    scdv::csr_wr(16, 0x3);
    if (m_env && m_env->scb) m_env->scb->push_expected(16, 0x0, 0xFFFFFFFF);
    (void)scdv::csr_rd(16);
    // Cleanup
    if (tl_bind::set_racl_enable) tl_bind::set_racl_enable(false);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_racl_test);



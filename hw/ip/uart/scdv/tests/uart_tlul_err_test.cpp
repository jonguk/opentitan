#include <uvm>
#include "../env/uart_env.hpp"
#include "../env/uvm_sc_compat.hpp"
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../../../dv/sc/csr_utils/csr_utils.hpp"
#include <tlm>
using namespace uvm;

class uart_tlul_err_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(uart_tlul_err_test);
  uart_env* m_env {};
  explicit uart_tlul_err_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = uart_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    if (tl_bind::reset) tl_bind::reset();
    // Helper to send a raw TLM GP
    auto send_gp = [](tlm::tlm_command cmd, uint64_t addr, unsigned len) {
      tlm::tlm_generic_payload t; sc_core::sc_time d = sc_core::SC_ZERO_TIME;
      unsigned char buf[8]{};
      t.set_command(cmd); t.set_address(addr);
      t.set_data_length(len); t.set_streaming_width(len);
      t.set_byte_enable_ptr(nullptr); t.set_data_ptr(buf);
      tl_bind::b_transport(t, d);
      return t.get_response_status();
    };

    // 1) Misaligned address (addr % 4 != 0) should error
    if (tl_bind::b_transport) {
      auto rs = send_gp(tlm::TLM_READ_COMMAND, /*addr=*/2, /*len=*/4);
      if (rs == tlm::TLM_OK_RESPONSE) {
        UVM_ERROR("TLUL_ERR", "Misaligned address read should error");
      } else {
        uvm::uvm_report_info("TLUL_ERR", "Misaligned address produced error as expected", uvm::UVM_LOW);
      }
    } else {
      UVM_FATAL("TLUL_ERR", "b_transport not bound");
    }

    // 2) Bad size (len != 4) should error
    {
      auto rs = send_gp(tlm::TLM_READ_COMMAND, /*addr=*/4, /*len=*/8);
      if (rs == tlm::TLM_OK_RESPONSE) {
        UVM_ERROR("TLUL_ERR", "Bad size read should error");
      }
    }

    // 3) Out-of-range address should address-error
    {
      auto rs = send_gp(tlm::TLM_READ_COMMAND, /*addr=*/0x100, /*len=*/4);
      if (rs == tlm::TLM_OK_RESPONSE) {
        UVM_ERROR("TLUL_ERR", "Out-of-range address should error");
      } else {
        uvm::uvm_report_info("TLUL_ERR", "Out-of-range address produced error as expected", uvm::UVM_LOW);
      }
    }
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(uart_tlul_err_test);



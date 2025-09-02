
#include <uvm>
#include "spi_host_env.hpp"
#include "uvm_sc_compat.hpp"
using namespace uvm;

class spi_host_smoke_test : public uvm_test {
 public:
  UVM_COMPONENT_UTILS(spi_host_smoke_test);
  spi_host_env* m_env {};
  explicit spi_host_smoke_test(uvm_component_name name) : uvm_test(name) {}
  void build_phase(uvm_phase &phase) override {
    uvm_test::build_phase(phase);
    m_env = spi_host_env::type_id::create("env", this);
  }
  void run_phase(uvm_phase &phase) override {
    phase.raise_objection(this);
    phase.drop_objection(this);
  }
};

UVM_COMPONENT_REGISTER(spi_host_smoke_test);

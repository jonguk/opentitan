#pragma once
#include "../env/csr_exclude.hpp"
namespace uart_test_cfg {
inline void register_exclusions() {
  using namespace scdv;
  // Example: skip writes to CTRL and STATUS in CSR tests
  add_excl("CTRL", ExclWrite);
  add_excl("STATUS", ExclWrite);
  // Example: skip reads of INTR_TEST (WO)
  add_excl("INTR_TEST", ExclRead);
}
}

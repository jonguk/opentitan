#pragma once
#include "../env/csr_exclude.hpp"
namespace i2c_test_cfg {
inline void register_exclusions() {
  using namespace scdv;
  // Example: skip reads of WDATA-like registers and writes to STATUS-like
  add_excl("WDATA", ExclRead);
  add_excl("STATUS", ExclWrite);
}
}

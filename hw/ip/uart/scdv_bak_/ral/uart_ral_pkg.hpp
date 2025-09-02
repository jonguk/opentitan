#pragma once
// SV-like naming for SystemC RAL package: uart_ral_pkg
// Re-exports the existing SystemC register model to keep names aligned

#include "../../tlm/uart_reg_pkg.hpp"

namespace uart_ral_pkg {
  using ::opentitan::uart::uart_reg_top;
}

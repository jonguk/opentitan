#pragma once
#include <uvm>
#include "uart_base_vseq.hpp"

// Common utility virtual sequence (placeholder)
class uart_common_vseq : public uart_base_vseq {
 public:
  UVM_OBJECT_UTILS(uart_common_vseq);
  explicit uart_common_vseq(const std::string &name = "uart_common_vseq") : uart_base_vseq(name) {}
};



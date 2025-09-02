#pragma once
#include <uvm>
#include "uart_base_vseq.hpp"
class uart_fifo_full_vseq : public uart_base_vseq {
 public:
  UVM_OBJECT_UTILS(uart_fifo_full_vseq);
  explicit uart_fifo_full_vseq(const std::string &name = "uart_fifo_full_vseq") : uart_base_vseq(name) {}
};



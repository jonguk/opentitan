#pragma once
#include <uvm>
#include "../../../dv/sc/tl_agent/tl_item.hpp"
#include "../../../dv/sc/csr_utils/csr_seq_lib.hpp"
class uart_csr_addr_sweep_seq : public uvm::uvm_sequence<tl_item> {
 public:
  UVM_OBJECT_UTILS(uart_csr_addr_sweep_seq);
  explicit uart_csr_addr_sweep_seq(const std::string &nm = "uart_csr_addr_sweep_seq") : uvm::uvm_sequence<tl_item>(nm) {}
  void body() override {
    const uint32_t addrs[] = { 0, 4, 8, 12, 16, 20, 24, 28, 32 };
    const uint32_t wmask[] = { 0xffff03f7, 0xffffffff, 0x0, 0x0, 0xfc, 0x0, 0x3, 0x0, 0x80ffffff };
    const uint32_t womask[] = { 0x0, 0x0, 0xffffffff, 0xff, 0x3, 0x0, 0x0, 0x0, 0x0 };
    const uint32_t w1cmask[] = { 0xffffffff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    const uint32_t rcmask[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    run_csr_rw_sweep(this, addrs, wmask, womask, w1cmask, rcmask, sizeof(addrs)/sizeof(addrs[0]));
  }
};

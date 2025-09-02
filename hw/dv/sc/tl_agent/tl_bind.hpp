#pragma once
#include <functional>
#include <vector>
#include <tlm>
#include <systemc>

namespace tl_bind {
  using BTransport = std::function<void(tlm::tlm_generic_payload&, sc_core::sc_time&)>;
  using ResetFn = std::function<void()>;
  inline BTransport b_transport;
  inline ResetFn reset;
  // Optional DUT control hooks (bound by main.cpp if available)
  inline std::function<void(bool)> set_racl_enable;
  inline std::function<void(bool)> set_racl_error_response;
  inline std::function<void(std::size_t,bool,bool)> set_reg_policy;
  inline std::function<void(uint32_t)> set_role_mask;
  // RAL hooks (bound by main.cpp)
  inline std::function<uint32_t(uint32_t)> ral_sw_read;
  inline std::function<void(uint32_t,uint32_t)> ral_sw_predict_write;
  inline std::function<uint32_t(void)> ral_num_bytes; // total CSR bytes
  inline std::function<uint32_t(void)> ral_word_bytes; // CSR word size in bytes
  // Observers (now include byte-enable mask for TL-UL PutPartial support)
  using ObserveFn = std::function<void(uint32_t addr, uint32_t data, bool is_write, uint32_t mask, uint8_t size_log2, bool d_error, uint8_t d_opcode, uint8_t a_opcode, uint8_t a_source)>;
  inline std::vector<ObserveFn> observers;
  inline void add_observer(const ObserveFn &fn) { observers.push_back(fn); }
}



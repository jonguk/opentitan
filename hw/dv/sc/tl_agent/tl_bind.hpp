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
  // Observers
  using ObserveFn = std::function<void(uint32_t addr, uint32_t data, bool is_write)>;
  inline std::vector<ObserveFn> observers;
  inline void add_observer(const ObserveFn &fn) { observers.push_back(fn); }
}



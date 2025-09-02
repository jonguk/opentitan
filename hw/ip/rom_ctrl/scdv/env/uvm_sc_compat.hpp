#pragma once
#include <uvm>
#include <systemc>
namespace uvm_sc_compat_detail {
  template <typename T>
  inline ::uvm::uvm_component_registry<T>* ensure_component_registered() { return ::uvm::uvm_component_registry<T>::get(); }
  template <typename T>
  inline ::uvm::uvm_object_registry<T>* ensure_object_registered() { return ::uvm::uvm_object_registry<T>::get(); }
}
#define UVM_COMPONENT_REGISTER(TYPE)   namespace { auto TYPE##_uvm_sc_reg = ::uvm_sc_compat_detail::ensure_component_registered<TYPE>(); }
#define UVM_OBJECT_REGISTER(TYPE)   namespace { auto TYPE##_uvm_sc_obj_reg = ::uvm_sc_compat_detail::ensure_object_registered<TYPE>(); }

// Drain helper: advance delta cycles to flush analysis/TLM activities
inline void drain_delta(unsigned num_cycles = 4) {
  for (unsigned i = 0; i < num_cycles; ++i) sc_core::wait(sc_core::SC_ZERO_TIME);
}

#pragma once
#include <uvm>
#include "tl_item.hpp"

// Skeleton device-mode driver to mirror SV tl_device_driver (not yet wired)
class tl_device_driver : public uvm::uvm_driver<tl_item> {
 public:
  UVM_COMPONENT_UTILS(tl_device_driver);
  explicit tl_device_driver(uvm::uvm_component_name name) : uvm::uvm_driver<tl_item>(name) {}

  void build_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_driver<tl_item>::build_phase(phase);
  }

  void run_phase(uvm::uvm_phase &phase) override {
    uvm::uvm_report_info("TL_DEV_DRV", "device driver idle (not wired)", uvm::UVM_LOW);
  }
};



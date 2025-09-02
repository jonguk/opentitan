#pragma once
#include <uvm>
#include <cstdint>
#include <unordered_map>
#include "../tl_agent/tl_bind.hpp"
#include "../scoreboard/scoreboard.hpp"
#include "../tl_agent/tl_reg_adapter.hpp"
#include "../bus_params_pkg/bus_params.hpp"

// Simple TL→RAL predictor: consumes reg_bus_op_sc, updates/checks RAL mirror
class uvm_reg_predictor : public uvm::uvm_component {
 public:
  UVM_COMPONENT_UTILS(uvm_reg_predictor);
  uvm::uvm_analysis_imp<reg_bus_op_sc, uvm_reg_predictor> imp;
  explicit uvm_reg_predictor(uvm::uvm_component_name nm)
      : uvm::uvm_component(nm), imp("imp", this) {}

  void write(const reg_bus_op_sc &op) {
    if (op.is_write) {
      // Merge write data into software mirror using byte enables
      const uint32_t cur = get_mirror(op.address);
      uint32_t out = 0;
      for (int i = 0; i < 4; ++i) {
        const bool byte_enabled = (op.byte_en_mask & (1u << i)) != 0;
        const uint32_t newb = (op.data >> (8 * i)) & 0xFFu;
        const uint32_t oldb = (cur     >> (8 * i)) & 0xFFu;
        const uint32_t selb = byte_enabled ? newb : oldb;
        out |= (selb & 0xFFu) << (8 * i);
      }
      mirror_[op.address] = out;
      if (tl_bind::ral_sw_predict_write) tl_bind::ral_sw_predict_write(op.address, out);
    } else {
      // Prefer RAL model if available
      uint32_t exp = tl_bind::ral_sw_read ? tl_bind::ral_sw_read(op.address) : get_mirror(op.address);
      if (exp != op.data) {
        uvm::uvm_report_error("SCB_MISMATCH", (std::string("@0x") + to_hex(op.address) + " exp=0x" + to_hex(exp) + " obs=0x" + to_hex(op.data)).c_str(), uvm::UVM_NONE);
      } else {
        uvm::uvm_report_info("SCB_MATCH", (std::string("@0x") + to_hex(op.address) + " =0x" + to_hex(exp)).c_str(), uvm::UVM_LOW);
      }
      // Mirror readback (no change) – keep mirror consistent
      mirror_[op.address] = op.data;
    }
    // Optional: basic response consistency (SV is_ok parity)
    const bool exp_write = op.is_write;
    const uint8_t exp_d = exp_write ? tlul::AccessAck : tlul::AccessAckData;
    if (op.d_opcode != exp_d) {
      uvm::uvm_report_warning("TL_RSP", "Unexpected d_opcode vs expected", uvm::UVM_LOW);
    }
    if (op.d_error) {
      uvm::uvm_report_warning("TL_RSP", "d_error asserted", uvm::UVM_LOW);
    }
    if (op.a_source != op.d_source) {
      uvm::uvm_report_warning("TL_RSP", "a_source != d_source", uvm::UVM_LOW);
    }
  }

 private:
  std::unordered_map<uint32_t, uint32_t> mirror_;
  uint32_t get_mirror(uint32_t address) const {
    auto it = mirror_.find(address);
    return (it == mirror_.end()) ? 0u : it->second;
  }
  static std::string to_hex(uint32_t v) {
    std::ostringstream oss; oss << std::hex << v; return oss.str();
  }
};



#pragma once
#include <uvm>
#include <unordered_map>
#include <deque>
#include <sstream>
#include <systemc>
#include "../tl_agent/tl_bind.hpp"
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include <fc4sc.hpp>
#include <xml_printer.hpp>
#include <json_printer.hpp>
#include <unistd.h>
#include <cstdlib>
#include <string>
#endif
// No direct RAL dependency; monitor performs RAL compare and logs

struct csr_event {
  uint32_t addr {0};
  uint32_t data {0};
  uint32_t mask {0xFFFFFFFFu};
  bool     is_write {false};
  uint8_t  size_log2 {2};
  bool     d_error {false};
  uint8_t  d_opcode {0};
  uint8_t  a_opcode {0};
  uint8_t  a_source {0};
  uint8_t  d_source {0};
};

class simple_scoreboard : public uvm::uvm_component {
 public:
  UVM_COMPONENT_UTILS(simple_scoreboard);
  // Analysis export to receive monitor events (SV-style)
  uvm::uvm_analysis_imp<csr_event, simple_scoreboard> item_collected_export;
  // Control whether to warn on observations without an expected (SV: often skipped)
  bool warn_on_unexpected {false};

  void set_warn_on_unexpected(bool en) { warn_on_unexpected = en; }

  // Summary counters
  std::size_t expected_count {0};
  std::size_t observed_count {0};
  std::size_t match_count {0};
  std::size_t mismatch_count {0};
  std::size_t unexpected_count {0};

  // Prefer RAL mirrored expectation for comparisons
  bool use_ral_expected {true};
  void set_use_ral_expected(bool en) { use_ral_expected = en; }

  explicit simple_scoreboard(uvm::uvm_component_name name)
      : uvm::uvm_component(name), item_collected_export("item_collected_export", this) {}

  void push_expected(uint32_t addr, uint32_t data, uint32_t mask = 0xFFFFFFFFu) {
    csr_event ev; ev.addr=addr; ev.data=data; ev.mask=mask; ev.is_write=false;
    expected_[addr].push_back(ev);
    ++expected_count;
  }

  void observe(uint32_t addr, uint32_t data) {
    ++observed_count;
    // Avoid duplicate comparisons when RAL predictor path is active (analysis write(ev) handles it with BE mask)
    if (use_ral_expected && tl_bind::ral_sw_read) return;
    // Fallback to queued-expected mechanism
    auto &q = expected_[addr];
    if (q.empty()) {
      ++unexpected_count;
      if (warn_on_unexpected) {
        uvm::uvm_report_warning("SCB_UNEXPECTED", (std::string("obs without exp @0x") + to_hex(addr)).c_str(), uvm::UVM_LOW);
      }
      return;
    }
    csr_event ev = q.front(); q.pop_front();
    uint32_t obs_m = data & ev.mask;
    uint32_t exp_m = ev.data & ev.mask;
    if (obs_m != exp_m) {
      ++mismatch_count;
      uvm::uvm_report_error("SCB_MISMATCH", (std::string("@0x") + to_hex(addr) + " exp=0x" + to_hex(exp_m) + " obs=0x" + to_hex(obs_m)).c_str(), uvm::UVM_NONE);
    } else {
      ++match_count;
      uvm::uvm_report_info("SCB_MATCH", (std::string("@0x") + to_hex(addr) + " =0x" + to_hex(obs_m)).c_str(), uvm::UVM_LOW);
    }
  }

  // SV-style analysis export callback
  void write(const csr_event &ev) {
    // In RAL-based flow, expected push happens via predictor; keep legacy path off by default
    if (ev.is_write) {
      if (warn_on_unexpected) { /* no-op for write unless legacy enabled */ }
    } else {
      if (use_ral_expected && tl_bind::ral_sw_read) {
        ++observed_count;
        ++expected_count;
        const uint32_t exp = tl_bind::ral_sw_read(ev.addr);
        // Build 32-bit mask from TL byte-enable lanes
        uint32_t be_mask = 0;
        for (int i = 0; i < 4; ++i) {
          if (ev.mask & (1u << i)) be_mask |= (0xFFu << (8 * i));
        }
        if (be_mask == 0) be_mask = 0xFFFFFFFFu;
        const uint32_t obs_m = ev.data & be_mask;
        const uint32_t exp_m = exp     & be_mask;
        if (obs_m != exp_m) {
          ++mismatch_count;
          uvm::uvm_report_error("SCB_MISMATCH", (std::string("@0x") + to_hex(ev.addr) + " exp=0x" + to_hex(exp_m) + " obs=0x" + to_hex(obs_m)).c_str(), uvm::UVM_NONE);
        } else {
          ++match_count;
          uvm::uvm_report_info("SCB_MATCH", (std::string("@0x") + to_hex(ev.addr) + " =0x" + to_hex(obs_m)).c_str(), uvm::UVM_LOW);
        }
        return;
      }
      observe(ev.addr, ev.data);
    }
  }

  void report_phase(uvm::uvm_phase &phase) override {
    (void)phase;
    std::ostringstream oss;
    oss << "SCB Summary: expected=" << expected_count
        << " observed=" << observed_count
        << " match=" << match_count
        << " mismatch=" << mismatch_count
        << " unexpected=" << unexpected_count;
    uvm::uvm_report_info("SCB_SUMMARY", oss.str(), uvm::UVM_LOW);
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    // Dump fc4sc coverage report (UCIS XML/JSON) with per-process unique filenames
    try {
      const char *outdir_env = std::getenv("FC4SC_OUTDIR");
      const char *basename_env = std::getenv("FC4SC_BASENAME");
      std::string outdir = outdir_env ? std::string(outdir_env) : std::string("/tmp");
      std::string base = basename_env ? std::string(basename_env) : std::string("fc4sc_cov");
      pid_t pid = getpid();
      std::string xml_path = outdir + "/" + base + "." + std::to_string(static_cast<long long>(pid)) + ".ucis.xml";
      std::string json_path = outdir + "/" + base + "." + std::to_string(static_cast<long long>(pid)) + ".json";
      xml_printer::coverage_save(xml_path.c_str(), fc4sc::global::getter());
      uvm::uvm_report_info("COV", (xml_path + " written").c_str(), uvm::UVM_LOW);
      json_printer::coverage_save(json_path.c_str(), fc4sc::global::getter());
      uvm::uvm_report_info("COV", (json_path + " written").c_str(), uvm::UVM_LOW);
    } catch (...) {
      uvm::uvm_report_warning("COV", "Failed to write FC4SC coverage files", uvm::UVM_LOW);
    }
#endif
  }

  // Query pending expected items across all addresses (for drain logic)
  std::size_t pending_expected_total() const {
    std::size_t total = 0;
    for (const auto &kv : expected_) total += kv.second.size();
    return total;
  }

  bool has_pending() const { return pending_expected_total() != 0; }

  // Flush all expected events (helpful to reset state between tests)
  void flush_expected() { expected_.clear(); }

  // Clear all counters and expected queues
  void clear() {
    expected_count = observed_count = match_count = mismatch_count = unexpected_count = 0;
    expected_.clear();
  }

  // No special drain; monitor publishes immediately

 private:
  static std::string to_hex(uint32_t v) {
    std::ostringstream oss; oss << std::hex << v; return oss.str();
  }
  std::unordered_map<uint32_t, std::deque<csr_event>> expected_;
};



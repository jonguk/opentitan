#pragma once
#include <uvm>
#include <unordered_map>
#include <deque>
#include <sstream>
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include <fc4sc/includes/fc4sc.hpp>
#include <fc4sc/includes/xml_printer.hpp>
#endif

struct csr_event {
  uint32_t addr {0};
  uint32_t data {0};
  uint32_t mask {0xFFFFFFFFu};
  bool     is_write {false};
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

  explicit simple_scoreboard(uvm::uvm_component_name name)
      : uvm::uvm_component(name), item_collected_export("item_collected_export", this) {}

  void push_expected(uint32_t addr, uint32_t data, uint32_t mask = 0xFFFFFFFFu) {
    expected_[addr].push_back({addr, data, mask, false});
    ++expected_count;
  }

  void observe(uint32_t addr, uint32_t data) {
    ++observed_count;
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
    if (ev.is_write) push_expected(ev.addr, ev.data, ev.mask);
    else observe(ev.addr, ev.data);
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
    // Dump fc4sc coverage report (UCIS XML)
    try {
      xml_printer::coverage_save("/tmp/fc4sc_cov.ucis.xml", fc4sc::global::getter());
      uvm::uvm_report_info("COV", "/tmp/fc4sc_cov.ucis.xml written", uvm::UVM_LOW);
    } catch (...) {
      uvm::uvm_report_warning("COV", "Failed to write fc4sc_cov.ucis.xml", uvm::UVM_LOW);
    }
#endif
  }

 private:
  static std::string to_hex(uint32_t v) {
    std::ostringstream oss; oss << std::hex << v; return oss.str();
  }
  std::unordered_map<uint32_t, std::deque<csr_event>> expected_;
};



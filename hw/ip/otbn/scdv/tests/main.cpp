#include <uvm>
#include <string>
#include <systemc>
#include <ctime>
#include <cstdlib>
#include <tlm_utils/simple_initiator_socket.h>
#include "../../../dv/sc/tl_agent/tl_bind.hpp"
#include "../env/csr_exclude.hpp"
#include "../../tlm/otbn_reg_pkg.hpp"
using namespace uvm;

static const char *get_testname_from_argv(int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i] ? argv[i] : "");
    const std::string k1 = "+UVM_TESTNAME=";
    const std::string k2 = "UVM_TESTNAME=";
    if (arg.rfind(k1, 0) == 0) return argv[i] + k1.size();
    if (arg.rfind(k2, 0) == 0) return argv[i] + k2.size();
  }
  return nullptr;
}

static std::string get_plusarg_str(int argc, char **argv, const std::string &key) {
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i] ? argv[i] : "");
    if (arg.rfind(key, 0) == 0) return arg.substr(key.size());
  }
  return "";
}

static bool has_plusarg(int argc, char **argv, const std::string &flag) {
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i] ? argv[i] : "");
    if (arg == flag) return true;
  }
  return false;
}

static sc_core::sc_time parse_time_arg(const std::string &s) {
  if (s.empty()) return sc_core::SC_ZERO_TIME;
  char *endp = nullptr;
  double val = std::strtod(s.c_str(), &endp);
  std::string unit = endp ? std::string(endp) : std::string();
  if (unit == "s" || unit == "S") return sc_core::sc_time(val, sc_core::SC_SEC);
  if (unit == "ms" || unit == "MS") return sc_core::sc_time(val, sc_core::SC_MS);
  if (unit == "us" || unit == "US") return sc_core::sc_time(val, sc_core::SC_US);
  if (unit == "ns" || unit == "NS") return sc_core::sc_time(val, sc_core::SC_NS);
  if (unit == "ps" || unit == "PS") return sc_core::sc_time(val, sc_core::SC_PS);
  return sc_core::sc_time(val, sc_core::SC_NS);
}

static void apply_uvm_flags(int argc, char **argv) {
  std::string verb = get_plusarg_str(argc, argv, "+UVM_VERBOSITY=");
  if (!verb.empty()) {
    uvm::uvm_verbosity v = uvm::UVM_MEDIUM;
    if (verb == "UVM_NONE") v = uvm::UVM_NONE;
    else if (verb == "UVM_LOW") v = uvm::UVM_LOW;
    else if (verb == "UVM_MEDIUM") v = uvm::UVM_MEDIUM;
    else if (verb == "UVM_HIGH") v = uvm::UVM_HIGH;
    else if (verb == "UVM_FULL") v = uvm::UVM_FULL;
    uvm::uvm_root::get()->set_report_verbosity_level_hier(v);
  }
  std::string mqc = get_plusarg_str(argc, argv, "+UVM_MAX_QUIT_COUNT=");
  if (!mqc.empty()) { int n = std::atoi(mqc.c_str()); (void)n; }
  std::string to = get_plusarg_str(argc, argv, "+UVM_TIMEOUT=");
  if (!to.empty()) { sc_core::sc_time t = parse_time_arg(to);
    sc_core::sc_spawn([t]{ sc_core::wait(t); uvm::uvm_report_fatal("UVM_TIMEOUT", "Simulation timed out", uvm::UVM_NONE); }); }
  std::string seed = get_plusarg_str(argc, argv, "+ntb_random_seed=");
  if (!seed.empty()) { unsigned s = 1u; if (seed == "random") s = static_cast<unsigned>(std::time(nullptr));
    else s = static_cast<unsigned>(std::strtoul(seed.c_str(), nullptr, 10)); std::srand(s);
    uvm::uvm_report_info("SEED", std::string("ntb_random_seed=") + std::to_string(s), uvm::UVM_LOW); }
  if (has_plusarg(argc, argv, "+UVM_PHASE_TRACE")) uvm::uvm_report_info("UVM", "Phase trace requested", uvm::UVM_LOW);
  if (has_plusarg(argc, argv, "+UVM_OBJECTION_TRACE")) uvm::uvm_report_info("UVM", "Objection trace requested", uvm::UVM_LOW);
  if (has_plusarg(argc, argv, "+UVM_CONFIG_DB_TRACE")) uvm::uvm_report_info("UVM", "Config DB trace requested", uvm::UVM_LOW);
  if (has_plusarg(argc, argv, "+UVM_RESOURCE_DB_TRACE")) uvm::uvm_report_info("UVM", "Resource DB trace requested", uvm::UVM_LOW);
  if (has_plusarg(argc, argv, "+UVM_NO_RELNOTES")) uvm::uvm_report_info("UVM", "Suppress release notes", uvm::UVM_LOW);
}

int sc_main(int argc, char **argv) {
  struct top : sc_core::sc_module {
    tlm_utils::simple_initiator_socket<top> init;
    opentitan::otbn::otbn_reg_top dut;
    SC_HAS_PROCESS(top);
    top(sc_core::sc_module_name n) : sc_core::sc_module(n), init("init"), dut("dut") { init.bind(dut.tlm_target); }
  } top_i("top");
  tl_bind::b_transport = [&](tlm::tlm_generic_payload &t, sc_core::sc_time &d){ top_i.init->b_transport(t, d); };
  tl_bind::reset = [&](){ top_i.dut.reset(); };
  tl_bind::set_racl_enable = [&](bool e){ top_i.dut.set_racl_enable(e); };
  tl_bind::set_racl_error_response = [&](bool e){ top_i.dut.set_racl_error_response(e); };
  tl_bind::set_reg_policy = [&](std::size_t idx, bool rd, bool wr){ top_i.dut.set_reg_policy(idx, rd, wr); };
  tl_bind::set_role_mask = [&](uint32_t m){ top_i.dut.set_role_mask(m); };
  // Bind RAL hooks to DUT's software model for accurate prediction
  tl_bind::ral_sw_read = [&](uint32_t addr) -> uint32_t { return top_i.dut.sw_read(addr); };
  tl_bind::ral_sw_predict_write = [&](uint32_t addr, uint32_t data){ top_i.dut.sw_predict_write(addr, data); };
  tl_bind::ral_num_bytes = [&](){ return static_cast<uint32_t>(opentitan::otbn::otbn_reg_top::kRegBytes); };
  tl_bind::ral_word_bytes = [&](){ return 4u; };

  // Initialize default CSR exclusions derived from HJSON
  scdv::init_default_exclusions();

  apply_uvm_flags(argc, argv);
  if (const char *tn = get_testname_from_argv(argc, argv)) { uvm::run_test(tn); } else {
    std::string prog = std::string(argv && argv[0] ? argv[0] : "");
    std::size_t pos = prog.find_last_of('/');
    if (pos != std::string::npos) prog = prog.substr(pos + 1);
    if (!prog.empty()) uvm::run_test(prog.c_str()); else uvm::run_test();
  }
  return 0;
}

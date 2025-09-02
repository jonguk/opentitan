#pragma once
#include <fstream>
#include <iomanip>
#include <string>
#include "fc4sc_base.hpp"

// Minimal JSON coverage printer for fc4sc (structure: scopes -> covergroups -> coverpoints -> bins)
namespace json_printer {

inline void coverage_save(const std::string &path, fc4sc::global *g = fc4sc::global::getter()) {
  std::ofstream ofs(path);
  if (!ofs.is_open()) return;
  ofs << "{\n  \"scopes\": [\n";
  const auto &scopes = fc4sc::global::get_top_scopes(g);
  for (size_t si = 0; si < scopes.size(); ++si) {
    const fc4sc::scp_base_data_model *s = scopes[si];
    ofs << "    {\n      \"name\": \"" << s->name << "\",\n      \"covergroups\": [\n";
    size_t gi = 0; size_t gcount = s->cvg_insts.size();
    for (const auto &kv : s->cvg_insts) {
      const fc4sc::cvg_base_data_model *cg = kv.second;
      ofs << "        {\n          \"name\": \"" << cg->name << "\",\n          \"coverpoints\": [\n";
      for (size_t pi = 0; pi < cg->cvps.size(); ++pi) {
        const fc4sc::cvp_base_data_model *cp = cg->cvps[pi];
        ofs << "            {\n              \"name\": \"" << cp->name << "\",\n              \"bins\": [\n";
        const auto *cpd = dynamic_cast<const fc4sc::coverpoint_base_data_model*>(cp);
        if (cpd) {
          const auto &bins = cpd->bins_data;
          for (size_t bi = 0; bi < bins.size(); ++bi) {
            auto *b = bins[bi];
            uint64_t total_hits = 0; for (auto h : b->get_interval_hits()) total_hits += h;
            int goal = 1; bool illegal = false; bool ignore = false; int weight = 1;
            ofs << "                { \"name\": \"" << b->get_name() << "\", \"hits\": " << total_hits
                << ", \"goal\": " << goal << ", \"illegal\": " << (illegal?"true":"false")
                << ", \"ignore\": " << (ignore?"true":"false") << ", \"weight\": " << weight << " }";
            ofs << (bi + 1 < bins.size() ? ",\n" : "\n");
          }
        }
        ofs << "              ]\n            }";
        ofs << (pi + 1 < cg->cvps.size() ? ",\n" : "\n");
      }
      ofs << "          ]\n        }";
      ofs << (++gi < gcount ? ",\n" : "\n");
    }
    ofs << "      ]\n    }";
    ofs << (si + 1 < scopes.size() ? ",\n" : "\n");
  }
  ofs << "  ]\n}\n";
}

} // namespace json_printer



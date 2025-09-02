#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "csr_map.hpp"
namespace scdv {
  enum CsrExclType : unsigned { ExclNone=0, ExclWrite=1u<<0, ExclRead=1u<<1, ExclAll=ExclWrite|ExclRead };
  inline bool glob_match(const char *pat, const char *str) {
    // simple '*' and '?' matcher
    if (!pat || !str) return false;
    const char *s = str, *p = pat, *star = nullptr, *ss = nullptr;
    while (*s) {
      if (*p == '?' || *p == *s) { ++p; ++s; continue; }
      if (*p == '*') { star = p++; ss = s; continue; }
      if (star) { p = star + 1; s = ++ss; continue; }
      return false;
    }
    while (*p == '*') ++p;
    return *p == 0;
  }
  inline std::vector<std::pair<std::string,unsigned>>& excl_rules() { static std::vector<std::pair<std::string,unsigned>> r; return r; }
  inline void add_excl(const std::string &pattern, unsigned flags) { excl_rules().emplace_back(pattern, flags); }
  inline unsigned query_flags_by_name(const char *name) { for (auto &pr : excl_rules()) if (glob_match(pr.first.c_str(), name)) return pr.second; return 0u; }
  inline const char* name_from_addr(uint32_t addr) { for (std::size_t i=0;i<csr_map_size;++i) if (csr_map[i].addr==addr) return csr_map[i].name; return (const char*)nullptr; }
  inline bool exclude_write(uint32_t addr) { if (const char* nm = name_from_addr(addr)) return (query_flags_by_name(nm) & ExclWrite)!=0; return false; }
  inline bool exclude_read(uint32_t addr)  { if (const char* nm = name_from_addr(addr)) return (query_flags_by_name(nm) & ExclRead)!=0; return false; }
  struct default_excl_t { const char* name; unsigned flags; };
  inline const default_excl_t* get_default_exclusions(size_t &n) {
    static const default_excl_t defs[] = {
  { "INTR_STATE", ExclWrite },
  { "INTR_TEST", ExclRead },
  { "ALERT_TEST", ExclWrite },
  { "STATUS", ExclWrite },
  { "RDATA", ExclWrite },
  { "FDATA", ExclRead },
  { "FIFO_CTRL", ExclRead },
  { "HOST_FIFO_STATUS", ExclWrite },
  { "TARGET_FIFO_STATUS", ExclWrite },
  { "VAL", ExclWrite },
  { "ACQDATA", ExclWrite },
  { "TXDATA", ExclRead },
  { "TARGET_NACK_COUNT", ExclWrite },
  { "ACQ_FIFO_NEXT_DATA", ExclWrite },
  { "CONTROLLER_EVENTS", ExclWrite },
  { "TARGET_EVENTS", ExclWrite }
    };
    n = sizeof(defs)/sizeof(defs[0]); return defs;
  }
  inline void init_default_exclusions() { size_t n=0; auto arr = get_default_exclusions(n); for (size_t i=0;i<n;++i) add_excl(arr[i].name, arr[i].flags); }
}

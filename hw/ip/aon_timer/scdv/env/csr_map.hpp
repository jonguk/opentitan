#pragma once
#include <cstdint>
#include <cstddef>
struct csr_name_offset { const char *name; uint32_t addr; };
namespace scdv {
  static constexpr csr_name_offset csr_map[] = {
  { "INTR_ENABLE", 4 },
  { "ALERT_TEST", 12 },
  { "WKUP_CTRL", 0 },
  { "WKUP_THOLD_HI", 4 },
  { "WKUP_THOLD_LO", 8 },
  { "WKUP_COUNT_HI", 12 },
  { "WKUP_COUNT_LO", 16 },
  { "WDOG_REGWEN", 20 },
  { "WDOG_CTRL", 24 },
  { "WDOG_BARK_THOLD", 28 },
  { "WDOG_BITE_THOLD", 32 },
  { "WDOG_COUNT", 36 },
  { "INTR_STATE", 40 },
  { "INTR_TEST", 44 },
  { "WKUP_CAUSE", 48 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

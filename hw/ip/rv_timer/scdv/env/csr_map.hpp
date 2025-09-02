#pragma once
#include <cstdint>
#include <cstddef>
struct csr_name_offset { const char *name; uint32_t addr; };
namespace scdv {
  static constexpr csr_name_offset csr_map[] = {
  { "INTR_STATE", 0 },
  { "INTR_ENABLE", 4 },
  { "INTR_TEST", 8 },
  { "ALERT_TEST", 12 },
  { "CTRL[0]", 0 },
  { "INTR_ENABLE0[0]", 8 },
  { "INTR_STATE0[0]", 12 },
  { "INTR_TEST0[0]", 16 },
  { "CFG0", 20 },
  { "TIMER_V_LOWER0", 24 },
  { "TIMER_V_UPPER0", 28 },
  { "COMPARE_LOWER0_0", 32 },
  { "COMPARE_UPPER0_0", 36 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

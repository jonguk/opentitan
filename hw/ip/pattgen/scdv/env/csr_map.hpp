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
  { "CTRL", 0 },
  { "PREDIV_CH0", 4 },
  { "PREDIV_CH1", 8 },
  { "DATA_CH0[0]", 12 },
  { "DATA_CH1[0]", 16 },
  { "SIZE", 20 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

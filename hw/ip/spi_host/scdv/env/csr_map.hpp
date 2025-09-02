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
  { "CONTROL", 0 },
  { "STATUS", 4 },
  { "CONFIGOPTS", 8 },
  { "CSID", 12 },
  { "COMMAND", 16 },
  { "ERROR_ENABLE", 28 },
  { "ERROR_STATUS", 32 },
  { "EVENT_ENABLE", 36 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

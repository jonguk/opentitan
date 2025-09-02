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
  { "STATUS", 4 },
  { "RDATA", 8 },
  { "WDATA", 12 },
  { "FIFO_CTRL", 16 },
  { "FIFO_STATUS", 20 },
  { "OVRD", 24 },
  { "VAL", 28 },
  { "TIMEOUT_CTRL", 32 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

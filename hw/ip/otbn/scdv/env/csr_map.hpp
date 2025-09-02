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
  { "CMD", 0 },
  { "CTRL", 4 },
  { "STATUS", 8 },
  { "ERR_BITS", 12 },
  { "FATAL_ALERT_CAUSE", 16 },
  { "INSN_CNT", 20 },
  { "LOAD_CHECKSUM", 24 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

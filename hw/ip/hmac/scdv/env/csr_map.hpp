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
  { "CFG", 0 },
  { "CMD", 4 },
  { "STATUS", 8 },
  { "ERR_CODE", 12 },
  { "WIPE_SECRET", 16 },
  { "KEY[0]", 20 },
  { "DIGEST[0]", 24 },
  { "MSG_LENGTH_LOWER", 28 },
  { "MSG_LENGTH_UPPER", 32 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

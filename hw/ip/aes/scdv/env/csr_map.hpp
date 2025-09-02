#pragma once
#include <cstdint>
#include <cstddef>
struct csr_name_offset { const char *name; uint32_t addr; };
namespace scdv {
  static constexpr csr_name_offset csr_map[] = {
  { "ALERT_TEST", 12 },
  { "KEY_SHARE0[0]", 0 },
  { "KEY_SHARE1[0]", 4 },
  { "IV[0]", 8 },
  { "DATA_IN[0]", 12 },
  { "DATA_OUT[0]", 16 },
  { "CTRL_SHADOWED", 20 },
  { "CTRL_AUX_SHADOWED", 24 },
  { "CTRL_AUX_REGWEN", 28 },
  { "TRIGGER", 32 },
  { "STATUS", 36 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

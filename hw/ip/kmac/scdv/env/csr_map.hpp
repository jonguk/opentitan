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
  { "CFG_REGWEN", 0 },
  { "CFG_SHADOWED", 4 },
  { "CMD", 8 },
  { "STATUS", 12 },
  { "ENTROPY_PERIOD", 16 },
  { "ENTROPY_REFRESH_HASH_CNT", 20 },
  { "ENTROPY_REFRESH_THRESHOLD_SHADOWED", 24 },
  { "ENTROPY_SEED", 28 },
  { "KEY_SHARE0[0]", 32 },
  { "KEY_SHARE1[0]", 36 },
  { "KEY_LEN", 40 },
  { "PREFIX[0]", 44 },
  { "ERR_CODE", 48 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

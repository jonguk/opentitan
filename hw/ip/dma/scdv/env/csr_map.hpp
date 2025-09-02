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
  { "SRC_ADDR_LO", 0 },
  { "SRC_ADDR_HI", 4 },
  { "DST_ADDR_LO", 8 },
  { "DST_ADDR_HI", 12 },
  { "ADDR_SPACE_ID", 16 },
  { "ENABLED_MEMORY_RANGE_BASE", 20 },
  { "ENABLED_MEMORY_RANGE_LIMIT", 24 },
  { "RANGE_VALID", 28 },
  { "RANGE_REGWEN", 32 },
  { "CFG_REGWEN", 36 },
  { "TOTAL_DATA_SIZE", 40 },
  { "CHUNK_DATA_SIZE", 44 },
  { "TRANSFER_WIDTH", 48 },
  { "CONTROL", 52 },
  { "SRC_CONFIG", 56 },
  { "DST_CONFIG", 60 },
  { "STATUS", 64 },
  { "ERROR_CODE", 68 },
  { "SHA2_DIGEST[0]", 72 },
  { "SHA2_DIGEST[1]", 76 },
  { "SHA2_DIGEST[2]", 80 },
  { "SHA2_DIGEST[3]", 84 },
  { "SHA2_DIGEST[4]", 88 },
  { "SHA2_DIGEST[5]", 92 },
  { "SHA2_DIGEST[6]", 96 },
  { "SHA2_DIGEST[7]", 100 },
  { "SHA2_DIGEST[8]", 104 },
  { "SHA2_DIGEST[9]", 108 },
  { "SHA2_DIGEST[10]", 112 },
  { "SHA2_DIGEST[11]", 116 },
  { "SHA2_DIGEST[12]", 120 },
  { "SHA2_DIGEST[13]", 124 },
  { "SHA2_DIGEST[14]", 128 },
  { "SHA2_DIGEST[15]", 132 },
  { "HANDSHAKE_INTR_ENABLE", 136 },
  { "CLEAR_INTR_SRC", 140 },
  { "CLEAR_INTR_BUS", 144 },
  { "INTR_SRC_ADDR[0]", 148 },
  { "INTR_SRC_WR_VAL[0]", 156 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

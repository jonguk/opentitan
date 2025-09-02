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
  { "START", 4 },
  { "CONTROL_SHADOWED", 8 },
  { "SIDELOAD_CLEAR", 12 },
  { "RESEED_INTERVAL_REGWEN", 16 },
  { "RESEED_INTERVAL_SHADOWED", 20 },
  { "SW_BINDING_REGWEN", 24 },
  { "SEALING_SW_BINDING[0]", 28 },
  { "ATTEST_SW_BINDING[0]", 32 },
  { "Salt[0]", 36 },
  { "KEY_VERSION[0]", 40 },
  { "MAX_CREATOR_KEY_VER_REGWEN", 44 },
  { "MAX_CREATOR_KEY_VER_SHADOWED", 48 },
  { "MAX_OWNER_INT_KEY_VER_REGWEN", 52 },
  { "MAX_OWNER_INT_KEY_VER_SHADOWED", 56 },
  { "MAX_OWNER_KEY_VER_REGWEN", 60 },
  { "MAX_OWNER_KEY_VER_SHADOWED", 64 },
  { "SW_SHARE0_OUTPUT[0]", 68 },
  { "SW_SHARE1_OUTPUT[0]", 72 },
  { "WORKING_STATE", 76 },
  { "OP_STATUS", 80 },
  { "ERR_CODE", 84 },
  { "FAULT_STATUS", 88 },
  { "DEBUG", 92 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

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
  { "SLOT_POLICY_REGWEN", 24 },
  { "SLOT_POLICY", 28 },
  { "SW_BINDING_REGWEN", 32 },
  { "SW_BINDING[0]", 36 },
  { "SALT[0]", 40 },
  { "KEY_VERSION[0]", 44 },
  { "MAX_KEY_VER_REGWEN", 48 },
  { "MAX_KEY_VER_SHADOWED", 52 },
  { "SW_SHARE0_OUTPUT[0]", 56 },
  { "SW_SHARE1_OUTPUT[0]", 60 },
  { "WORKING_STATE", 64 },
  { "OP_STATUS", 68 },
  { "ERR_CODE", 72 },
  { "FAULT_STATUS", 76 },
  { "DEBUG", 80 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

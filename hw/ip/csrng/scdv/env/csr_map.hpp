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
  { "REGWEN", 0 },
  { "CTRL", 4 },
  { "CMD_REQ", 8 },
  { "RESEED_INTERVAL", 12 },
  { "RESEED_COUNTER[0]", 16 },
  { "SW_CMD_STS", 20 },
  { "GENBITS_VLD", 24 },
  { "GENBITS", 28 },
  { "INT_STATE_READ_ENABLE", 32 },
  { "INT_STATE_READ_ENABLE_REGWEN", 36 },
  { "INT_STATE_NUM", 40 },
  { "INT_STATE_VAL", 44 },
  { "FIPS_FORCE", 48 },
  { "HW_EXC_STS", 52 },
  { "RECOV_ALERT_STS", 56 },
  { "ERR_CODE", 60 },
  { "ERR_CODE_TEST", 64 },
  { "MAIN_SM_STATE", 68 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

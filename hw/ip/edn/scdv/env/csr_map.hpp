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
  { "BOOT_INS_CMD", 8 },
  { "BOOT_GEN_CMD", 12 },
  { "SW_CMD_REQ", 16 },
  { "SW_CMD_STS", 20 },
  { "HW_CMD_STS", 24 },
  { "RESEED_CMD", 28 },
  { "GENERATE_CMD", 32 },
  { "MAX_NUM_REQS_BETWEEN_RESEEDS", 36 },
  { "RECOV_ALERT_STS", 40 },
  { "ERR_CODE", 44 },
  { "ERR_CODE_TEST", 48 },
  { "MAIN_SM_STATE", 52 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

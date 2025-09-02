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
  { "EC_RST_CTL", 4 },
  { "ULP_AC_DEBOUNCE_CTL", 8 },
  { "ULP_LID_DEBOUNCE_CTL", 12 },
  { "ULP_PWRB_DEBOUNCE_CTL", 16 },
  { "ULP_CTL", 20 },
  { "ULP_STATUS", 24 },
  { "WKUP_STATUS", 28 },
  { "KEY_INVERT_CTL", 32 },
  { "PIN_ALLOWED_CTL", 36 },
  { "PIN_OUT_CTL", 40 },
  { "PIN_OUT_VALUE", 44 },
  { "PIN_IN_VALUE", 48 },
  { "KEY_INTR_CTL", 52 },
  { "KEY_INTR_DEBOUNCE_CTL", 56 },
  { "AUTO_BLOCK_DEBOUNCE_CTL", 60 },
  { "AUTO_BLOCK_OUT_CTL", 64 },
  { "COM_PRE_SEL_CTL[0]", 68 },
  { "COM_PRE_DET_CTL[0]", 72 },
  { "COM_SEL_CTL[0]", 76 },
  { "COM_DET_CTL[0]", 80 },
  { "COM_OUT_CTL[0]", 84 },
  { "COMBO_INTR_STATUS", 88 },
  { "KEY_INTR_STATUS", 92 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

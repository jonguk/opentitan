#pragma once
#include <cstdint>
#include <cstddef>
struct csr_name_offset { const char *name; uint32_t addr; };
namespace scdv {
  static constexpr csr_name_offset csr_map[] = {
  { "ALERT_TEST", 12 },
  { "KEY_SHARE0[0]", 0 },
  { "KEY_SHARE1[0]", 4 },
  { "NONCE_SHARE0[0]", 8 },
  { "NONCE_SHARE1[0]", 12 },
  { "DATA_IN_SHARE0[0]", 16 },
  { "DATA_IN_SHARE1[0]", 20 },
  { "TAG_IN[0]", 24 },
  { "MSG_OUT[0]", 28 },
  { "TAG_OUT[0]", 32 },
  { "CTRL_SHADOWED", 36 },
  { "CTRL_AUX_SHADOWED", 40 },
  { "CTRL_AUX_REGWEN", 44 },
  { "BLOCK_CTRL_SHADOWED", 48 },
  { "TRIGGER", 52 },
  { "STATUS", 56 },
  { "OUTPUT_VALID", 60 },
  { "FSM_STATE", 64 },
  { "FSM_STATE_REGREN", 68 },
  { "ERROR", 72 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

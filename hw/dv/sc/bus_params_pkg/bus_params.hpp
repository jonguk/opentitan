#pragma once
#include <cstdint>

// Basic bus parameter constants mirroring SV bus_params_pkg
namespace bus_params_pkg {
  // Address width (bits)
  inline constexpr std::uint32_t BUS_AW = 32;
  // Data width (bits)
  inline constexpr std::uint32_t BUS_DW = 32;
  // Data byte width (bytes)
  inline constexpr std::uint32_t BUS_DBW = BUS_DW / 8;
}

namespace tlul {
  // Opcodes (align with SV bus_params_pkg)
  enum opcode : uint8_t {
    PutFullData    = 0,
    PutPartialData = 1,
    Get            = 4,
    AccessAck      = 0,
    AccessAckData  = 1,
    HintAck        = 2
  };

  // Response codes
  enum resp : uint8_t {
    OK    = 0,
    ERROR = 1
  };

  // Size field encodings: bytes = 1 << size
  inline constexpr uint8_t size_from_bytes(uint32_t bytes) {
    return bytes <= 1 ? 0 : (bytes == 2 ? 1 : (bytes == 4 ? 2 : (bytes == 8 ? 3 : 4)));
  }

  inline bool is_power_of_two(unsigned x){ return x && !(x & (x-1)); }
}

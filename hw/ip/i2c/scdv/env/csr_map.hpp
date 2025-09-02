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
  { "CTRL", 0 },
  { "STATUS", 4 },
  { "RDATA", 8 },
  { "FDATA", 12 },
  { "FIFO_CTRL", 16 },
  { "HOST_FIFO_CONFIG", 20 },
  { "TARGET_FIFO_CONFIG", 24 },
  { "HOST_FIFO_STATUS", 28 },
  { "TARGET_FIFO_STATUS", 32 },
  { "OVRD", 36 },
  { "VAL", 40 },
  { "TIMING0", 44 },
  { "TIMING1", 48 },
  { "TIMING2", 52 },
  { "TIMING3", 56 },
  { "TIMING4", 60 },
  { "TIMEOUT_CTRL", 64 },
  { "TARGET_ID", 68 },
  { "ACQDATA", 72 },
  { "TXDATA", 76 },
  { "HOST_TIMEOUT_CTRL", 80 },
  { "TARGET_TIMEOUT_CTRL", 84 },
  { "TARGET_NACK_COUNT", 88 },
  { "TARGET_ACK_CTRL", 92 },
  { "ACQ_FIFO_NEXT_DATA", 96 },
  { "HOST_NACK_HANDLER_TIMEOUT", 100 },
  { "CONTROLLER_EVENTS", 104 },
  { "TARGET_EVENTS", 108 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

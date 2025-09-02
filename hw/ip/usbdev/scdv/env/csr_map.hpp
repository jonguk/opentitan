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
  { "usbctrl", 0 },
  { "ep_out_enable[0]", 4 },
  { "ep_in_enable[0]", 8 },
  { "usbstat", 12 },
  { "avoutbuffer", 16 },
  { "avsetupbuffer", 20 },
  { "rxfifo", 24 },
  { "rxenable_setup[0]", 28 },
  { "rxenable_out", 32 },
  { "set_nak_out[0]", 36 },
  { "in_sent[0]", 40 },
  { "out_stall[0]", 44 },
  { "in_stall[0]", 48 },
  { "configin[0]", 52 },
  { "out_iso[0]", 56 },
  { "in_iso[0]", 60 },
  { "out_data_toggle", 64 },
  { "in_data_toggle", 68 },
  { "phy_pins_sense", 72 },
  { "phy_pins_drive", 76 },
  { "phy_config", 80 },
  { "wake_control", 84 },
  { "wake_events", 88 },
  { "fifo_ctrl", 92 },
  { "count_out", 96 },
  { "count_in", 100 },
  { "count_nodata_in", 104 },
  { "count_errors", 108 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

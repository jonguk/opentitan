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
  { "adc_en_ctl", 0 },
  { "adc_pd_ctl", 4 },
  { "adc_lp_sample_ctl", 8 },
  { "adc_sample_ctl", 12 },
  { "adc_fsm_rst", 16 },
  { "adc_chn0_filter_ctl[0]", 20 },
  { "adc_chn1_filter_ctl[0]", 24 },
  { "adc_chn_val[0]", 28 },
  { "adc_wakeup_ctl", 32 },
  { "filter_status", 36 },
  { "adc_intr_ctl", 40 },
  { "adc_intr_status", 44 },
  { "adc_fsm_state", 48 }
  };
  static constexpr std::size_t csr_map_size = sizeof(csr_map)/sizeof(csr_map[0]);
}

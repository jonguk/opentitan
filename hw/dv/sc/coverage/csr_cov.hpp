#pragma once
#include <cstdint>
#include <unordered_set>
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include <fc4sc.hpp>
#endif

class CsrCov
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    : public covergroup
#endif
{
 public:
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
  CG_CONS(CsrCov) { }
#endif
  static CsrCov &get() { static CsrCov inst; return inst; }

  void register_w1c(uint32_t addr) { w1c_addrs_.insert(addr); }
  void register_rc(uint32_t addr) { rc_addrs_.insert(addr); }
  void register_intr(uint32_t addr) { intr_addrs_.insert(addr); }
  void register_alert(uint32_t addr) { alert_addrs_.insert(addr); }
  void register_shadow(uint32_t addr) { shadow_addrs_.insert(addr); }
  void register_regwen(uint32_t addr) { regwen_addrs_.insert(addr); }

  void sample(uint32_t addr, uint32_t data, bool is_write, uint32_t be_mask)
  {
    (void)data; (void)be_mask;
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
    addr_val_ = addr;
    is_write_val_ = is_write ? 1u : 0u;
    category_val_ = categorize(addr);
    mask_class_val_ = classify_mask(be_mask & 0xFu);
    covergroup::sample();
#else
    (void)addr; (void)is_write;
#endif
  }

 private:
  CsrCov(const CsrCov&) = delete; CsrCov& operator=(const CsrCov&) = delete;

  std::unordered_set<uint32_t> w1c_addrs_{};
  std::unordered_set<uint32_t> rc_addrs_{};
  std::unordered_set<uint32_t> intr_addrs_{};
  std::unordered_set<uint32_t> alert_addrs_{};
  std::unordered_set<uint32_t> shadow_addrs_{};
  std::unordered_set<uint32_t> regwen_addrs_{};

  uint8_t categorize(uint32_t addr) const {
    if (w1c_addrs_.count(addr)) return 1; // W1C
    if (rc_addrs_.count(addr)) return 2;  // RC
    if (intr_addrs_.count(addr)) return 3; // INTR
    if (alert_addrs_.count(addr)) return 4; // ALERT
    if (shadow_addrs_.count(addr)) return 5; // SHADOW
    if (regwen_addrs_.count(addr)) return 6; // REGWEN
    return 0; // OTHER
  }

  static uint8_t classify_mask(uint32_t be) {
    // be is 4-bit TL-UL lane mask
    if (be == 0) return 0; // none
    // contiguous masks
    switch (be) {
      case 0x1: case 0x2: case 0x4: case 0x8: return 1; // single lane
      case 0x3: case 0x6: case 0xC:            return 2; // two contiguous lanes
      case 0x7: case 0xE:                      return 3; // three contiguous lanes
      case 0xF:                                return 4; // full word
      default:                                 return 5; // non-contiguous
    }
  }

#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
  uint32_t addr_val_ {0};
  uint8_t is_write_val_ {0};
  uint8_t category_val_ {0};
  uint8_t mask_class_val_ {0};

  COVERPOINT(uint8_t, is_write_cp, is_write_val_) {
    bin<uint8_t>("read", fc4sc::interval<uint8_t>(0,0)),
    bin<uint8_t>("write", fc4sc::interval<uint8_t>(1,1))
  };

  COVERPOINT(uint8_t, category_cp, category_val_) {
    bin<uint8_t>("other", fc4sc::interval<uint8_t>(0,0)),
    bin<uint8_t>("w1c",   fc4sc::interval<uint8_t>(1,1)),
    bin<uint8_t>("rc",    fc4sc::interval<uint8_t>(2,2)),
    bin<uint8_t>("intr",  fc4sc::interval<uint8_t>(3,3)),
    bin<uint8_t>("alert", fc4sc::interval<uint8_t>(4,4)),
    bin<uint8_t>("shadow", fc4sc::interval<uint8_t>(5,5)),
    bin<uint8_t>("regwen", fc4sc::interval<uint8_t>(6,6))
  };

  cross<uint8_t,uint8_t> rw_vs_cat = cross<uint8_t,uint8_t>(this, "rw_vs_cat", &is_write_cp, &category_cp);

  COVERPOINT(uint8_t, mask_class_cp, mask_class_val_) {
    bin<uint8_t>("none",        fc4sc::interval<uint8_t>(0,0)),
    bin<uint8_t>("single",      fc4sc::interval<uint8_t>(1,1)),
    bin<uint8_t>("two_contig",  fc4sc::interval<uint8_t>(2,2)),
    bin<uint8_t>("three_contig",fc4sc::interval<uint8_t>(3,3)),
    bin<uint8_t>("full",        fc4sc::interval<uint8_t>(4,4)),
    bin<uint8_t>("non_contig",  fc4sc::interval<uint8_t>(5,5))
  };

  cross<uint8_t,uint8_t> cat_vs_mask = cross<uint8_t,uint8_t>(this, "cat_vs_mask", &category_cp, &mask_class_cp);
  cross<uint8_t,uint8_t,uint8_t> rw_x_cat_x_mask = cross<uint8_t,uint8_t,uint8_t>(this, "rw_x_cat_x_mask", &is_write_cp, &category_cp, &mask_class_cp);

  // Optional IP-specific address bins: defined via macro before including this header
#ifdef CSR_ADDR_BINS_DEF
  CSR_ADDR_BINS_DEF
#endif
#endif
};



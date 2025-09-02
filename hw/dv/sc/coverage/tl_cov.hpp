#pragma once
#if defined(ENABLE_FC4SC) && defined(FC4SC_READY)
#include <fc4sc.hpp>
#include <cstdint>

class TLulCov : public covergroup {
 public:
  CG_CONS(TLulCov) { }

  static TLulCov &get() { static TLulCov inst; return inst; }

  void sample(uint8_t a_op, uint8_t d_op, uint8_t size_l2, uint32_t mask, bool derr) {
    a_op_val = a_op;
    d_op_val = d_op;
    size_l2_val = size_l2;
    mask_cnt_val = static_cast<uint8_t>(__builtin_popcount(mask & 0xF));
    all_en_mask_val = ((mask & 0xF) == 0xF) ? 1u : 0u;
    d_err_val = derr;
    covergroup::sample();
  }

 private:
  uint8_t a_op_val{};
  uint8_t d_op_val{};
  uint8_t size_l2_val{};
  uint8_t mask_cnt_val{};
  uint8_t all_en_mask_val{};
  bool    d_err_val{};

  COVERPOINT(uint8_t, a_opcode_cp, a_op_val) {
    bin<uint8_t>("Get",            fc4sc::interval<uint8_t>(4, 4)),
    bin<uint8_t>("PutFullData",    fc4sc::interval<uint8_t>(0, 0)),
    bin<uint8_t>("PutPartialData", fc4sc::interval<uint8_t>(1, 1))
  };

  COVERPOINT(uint8_t, d_opcode_cp, d_op_val) {
    bin<uint8_t>("AccessAck",     fc4sc::interval<uint8_t>(0, 0)),
    bin<uint8_t>("AccessAckData", fc4sc::interval<uint8_t>(1, 1))
  };

  COVERPOINT(uint8_t, size_cp, size_l2_val) {
    bin<uint8_t>("1B", fc4sc::interval<uint8_t>(0, 0)),
    bin<uint8_t>("2B", fc4sc::interval<uint8_t>(1, 1)),
    bin<uint8_t>("4B", fc4sc::interval<uint8_t>(2, 2))
  };

  COVERPOINT(uint8_t, mask_count_cp, mask_cnt_val) {
    bin<uint8_t>("0", fc4sc::interval<uint8_t>(0, 0)),
    bin<uint8_t>("1", fc4sc::interval<uint8_t>(1, 1)),
    bin<uint8_t>("2", fc4sc::interval<uint8_t>(2, 2)),
    bin<uint8_t>("3", fc4sc::interval<uint8_t>(3, 3)),
    bin<uint8_t>("4", fc4sc::interval<uint8_t>(4, 4))
  };

  COVERPOINT(uint8_t, all_en_mask_cp, all_en_mask_val) {
    bin<uint8_t>("not_all", fc4sc::interval<uint8_t>(0, 0)),
    bin<uint8_t>("all",     fc4sc::interval<uint8_t>(1, 1))
  };

  COVERPOINT(bool, d_error_cp, d_err_val) {
    bin<bool>("ok",  fc4sc::interval<bool>(0, 0)),
    bin<bool>("err", fc4sc::interval<bool>(1, 1))
  };

  cross<uint8_t, uint8_t> a_vs_d_cross = cross<uint8_t, uint8_t>(this, "a_vs_d_cross", &a_opcode_cp, &d_opcode_cp);
  cross<uint8_t, uint8_t> a_vs_size_cross = cross<uint8_t, uint8_t>(this, "a_vs_size_cross", &a_opcode_cp, &size_cp);
  cross<uint8_t, uint8_t> a_vs_mask_cross = cross<uint8_t, uint8_t>(this, "a_vs_mask_cross", &a_opcode_cp, &mask_count_cp);
  cross<uint8_t, bool>     a_vs_err_cross = cross<uint8_t, bool>(this, "a_vs_err_cross", &a_opcode_cp, &d_error_cp);
  cross<uint8_t, uint8_t, uint8_t> a_x_size_x_mask = cross<uint8_t, uint8_t, uint8_t>(this, "a_x_size_x_mask", &a_opcode_cp, &size_cp, &mask_count_cp);
  cross<uint8_t, uint8_t> a_vs_allmask_cross = cross<uint8_t, uint8_t>(this, "a_vs_allmask_cross", &a_opcode_cp, &all_en_mask_cp);
};

inline TLulCov &GetTLulCov() { return TLulCov::get(); }

#endif



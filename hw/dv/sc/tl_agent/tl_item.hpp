#pragma once
#include <uvm>
#include <tlm>
#include <cstdint>
#include <algorithm>

// Minimal TL-UL opcodes (SV tlul_pkg parity)
enum class tl_a_op_e : uint8_t { Get = 4, PutFullData = 0, PutPartialData = 1 };
enum class tl_d_op_e : uint8_t { AccessAck = 0, AccessAckData = 1 };

class tl_item : public uvm::uvm_sequence_item {
 public:
  UVM_OBJECT_UTILS(tl_item);

  // A channel fields
  uint64_t       a_addr {0};
  uint32_t       a_data {0};
  uint8_t        a_mask {0xF};   // 4-bit mask for 32b bus
  uint8_t        a_size {2};     // log2(bytes): 0,1,2 -> 1,2,4 bytes
  uint8_t        a_param {0};
  uint8_t        a_source {0};
  tl_a_op_e      a_opcode {tl_a_op_e::Get};
  uint8_t        a_user {0};

  // D channel fields (subset)
  uint8_t        d_param {0};
  uint32_t       d_data {0};
  uint8_t        d_source {0};
  uint8_t        d_size {2};
  tl_d_op_e      d_opcode {tl_d_op_e::AccessAck};
  bool           d_error {false};
  uint8_t        d_user {0};
  bool           d_sink {false};

  // Timing / handshake model placeholders (SV parity)
  uint32_t       a_valid_delay {0};
  uint32_t       a_valid_len {1};
  uint32_t       d_valid_delay {0};
  uint32_t       d_valid_len {1};
  bool           req_abort_after_a_valid_len {false};
  bool           rsp_abort_after_d_valid_len {false};
  bool           req_completed {true};
  bool           rsp_completed {true};

  // Helpers
  static inline unsigned size_bytes_from_size(uint8_t size_log2) { return 1u << size_log2; }
  inline bool is_write() const { return a_opcode == tl_a_op_e::PutFullData || a_opcode == tl_a_op_e::PutPartialData; }

  inline bool is_ok() const {
    tl_d_op_e exp = is_write() ? tl_d_op_e::AccessAck : tl_d_op_e::AccessAckData;
    return d_opcode == exp;
  }

  inline uint32_t get_written_data() const {
    uint32_t masked = 0;
    unsigned nbytes = size_bytes_from_size(a_size);
    for (unsigned i = 0; i < nbytes; ++i) {
      if ((a_mask >> i) & 0x1u) {
        reinterpret_cast<uint8_t*>(&masked)[i] = reinterpret_cast<const uint8_t*>(&a_data)[i];
      }
    }
    return masked;
  }

  // SV-like protocol checks
  inline bool get_error_a_opcode_invalid() const {
    return !(a_opcode == tl_a_op_e::Get || a_opcode == tl_a_op_e::PutFullData || a_opcode == tl_a_op_e::PutPartialData);
  }

  inline bool get_error_PutFullData_mask_size_mismatched() const {
    if (a_opcode != tl_a_op_e::PutFullData) return false;
    const unsigned lanes = size_bytes_from_size(a_size);
    unsigned cnt = 0; for (unsigned i = 0; i < lanes; ++i) cnt += ((a_mask >> i) & 0x1u);
    return cnt != lanes;
  }

  inline bool get_error_addr_size_misaligned() const {
    const uint64_t align = static_cast<uint64_t>(size_bytes_from_size(a_size));
    return (a_addr & (align - 1)) != 0;
  }

  inline bool get_error_mask_not_in_active_lanes() const {
    // Active lanes are the bottom (1<<a_size) lanes starting at a_addr[SizeWidth-1:0]
    const unsigned lanes = size_bytes_from_size(a_size);
    const unsigned start = static_cast<unsigned>(a_addr & (lanes - 1));
    unsigned active_mask = ((1u << lanes) - 1u) << start;
    return (a_mask & ~active_mask) != 0;
  }

  inline bool get_error_size_over_max() const { return a_size > 2; }

  inline bool get_exp_d_error() const {
    bool err = get_error_a_opcode_invalid() || get_error_PutFullData_mask_size_mismatched() ||
               get_error_addr_size_misaligned() || get_error_mask_not_in_active_lanes() ||
               get_error_size_over_max();
    // Treat zero mask on PutPartialData as illegal (no lanes selected)
    if (a_opcode == tl_a_op_e::PutPartialData && (a_mask & 0xF) == 0) err = true;
    return err;
  }

  explicit tl_item(const std::string &name = "tl_item") : uvm::uvm_sequence_item(name) {}
};

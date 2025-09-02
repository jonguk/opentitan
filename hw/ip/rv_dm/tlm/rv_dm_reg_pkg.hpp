// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0




#pragma once

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <cstdint>
#include <array>

namespace opentitan {
namespace rv_dm {

// ----- Types mirrored from rv_dm_reg_pkg.sv -----


class rv_dm_reg_top : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<rv_dm_reg_top> tlm_target;

  SC_HAS_PROCESS(rv_dm_reg_top);
  explicit rv_dm_reg_top(sc_core::sc_module_name name);

  // Register reset helper
  void reset();

  // Soft-access helpers for DV: bypass TLM socket to read/predict mirror
  void sw_predict_write(uint32_t addr, uint32_t data);
  uint32_t sw_read(uint32_t addr);

  // RACL controls (optional; default disabled and all allowed)
  void set_racl_enable(bool enable) { enable_racl_ = enable; }
  void set_racl_error_response(bool enable) { racl_error_rsp_ = enable; }
  void set_role_mask(uint32_t role_mask) { role_mask_ = role_mask; }
  void set_reg_policy(std::size_t reg_index, bool allow_read, bool allow_write);

  // Expose total register bytes for DV convenience
  static constexpr std::size_t kRegBytes = 1136;

 private:
  // Backing storage for CSRs (word addressed, regwidth bits)
  static constexpr uint32_t kRegWidthBits = 32;
  static constexpr std::size_t kWordBytes = kRegWidthBits / 8;

  // Memory mapped bus handling
  void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

  // Per-register offsets
  enum class reg_regs_e : uint32_t {
    ALERT_TEST = 0,
    LATE_DEBUG_ENABLE_REGWEN = 4,
    LATE_DEBUG_ENABLE = 8,
  };
  enum class reg_mem_e : uint32_t {
    HALTED = 256,
    GOING = 264,
    RESUMING = 272,
    EXCEPTION = 280,
    WHERETO = 768,
    ABSTRACTCMD_0 = 824,
    ABSTRACTCMD_1 = 828,
    ABSTRACTCMD_2 = 832,
    ABSTRACTCMD_3 = 836,
    ABSTRACTCMD_4 = 840,
    ABSTRACTCMD_5 = 844,
    ABSTRACTCMD_6 = 848,
    ABSTRACTCMD_7 = 852,
    ABSTRACTCMD_8 = 856,
    ABSTRACTCMD_9 = 860,
    PROGRAM_BUFFER_0 = 864,
    PROGRAM_BUFFER_1 = 868,
    PROGRAM_BUFFER_2 = 872,
    PROGRAM_BUFFER_3 = 876,
    PROGRAM_BUFFER_4 = 880,
    PROGRAM_BUFFER_5 = 884,
    PROGRAM_BUFFER_6 = 888,
    PROGRAM_BUFFER_7 = 892,
    DATAADDR_0 = 896,
    DATAADDR_1 = 900,
    FLAGS_0 = 1024,
    FLAGS_1 = 1028,
    FLAGS_2 = 1032,
    FLAGS_3 = 1036,
    FLAGS_4 = 1040,
    FLAGS_5 = 1044,
    FLAGS_6 = 1048,
    FLAGS_7 = 1052,
    FLAGS_8 = 1056,
    FLAGS_9 = 1060,
    FLAGS_10 = 1064,
    FLAGS_11 = 1068,
    FLAGS_12 = 1072,
    FLAGS_13 = 1076,
    FLAGS_14 = 1080,
    FLAGS_15 = 1084,
    FLAGS_16 = 1088,
    FLAGS_17 = 1092,
    FLAGS_18 = 1096,
    FLAGS_19 = 1100,
    FLAGS_20 = 1104,
    FLAGS_21 = 1108,
    FLAGS_22 = 1112,
    FLAGS_23 = 1116,
    FLAGS_24 = 1120,
    FLAGS_25 = 1124,
    FLAGS_26 = 1128,
    FLAGS_27 = 1132,
    FLAGS_28 = 1136,
    FLAGS_29 = 1140,
    FLAGS_30 = 1144,
    FLAGS_31 = 1148,
    FLAGS_32 = 1152,
    FLAGS_33 = 1156,
    FLAGS_34 = 1160,
    FLAGS_35 = 1164,
    FLAGS_36 = 1168,
    FLAGS_37 = 1172,
    FLAGS_38 = 1176,
    FLAGS_39 = 1180,
    FLAGS_40 = 1184,
    FLAGS_41 = 1188,
    FLAGS_42 = 1192,
    FLAGS_43 = 1196,
    FLAGS_44 = 1200,
    FLAGS_45 = 1204,
    FLAGS_46 = 1208,
    FLAGS_47 = 1212,
    FLAGS_48 = 1216,
    FLAGS_49 = 1220,
    FLAGS_50 = 1224,
    FLAGS_51 = 1228,
    FLAGS_52 = 1232,
    FLAGS_53 = 1236,
    FLAGS_54 = 1240,
    FLAGS_55 = 1244,
    FLAGS_56 = 1248,
    FLAGS_57 = 1252,
    FLAGS_58 = 1256,
    FLAGS_59 = 1260,
    FLAGS_60 = 1264,
    FLAGS_61 = 1268,
    FLAGS_62 = 1272,
    FLAGS_63 = 1276,
    FLAGS_64 = 1280,
    FLAGS_65 = 1284,
    FLAGS_66 = 1288,
    FLAGS_67 = 1292,
    FLAGS_68 = 1296,
    FLAGS_69 = 1300,
    FLAGS_70 = 1304,
    FLAGS_71 = 1308,
    FLAGS_72 = 1312,
    FLAGS_73 = 1316,
    FLAGS_74 = 1320,
    FLAGS_75 = 1324,
    FLAGS_76 = 1328,
    FLAGS_77 = 1332,
    FLAGS_78 = 1336,
    FLAGS_79 = 1340,
    FLAGS_80 = 1344,
    FLAGS_81 = 1348,
    FLAGS_82 = 1352,
    FLAGS_83 = 1356,
    FLAGS_84 = 1360,
    FLAGS_85 = 1364,
    FLAGS_86 = 1368,
    FLAGS_87 = 1372,
    FLAGS_88 = 1376,
    FLAGS_89 = 1380,
    FLAGS_90 = 1384,
    FLAGS_91 = 1388,
    FLAGS_92 = 1392,
    FLAGS_93 = 1396,
    FLAGS_94 = 1400,
    FLAGS_95 = 1404,
    FLAGS_96 = 1408,
    FLAGS_97 = 1412,
    FLAGS_98 = 1416,
    FLAGS_99 = 1420,
    FLAGS_100 = 1424,
    FLAGS_101 = 1428,
    FLAGS_102 = 1432,
    FLAGS_103 = 1436,
    FLAGS_104 = 1440,
    FLAGS_105 = 1444,
    FLAGS_106 = 1448,
    FLAGS_107 = 1452,
    FLAGS_108 = 1456,
    FLAGS_109 = 1460,
    FLAGS_110 = 1464,
    FLAGS_111 = 1468,
    FLAGS_112 = 1472,
    FLAGS_113 = 1476,
    FLAGS_114 = 1480,
    FLAGS_115 = 1484,
    FLAGS_116 = 1488,
    FLAGS_117 = 1492,
    FLAGS_118 = 1496,
    FLAGS_119 = 1500,
    FLAGS_120 = 1504,
    FLAGS_121 = 1508,
    FLAGS_122 = 1512,
    FLAGS_123 = 1516,
    FLAGS_124 = 1520,
    FLAGS_125 = 1524,
    FLAGS_126 = 1528,
    FLAGS_127 = 1532,
    FLAGS_128 = 1536,
    FLAGS_129 = 1540,
    FLAGS_130 = 1544,
    FLAGS_131 = 1548,
    FLAGS_132 = 1552,
    FLAGS_133 = 1556,
    FLAGS_134 = 1560,
    FLAGS_135 = 1564,
    FLAGS_136 = 1568,
    FLAGS_137 = 1572,
    FLAGS_138 = 1576,
    FLAGS_139 = 1580,
    FLAGS_140 = 1584,
    FLAGS_141 = 1588,
    FLAGS_142 = 1592,
    FLAGS_143 = 1596,
    FLAGS_144 = 1600,
    FLAGS_145 = 1604,
    FLAGS_146 = 1608,
    FLAGS_147 = 1612,
    FLAGS_148 = 1616,
    FLAGS_149 = 1620,
    FLAGS_150 = 1624,
    FLAGS_151 = 1628,
    FLAGS_152 = 1632,
    FLAGS_153 = 1636,
    FLAGS_154 = 1640,
    FLAGS_155 = 1644,
    FLAGS_156 = 1648,
    FLAGS_157 = 1652,
    FLAGS_158 = 1656,
    FLAGS_159 = 1660,
    FLAGS_160 = 1664,
    FLAGS_161 = 1668,
    FLAGS_162 = 1672,
    FLAGS_163 = 1676,
    FLAGS_164 = 1680,
    FLAGS_165 = 1684,
    FLAGS_166 = 1688,
    FLAGS_167 = 1692,
    FLAGS_168 = 1696,
    FLAGS_169 = 1700,
    FLAGS_170 = 1704,
    FLAGS_171 = 1708,
    FLAGS_172 = 1712,
    FLAGS_173 = 1716,
    FLAGS_174 = 1720,
    FLAGS_175 = 1724,
    FLAGS_176 = 1728,
    FLAGS_177 = 1732,
    FLAGS_178 = 1736,
    FLAGS_179 = 1740,
    FLAGS_180 = 1744,
    FLAGS_181 = 1748,
    FLAGS_182 = 1752,
    FLAGS_183 = 1756,
    FLAGS_184 = 1760,
    FLAGS_185 = 1764,
    FLAGS_186 = 1768,
    FLAGS_187 = 1772,
    FLAGS_188 = 1776,
    FLAGS_189 = 1780,
    FLAGS_190 = 1784,
    FLAGS_191 = 1788,
    FLAGS_192 = 1792,
    FLAGS_193 = 1796,
    FLAGS_194 = 1800,
    FLAGS_195 = 1804,
    FLAGS_196 = 1808,
    FLAGS_197 = 1812,
    FLAGS_198 = 1816,
    FLAGS_199 = 1820,
    FLAGS_200 = 1824,
    FLAGS_201 = 1828,
    FLAGS_202 = 1832,
    FLAGS_203 = 1836,
    FLAGS_204 = 1840,
    FLAGS_205 = 1844,
    FLAGS_206 = 1848,
    FLAGS_207 = 1852,
    FLAGS_208 = 1856,
    FLAGS_209 = 1860,
    FLAGS_210 = 1864,
    FLAGS_211 = 1868,
    FLAGS_212 = 1872,
    FLAGS_213 = 1876,
    FLAGS_214 = 1880,
    FLAGS_215 = 1884,
    FLAGS_216 = 1888,
    FLAGS_217 = 1892,
    FLAGS_218 = 1896,
    FLAGS_219 = 1900,
    FLAGS_220 = 1904,
    FLAGS_221 = 1908,
    FLAGS_222 = 1912,
    FLAGS_223 = 1916,
    FLAGS_224 = 1920,
    FLAGS_225 = 1924,
    FLAGS_226 = 1928,
    FLAGS_227 = 1932,
    FLAGS_228 = 1936,
    FLAGS_229 = 1940,
    FLAGS_230 = 1944,
    FLAGS_231 = 1948,
    FLAGS_232 = 1952,
    FLAGS_233 = 1956,
    FLAGS_234 = 1960,
    FLAGS_235 = 1964,
    FLAGS_236 = 1968,
    FLAGS_237 = 1972,
    FLAGS_238 = 1976,
    FLAGS_239 = 1980,
    FLAGS_240 = 1984,
    FLAGS_241 = 1988,
    FLAGS_242 = 1992,
    FLAGS_243 = 1996,
    FLAGS_244 = 2000,
    FLAGS_245 = 2004,
    FLAGS_246 = 2008,
    FLAGS_247 = 2012,
    FLAGS_248 = 2016,
    FLAGS_249 = 2020,
    FLAGS_250 = 2024,
    FLAGS_251 = 2028,
    FLAGS_252 = 2032,
    FLAGS_253 = 2036,
    FLAGS_254 = 2040,
    FLAGS_255 = 2044,
  };
  enum class reg_dbg_e : uint32_t {
  };

  // Aggregate structs mirroring rv_dm_reg_pkg

//   ///////////////////////////////////////////////
  // Typedefs for registers for regs interface //
  ///////////////////////////////////////////////

  struct rv_dm_reg2hw_alert_test_reg_t {
    sc_dt::sc_bv<1> q;
    bool qe;
  };

  struct rv_dm_reg2hw_late_debug_enable_reg_t {
    sc_dt::sc_bv<32> q;
  };

  // Register -> HW type for regs interface
  struct rv_dm_regs_reg2hw_t {
    rv_dm_reg2hw_alert_test_reg_t alert_test;
    rv_dm_reg2hw_late_debug_enable_reg_t late_debug_enable;
  };

//   //////////////////////////////////////////////
  // Typedefs for registers for mem interface //
  //////////////////////////////////////////////

  struct rv_dm_reg2hw_halted_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct rv_dm_reg2hw_going_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct rv_dm_reg2hw_resuming_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct rv_dm_reg2hw_exception_reg_t {
    sc_dt::sc_bv<1> q;
  };

  struct rv_dm_reg2hw_whereto_reg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rv_dm_reg2hw_abstractcmd_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rv_dm_reg2hw_program_buffer_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rv_dm_reg2hw_dataaddr_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rv_dm_reg2hw_flags_mreg_t {
    sc_dt::sc_bv<32> q;
  };

  struct rv_dm_hw2reg_halted_reg_t {
    sc_dt::sc_bv<1> d;
    bool        de;
  };

  struct rv_dm_hw2reg_whereto_reg_t {
    sc_dt::sc_bv<32> d;
    bool        de;
  };

  // Register -> HW type for mem interface
  struct rv_dm_mem_reg2hw_t {
    rv_dm_reg2hw_halted_reg_t halted;
    rv_dm_reg2hw_going_reg_t going;
    rv_dm_reg2hw_resuming_reg_t resuming;
    rv_dm_reg2hw_exception_reg_t exception;
    rv_dm_reg2hw_whereto_reg_t whereto;
    std::array<rv_dm_reg2hw_abstractcmd_mreg_t, 10> abstractcmd;
    std::array<rv_dm_reg2hw_program_buffer_mreg_t, 8> program_buffer;
    std::array<rv_dm_reg2hw_dataaddr_mreg_t, 2> dataaddr;
    std::array<rv_dm_reg2hw_flags_mreg_t, 256> flags;
  };

  // HW -> register type for mem interface
  struct rv_dm_mem_hw2reg_t {
    rv_dm_hw2reg_halted_reg_t halted;
    rv_dm_hw2reg_whereto_reg_t whereto;
  };

  // Flat register storage
  std::array<uint32_t, 3> regs_regs_;
  std::array<uint32_t, 281> regs_mem_;
  std::array<uint32_t, 0> regs_dbg_;

  // Shadow register support (per-register staged/phase; only used when applicable)
  std::array<uint32_t, 3> shadow_stage_regs_{};
  std::array<uint8_t,  3> shadow_phase_regs_{}; // 0:first,1:second
  std::array<uint32_t, 281> shadow_stage_mem_{};
  std::array<uint8_t,  281> shadow_phase_mem_{}; // 0:first,1:second
  std::array<uint32_t, 0> shadow_stage_dbg_{};
  std::array<uint8_t,  0> shadow_phase_dbg_{}; // 0:first,1:second

  // RACL per-register policy (true means allowed)
  std::array<bool, 3> racl_allow_rd_regs_{};
  std::array<bool, 3> racl_allow_wr_regs_{};
  std::array<bool, 281> racl_allow_rd_mem_{};
  std::array<bool, 281> racl_allow_wr_mem_{};
  std::array<bool, 0> racl_allow_rd_dbg_{};
  std::array<bool, 0> racl_allow_wr_dbg_{};

  // Helpers
  bool check_racl(bool is_write, uint32_t addr) const;
  bool apply_sw_write(uint32_t addr, uint32_t wdata);
  uint32_t read_sw(uint32_t addr);

  // Exposed bundles, mirroring SV
  rv_dm_regs_reg2hw_t reg2hw_regs;
  rv_dm_regs_hw2reg_t  hw2reg_regs;
  rv_dm_mem_reg2hw_t reg2hw_mem;
  rv_dm_mem_hw2reg_t  hw2reg_mem;
  rv_dm_dbg_reg2hw_t reg2hw_dbg;
  rv_dm_dbg_hw2reg_t  hw2reg_dbg;

  bool enable_racl_ = false;
  bool racl_error_rsp_ = true;
  uint32_t role_mask_ = 0; // externally assigned; model treats nonzero as privileged
};

}  // namespace rv_dm
}  // namespace opentitan



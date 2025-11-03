// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
//
// util/topgen.py -t hw/top_assam/data/top_assam.hjson \
//                -o hw/top_assam/
//
// File is generated based on the following seed configuration:
//   hw/top_assam/data/top_assam_seed.testing.hjson


package top_assam_rnd_cnst_pkg;

  ////////////////////////////////////////////
  // aes
  ////////////////////////////////////////////
  // Default seed of the PRNG used for register clearing.
  parameter aes_pkg::clearing_lfsr_seed_t RndCnstAesClearingLfsrSeed = {
    64'h16A19D1E_2E0F15A5
  };

  // Permutation applied to the LFSR of the PRNG used for clearing.
  parameter aes_pkg::clearing_lfsr_perm_t RndCnstAesClearingLfsrPerm = {
    128'hD409ED54_C5985EA6_BC7AF1D4_3852F365,
    256'h3AE1A087_4F1364B4_F90DCD8F_B4606F8A_7AC9F1AE_07990A27_1F65B309_BF09C0E2
  };

  // Permutation applied to the clearing PRNG output for clearing the second share of registers.
  parameter aes_pkg::clearing_lfsr_perm_t RndCnstAesClearingSharePerm = {
    128'h6F21F175_F42F612E_CEC0526D_AFF9F40A,
    256'h00651803_365DA54D_993DD17A_1CD4A4BF_B871622C_8B9F35AB_D0E864F9_23E842AE
  };

  // Default seed of the PRNG used for masking.
  parameter aes_pkg::masking_lfsr_seed_t RndCnstAesMaskingLfsrSeed = {
    32'hA421ACE2,
    256'h0B50EC6B_A7624A9F_75EE1204_1D292C75_940B5125_7E2224BB_78951511_213905E9
  };

  // Permutation applied to the output of the PRNG used for masking.
  parameter aes_pkg::masking_lfsr_perm_t RndCnstAesMaskingLfsrPerm = {
    256'h8A1B469E_7B5F5158_11795714_2E946A44_5E711849_3C4F4A95_73561A7E_550D5B17,
    256'h012D6D0E_253F8159_76124C84_41291C53_47024520_7C8E3078_6C273635_9D8B8275,
    256'h90972303_6886703A_67624E10_34398800_6999401E_83421337_16199C4B_987D3287,
    256'h6B1D8021_508D0454_2A96093D_225D9B9A_2F289105_2B159F43_3E775C60_241F3189,
    256'h6F660F4D_65638F07_8C61380A_0C337408_7A48925A_9326720B_6E2C0664_7F523B85
  };

  ////////////////////////////////////////////
  // sram_ctrl_main
  ////////////////////////////////////////////
  // Compile-time random reset value for SRAM scrambling key.
  parameter otp_ctrl_pkg::sram_key_t RndCnstSramCtrlMainSramKey = {
    128'h46437EA1_9C961290_CC380044_B6A20A92
  };

  // Compile-time random reset value for SRAM scrambling nonce.
  parameter otp_ctrl_pkg::sram_nonce_t RndCnstSramCtrlMainSramNonce = {
    128'h0684BC78_4B3C5EC3_F27BD1B3_F4FF8071
  };

  // Compile-time random bits for initial LFSR seed
  parameter sram_ctrl_pkg::lfsr_seed_t RndCnstSramCtrlMainLfsrSeed = {
    64'hC1B01AD2_AAC35E76
  };

  // Compile-time random permutation for LFSR output
  parameter sram_ctrl_pkg::lfsr_perm_t RndCnstSramCtrlMainLfsrPerm = {
    128'hA3FD73B2_2AEE7345_20450299_C483DF35,
    256'hD124C83E_C26277B6_253E2117_9E9604EC_150375EA_B47BFDFB_C48B6DA9_4C1A3A66
  };

  ////////////////////////////////////////////
  // rom_ctrl
  ////////////////////////////////////////////
  // Fixed nonce used for address / data scrambling
  parameter bit [63:0] RndCnstRomCtrlScrNonce = {
    64'h1A461A19_4ED733BE
  };

  // Randomised constant used as a scrambling key for ROM data
  parameter bit [127:0] RndCnstRomCtrlScrKey = {
    128'hBE1F3DDA_72E90F59_58729C42_A144D23A
  };

  ////////////////////////////////////////////
  // rv_core_ibex
  ////////////////////////////////////////////
  // Default seed of the PRNG used for random instructions.
  parameter ibex_pkg::lfsr_seed_t RndCnstRvCoreIbexLfsrSeed = {
    32'h353CF44F
  };

  // Permutation applied to the LFSR of the PRNG used for random instructions.
  parameter ibex_pkg::lfsr_perm_t RndCnstRvCoreIbexLfsrPerm = {
    160'h0A71A1A9_F122C534_1F3EA16A_D95AFBF9_810671DD
  };

  // Default icache scrambling key
  parameter logic [ibex_pkg::SCRAMBLE_KEY_W-1:0] RndCnstRvCoreIbexIbexKeyDefault = {
    128'h5BF3DC67_A11A5347_DBF6C405_E6A3DF48
  };

  // Default icache scrambling nonce
  parameter logic [ibex_pkg::SCRAMBLE_NONCE_W-1:0] RndCnstRvCoreIbexIbexNonceDefault = {
    64'h6CEC0ED3_F8B34D3A
  };

endpackage : top_assam_rnd_cnst_pkg

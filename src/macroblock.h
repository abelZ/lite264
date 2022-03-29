// Copyright © 2022 LGPL3. All Rights Reserved.
// Author:         abel.zhang
// Created:        Fri 25 Mar 2022 12:35:37 PM CST
// Last Modified:  Fri 25 Mar 2022 12:35:37 PM CST

#include "pixel.h"

namespace lite264 {

enum MacroBlockType { MB_P_8x8, MB_P_8x8ref0, MB_B_8x8 };

enum SubMacroBlockType {};

// @Ref Table 6-2
const Location kDiffA = {-1, 0};
const Location kDiffB = {0, -1};
const int kDiffCY = -1;
const Location kDiffD = {-1, -1};

int MbPartWidth(MacroBlockType mb_type);
int MbPartHeight(MacroBlockType mb_type);
int SubMbPartWidth(SubMacroBlockType sub_mb_type);
int SubMbPartHeight(SubMacroBlockType sub_mb_type);

// @Ref Table 6-1
Resolution GetSubC(int chroma_format_idc, int separate_colour_plane_flag,
                   PixelFormat chroma_format);

// @Ref Equation 6-1 6-2
Resolution GetMbC(int chroma_format_idc, int separate_colour_plane_flag, PixelFormat chroma_format);

// @Ref Equation 6-3 ~ 6-10
Location GetLocationFromMbAddr(int mb_addr, int mb_aff, int frame_or_field_flag, int pic_width);

// @Ref Equation 6-11 6-12
Location GetLocationFromMbPartIdx(int mb_part_idx, MacroBlockType mb_type);

// @Ref Equation 6-13 ~ 6-16
Location GetLocationFromSubMbPartIdx(int sub_mb_part_idx, SubMacroBlockType sub_mb_type,
                                     MacroBlockType mb_type);

// @Ref Equation 6-17 6-18
// reused by Cb & Cr when chroma_array_type == 3
Location GetLocationFromLuma4x4Blockidx(int luma_4x4_block_idx);

// @Ref Equation 6-19 6-20
// reused by Cb & Cr when chroma_array_type == 3
Location GetLocationFromLuma8x8BlockIdx(int luma_8x8_block_idx);

// @Ref Equation 6-21 6-22
Location GetLocationFromChroma4x4BlockIdx(int chroma_4x4_block_idx);

// @Ref 6.4.8
int CheckMbAvailability(int mb_addr, int cur_mb_addr, int slice_idx);

// @Ref 6.4.9, only useful when mbaff_frame_flag == 0
// return specified mb_addr or -1 if not available
int CheckMbAvailabilityForAddrA0(int cur_mb_addr, int pic_width_in_mbs);
int CheckMbAvailabilityForAddrB0(int cur_mb_addr, int pic_width_in_mbs);
int CheckMbAvailabilityForAddrC0(int cur_mb_addr, int pic_width_in_mbs);
int CheckMbAvailabilityForAddrD0(int cur_mb_addr, int pic_width_in_mbs);

// @Ref 6.4.10, only useful when mbaff_frame_flag == 1
// return specified mb_addr or -1 if not available
int CheckMbAvailabilityForAddrA1(int cur_mb_addr, int pic_width_in_mbs);
int CheckMbAvailabilityForAddrB1(int cur_mb_addr, int pic_width_in_mbs);
int CheckMbAvailabilityForAddrC1(int cur_mb_addr, int pic_width_in_mbs);
int CheckMbAvailabilityForAddrD1(int cur_mb_addr, int pic_width_in_mbs);

void Get8x8LumaNeighbourN(int cur_mb_addr, int luma_8x8_block_idx, Location diff_n, int& mb_addr_n,
                          int& luma_8x8_block_idx_n);

// @Ref 6.4.12.1
void GetNeighbourLocation0(int cur_mb_addr, Location n, Resolution r, int pic_width_in_mbs,
                           int& mb_addr_n, Location& w);

// @Ref 6.4.13.1~6.4.13.3
int Get4x4LumaBlockIdxFromLocationP(Location p);
int Get4x4ChromaBlockIdxFromLocationP(Location p);
int Get8x8LumaBlockIdxFromLocationP(Location p);

}  // namespace lite264

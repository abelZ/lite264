// Copyright © 2022 LGPL3. All Rights Reserved.
// Author:         abel.zhang
// Created:        Mon 28 Mar 2022 04:05:25 PM CST
// Last Modified:  Mon 28 Mar 2022 04:05:25 PM CST

#include "macroblock.h"

namespace lite264 {

Resolution GetSubC(int chroma_format_idc, int separate_colour_plane_flag,
                   PixelFormat chroma_format) {
    Resolution subc;
    switch (chroma_format) {
        case PixelFormatMono:
            break;
        case PixelFormat420: {
            subc.width = 2;
            subc.height = 2;
        } break;
        case PixelFormat422: {
            subc.width = 2;
            subc.height = 1;
        } break;
        case PixelFormat444: {
            if (!separate_colour_plane_flag) {
                subc.width = 1;
                subc.height = 1;
            }
        } break;
        default:
            break;
    }

    return subc;
}

Resolution GetMbC(int chroma_format_idc, int separate_colour_plane_flag,
                  PixelFormat chroma_format) {
    Resolution mb;
    if (chroma_format_idc == 0 || separate_colour_plane_flag == 1)
        return mb;
    Resolution subc = GetSubC(chroma_format_idc, separate_colour_plane_flag, chroma_format);
    mb.width = 16 / subc.width;
    mb.height = 16 / subc.height;
    return mb;
}

Location GetLocationFromMbAddr(int mb_addr, int mb_aff, int frame_or_field_flag, int pic_width) {
    Location loc;
    if (!mb_aff) {
        loc.x = InverseRasterScan(mb_addr, 16, 16, pic_width, 0);
        loc.y = InverseRasterScan(mb_addr, 16, 16, pic_width, 1);
    } else {
        int x0 = InverseRasterScan(mb_addr / 2, 16, 32, pic_width, 0);
        int y0 = InverseRasterScan(mb_addr / 2, 16, 32, pic_width, 1);
        if (frame_or_field_flag) {
            // frame
            loc.x = x0;
            loc.y = y0 + (mb_addr % 2) * 16;
        } else {
            // field
            loc.x = x0;
            loc.y = y0 + (mb_addr % 2);
        }
    }
    return loc;
}

Location GetLocationFromMbPartIdx(int mb_part_idx, MacroBlockType mb_type) {
    Location loc;
    loc.x = InverseRasterScan(mb_part_idx, MbPartWidth(mb_type), MbPartHeight(mb_type), 16, 0);
    loc.y = InverseRasterScan(mb_part_idx, MbPartWidth(mb_type), MbPartHeight(mb_type), 16, 1);
    return loc;
}

Location GetLocationFromSubMbPartIdx(int sub_mb_part_idx, SubMacroBlockType sub_mb_type,
                                     MacroBlockType mb_type) {
    Location loc;
    if (mb_type == MB_P_8x8 || mb_type == MB_P_8x8ref0 || mb_type == MB_B_8x8) {
        loc.x = InverseRasterScan(sub_mb_part_idx, SubMbPartWidth(sub_mb_type),
                                  SubMbPartHeight(sub_mb_type), 8, 0);
        loc.y = InverseRasterScan(sub_mb_part_idx, SubMbPartWidth(sub_mb_type),
                                  SubMbPartHeight(sub_mb_type), 8, 1);
    } else {
        loc.x = InverseRasterScan(sub_mb_part_idx, 4, 4, 8, 0);
        loc.y = InverseRasterScan(sub_mb_part_idx, 4, 4, 8, 1);
    }
    return loc;
}

Location GetLocationFromLuma4x4Blockidx(int luma_4x4_block_idx) {
    Location loc;
    loc.x = InverseRasterScan(luma_4x4_block_idx / 4, 8, 8, 16, 0) +
            InverseRasterScan(luma_4x4_block_idx % 4, 4, 4, 8, 0);
    loc.y = InverseRasterScan(luma_4x4_block_idx / 4, 8, 8, 16, 1) +
            InverseRasterScan(luma_4x4_block_idx % 4, 4, 4, 8, 1);
    return loc;
}

Location GetLocationFromLuma8x8BlockIdx(int luma_8x8_block_idx) {
    Location loc;
    loc.x = InverseRasterScan(luma_8x8_block_idx, 8, 8, 16, 0);
    loc.y = InverseRasterScan(luma_8x8_block_idx, 8, 8, 16, 1);
    return loc;
}

Location GetLocationFromChroma4x4BlockIdx(int chroma_4x4_block_idx) {
    Location loc;
    loc.x = InverseRasterScan(chroma_4x4_block_idx, 4, 4, 8, 0);
    loc.y = InverseRasterScan(chroma_4x4_block_idx, 4, 4, 8, 1);
    return loc;
}

int CheckMbAvailability(int mb_addr, int cur_mb_addr, int slice_idx) {
    if (mb_addr < 0 || mb_addr > cur_mb_addr)
        return -1;
    // TODO(abel): if mb_addr didn't belong to slice at slice_idx, return -1;
    return mb_addr;
}

int CheckMbAvailabilityForAddrA0(int cur_mb_addr, int pic_width_in_mbs) {
    // TODO(abel): get slice_idx
    if ((cur_mb_addr % pic_width_in_mbs) == 0)
        return -1;
    return CheckMbAvailability(cur_mb_addr - 1, cur_mb_addr, 0);
}

int CheckMbAvailabilityForAddrB0(int cur_mb_addr, int pic_width_in_mbs) {
    // TODO(abel): get slice_idx
    return CheckMbAvailability(cur_mb_addr - pic_width_in_mbs, cur_mb_addr, 0);
}

int CheckMbAvailabilityForAddrC0(int cur_mb_addr, int pic_width_in_mbs) {
    // TODO(abel): get slice_idx
    if ((cur_mb_addr + 1) % pic_width_in_mbs == 0)
        return -1;
    return CheckMbAvailability(cur_mb_addr - pic_width_in_mbs + 1, cur_mb_addr, 0);
}

int CheckMbAvailabilityForAddrD0(int cur_mb_addr, int pic_width_in_mbs) {
    // TODO(abel): get slice_idx
    if ((cur_mb_addr % pic_width_in_mbs) == 0)
        return -1;
    return CheckMbAvailability(cur_mb_addr - pic_width_in_mbs - 1, cur_mb_addr, 0);
}

int CheckMbAvailabilityForAddrA1(int cur_mb_addr, int pic_width_in_mbs) {
    if ((cur_mb_addr / 2) % pic_width_in_mbs == 0)
        return -1;
    return CheckMbAvailability(2 * (cur_mb_addr / 2 - 1), cur_mb_addr, 0);
}

int CheckMbAvailabilityForAddrB1(int cur_mb_addr, int pic_width_in_mbs) {
    return CheckMbAvailability(2 * (cur_mb_addr / 2 - pic_width_in_mbs), cur_mb_addr, 0);
}

int CheckMbAvailabilityForAddrC1(int cur_mb_addr, int pic_width_in_mbs) {
    if ((cur_mb_addr / 2 + 1) % pic_width_in_mbs != 0)
        return -1;
    return CheckMbAvailability(2 * (cur_mb_addr / 2 - pic_width_in_mbs + 1), cur_mb_addr, 0);
}

int CheckMbAvailabilityForAddrD1(int cur_mb_addr, int pic_width_in_mbs) {
    if ((cur_mb_addr / 2) % pic_width_in_mbs != 0)
        return -1;
    return CheckMbAvailability(2 * (cur_mb_addr / 2 - pic_width_in_mbs - 1), cur_mb_addr, 0);
}

void Get8x8LumaNeighbourN(int cur_mb_addr, int luma_8x8_block_idx, Location diff_n, int& mb_addr_n,
                          int& luma_8x8_block_idx_n) {
    Location n;
    n.x = (luma_8x8_block_idx_n % 2) * 8 + diff_n.x;
    n.y = (luma_8x8_block_idx_n / 2) * 8 + diff_n.y;
}

void GetNeighbourLocation0(int cur_mb_addr, Location n, Resolution r, int pic_width_in_mbs,
                           int& mb_addr_n, Location& w) {
    int mb_addr{-1};
    if (n.x < 0 && n.y < 0) {
        mb_addr = CheckMbAvailabilityForAddrD0(cur_mb_addr, pic_width_in_mbs);
    } else if (n.x < 0 && n.y >= 0 && n.y < r.height) {
        mb_addr = CheckMbAvailabilityForAddrA0(cur_mb_addr, pic_width_in_mbs);
    } else if (n.x >= 0 && n.x < r.width && n.y < 0) {
        mb_addr = CheckMbAvailabilityForAddrB0(cur_mb_addr, pic_width_in_mbs);
    } else if (n.x >= 0 && n.x < r.width && n.y > 0 && n.y < r.height) {
        mb_addr = cur_mb_addr;
    } else if (n.x >= r.width && n.y < 0) {
        mb_addr = CheckMbAvailabilityForAddrC0(cur_mb_addr, pic_width_in_mbs);
    } else {
        mb_addr = -1;
    }

    if (mb_addr >= 0) {
        w.x = (n.x + r.width) % r.width;
        w.y = (n.y + r.height) % r.height;
    }
}

int Get4x4LumaBlockIdxFromLocationP(Location p) {
    return 8 * (p.y / 8) + 4 * (p.x / 8) + 2 * ((p.y % 8) / 4) + ((p.x % 8) / 4);
}

int Get4x4ChromaBlockIdxFromLocationP(Location p) {
    return 2 * (p.y / 4) + (p.x / 4);
}

int Get8x8LumaBlockIdxFromLocationP(Location p) {
    return 2 * (p.y / 8) + (p.x / 8);
}

}  // namespace lite264

// Copyright © 2022 LGPL3. All Rights Reserved.
// Author:         abel.zhang
// Created:        Fri 25 Mar 2022 12:35:37 PM CST
// Last Modified:  Fri 25 Mar 2022 12:35:37 PM CST

#include "pixel.h"

namespace lite264 {

struct Lite264 {
    Resolution luma;
    Resolution chroma;

    int cur_mb_addr{-1};
    int mbaff_frame_flag{0};
};

}  // namespace lite264

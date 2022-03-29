// Copyright © 2022 LGPL3. All Rights Reserved.
// Author:         abel.zhang
// Created:        Mon 28 Mar 2022 04:05:25 PM CST
// Last Modified:  Mon 28 Mar 2022 04:05:25 PM CST

#include "math264.h"

namespace lite264 {

enum PixelFormat {
    PixelFormatMono = 0,
    PixelFormat420 = 1,
    PixelFormat422 = 2,
    PixelFormat444 = 3
};

using ChromaArrayType = PixelFormat;

struct Resolution {
    int width{0};
    int height{0};
};

struct Location {
    int x{0};
    int y{0};
};

}  // namespace lite264

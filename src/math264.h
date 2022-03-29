// Copyright © 2022 LGPL3. All Rights Reserved.
// Author:         abel.zhang
// Created:        Fri 25 Mar 2022 12:35:37 PM CST
// Last Modified:  Fri 25 Mar 2022 12:35:37 PM CST

#include <math.h>
#include <algorithm>

namespace lite264 {

template <typename T>
int Sign(T val) {
    return (T(0) < val) - (val < T(0));
}

inline int Clip3(int x, int y, int z) {
    if (z < x) {
        return x;
    } else if (z > y) {
        return y;
    } else {
        return z;
    }
}

inline int Clip1(int x, int bit_depth = 8) {
    return Clip3(0, (1 << bit_depth) - 1, x);
}

inline int InverseRasterScan(int a, int b, int c, int d, int e) {
    if (e == 0) {
        return a % (d / b) * b;
    } else {
        return a / (d / b) * c;
    }
}

inline int Median(int x, int y, int z) {
    return x + y + z - std::min(x, std::min(y, z)) - std::max(x, std::max(y, z));
}

inline int Round(double x) {
    return Sign(x) * floor(std::abs(x) + 0.5);
}

}  // namespace lite264

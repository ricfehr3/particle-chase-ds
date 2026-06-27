#pragma once

#include <nds.h>

#define HALF_WIDTH  SCREEN_WIDTH / 2
#define HALF_HEIGHT SCREEN_HEIGHT / 2

#define SCREEN_WIDTH_FIXED  inttof32(SCREEN_WIDTH)
#define SCREEN_HEIGHT_FIXED inttof32(SCREEN_HEIGHT)

#define SIGN(x) ((x > 0) - (x < 0))

#define COLOR15_VAL_MAX 0x1F
#define COLOR15_WHITE   RGB15(COLOR15_VAL_MAX, COLOR15_VAL_MAX, COLOR15_VAL_MAX)

typedef short Color15;

enum ColorTypes
{
    COLOR_STATIC,
    COLOR_SPEED,
    COLOR_RANDOM,
    COLOR_MAX,
};

inline int clampi(int x, int lo, int hi)
{
    if (x < lo)
        return lo;
    if (x > hi)
        return hi;
    return x;
}

Color15 lerp_color15(int r0, int g0, int b0, int r1, int g1, int b1, int num, int den);

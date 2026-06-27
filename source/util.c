#include "util.h"

Color15 lerp_color15(int r0, int g0, int b0, int r1, int g1, int b1, int num, int den)
{
    if (den <= 0)
        den = 1;
    num = clampi(num, 0, den);

    int r = r0 + ((r1 - r0) * num) / den;
    int g = g0 + ((g1 - g0) * num) / den;
    int b = b0 + ((b1 - b0) * num) / den;

    return RGB15(r, g, b);
}

#pragma once

#include <nds.h>

#define HALF_WIDTH  SCREEN_WIDTH / 2
#define HALF_HEIGHT SCREEN_HEIGHT / 2

#define SCREEN_WIDTH_FIXED  inttof32(SCREEN_WIDTH)
#define SCREEN_HEIGHT_FIXED inttof32(SCREEN_HEIGHT)

#define SIGN(x) ((x > 0) - (x < 0))

#define COLOR15_WHITE RGB15(0x1F, 0x1F, 0x1F)

typedef short Color15;

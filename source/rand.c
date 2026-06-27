#include "rand.h"

#include "game_variables.h"
#include "nds.h"
#include "nds/arm9/video.h"
#include "stdlib.h"
#include "time.h"
#include "util.h"

int rand_between(int min, int max)
{
    return min + rand() % (max - min + 1);
}

Vec2d get_rand_screen_coord(void)
{
    Vec2d coord = {
        .x = rand_between(0, SCREEN_WIDTH - 1),
        .y = rand_between(0, SCREEN_HEIGHT - 1),
    };

    return coord;
}

int get_rand_mass(void)
{
    return rand_between(inttof32(1) - g_game_vars.mass_range, inttof32(1) + g_game_vars.mass_range);
}

Vec2d get_rand_starting_vel(void)
{
    Vec2d vec = {
        .x = rand_between(-g_game_vars.init_vel, g_game_vars.init_vel),
        .y = rand_between(-g_game_vars.init_vel, g_game_vars.init_vel),
    };

    return vec;
}

Color15 get_rand_bright_color15(void)
{
    Color15 r,g,b;
    r = rand_between(0x07, 0x1F);
    g = rand_between(0x07, 0x1F);
    b = rand_between(0x07, 0x1F);

    return RGB15(r, g, b);
}

void seed_rand(void)
{
    srand((unsigned)time(NULL));
}

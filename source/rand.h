#pragma once

#include "vec2d.h"
#include "util.h"

int rand_between(int min, int max);
Vec2d get_rand_screen_coord(void);
int get_rand_mass(void);
Vec2d get_rand_starting_vel(void);
Color15 get_rand_bright_color15(void);
void seed_rand(void);

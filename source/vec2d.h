#pragma once

#define DEFAULT_VEC2D {.x = 0, .y = 0}

typedef struct
{
    int x;
    int y;
} Vec2d;

Vec2d vec2d_add(Vec2d a, Vec2d b);
Vec2d vec2d_sub(Vec2d a, Vec2d b);
Vec2d vec2d_fixed_to_int(Vec2d in);
Vec2d vec2d_int_to_fixed(Vec2d in);
Vec2d vec2d_fixed_scalar_mult(Vec2d in, int fixed_scalar);
Vec2d vec2d_scalar_mult(Vec2d in, int scalar);
int vec2_fixed_dist(Vec2d a, Vec2d b);
Vec2d vec2d_normalize(Vec2d v);

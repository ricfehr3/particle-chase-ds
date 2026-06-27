#include "vec2d.h"

#include "nds/arm9/math.h"

Vec2d vec2d_fixed_to_int(Vec2d in)
{
    Vec2d out = {
        .x = f32toint(in.x),
        .y = f32toint(in.y),
    };

    return out;
}

Vec2d vec2d_int_to_fixed(Vec2d in)
{
    Vec2d out = {
        .x = inttof32(in.x),
        .y = inttof32(in.y),
    };

    return out;
}

Vec2d vec2d_fixed_scalar_mult(Vec2d in, int fixed_scalar)
{
    Vec2d out = {
        .x = mulf32(in.x, fixed_scalar),
        .y = mulf32(in.y, fixed_scalar),
    };

    return out;
}

Vec2d vec2d_scalar_mult(Vec2d in, int scalar)
{
    Vec2d out = {
        .x = in.x * scalar,
        .y = in.y * scalar,
    };

    return out;
}

int vec2_fixed_dist(Vec2d a, Vec2d b)
{
    int dx = b.x - a.x;
    int dy = b.y - a.y;

    return inttof32(sqrt32(f32toint(mulf32(dx, dx) + mulf32(dy, dy))));
}

Vec2d vec2d_add(Vec2d a, Vec2d b)
{
    Vec2d out = {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };

    return out;
}

Vec2d vec2d_sub(Vec2d a, Vec2d b)
{
    Vec2d out = {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };

    return out;
}

Vec2d vec2d_fixed_normalize(Vec2d v)
{
    int len = vec2d_fixed_magnitude(v);

    if (len == 0)
        return (Vec2d){0, 0};

    return (Vec2d){divf32(v.x, len), divf32(v.y, len)};
}

int vec2d_fixed_magnitude(Vec2d v)
{
    return sqrt32(mulf32(v.x, v.x) + mulf32(v.y, v.y));
}

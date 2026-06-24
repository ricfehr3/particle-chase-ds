#include "nds/arm9/math.h"

#include "rigid_body.h"

#include "game_variables.h"
#include "util.h"

#include <stdlib.h>

int gravity_strength = inttof32(10);

void update_rigidbody(RigidBody* rb)
{
    rb->vel = vec2d_add(rb->vel, vec2d_fixed_scalar_mult(rb->acc, DT));

    const int epsilon = 10;
    if (abs(rb->vel.x) < epsilon)
        rb->vel.x = 0;
    if (abs(rb->vel.y) < epsilon)
        rb->vel.y = 0;

    rb->pos = vec2d_add(rb->pos, vec2d_fixed_scalar_mult(rb->vel, DT));

    if (rb->pos.x >= SCREEN_WIDTH_FIXED || rb->pos.x < 0)
    {
        rb->pos.x = SCREEN_WIDTH_FIXED - (rb->pos.x - (SCREEN_WIDTH_FIXED * SIGN(rb->pos.x)));
        rb->vel.x = -rb->vel.x;
    }

    if (rb->pos.y >= SCREEN_HEIGHT_FIXED || rb->pos.y < 0)
    {
        rb->pos.y = SCREEN_HEIGHT_FIXED - (rb->pos.y - (SCREEN_HEIGHT_FIXED * SIGN(rb->pos.y)));
        rb->vel.y = -rb->vel.y;
    }

    rb->acc.x = 0;
    rb->acc.y = 0;
}


/*
    bool has_point = false;

    for (int i = 0; i < MAX_GRAV_POINTS; i++)
    {
        if (!grav_points[i].on)
            continue;
        has_point = true;
        int dist = vec2_fixed_dist(rb->pos, grav_points[i].pos);
        int scalar = divf32(mulf32(grav_points[i].strength, rb->mass), dist);
        // this is right
        //Vec2d dir = vec2d_normalize(vec2d_sub(grav_points[i].pos, rb->pos));
        // but this is fun
        Vec2d dir = vec2d_sub(grav_points[i].pos, rb->pos);
        rb->acc.x += mulf32(dir.x, scalar);
        rb->acc.y += mulf32(dir.y, scalar);
    }

    if (!has_point)
    {
        rb->acc.x = 0;
        rb->acc.y = vert_strength;
    }

    rb->vel = vec2d_add(rb->vel, vec2d_fixed_scalar_mult(rb->acc, DT));
    rb->vel = vec2d_fixed_scalar_mult(rb->vel, inttof32(1) - drag);

    const int epsilon = 10;
    if (abs(rb->vel.x) < epsilon)
        rb->vel.x = 0;
    if (abs(rb->vel.y) < epsilon)
        rb->vel.y = 0;

    rb->pos = vec2d_add(rb->pos, vec2d_fixed_scalar_mult(rb->vel, DT));

    if (rb->pos.x >= SCREEN_WIDTH_FIXED || rb->pos.x < 0)
    {
        rb->pos.x = SCREEN_WIDTH_FIXED - (rb->pos.x - (SCREEN_WIDTH_FIXED * SIGN(rb->pos.x)));
        rb->vel.x = -rb->vel.x;
    }

    if (rb->pos.y >= SCREEN_HEIGHT_FIXED || rb->pos.y < 0)
    {
        rb->pos.y = SCREEN_HEIGHT_FIXED - (rb->pos.y - (SCREEN_HEIGHT_FIXED * SIGN(rb->pos.y)));
        rb->vel.y = -rb->vel.y;
    }

    rb->acc.x = 0;
    rb->acc.y = 0;
*/

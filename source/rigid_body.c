#include "rigid_body.h"

#include "game_variables.h"
#include "nds/arm9/math.h"
#include "util.h"

#include <stdlib.h>

int gravity_strength = inttof32(10);

void update_rigidbody(RigidBody* rb)
{
    rb->vel = vec2d_add(rb->vel, vec2d_fixed_scalar_mult(rb->acc, g_game_vars.dt));

    const int epsilon = 10;
    if (abs(rb->vel.x) < epsilon)
        rb->vel.x = 0;
    if (abs(rb->vel.y) < epsilon)
        rb->vel.y = 0;

    rb->pos = vec2d_add(rb->pos, vec2d_fixed_scalar_mult(rb->vel, g_game_vars.dt));

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

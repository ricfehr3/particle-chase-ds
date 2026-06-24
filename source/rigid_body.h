#pragma once

#include "vec2d.h"

typedef struct
{
    Vec2d pos; // position, fixed point
    Vec2d vel; // velocity, fixed point
    Vec2d acc; // acceleration, fixed point
    int mass;
} RigidBody;

#define DEFAULT_RIGIDBODY     \
    {                         \
        .pos = DEFAULT_VEC2D, \
        .vel = DEFAULT_VEC2D, \
        .acc = DEFAULT_VEC2D, \
        .mass = inttof32(1),  \
    }

void update_rigidbody(RigidBody* rb);

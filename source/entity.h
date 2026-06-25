#pragma once

#include "rigid_body.h"
#include "vec2d.h"

#define MAX_ENTITIES      2500

typedef struct
{
    short color;
    Vec2d screen_pos; // only valid after update
    RigidBody rb;
} Entity;

void update_entities(void);

void randomize_masses(void);
void init_entities(bool reset_pos);
void display_entities(void);

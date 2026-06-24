#pragma once

#include "stdbool.h"
#include "vec2d.h"

#define MAX_GRAVITY       inttof32(1000)
#define MAX_GRAV_POINTS 5

typedef struct
{
    Vec2d pos;
    int strength;
    bool on;
    // add movement script based on frame length
} GravWell;

GravWell* gravity_wells(void);
void remove_grav_point(unsigned int offset);
void remove_all_grav_points(void);
bool has_wells(void);
void update_gravity_wells(void);
int register_grav_point(Vec2d point, int strength);

#pragma once

#include "stdbool.h"
#include "vec2d.h"

#define MAX_GRAVITY     inttof32(1000)
#define MAX_GRAV_POINTS 10

enum GravWellTypes
{
    GRAV_WELL_NORMAL,
    GRAV_WELL_SPRING,
};

enum GravWellDir
{
    GRAV_WELL_ATTRACT,
    GRAV_WELL_REPULSE,
};

typedef struct
{
    Vec2d pos;
    int strength;
    bool on;
    enum GravWellTypes type;
    enum GravWellDir dir;
    // add movement script based on frame length
} GravWell;

GravWell* gravity_wells(void);
void remove_grav_point(unsigned int offset);
void remove_all_grav_points(void);
bool has_wells(void);
void display_gravity_wells(void);
int register_grav_point(Vec2d point, int strength, enum GravWellTypes type, enum GravWellDir dir);

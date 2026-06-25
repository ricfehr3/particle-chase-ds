#pragma once

#include "gravity_well.h"
#define MAX_DRAG          floattof32(0.5)
#define MAX_INIT_VEL      inttof32(1000)
#define MAX_DT            3000
#define MIN_MASS_RANGE    100
#define MAX_MASS_RANGE    4000

typedef struct
{
    int mass_range;
    int init_vel;
    int num_entities;
    int gravity_strength;
    enum GravWellDir grav_dir;
    enum GravWellTypes grav_type;
    int vert_strength;
    int drag;
    int dt;
} GameVariables;

extern GameVariables g_game_vars;

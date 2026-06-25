#pragma once

#include "gravity_well.h"
#define MAX_DRAG          floattof32(0.9)
#define MAX_INIT_VEL      inttof32(1000)
#define MAX_DT            3000
#define MIN_MASS_RANGE    0
#define MAX_MASS_RANGE    4000

#define DEFAULT_MASS_RANGE  400
#define DEFAULT_INIT_VEL floattof32(10.0)
#define DEFAULT_NUM_ENTITIES 10
#define DEFAULT_GRAVITY_STRENGTH floattof32(10.0)
#define DEFAULT_VERT_STRENGTH floattof32(10.0)
#define DEFAULT_DRAG floattof32(0.001)
#define DEFAULT_DT 400

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

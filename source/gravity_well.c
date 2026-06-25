#include "gravity_well.h"

#include "gl2d.h"
#include "nds/arm9/video.h"
#include "util.h"

GravWell grav_points[MAX_GRAV_POINTS] = {0};

static unsigned int used_wells = 0;

GravWell* gravity_wells(void)
{
    return &grav_points[0];
}

void remove_grav_point(unsigned int offset)
{
    if(!has_wells()) return;
    if (offset < MAX_GRAV_POINTS)
    {
        grav_points[offset].on = false;
        used_wells--;
    }
}

void remove_all_grav_points(void)
{
    for(int i = 0; i < MAX_GRAV_POINTS; i++)
    {
        grav_points[i].on = false;
    }
    used_wells = 0;
}

int register_grav_point(Vec2d point, int strength, enum GravWellTypes type, enum GravWellDir dir)
{
    for (int i = 0; i < MAX_GRAV_POINTS; i++)
    {
        if (grav_points[i].on)
            continue;
        grav_points[i].strength = strength;
        grav_points[i].pos = point;
        grav_points[i].on = true;
        grav_points[i].type = type;
        grav_points[i].dir = dir;
        used_wells++;
        return i;
    }
    return -1;
}

bool has_wells(void)
{
    return used_wells > 0;
}

void display_gravity_wells(void)
{
    glBegin2D();

    for (int i = 0; i < MAX_GRAV_POINTS; i++)
    {
        if (!grav_points[i].on)
            continue;

        int bb = 1;                                        // box boundary
        Vec2d bo = vec2d_fixed_to_int(grav_points[i].pos); // box origin
        Color15 col = (grav_points[i].dir == GRAV_WELL_ATTRACT) ? RGB15(0, 0, 0x1F) : RGB15(0x1F, 0, 0);
        col |= (grav_points[i].type == GRAV_WELL_NORMAL) ? RGB15(0, 0, 0) : RGB15(0, 0x1F, 0);
        glBoxFilled(bo.x - bb, bo.y - bb, bo.x + bb, bo.y + bb, col);
    }

    glEnd2D();
}

#include "entity.h"

#include "game_variables.h"
#include "gl2d.h"
#include "gravity_well.h"
#include "nds/arm9/math.h"
#include "rand.h"
#include "rigid_body.h"
#include "stdio.h"
#include "util.h"
#include "vec2d.h"

#include <stdbool.h>

#define DEFAULT_ENTITY               \
    {                                \
        .rb = DEFAULT_RIGIDBODY,     \
        .screen_pos = DEFAULT_VEC2D, \
        .color = COLOR15_WHITE,      \
    }

Entity entities[MAX_ENTITIES];

static Entity build_default_entity(void);

void update_entities(void)
{
    // points of gravity
    auto grav_points = gravity_wells();

    for (int i = 0; i < g_game_vars.num_entities; i++)
    {
        Entity* e = &entities[i];

        auto rb = &e->rb;
        if (has_wells())
        {
            for (int i = 0; i < MAX_GRAV_POINTS; i++)
            {
                if (!grav_points[i].on)
                    continue;

                int dist = vec2_fixed_dist(rb->pos, grav_points[i].pos);
                int scalar = divf32(mulf32(grav_points[i].strength, rb->mass), dist);
                Vec2d dir;

                switch (grav_points[i].type)
                {
                    case GRAV_WELL_NORMAL:
                        dir = vec2d_normalize(vec2d_sub(grav_points[i].pos, rb->pos));
                        break;
                    case GRAV_WELL_SPRING:
                        dir = vec2d_sub(grav_points[i].pos, rb->pos);
                        break;
                    default:
                        break;
                };

                int sign = grav_points[i].dir == GRAV_WELL_ATTRACT ? 1 : -1;

                rb->acc.x += sign * mulf32(dir.x, scalar);
                rb->acc.y += sign * mulf32(dir.y, scalar);
            }
        }
        else
        {
            rb->acc.x = 0;
            rb->acc.y = g_game_vars.vert_strength;
        }

        update_rigidbody(rb);

        rb->vel = vec2d_fixed_scalar_mult(
            rb->vel,
            inttof32(1) - mulf32(g_game_vars.drag, g_game_vars.dt)
        );

        e->screen_pos = vec2d_fixed_to_int(rb->pos);
    }
}

void randomize_masses(void)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        entities[i].rb.mass = get_rand_mass();
    }
}

void init_entities(bool reset_pos)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (reset_pos)
        {
            entities[i] = build_default_entity();
            Vec2d rand_screen_pos = get_rand_screen_coord();
            entities[i].rb.pos = vec2d_int_to_fixed(rand_screen_pos);
        }

        entities[i].rb.vel = get_rand_starting_vel();
        if(g_game_vars.color)
            entities[i].color = get_rand_bright_color15();
        else
            entities[i].color = COLOR15_WHITE;
    }

    if (reset_pos)
        randomize_masses();
}

void display_entities(void)
{
    auto grav_points = gravity_wells();

    glBegin2D();

    for (int i = 0; i < g_game_vars.num_entities; i++)
    {
        Entity* e = &entities[i];

        glPutPixel(e->screen_pos.x, e->screen_pos.y, e->color);
    }

    glEnd2D();
}

static Entity build_default_entity(void)
{
    Entity entity = DEFAULT_ENTITY;
    return entity;
}

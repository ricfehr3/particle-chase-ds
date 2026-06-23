#include "nds/arm9/math.h"
#include "nds/arm9/trig_lut.h"
#include "nds/arm9/video.h"

#include "nds/arm9/math.h"
#include <gl2d.h>
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

#define HALF_WIDTH  (SCREEN_WIDTH / 2)
#define HALF_HEIGHT (SCREEN_HEIGHT / 2)

#define SCREEN_WIDTH_FIXED  inttof32(SCREEN_WIDTH)
#define SCREEN_HEIGHT_FIXED inttof32(SCREEN_HEIGHT)

int rand_between(int min, int max)
{
    return min + rand() % (max - min + 1);
}

typedef short Color15;

typedef struct
{
    int x;
    int y;
} Vec2d;

Vec2d get_rand_screen_coord(void)
{
    Vec2d coord = {
        .x = rand_between(0, SCREEN_WIDTH - 1),
        .y = rand_between(0, SCREEN_HEIGHT - 1),
    };

    return coord;
}

#define MAX_VEL inttof32(10)

Vec2d get_rand_starting_vel(void)
{
    Vec2d vec = {
        .x = rand_between(-MAX_VEL, MAX_VEL),
        .y = rand_between(-MAX_VEL, MAX_VEL),
    };

    return vec;
}

Vec2d vec2d_fixed_to_int(Vec2d in)
{
    Vec2d out = {
        .x = f32toint(in.x),
        .y = f32toint(in.y),
    };

    return out;
}

Vec2d vec2d_int_to_fixed(Vec2d in)
{
    Vec2d out = {
        .x = inttof32(in.x),
        .y = inttof32(in.y),
    };

    return out;
}

#define DEFAULT_VEC2D {.x = 0, .y = 0}

typedef struct
{
    Vec2d pos; // position, fixed point
    Vec2d vel; // velocity, fixed point
    Vec2d acc; // acceleration, fixed point
} RigidBody;

#define DEFAULT_RIGIDBODY     \
    {                         \
        .pos = DEFAULT_VEC2D, \
        .vel = DEFAULT_VEC2D, \
        .acc = DEFAULT_VEC2D, \
    }

#define COLOR15_WHITE RGB15(0x1F, 0x1F, 0x1F)

typedef struct
{
    Color15 color;
    Vec2d screen_pos; // only valid after update
    RigidBody rb;
} Entity;

#define DEFAULT_ENTITY               \
    {                                \
        .rb = DEFAULT_RIGIDBODY,     \
        .screen_pos = DEFAULT_VEC2D, \
        .color = COLOR15_WHITE,      \
    }

Entity build_default_entity(void)
{
    Entity entity = DEFAULT_ENTITY;
    return entity;
}

#define NUM_ENTITIES 100
#define DT           floattof32(1.0f / 2500.0f)

Entity entities[NUM_ENTITIES];

void init_entities(void)
{
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        entities[i] = build_default_entity();

        Vec2d rand_screen_pos = get_rand_screen_coord();

        entities[i].rb.pos = vec2d_int_to_fixed(rand_screen_pos);
        entities[i].rb.vel = get_rand_starting_vel();
    }
}

#define SIGN(x) (x > 0) - (x < 0)

void update_rigidbody(RigidBody* rb)
{
    int drag = floattof32(0.99);

    rb->pos.x += rb->vel.x * DT;
    rb->pos.y += rb->vel.y * DT;

    rb->vel.x += mulf32(rb->acc.x, DT);
    rb->vel.y += mulf32(rb->acc.y, DT);
    rb->vel.x = mulf32(rb->vel.x, drag);
    rb->vel.y = mulf32(rb->vel.y, drag);
    if(abs(rb->vel.x) < DT) rb->vel.x = 0;
    if(abs(rb->vel.y) < DT) rb->vel.y = 0;

    if(rb->pos.x >= SCREEN_WIDTH_FIXED || rb->pos.x < 0)
    {
        int itr = (rb->pos.x < 0) ? -1 : 1;
        rb->pos.x = SCREEN_WIDTH_FIXED - (rb->pos.x - (SCREEN_WIDTH_FIXED * itr));
        rb->vel.x = -rb->vel.x;
    }

    if(rb->pos.y >= SCREEN_HEIGHT_FIXED || rb->pos.y < 0)
    {
        int itr = (rb->pos.y < 0) ? -1 : 1;
        rb->pos.y = SCREEN_HEIGHT_FIXED - (rb->pos.y - (SCREEN_HEIGHT_FIXED * itr));
        rb->vel.y = -rb->vel.y;
    }

    rb->acc.x = 0;
    rb->acc.y = 0;
}

void update_entities(int frame)
{
    (void)frame;

    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        Entity* e = &entities[i];

        update_rigidbody(&e->rb);

        e->screen_pos = vec2d_fixed_to_int(e->rb.pos);
    }
}

void display_entities(void)
{
    glBegin2D();

    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        Entity* e = &entities[i];
        glPutPixel(e->screen_pos.x, e->screen_pos.y, e->color);
    }

    glEnd2D();
}

int main()
{
    consoleDemoInit();

    videoSetMode(MODE_5_3D);

    consoleDemoInit();

    glScreen2D();

    int frame = 0;

    init_entities();

    iprintf("\x1b[1;1HGravity Demo");

    while (pmMainLoop())
    {
        frame++;

        update_entities(frame);

        display_entities();

        glFlush(0);
    }

    return 0;
}

#include "nds/arm9/math.h"
#include "nds/arm9/trig_lut.h"
#include "nds/arm9/video.h"
#include "vec2d.h"

#include <gl2d.h>
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

#define HALF_WIDTH  (SCREEN_WIDTH / 2)
#define HALF_HEIGHT (SCREEN_HEIGHT / 2)

#define SCREEN_WIDTH_FIXED  inttof32(SCREEN_WIDTH)
#define SCREEN_HEIGHT_FIXED inttof32(SCREEN_HEIGHT)

#define DT                floattof32(0.1)
#define MAX_ENTITIES      2500
#define MAX_GRAVITY       100000
#define MAX_DRAG          floattof32(0.5)
#define MAX_INIT_VEL      inttof32(10)
#define MAX_VERT_STRENGTH 1000

int num_entities = 1000;
int init_vel = inttof32(10);
int gravity_strength = inttof32(10);
int vert_strength = inttof32(10);
// int drag = floattof32(0.001);
int drag = 64;
int mass_range = floattof32(0.01); // +/- 1.0 fixed

typedef struct
{
    Vec2d pos;
    int strength;
    bool on;
    // add movement script based on frame length
} GravWell;

void print_config(void)
{
    iprintf("\x1b[3;1HEntities:   %10d", num_entities);
    iprintf("\x1b[4;1HReset Vel:  %10d", init_vel);
    iprintf("\x1b[5;1HGrav Well:  %10d", gravity_strength);
    iprintf("\x1b[6;1HGravity:    %10d", vert_strength);
    iprintf("\x1b[7;1HDrag:       %10d", drag);
    iprintf("\x1b[8;1HMass Range: %10d", mass_range);
    iprintf("\x1b[9;1HDT:         %10d", DT);
}

int rand_between(int min, int max)
{
    return min + rand() % (max - min + 1);
}

typedef short Color15;

Vec2d get_rand_screen_coord(void)
{
    Vec2d coord = {
        .x = rand_between(0, SCREEN_WIDTH - 1),
        .y = rand_between(0, SCREEN_HEIGHT - 1),
    };

    return coord;
}

int get_rand_mass(void)
{
    return rand_between(inttof32(1) - mass_range, inttof32(1) + mass_range);
}

Vec2d get_rand_starting_vel(void)
{
    Vec2d vec = {
        .x = rand_between(-init_vel, init_vel),
        .y = rand_between(-init_vel, init_vel),
    };

    return vec;
}

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

Entity entities[MAX_ENTITIES];

void init_entities(bool reset_pos)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (reset_pos)
            entities[i] = build_default_entity();

        if (reset_pos)
            entities[i].rb.mass = get_rand_mass();

        Vec2d rand_screen_pos = get_rand_screen_coord();

        if (reset_pos)
            entities[i].rb.pos = vec2d_int_to_fixed(rand_screen_pos);

        entities[i].rb.vel = get_rand_starting_vel();
    }
}

// Vec2d grav_point = DEFAULT_VEC2D;
bool has_grav_point = false;

#define MAX_GRAV_POINTS 5
GravWell grav_points[MAX_GRAV_POINTS];

#define SIGN(x) ((x > 0) - (x < 0))

void update_rigidbody(RigidBody* rb)
{
    bool has_point = false;

    for (int i = 0; i < MAX_GRAV_POINTS; i++)
    {
        if (!grav_points[i].on)
            continue;
        has_point = true;
        int dist = vec2_fixed_dist(rb->pos, grav_points[i].pos);
        int scalar = divf32(mulf32(grav_points[i].strength, rb->mass), dist);
        // int scalar = divf32(gravity_strength, dist);
        rb->acc.x += mulf32(grav_points[i].pos.x - rb->pos.x, scalar);
        rb->acc.y += mulf32(grav_points[i].pos.y - rb->pos.y, scalar);
    }

    if (!has_point)
    {
        rb->acc.x = 0;
        rb->acc.y = vert_strength;
    }

    rb->vel = vec2d_add(rb->vel, vec2d_fixed_scalar_mult(rb->acc, DT));
    rb->vel = vec2d_fixed_scalar_mult(rb->vel, inttof32(1) - drag);

    const int epsilon = 10;
    if (abs(rb->vel.x) < epsilon)
        rb->vel.x = 0;
    if (abs(rb->vel.y) < epsilon)
        rb->vel.y = 0;

    rb->pos = vec2d_add(rb->pos, vec2d_fixed_scalar_mult(rb->vel, DT));

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

void update_entities(int frame)
{
    (void)frame;

    for (int i = 0; i < num_entities; i++)
    {
        Entity* e = &entities[i];

        update_rigidbody(&e->rb);

        e->screen_pos = vec2d_fixed_to_int(e->rb.pos);
    }
}

void display_entities(void)
{
    glBegin2D();

    for (int i = 0; i < num_entities; i++)
    {
        Entity* e = &entities[i];
        glPutPixel(e->screen_pos.x, e->screen_pos.y, e->color);
        for (int i = 0; i < MAX_GRAV_POINTS; i++)
        {
            if (!grav_points[i].on)
                continue;
            int bb = 1;                                        // box boundary
            Vec2d bo = vec2d_fixed_to_int(grav_points[i].pos); // box origin
            glBoxFilled(bo.x - bb, bo.y - bb, bo.x + bb, bo.y + bb, RGB15(0, 0x1F, 0x1F));
        }
    }

    glEnd2D();
}

int register_grav_point(Vec2d point, int strength)
{
    for (int i = 0; i < MAX_GRAV_POINTS; i++)
    {
        if (grav_points[i].on)
            continue;
        grav_points[i].strength = strength;
        grav_points[i].pos = point;
        grav_points[i].on = true;
        return i;
    }
    return -1;
}

void remove_grav_point(unsigned int offset)
{
    if (offset < MAX_GRAV_POINTS)
        grav_points[offset].on = false;
}

int main()
{
    consoleDemoInit();

    videoSetMode(MODE_5_3D);

    consoleDemoInit();

    glScreen2D();

    int frame = 0;
    touchPosition touchXY;

    init_entities(true);

    Vec2d middle = {
        .x = inttof32(HALF_WIDTH),
        .y = inttof32(HALF_HEIGHT),
    };

    register_grav_point(middle, floattof32(100));

    while (pmMainLoop())
    {
        frame++;

        has_grav_point = touchRead(&touchXY);

        scanKeys();
        int pressed = keysDown();
        if (pressed & KEY_UP)
        {
            int incr = (num_entities >= 100) ? 100 : (num_entities >= 10) ? 10 : 1;
            num_entities += incr;
            if (num_entities > MAX_ENTITIES)
                num_entities = MAX_ENTITIES;
        }
        if (pressed & KEY_DOWN)
        {
            int incr = (num_entities > 100) ? 100 : (num_entities > 10) ? 10 : 1;
            num_entities -= incr;
            if (num_entities < 1)
                num_entities = 1;
        }
        if (pressed & KEY_RIGHT)
        {
            gravity_strength += 1000;
            if (gravity_strength > MAX_GRAVITY)
                gravity_strength = MAX_GRAVITY;
        }
        if (pressed & KEY_LEFT)
        {
            gravity_strength -= 1000;
            if (gravity_strength < 1000)
                gravity_strength = 1000;
        }
        if (pressed & KEY_X)
        {
            drag += floattof32(0.001);
            if (drag > MAX_DRAG)
                drag = MAX_DRAG;
        }
        if (pressed & KEY_Y)
        {
            drag -= floattof32(0.001);
            if (drag < 0)
                drag = 0;
        }
        if (pressed & KEY_A)
        {
            init_vel += floattof32(0.1);
            if (init_vel > MAX_INIT_VEL)
                init_vel = MAX_INIT_VEL;
        }
        if (pressed & KEY_B)
        {
            init_vel -= floattof32(0.1);
            if (init_vel < 0)
                init_vel = 0;
        }
        if (pressed & KEY_R)
        {
            vert_strength += 100;
            if (vert_strength > MAX_VERT_STRENGTH)
                vert_strength = MAX_VERT_STRENGTH;
        }
        if (pressed & KEY_L)
        {
            vert_strength -= 100;
            if (vert_strength < 0)
                vert_strength = 0;
        }
        if (pressed & KEY_START)
            init_entities(true);
        if (pressed & KEY_SELECT)
            init_entities(false);

        Vec2d touch_fixed = {
            .x = inttof32(touchXY.px),
            .y = inttof32(touchXY.py),
        };

        int reg = -1;
        if (has_grav_point)
            reg = register_grav_point(touch_fixed, gravity_strength);

        update_entities(frame);

        display_entities();

        print_config();

        glFlush(0);

        if (has_grav_point)
            remove_grav_point(reg);
    }

    return 0;
}

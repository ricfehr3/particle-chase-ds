#include "ball_pcx.h"
#include "entity.h"
#include "game_variables.h"
#include "gravity_well.h"
#include "nds/arm9/input.h"
#include "nds/arm9/math.h"
#include "nds/arm9/sprite.h"
#include "nds/arm9/trig_lut.h"
#include "nds/arm9/video.h"
#include "rand.h"
#include "rigid_body.h"
#include "sprite.h"
#include "util.h"
#include "vec2d.h"

#include <gl2d.h>
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

int grav_head = -1;
int grav_stack[MAX_GRAV_POINTS] = {0};

#define VERSION "v0.0.1"

void print_config(void)
{
    iprintf("\x1b[3;1HEntities:  %7d UP/DOWN", g_game_vars.num_entities);
    iprintf("\x1b[4;1HGrav Well: %7d LEFT/RIGHT", g_game_vars.gravity_strength);
    iprintf("\x1b[5;1HGrav Floor:%7d R+UP/DOWN", g_game_vars.vert_strength);
    iprintf("\x1b[6;1HMass Range:%7d R+LEFT/RIGHT", g_game_vars.mass_range);
    iprintf("\x1b[7;1HDrag:      %7d L+UP/DOWN", g_game_vars.drag);
    iprintf("\x1b[8;1HReset Vel: %7d L+LEFT/RIGHT", g_game_vars.init_vel);
    iprintf("\x1b[9;1HSim Time:  %7d L+A/B", g_game_vars.dt);
    iprintf(
        "\x1b[10;1HDirection: %7s X/Y",
        g_game_vars.grav_dir == GRAV_WELL_ATTRACT ? "Attract" : "Repulse"
    );
    iprintf(
        "\x1b[11;1HMode:      %7s A/B",
        g_game_vars.grav_type == GRAV_WELL_NORMAL ? "Normal" : "Spring"
    );
    iprintf("\x1b[13;1HSave Pos:          Touch+L/R");
    iprintf("\x1b[14;1HDelete Last:       L+X");
    iprintf("\x1b[15;1HReset Entities:    Start");
    iprintf("\x1b[16;1HRandomize Vels:    Select");
    iprintf("\x1b[17;1HCycle Color:       R+Select");
    iprintf("\x1b[18;1HReset All:         R+Start");

    iprintf("\x1b[23;1H                RF3 2026 %6s", VERSION);
}

GameVariables g_game_vars = {
    .mass_range = DEFAULT_MASS_RANGE,
    .init_vel = DEFAULT_INIT_VEL,
    .num_entities = DEFAULT_NUM_ENTITIES,
    .gravity_strength = DEFAULT_GRAVITY_STRENGTH,
    .vert_strength = DEFAULT_VERT_STRENGTH,
    .drag = DEFAULT_DRAG,
    .dt = DEFAULT_DT,
    .color = COLOR_STATIC,
};

void reset_game_variables()
{
    g_game_vars.mass_range = DEFAULT_MASS_RANGE;
    g_game_vars.init_vel = DEFAULT_INIT_VEL;
    g_game_vars.num_entities = DEFAULT_NUM_ENTITIES;
    g_game_vars.gravity_strength = DEFAULT_GRAVITY_STRENGTH;
    g_game_vars.vert_strength = DEFAULT_VERT_STRENGTH;
    g_game_vars.drag = DEFAULT_DRAG;
    g_game_vars.dt = DEFAULT_DT;
    g_game_vars.color = COLOR_STATIC;
}

int main()
{
    PrintConsole bottomConsole;

    bool pen_is_down = false;

    seed_rand();

    videoSetMode(MODE_0_3D);

    videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);

    vramSetBankA(VRAM_A_TEXTURE); // 128 KB texture VRAM
    vramSetBankB(VRAM_B_TEXTURE); // 128 KB texture VRAM
    vramSetBankC(VRAM_C_SUB_BG);
    vramSetBankD(VRAM_D_SUB_SPRITE); // bottom sprite graphics

    glScreen2D();

    consoleInit(
        &bottomConsole,
        0, // BG layer
        BgType_Text4bpp,
        BgSize_T_256x256,
        31,    // map base, high end of BG VRAM
        0,     // tile base
        false, // false = sub screen
        true
    );

    consoleSelect(&bottomConsole);
    u16* map = bgGetMapPtr(bottomConsole.bgId);
    dmaFillHalfWords(0, map, 32 * 32 * sizeof(u16));

    consoleClear();
    touchPosition touchXY;

    init_entities(true);

    initOAM();

    sImage ball;

    int i;

    // load our ball pcx file into an image
    loadPCX((u8*)ball_pcx, &ball);
    imageTileData(&ball);

    for (i = 0; i < 256; i++)
        SPRITE_PALETTE_SUB[i] = ball.palette[i];

    for (i = 0; i < 32 * 16; i++)
        SPRITE_GFX_SUB[i] = ball.image.data16[i];

    int ball_center_offset = inttof32(16);
    Sprite sprites[1];
    for (i = 0; i < 1; i++)
    {
        // random place and speed
        sprites[i].x = inttof32(HALF_WIDTH - ball_center_offset);
        sprites[i].y = inttof32(HALF_HEIGHT);
        sprites[i].dx = (rand() & 0xFF) + 0x100;
        sprites[i].dy = (rand() & 0xFF) + 0x100;

        if (rand() & 1)
            sprites[i].dx = -sprites[i].dx;
        if (rand() & 1)
            sprites[i].dy = -sprites[i].dy;

        sprites[i].oam = getOAMCopySub(i);
        sprites[i].gfxID = 0;

        // set up our sprites OAM entry attributes
        sprites[i].oam->attribute[0] = ATTR0_COLOR_256 | ATTR0_SQUARE;
        sprites[i].oam->attribute[1] = ATTR1_SIZE_32;
        sprites[i].oam->attribute[2] = sprites[i].gfxID;
    }

    // make this configurable
    BG_PALETTE_SUB[0] = RGB15(0, 1, 5);
    // BG_PALETTE_SUB[1] = RGB15(0,16,0);
    // BG_PALETTE_SUB[2] = RGB15(0,0,31);

    while (pmMainLoop())
    {
        pen_is_down = touchRead(&touchXY);

        if (pen_is_down)
            sprites[0].x = inttof32(touchXY.px) - ball_center_offset;
        else
            sprites[0].x = inttof32(HALF_WIDTH) - ball_center_offset;

        scanKeys();

        int pressed = keysDownRepeat();

        Vec2d touch_fixed = {
            .x = inttof32(touchXY.px),
            .y = inttof32(touchXY.py),
        };

        if ((pressed & KEY_R || pressed & KEY_L) && pen_is_down)
        {
            int val = register_grav_point(
                touch_fixed,
                g_game_vars.gravity_strength,
                g_game_vars.grav_type,
                g_game_vars.grav_dir
            );
            if (val != -1)
            {
                grav_stack[++grav_head] = val;
            }
        }
        else if (keysHeld() & KEY_L)
        {
            if (pressed & KEY_UP)
            {
                int incr = (g_game_vars.drag >= 500) ? 100 : (g_game_vars.drag >= 25) ? 25 : 5;
                g_game_vars.drag += incr;
                if (g_game_vars.drag > MAX_DRAG)
                    g_game_vars.drag = MAX_DRAG;
            }
            if (pressed & KEY_DOWN)
            {
                int incr = (g_game_vars.drag > 500) ? 100 : (g_game_vars.drag > 25) ? 25 : 5;
                g_game_vars.drag -= incr;
                if (g_game_vars.drag < 0)
                    g_game_vars.drag = 0;
            }
            if (pressed & KEY_RIGHT)
            {
                g_game_vars.init_vel += inttof32(1);
                if (g_game_vars.init_vel > MAX_INIT_VEL)
                    g_game_vars.init_vel = MAX_INIT_VEL;
            }
            if (pressed & KEY_LEFT)
            {
                g_game_vars.init_vel -= floattof32(0.1);
                if (g_game_vars.init_vel < 0)
                    g_game_vars.init_vel = 0;
            }
            if (pressed & KEY_A)
            {
                int incr = (g_game_vars.dt >= 400) ? 100
                         : (g_game_vars.dt >= 100) ? 50
                         : (g_game_vars.dt >= 10)  ? 10
                                                   : 1;
                g_game_vars.dt += incr;
                if (g_game_vars.dt > MAX_DT)
                    g_game_vars.dt = MAX_DT;
            }
            if (pressed & KEY_B)
            {
                int incr = (g_game_vars.dt > 400) ? 100
                         : (g_game_vars.dt > 100) ? 50
                         : (g_game_vars.dt > 10)  ? 10
                                                  : 1;
                g_game_vars.dt -= incr;
                if (g_game_vars.dt < 0)
                    g_game_vars.dt = 0;
            }
            if (pressed & KEY_X)
            {
                if (grav_head >= 0)
                {
                    remove_grav_point(grav_stack[grav_head--]);
                }
            }
        }
        else if (keysHeld() & KEY_R)
        {
            if (pressed & KEY_UP)
            {
                g_game_vars.vert_strength += inttof32(1);
                if (g_game_vars.vert_strength > MAX_GRAVITY)
                    g_game_vars.vert_strength = MAX_GRAVITY;
            }
            if (pressed & KEY_DOWN)
            {
                g_game_vars.vert_strength -= inttof32(1);
                if (g_game_vars.vert_strength < 0)
                    g_game_vars.vert_strength = 0;
            }
            if (pressed & KEY_RIGHT)
            {
                g_game_vars.mass_range += 100;
                if (g_game_vars.mass_range > MAX_MASS_RANGE)
                    g_game_vars.mass_range = MAX_MASS_RANGE;
                randomize_masses();
            }
            if (pressed & KEY_LEFT)
            {
                g_game_vars.mass_range -= 100;
                if (g_game_vars.mass_range < MIN_MASS_RANGE)
                    g_game_vars.mass_range = MIN_MASS_RANGE;
                randomize_masses();
            }
            if (pressed & KEY_START)
            {
                remove_all_grav_points();
                init_entities(true);
                reset_game_variables();
            }
            if (pressed & KEY_SELECT)
            {
                g_game_vars.color = (g_game_vars.color + 1) % COLOR_MAX;
                init_entities(false);
            }
        }
        else
        {
            if (pressed & KEY_UP)
            {
                int incr = (g_game_vars.num_entities >= 100) ? 100
                         : (g_game_vars.num_entities >= 10)  ? 10
                                                             : 1;
                g_game_vars.num_entities += incr;
                if (g_game_vars.num_entities > MAX_ENTITIES)
                    g_game_vars.num_entities = MAX_ENTITIES;
            }
            if (pressed & KEY_DOWN)
            {
                int incr = (g_game_vars.num_entities > 100) ? 100
                         : (g_game_vars.num_entities > 10)  ? 10
                                                            : 1;
                g_game_vars.num_entities -= incr;
                if (g_game_vars.num_entities < 1)
                    g_game_vars.num_entities = 1;
            }
            if (pressed & KEY_RIGHT)
            {
                g_game_vars.gravity_strength += inttof32(1);
                if (g_game_vars.gravity_strength > MAX_GRAVITY)
                    g_game_vars.gravity_strength = MAX_GRAVITY;
            }
            if (pressed & KEY_LEFT)
            {
                g_game_vars.gravity_strength -= inttof32(1);
                if (g_game_vars.gravity_strength < 0)
                    g_game_vars.gravity_strength = 0;
            }
            if (pressed & KEY_X)
            {
                g_game_vars.grav_dir = GRAV_WELL_ATTRACT;
            }
            if (pressed & KEY_Y)
            {
                g_game_vars.grav_dir = GRAV_WELL_REPULSE;
            }
            if (pressed & KEY_A)
            {
                g_game_vars.grav_type = GRAV_WELL_NORMAL;
            }
            if (pressed & KEY_B)
            {
                g_game_vars.grav_type = GRAV_WELL_SPRING;
            }
            if (pressed & KEY_START)
            {
                init_entities(true);
            }
            if (pressed & KEY_SELECT)
            {
                init_entities(false);
            }
        }

        int reg = -1;
        if (pen_is_down)
            reg = register_grav_point(
                touch_fixed,
                g_game_vars.gravity_strength,
                g_game_vars.grav_type,
                g_game_vars.grav_dir
            );

        display_gravity_wells();
        update_entities();

        display_entities();

        print_config();

        MoveSprite(&sprites[0]);

        glFlush(0);

        if (pen_is_down)
            remove_grav_point(reg);

        // updateOAM();
    }

    return 0;
}

#include "nds/arm9/math.h"
#include "nds/arm9/sprite.h"
#include "nds/arm9/trig_lut.h"
#include "nds/arm9/video.h"
#include "gravity_well.h"
#include "rigid_body.h"
#include "sprite.h"
#include "vec2d.h"
#include "util.h"
#include "game_variables.h"

#include "entity.h"

#include "ball_pcx.h"

#include <gl2d.h>
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

void print_config(void)
{
    iprintf("\x1b[3;1HEntities:   %10d", g_game_vars.num_entities);
    iprintf("\x1b[4;1HReset Vel:  %10d", g_game_vars.init_vel);
    iprintf("\x1b[5;1HGrav Well:  %10d", g_game_vars.gravity_strength);
    iprintf("\x1b[6;1HGravity:    %10d", g_game_vars.vert_strength);
    iprintf("\x1b[7;1HDrag:       %10d", g_game_vars.drag);
    iprintf("\x1b[8;1HMass Range: %10d", g_game_vars.mass_range);
    iprintf("\x1b[9;1HDT:         %10d", DT);
}

GameVariables g_game_vars = {
    .mass_range = floattof32(0.1),
    .init_vel = floattof32(10.0),
    .num_entities = 10,
    .gravity_strength = floattof32(1.0),
    .vert_strength = floattof32(1.0),
    .drag = floattof32(0.001),
};

int main()
{
    PrintConsole bottomConsole;

    bool has_grav_point = false;


    videoSetMode(MODE_0_3D);

	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE |
					DISPLAY_SPR_1D_LAYOUT |
					DISPLAY_SPR_ACTIVE);

    vramSetBankA(VRAM_A_TEXTURE);      // 128 KB texture VRAM
    vramSetBankB(VRAM_B_TEXTURE);      // 128 KB texture VRAM
    vramSetBankC(VRAM_C_SUB_BG); 
    vramSetBankD(VRAM_D_SUB_SPRITE);   // bottom sprite graphics

    glScreen2D();

    consoleInit(
        &bottomConsole,
        0,                 // BG layer
        BgType_Text4bpp,
        BgSize_T_256x256,
        31,                // map base, high end of BG VRAM
        0,                 // tile base
        false,             // false = sub screen
        true
    );

    consoleSelect(&bottomConsole);
    u16 *map = bgGetMapPtr(bottomConsole.bgId);
    dmaFillHalfWords(0, map, 32 * 32 * sizeof(u16));

    consoleClear();
    touchPosition touchXY;

    init_entities(true);

    Vec2d middle = {
        .x = inttof32(HALF_WIDTH),
        .y = inttof32(HALF_HEIGHT),
    };

    //register_grav_point(middle, floattof32(1));

    initOAM();

	sImage ball;

    int i;

	//load our ball pcx file into an image
	loadPCX((u8*)ball_pcx, &ball);
	imageTileData(&ball);

	for(i = 0; i < 256; i++)
		SPRITE_PALETTE_SUB[i] = ball.palette[i];

	for(i = 0; i< 32*16; i++)
		SPRITE_GFX_SUB[i] = ball.image.data16[i];

    int ball_center_offset = inttof32(16);
	Sprite sprites[1];
	for(i = 0; i < 1; i++) {
		//random place and speed
		sprites[i].x = inttof32(HALF_WIDTH - ball_center_offset);
		sprites[i].y = inttof32(HALF_HEIGHT);
		sprites[i].dx = (rand() & 0xFF) + 0x100;
		sprites[i].dy = (rand() & 0xFF) + 0x100;

		if(rand() & 1)
			sprites[i].dx = -sprites[i].dx;
		if(rand() & 1)
			sprites[i].dy = -sprites[i].dy;

		sprites[i].oam = getOAMCopySub(i);
		sprites[i].gfxID = 0;

		//set up our sprites OAM entry attributes
		sprites[i].oam->attribute[0] = ATTR0_COLOR_256 | ATTR0_SQUARE;
		sprites[i].oam->attribute[1] = ATTR1_SIZE_32;
		sprites[i].oam->attribute[2] = sprites[i].gfxID;
	}

    // make this configurable
	BG_PALETTE_SUB[0] = RGB15(0,1,5);
	//BG_PALETTE_SUB[1] = RGB15(0,16,0);
	//BG_PALETTE_SUB[2] = RGB15(0,0,31);

    while (pmMainLoop())
    {
        has_grav_point = touchRead(&touchXY);

        if(has_grav_point)
            sprites[0].x = inttof32(touchXY.px) - ball_center_offset;
        else
            sprites[0].x = inttof32(HALF_WIDTH) - ball_center_offset;

        scanKeys();
        int pressed = keysDownRepeat();
        if (pressed & KEY_UP)
        {
            int incr = (g_game_vars.num_entities >= 100) ? 100 : (g_game_vars.num_entities >= 10) ? 10 : 1;
            g_game_vars.num_entities += incr;
            if (g_game_vars.num_entities > MAX_ENTITIES)
                g_game_vars.num_entities = MAX_ENTITIES;
        }
        if (pressed & KEY_DOWN)
        {
            int incr = (g_game_vars.num_entities > 100) ? 100 : (g_game_vars.num_entities > 10) ? 10 : 1;
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
            if (g_game_vars.gravity_strength < 1000)
                g_game_vars.gravity_strength = 1000;
        }
        if (pressed & KEY_X)
        {
            g_game_vars.drag += floattof32(0.001);
            if (g_game_vars.drag > MAX_DRAG)
                g_game_vars.drag = MAX_DRAG;
        }
        if (pressed & KEY_Y)
        {
            g_game_vars.drag -= floattof32(0.001);
            if (g_game_vars.drag < 0)
                g_game_vars.drag = 0;
        }
        if (pressed & KEY_A)
        {
            g_game_vars.init_vel += inttof32(1);
            if (g_game_vars.init_vel > MAX_INIT_VEL)
                g_game_vars.init_vel = MAX_INIT_VEL;
        }
        if (pressed & KEY_B)
        {
            g_game_vars.init_vel -= floattof32(0.1);
            if (g_game_vars.init_vel < 0)
                g_game_vars.init_vel = 0;
        }
        if (pressed & KEY_R)
        {
            g_game_vars.vert_strength += inttof32(1);
            if (g_game_vars.vert_strength > MAX_GRAVITY)
                g_game_vars.vert_strength = MAX_GRAVITY;
        }
        if (pressed & KEY_L)
        {
            g_game_vars.vert_strength -= inttof32(1);
            if (g_game_vars.vert_strength < 0)
                g_game_vars.vert_strength = 0;
        }
        if (pressed & KEY_START)
        {
            if(has_grav_point)
                remove_all_grav_points();
            init_entities(true);
        }

        Vec2d touch_fixed = {
            .x = inttof32(touchXY.px),
            .y = inttof32(touchXY.py),
        };

        if (pressed & KEY_SELECT)
        {
            if(has_grav_point)
                register_grav_point(touch_fixed, g_game_vars.gravity_strength);
            init_entities(false);
        }

        int reg = -1;
        if (has_grav_point)
            reg = register_grav_point(touch_fixed, g_game_vars.gravity_strength);

        update_gravity_wells();
        update_entities();

        display_entities();

        print_config();

        MoveSprite(&sprites[0]);

        glFlush(0);

        if (has_grav_point)
            remove_grav_point(reg);

        updateOAM();

    }

    return 0;
}

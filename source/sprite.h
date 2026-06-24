#pragma once

#include <nds.h>

// simple sprite struct
typedef struct
{
    int x, y;         // screen co-ordinates
    int dx, dy;       // velocity
    SpriteEntry* oam; // pointer to the sprite attributes in OAM
    int gfxID;        // graphics lovation
} Sprite;


void initOAM(void);
void MoveSprite(Sprite* sp);
void updateOAM(void);
SpriteEntry* getOAMCopySub(unsigned int at);

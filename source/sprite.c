#include "sprite.h"
#include "nds/arm9/math.h"
#include "sys/_types.h"

SpriteEntry OAMCopySub[128];

SpriteEntry* getOAMCopySub(unsigned int at)
{
    if (at < 128) return &OAMCopySub[at];
}

void initOAM(void)
{
    int i;

    for (i = 0; i < 128; i++)
    {
        OAMCopySub[i].attribute[0] = ATTR0_DISABLED;
    }

    oamClear(&oamSub, 0, 128);
}

void MoveSprite(Sprite* sp)
{
    int x = f32toint(sp->x);
    int y = f32toint(sp->y);

    sp->oam->x = x;
    sp->oam->y = y;
}

void updateOAM(void)
{
    memcpy(OAM_SUB, OAMCopySub, 128 * sizeof(SpriteEntry));
}

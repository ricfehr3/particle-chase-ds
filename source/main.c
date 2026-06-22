#include "nds/arm9/video.h"
#include <nds.h>
#include <gl2d.h>

#include <stdio.h>

#define HALF_WIDTH (SCREEN_WIDTH/2)
#define HALF_HEIGHT (SCREEN_HEIGHT/2)

void pixels( int frame)
{
	glBegin2D();

    auto white = RGB15(-1,-1,-1);

    glPutPixel(frame % SCREEN_WIDTH, HALF_HEIGHT, white);

	glEnd2D();
}

int main()
{
    consoleDemoInit();

    videoSetMode( MODE_5_3D );

    consoleDemoInit();

    glScreen2D();

    iprintf("\x1b[1;1HDoin Ya Mom Doin Doin Ya Mom");

    int frame = 0;

    while( pmMainLoop() )
    {
        frame++;

        pixels( frame );

        glFlush(0);
    }

    return 0;
}

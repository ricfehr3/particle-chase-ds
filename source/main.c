#include <nds.h>
#include <stdio.h>

int main()
{
    consoleDemoInit();

    printf("Ric is big coolio \n");

    while (pmMainLoop())
    {
        swiWaitForVBlank();
    }

    return 0;
}

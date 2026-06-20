#include <nds.h>
#include <stdio.h>

int main()
{
    consoleDemoInit();

    printf("Ric is big cool \n");

    while (pmMainLoop())
    {
        swiWaitForVBlank();
    }

    return 0;
}

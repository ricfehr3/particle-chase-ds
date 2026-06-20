#include <nds.h>
#include <stdio.h>

int main()
{
    consoleDemoInit();

    printf("Wow, welcome\n");

    while (pmMainLoop())
    {
        swiWaitForVBlank();
    }

    return 0;
}

#include "main_helpers_types.h"

/* Exit byte identity corroborated by target GameUpdateFrame/MAT3 callers. */
signed char gExitGame = 0;

void func_8C03A360(int check)
{
    static unsigned int exit_savedCounter = 0;
    unsigned int *last;
    if (check != 1) goto done;
    last = &exit_savedCounter;
    if (gFrameCounter <= *last + 420) goto done;
    *last = gFrameCounter;
    func_8C06B976();
    if (func_8C06BA20() == 1) gExitGame = 1;
done:
    if (gSoftReset && !gGameStarted) gExitGame = 1;
}

void func_8C03A3AE(void)
{
    for (;;) {
        func_8C02E350();
        do {
            func_8C03A360(0);
        } while (!gExitGame && !GameLoop());
        func_8C02E390();
        func_8C03A360(1);
        if (gExitGame == 1) func_8C039718();
    }
}

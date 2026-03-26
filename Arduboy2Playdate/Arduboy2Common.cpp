#include <pdcpp/pdnewlib.h>
#include "Arduboy2Common.h"
#include "Arduboy2.h"

void randomSeed(unsigned int dwSeed)
{
    srand(dwSeed);
}

long random(long howbig)
{
    if (howbig == 0) {
        return 0;
    }

    return rand() % howbig;
}

long random(long howsmall, long howbig)
{
    if (howsmall >= howbig) {
        return howsmall;
    }

    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

char *ltoa(long value, char *string, int radix)
{
    char tmp[33];
    char *tp = tmp;
    long i;
    unsigned long v;
    int sign;
    char *sp;

    if (string == NULL) {
        return 0 ;
    }

    if (radix > 36 || radix <= 1) {
        return 0 ;
    }

    sign = (radix == 10 && value < 0);
    if (sign) {
        v = -value;
    } else {
        v = (unsigned long)value;
    }

    while (v || tp == tmp) {
        i = v % radix;
        v = v / radix;
        if (i < 10) {
            *tp++ = i + '0';
        } else {
            *tp++ = i + 'a' - 10;
        }
    }

    sp = string;

    if (sign) {
        *sp++ = '-';
    }
    while (tp > tmp) {
        *sp++ = *--tp;
    }
    *sp = 0;

    return string;
}
char *itoa(int value, char *string, int radix)
{
    return ltoa(value, string, radix) ;
}


unsigned long millis()
{
    return pd->system->getCurrentTimeMilliseconds();
}

void delay(int ms)
{
    //force a draw before delay (fixes dark-and-under)
    pd->graphics->display();
    pd->system->delay((uint32_t)ms);
}

PlaydateAPI *pd;

int update(__attribute__ ((unused)) void* ud)
{
    uint32_t now = pd->system->getCurrentTimeMilliseconds();
    uint32_t frameStart = Arduboy2Base::getThisFrameStart();
    uint32_t elapsed = now - frameStart;
    uint32_t frameMs = Arduboy2Base::getEachFrameMillis();

    if (elapsed < frameMs) {
        // Sleep for the remaining time so we don't spin and burn CPU.
        // Without this, returning 0 causes the runtime to call update()
        // in a tight loop resulting in 8000+ fps reported by device view
        // and high system load even when the game runs at 25fps.
        uint32_t remaining = frameMs - elapsed;
        if (remaining > 1) {
            pd->system->delay(remaining - 1);
        }
        return 0;
    }

    loop();

    // If loop() didn't call nextFrame() (or nextFrame() wasn't called),
    // thisFrameStart is unchanged — advance it ourselves so we don't fire
    // again immediately on the next callback.
    if (Arduboy2Base::getThisFrameStart() == frameStart) {
        Arduboy2Base::advanceFrameStart(frameMs, elapsed);
    }

#ifdef ARDUBOY_PLAYDATE_SHOW_FPS
    pd->system->drawFPS(0,0);
#endif
    return 1;
}

extern "C" {
#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg) {
    if (event == kEventInit) {
        eventHandler_pdnewlib(pd, event, arg);
        pd = playdate;
        setup();
        pd->system->setUpdateCallback(update, NULL);
    }
    return 0;
}

}
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

#ifdef ARDUBOY_PLAYDATE_SHOW_FPS
// Tracks actual game loop invocations per second (our logical frame rate),
// independent of the Playdate simulator's own FPS counter which counts
// display refreshes and typically shows double the actual game rate.
static uint32_t gameFpsFrameCount = 0;
static uint32_t gameFpsWindowStart = 0;
static int gameFpsDisplay = 0;
#endif

int update(__attribute__ ((unused)) void* ud)
{
    uint32_t now = pd->system->getCurrentTimeMilliseconds();
    uint32_t frameStart = Arduboy2Base::getThisFrameStart();
    uint32_t elapsed = now - frameStart;
    uint32_t frameMs = Arduboy2Base::getEachFrameMillis();

    // For games using setFrameRate() <= 50fps, the Playdate runtime already
    // gates update() to the correct rate via setRefreshRate(), so elapsed
    // will always be >= frameMs here. The check below handles games running
    // at > 50fps (setRefreshRate(0)) where we do our own time-based gating.
    if (elapsed < frameMs) {
        uint32_t remaining = frameMs - elapsed;
        if (remaining > 1) {
            pd->system->delay(remaining - 1);
        }
        return 0;
    }

    loop();

    // If loop() didn't call nextFrame(), advance thisFrameStart ourselves
    // so games without nextFrame() run at the correct rate.
    if (Arduboy2Base::getThisFrameStart() == frameStart) {
        Arduboy2Base::advanceFrameStart(frameMs, elapsed);
    }


#ifdef ARDUBOY_PLAYDATE_SHOW_FPS
    // Count actual game loop fires and update display once per second.
    gameFpsFrameCount++;
    uint32_t windowElapsed = now - gameFpsWindowStart;
    if (windowElapsed >= 1000) {
        gameFpsDisplay = (int)((gameFpsFrameCount * 1000) / windowElapsed);
        gameFpsFrameCount = 0;
        gameFpsWindowStart = now;
    }
 
    char *buf;  
    pd->system->formatString(&buf, "PD:%2d G:%2d", (int)pd->display->getFPS(), gameFpsDisplay);
    pd->graphics->fillRect(0, 0, 90, 18, kColorWhite);
    pd->graphics->drawText(buf, strlen(buf), kASCIIEncoding, 1, 1);
    pd->system->realloc(buf, 0);
    //pd->system->drawFPS(0,0);
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
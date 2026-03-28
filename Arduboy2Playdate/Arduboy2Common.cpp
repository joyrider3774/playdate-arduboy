#include <pdcpp/pdnewlib.h>
#include "Arduboy2Common.h"
#include "Arduboy2.h"

extern bool arduboyFullscreenEnabled;
extern bool arduboyFpsEnabled;


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

// Tracks actual game loop invocations per second (our logical frame rate),
// independent of the Playdate simulator's own FPS counter which counts
// display refreshes and typically shows double the actual game rate.
static uint32_t gameFpsFrameCount = 0;
static uint32_t gameFpsWindowStart = 0;
static int gameFpsDisplay = 0;
static char str[3];
int update(__attribute__ ((unused)) void* ud)
{
    loop();

    if(arduboyFpsEnabled)
    {
        uint32_t now = pd->system->getCurrentTimeMilliseconds();
        uint32_t windowElapsed = now - gameFpsWindowStart;
        if (windowElapsed >= 1000) {
            gameFpsDisplay = Arduboy2Base::frameCount - gameFpsFrameCount;
            gameFpsFrameCount = Arduboy2Base::frameCount;
            str[0] = (gameFpsDisplay < 10) ? ' ' : ('0' + gameFpsDisplay / 10);
            str[1] = '0' + (gameFpsDisplay % 10);
            str[2] = '\0';
            gameFpsWindowStart = now;
        }

        pd->graphics->fillRect(0, 0, 20, 18, kColorWhite);
        pd->graphics->drawText(str, strlen(str), kASCIIEncoding, 1, 1);
        pd->system->drawFPS(0,20);

    }
    return 1;
}

extern "C" {
#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg) {
    if (event == kEventInit) 
    {
        eventHandler_pdnewlib(pd, event, arg);
        pd = playdate;
        SDFile *f;
        f = pd->file->open("settings.dat", kFileReadData);
        if(f)
        {
            pd->file->read(f, &arduboyFpsEnabled, sizeof(arduboyFpsEnabled));
            pd->file->read(f, &arduboyFullscreenEnabled, sizeof(arduboyFullscreenEnabled));
            pd->file->close(f);
        }
        setup();
        pd->system->setUpdateCallback(update, NULL);
    }
    else if (event == kEventTerminate)
    {
        SDFile *f;
        f = pd->file->open("settings.dat", kFileWrite);
        if(f)
        {
            pd->file->write(f, &arduboyFpsEnabled, sizeof(arduboyFpsEnabled));
            pd->file->write(f, &arduboyFullscreenEnabled, sizeof(arduboyFullscreenEnabled));
            pd->file->close(f);
        }    
    }
    return 0;
}

}
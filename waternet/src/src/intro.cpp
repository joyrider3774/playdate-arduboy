#include <stdint.h>

#include "helperfuncs.h"
#include "commonvars.h"
#include "printfuncs.h"

#include "../res/titlescreenmap.h"
#include "../res/blocktiles.h"

uint16_t frames;
float titlePosY;
constexpr uint8_t frameDelay = 16 * frameRate / 15;    

void initIntro()
{
    setBlockTilesAsBackground();
    titlePosY = arduboy.height();
    frames = 0;   
}

void intro() 
{
    if (gameState == gsInitIntro)
    {
        initIntro();
        gameState -= gsInitDiff;
    }
    
    frames++;
    arduboy.clear();
    if (frames < frameDelay)
    {
        //16-12
        printMessage(4 >> 1, 4, F("WILLEMS DAVY"));
    }
    else
    {
        if (frames < frameDelay *2)
        {
            //16-8
            printMessage(8 >> 1, 4, F("PRESENTS"));
        }
        else
        {
            arduboy.drawCompressed(0, (uint16_t)titlePosY, titlescreenMap);
            if(titlePosY > 0)
            {
                titlePosY -= (float)30/(float)frameRate;
            }
            else
            {
                gameState = gsInitTitle;
            }
        }
    }
    
    if (arduboy.justReleased(A_BUTTON) || 
        arduboy.justReleased(B_BUTTON)) 
    {            
        gameState = gsInitTitle;
    }
    if(arduboy.justReleased(UP_BUTTON))
    {
        debugMode = 1;
    }
}

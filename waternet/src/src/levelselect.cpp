#include <stdint.h>

#include "helperfuncs.h"
#include "commonvars.h"
#include "printfuncs.h"
#include "savestate.h"
#include "sound.h"
#include "level.h"


void drawLevelSelect() 
{
    arduboy.clear();
    //LEVEL:
    printMessage(maxBoardBgWidth  , 0 , F("LEVEL:"));
    
    //[LEVEL NR] 2 chars
    printNumber(maxBoardBgWidth + 4 , 1 , selectedLevel, 2);
    
    //B:BACK
    printMessage(maxBoardBgWidth  , 6 , F("b:"));
    printMessage(maxBoardBgWidth  , 7 , F("BACK"));
    
    //A:PLAY
    printMessage(maxBoardBgWidth  , 4 , F("a:"));
    printMessage(maxBoardBgWidth  , 5 , F("PLAY"));
    
    //Locked & Unlocked keywoard
    uint8_t tmpUnlocked = levelUnlocked(gameMode, difficulty, selectedLevel -1);
    if(!tmpUnlocked)
        printMessage(maxBoardBgWidth , 2 , F("LOCKED"));
    else
        printMessage(maxBoardBgWidth , 2 , F("OPEN"));
    
    //Draw arrows for vertical / horizontal movement
    if(gameMode != gmRotate)
    {
        for (uint8_t x = 0; x != boardWidth; x++)
        {
            set_bkg_tile_xy(boardX + x , boardY -1 , arrowDown);
            set_bkg_tile_xy(boardX + x , boardY + boardHeight , arrowUp);
        }

        for (uint8_t y = 0; y != boardHeight; y++)
        {
            set_bkg_tile_xy(boardX - 1 , boardY + y , arrowRight);
            set_bkg_tile_xy(boardX + boardWidth , boardY + y , arrowLeft);
        }
    }

    uint16_t i16 = 0; 
    for (uint8_t y = 0; y < boardHeight; y++)
    {
        for(uint8_t x = 0; x <boardWidth; x++)
        {
            set_bkg_tile_xy(boardX  + x , boardY  + y, level[i16 + x]);
        }
        i16 += boardWidth;
    }
}

void initLevelSelect()
{
    setBlockTilesAsBackground();
    SelectMusic(musTitle);
    needRedraw = 1;
}

void levelSelect()
{
    if(gameState == gsInitLevelSelect)
    {
        initLevelSelect();
        gameState -= gsInitDiff;
    }
        
    if(needRedraw)
    {
        drawLevelSelect();
        needRedraw = 0;
    }
    
    uint8_t tmpUnlocked = levelUnlocked(gameMode, difficulty, selectedLevel -1);


    if (arduboy.justReleased(B_BUTTON))
    {
        playMenuBackSound();
        gameState = gsInitTitle;
    }
    if (arduboy.justReleased(A_BUTTON))
    {
        if(tmpUnlocked)
        {
            gameState = gsInitGame;
            playMenuAcknowlege();
        }
        else
        {
            playErrorSound();
        }
    }
    if (arduboy.justReleased(LEFT_BUTTON))
    {
        if (difficulty == diffRandom)
        {
            playMenuSelectSound();
            randomSeedGame = arduboy.generateRandomSeed();
            initLevel(randomSeedGame);
            needRedraw = 1;
        }
        else
        {
            if (selectedLevel > 1)
            {
                playMenuSelectSound();
                selectedLevel--;
                initLevel(randomSeedGame);
                needRedraw = 1;
            }
        } 
    }
    if (arduboy.justReleased(RIGHT_BUTTON))
    {
        if (difficulty == diffRandom)
        {
            playMenuSelectSound();
            //need new seed based on time
            randomSeedGame = arduboy.generateRandomSeed();
            initLevel(randomSeedGame);
            needRedraw = 1;
        }
        else
        {
            if (selectedLevel < maxLevel)
            {
                playMenuSelectSound();
                selectedLevel++;
                initLevel(randomSeedGame);
                needRedraw = 1;
            }
        }
    }
}

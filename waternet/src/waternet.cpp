#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "src\commonvars.h"
#include "src\intro.h"
#include "src\levelselect.h"
#include "src\helpscreens.h"
#include "src\sound.h"
#include "src\cursor.h"
#include "src\printfuncs.h"
#include "src\game.h"
#include "src\titlescreen.h"
#include "src\levelscleared.h"
#include "src\savestate.h"
#include "src\level.h"
#include "src\helperfuncs.h"

//intialisation of game & global variables
void setup()
{
    option = 0;
    debugMode = 0;
    difficulty = diffNormal;
    selectedLevel = 1;
    mainMenu = mmStartGame;
    gameState = gsInitIntro;
    titleStep = tsMainMenu;
    gameMode = gmRotate;
    arduboy.begin();
    arduboy.setFrameRate(frameRate);
    //has to be called first because initsound and initmusic read savestate sound to set intial flags
    initSaveState();
    initSound();
    initMusic();
    setMusicOn(isMusicOnSaveState() && arduboy.audio.enabled());
    setSoundOn(isSoundOnSaveState() && arduboy.audio.enabled());
 }

void loop()
{
    if (debugMode)
    {
        if  (!arduboy.nextFrameDEV())
            return;
    }
    else
    {
        if(!arduboy.nextFrame())
            return;
    }

    soundTimer();
    
    arduboy.pollButtons();
    
    //gamestate handling
    switch (gameState)
    {
        case gsInitTitle:
        case gsTitle:
            titleScreen();
            break;
        case gsInitLevelSelect:
        case gsLevelSelect:
            levelSelect();
            break;
        case gsInitGame:
        case gsGame:
            game();
            break;
        case gsInitLevelsCleared:
        case gsLevelsCleared:
            levelsCleared();
            break;
        case gsInitHelpSlide:
        case gsHelpSlide:
            helpSlide();
            break;
        case gsInitHelpSlide2:
        case gsHelpSlide2:
            helpSlide2();
            break;
        case gsInitHelpSlide3:
        case gsHelpSlide3:
            helpSlide3();
            break;
        case gsHelpRotateSlide:
        case gsInitHelpRotateSlide:
            helpRotateSlide();
            break;
        case gsInitHelpRotateSlide2:
        case gsHelpRotateSlide2:
            helpRotateSlide2();
            break;
        case gsInitHelpRotateSlide3:
        case gsHelpRotateSlide3:
            helpRotateSlide3();
            break;
        case gsInitHelpRotateSlide4:
        case gsHelpRotateSlide4:
            helpRotateSlide4();
            break;
        case gsInitHelpRotate:
        case gsHelpRotate:
            helpRotate();
            break;
        case gsInitHelpRotate2:
        case gsHelpRotate2:
            helpRotate2();
            break;
        case gsInitHelpRotate3:
        case gsHelpRotate3:
            helpRotate3();
            break;
        case gsInitIntro:
        case gsIntro:
            intro();
            break;
    } 
    printDebugCpuLoad();
    arduboy.display();        
}
#include <stdint.h>

#include "commonvars.h"
#include "helperfuncs.h"
#include "printfuncs.h"
#include "cursor.h"
#include "sound.h"

//LEGEND STATE
void inithelpLegend() 
{
    setBlockTilesAsBackground();
    SelectMusic(musTitle);
    needRedraw = 1;
}

//LEGEND STATE
void helpLegend(uint8_t nextState) 
{
    if ((gameState == gsInitHelpSlide) ||
        (gameState == gsInitHelpRotate) ||
        (gameState == gsInitHelpRotateSlide))
    {
        inithelpLegend();
        gameState -= gsInitDiff;
    }
   
    if (needRedraw)
    {
        arduboy.clear();
        switch(gameState)
        {
            case gsHelpSlide:
                printMessage(2, 0, F("HELP: SLIDE"));
                break;
            case gsHelpRotate:
                printMessage(2, 0, F("HELP: ROTATE"));
                break;
            case gsHelpRotateSlide:
                printMessage(2, 0, F("HELP: ROSLID"));
                break;
        }
        
        set_bkg_tile_xy(0, 1, 33);
        printMessage(1, 1, F(":WATER SOURCE"));
        set_bkg_tile_xy(0, 2, 11);
        set_bkg_tile_xy(1, 2, 6);
        set_bkg_tile_xy(2, 2, 12);
        printMessage(3, 2, F(":NOT FILLED"));
        set_bkg_tile_xy(0, 3, 27);
        set_bkg_tile_xy(1, 3, 22);
        set_bkg_tile_xy(2, 3, 28);
        printMessage(3, 3, F(":FILLED"));

        if((gameState == gsHelpRotateSlide) ||
        (gameState == gsHelpSlide))
        {
            set_bkg_tile_xy(0, 4, 121);
            printMessage(1, 4, F(":SLID ROW RIGHT"));
            set_bkg_tile_xy(0, 5, 123);
            printMessage(1, 5, F(":SLID ROW LEFT"));
            set_bkg_tile_xy(0, 6, 122);
            printMessage(1, 6, F(":SLID COL DOWN"));
            set_bkg_tile_xy(0, 7, 120);
            printMessage(1, 7, F(":SLID COL UP"));
        }
        needRedraw = 0;
    }
    if (arduboy.justReleased(A_BUTTON))
    {
        playMenuAcknowlege();
        gameState = nextState;
    }
}

//FINISH LEVEL STATE
void initHelpFinishLevel() 
{
    setBlockTilesAsBackground();
    SelectMusic(musTitle);
    needRedraw = 1;
}

//FINISH LEVEL STATE
void helpFinishLevel(uint8_t nextState) 
{
    if ((gameState == gsInitHelpSlide2) ||
        (gameState == gsInitHelpRotate2) ||
        (gameState == gsInitHelpRotateSlide2))
    {
        initHelpFinishLevel();
        gameState -= gsInitDiff;
    }
   
    if(needRedraw)
    {
        arduboy.clear();
        switch(gameState)
        {
            case gsHelpSlide2:
                printMessage(2, 0, F("HELP: SLIDE"));
                break;
            case gsHelpRotate2:
                printMessage(2, 0, F("HELP: ROTATE"));
                break;
            case gsHelpRotateSlide2:
                printMessage(2, 0, F("HELP: ROSLID"));
                break;
        }
        printMessage(0, 2, F("LEVEL FINISH:"));
        
        if((gameState == gsHelpSlide2) ||
        (gameState == gsHelpRotateSlide2))
        {
            //arrows top
            set_bkg_tile_xy(2, 3, 122);
            set_bkg_tile_xy(3, 3, 122);
            set_bkg_tile_xy(4, 3, 122);

            //arrows left / right row 1
            set_bkg_tile_xy(1, 4, 121);
            set_bkg_tile_xy(5, 4, 123);

            //arrows left / right row 2
            set_bkg_tile_xy(1, 5, 121);
            set_bkg_tile_xy(5, 5, 123);

            //arrows left / right row 3
            set_bkg_tile_xy(1, 6, 121);
            set_bkg_tile_xy(5, 6, 123);

            //arrows bottom
            set_bkg_tile_xy(2, 7, 120);
            set_bkg_tile_xy(3, 7, 120);
            set_bkg_tile_xy(4, 7, 120);
        }

        set_bkg_tile_xy(2, 4, 25);
        set_bkg_tile_xy(3, 4, 23);
        set_bkg_tile_xy(4, 4, 27);
        printMessage(7, 4, F("ALL WATER"));      

        set_bkg_tile_xy(2, 5, 28);
        set_bkg_tile_xy(3, 5, 33);
        set_bkg_tile_xy(4, 5, 22);
        printMessage(7, 5, F("PIPES ARE"));      

        set_bkg_tile_xy(2, 6, 29);
        set_bkg_tile_xy(3, 6, 20);
        set_bkg_tile_xy(4, 6, 23);
        printMessage(7, 6, F("FILLED"));
        needRedraw = 0;
    }

    if (arduboy.justReleased(A_BUTTON))
    {
        playMenuAcknowlege();
        gameState = nextState;           
    }
}

void initHelpDoSlideRotate()
{
    setBlockTilesAsBackground();
    SelectMusic(musTitle);

    //DRAW CURSOR SPRITES
    initCursors();

    if((gameState == gsInitHelpRotateSlide4) ||
      (gameState == gsInitHelpSlide3))
    {
        setCursorPos(0, 0, 5);
        setCursorPos(1, 11, 5);
    }
    else
    {
        setCursorPos(0, 1, 4);
        setCursorPos(1, 12, 4);
    }

    showCursors();
    needRedraw = 1;
}

void helpDoSlideRotate(uint8_t nextState)
{
    if ((gameState == gsInitHelpSlide3) ||
        (gameState == gsInitHelpRotate3) ||
        (gameState == gsInitHelpRotateSlide3) ||
        (gameState == gsInitHelpRotateSlide4))
    {
        initHelpDoSlideRotate();
        gameState -= gsInitDiff;
    }
   
    if(needRedraw)
    {
        arduboy.clear();

        switch(gameState)
        {
            case gsHelpSlide3:
                printMessage(2, 0, F("HELP: SLIDE"));
                break;
            case gsHelpRotate3:
                printMessage(2, 0, F("HELP: ROTATE"));
                break;
            case gsHelpRotateSlide3:
            case gsHelpRotateSlide4:
                printMessage(2, 0, F("HELP: ROSLID"));
                break;
        }

        if((gameState == gsHelpRotateSlide3) || 
            (gameState == gsHelpRotate3))
            printMessage(5, 2, F("ROTATE"));
        else
            printMessage(6, 2, F("SLIDE"));

        // 'A' + '=>'
        set_bkg_tile_xy(6, 5, 119);
        set_bkg_tile_xy(9, 5, 118);

        if((gameState == gsHelpSlide3) || 
        (gameState == gsHelpRotateSlide3) ||
        (gameState == gsHelpRotateSlide4))
        {
            //Top Arrows
            set_bkg_tile_xy(1, 3, 122);
            set_bkg_tile_xy(2, 3, 122);
            set_bkg_tile_xy(3, 3, 122);

            //arrows 1st row
            set_bkg_tile_xy(0, 4, 121);
            set_bkg_tile_xy(4, 4, 123);

            //arrows 2nd row
            set_bkg_tile_xy(0, 5, 121);
            set_bkg_tile_xy(4, 5, 123);
            
            //arrows 3rd row
            set_bkg_tile_xy(0, 6, 121);
            set_bkg_tile_xy(4, 6, 123);
        
            //arrows bottom
            set_bkg_tile_xy(1, 7, 120);
            set_bkg_tile_xy(2, 7, 120);
            set_bkg_tile_xy(3, 7, 120);

            //2nd grid

            //Top Arrows
            set_bkg_tile_xy(12, 3, 122);
            set_bkg_tile_xy(13, 3, 122);
            set_bkg_tile_xy(14, 3, 122);
            
            //arrows 1st row
            set_bkg_tile_xy(11, 4, 121);
            set_bkg_tile_xy(15, 4, 123);
            
            //arrows 2nd row
            set_bkg_tile_xy(11, 5, 121);
            set_bkg_tile_xy(15, 5, 123);

            //arrows 3rd row
            set_bkg_tile_xy(11, 6, 121);
            set_bkg_tile_xy(15, 6, 123);

            //bottoms arrows
            set_bkg_tile_xy(12, 7, 120);
            set_bkg_tile_xy(13, 7, 120);
            set_bkg_tile_xy(14, 7, 120);       
        }
        
        //1st grid
        if ((gameState == gsHelpRotate3) ||
            (gameState == gsHelpRotateSlide3)) 
        {
            set_bkg_tile_xy(1, 4, 12);
            set_bkg_tile_xy(2, 4, 7);
            set_bkg_tile_xy(3, 4, 27);

            set_bkg_tile_xy(1, 5, 28);
            set_bkg_tile_xy(2, 5, 33);
            set_bkg_tile_xy(3, 5, 22);

            set_bkg_tile_xy(1, 6, 29);
            set_bkg_tile_xy(2, 6, 20);
            set_bkg_tile_xy(3, 6, 23);
        }
        else
        {
            set_bkg_tile_xy(1, 4, 9);
            set_bkg_tile_xy(2, 4, 7);
            set_bkg_tile_xy(3, 4, 11);

            set_bkg_tile_xy(1, 5, 17);
            set_bkg_tile_xy(2, 5, 38);
            set_bkg_tile_xy(3, 5, 12);

            set_bkg_tile_xy(1, 6, 13);
            set_bkg_tile_xy(2, 6, 4);
            set_bkg_tile_xy(3, 6, 7);
        }
    

        //2nd grid
    
        set_bkg_tile_xy(12, 4, 25);
        set_bkg_tile_xy(13, 4, 23);
        set_bkg_tile_xy(14, 4, 27);
        
        set_bkg_tile_xy(12, 5, 28);
        set_bkg_tile_xy(13, 5, 33);
        set_bkg_tile_xy(14, 5, 22);

        set_bkg_tile_xy(12, 6, 29);
        set_bkg_tile_xy(13, 6, 20);
        set_bkg_tile_xy(14, 6, 23);

        drawCursors(); 
        needRedraw = 0;   
    }

    needRedraw = updateCursorFrame();
   
    if (arduboy.justReleased(A_BUTTON))
    {
        playMenuAcknowlege();
        gameState = nextState;           
        hideCursors();
    }
}


//LEGEND STATE
void helpRotateSlide() 
{
    helpLegend(gsInitHelpRotateSlide2);
}

//FINISH LEVEL STATE
void helpRotateSlide2() 
{
    helpFinishLevel(gsInitHelpRotateSlide3);
}

//SLIDE STATE
void helpRotateSlide3()
{
   helpDoSlideRotate(gsInitHelpRotateSlide4);
}

//ROTATE STATE
void helpRotateSlide4()
{
    helpDoSlideRotate(gsInitTitle);
}

void helpRotate()
{
    helpLegend(gsInitHelpRotate2);
}

//FINISH LEVEL STATE
void helpRotate2()
{
    helpFinishLevel(gsInitHelpRotate3);
}

//ROTATE STATE
void helpRotate3()
{
    helpDoSlideRotate(gsInitTitle);
}

//LEGEND STATE
void helpSlide()
{
    helpLegend(gsInitHelpSlide2);
}

//FINISH LEVEL STATE
void helpSlide2()
{
    helpFinishLevel(gsInitHelpSlide3);
}

//SLIDE STATE
void helpSlide3()
{
    helpDoSlideRotate(gsInitTitle);
}
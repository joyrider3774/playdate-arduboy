#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Constants.h"
#include "src/images/Images.h"
#include "Astarok_IntroText.h"
#include <ArduboyPlaytune.h>
#include "AstarokGame.h"
#include "src/utils/Structs.h"
#include "src/font/Font4x6.h"

extern Arduboy2Ext arduboy;

#ifdef SOUNDS
extern ArduboyPlaytune tunes;
extern AstarokGame game;
#else
extern AstarokGame game;
#endif
extern GameState gameState;
extern HighScoreVars highScoreVars;
extern SeedVars seedVars;
extern IntroTextVars introTextVars;

const uint8_t textLengths[] = { 88, 81, 84 };

const uint8_t PROGMEM IntroText_00[] = {
    3,120/*E*/,'a','c','h',' ','y','e','a','r',' ','t','h','e',' ','p','e','o','p','l','e',' ','o','f',
    0,'t','h','e',' ','v','i','l','l','a','g','e',' ','c','h','o','o','s','e',' ','a',' ','n','e','w',
    12,'c','h','a','m','p','i','o','n',' ','t','o',' ','p','r','o','t','e','c','t',
    15,'t','h','e','m',' ','f','r','o','m',' ',106/*A*/,'s','t','a','r','o','k','.'
};

const uint8_t PROGMEM IntroText_01[] = {
    7, 113/*C*/,'o','m','p','e','t','e',' ','i','n',' ','t','h','e',' ','t','r','i','a','l','s',
    6,'t','o',' ','s','e','e',' ','h','o','w',' ','f','a','r',' ','y','o','u',' ','c','a','n',
    11,'g','e','t',' ','a','n','d',' ','y','o','u',' ','c','o','u','l','d',' ','b','e',
    25,'t','h','a','t',' ','c','h','a','m','p','i','o','n','!'
};

const uint8_t PROGMEM IntroText_02[] = { 
    10,122/*Y*/,'o','u','r',' ','c','h','o','i','c','e',' ','o','f',' ','r','u','n','e','s',
    7,'w','i','l','l',' ','s','e','l','e','c','t',' ','d','i','f','f','e','r','e','n','t',
    12,'c','o','u','r','s','e','s','.','.','.',' ','s','o','m','e',' ','a','r','e',
    13,'e','a','s','i','e','r',' ','t','h','a','n',' ','o','t','h','e','r','s','!'
};

const uint8_t * const IntroTexts[] = { IntroText_00, IntroText_01, IntroText_02 }; 

void introText_Init() {

    introTextVars.reset();
    gameState = GameState::IntroText;

}

void introText() {

    uint8_t line = 0;
    uint8_t x = 0;
    
    Sprites::drawOverwrite(  1,  1, Images::EnterRunes_Frame, 0);
    Sprites::drawOverwrite(  1, 54, Images::EnterRunes_Frame, 2);
    Sprites::drawOverwrite(120,  1, Images::EnterRunes_Frame, 1);
    Sprites::drawOverwrite(120, 54, Images::EnterRunes_Frame, 3);

    arduboy.drawFastHLine(1, 1, 125);
    arduboy.drawFastHLine(1, 61, 125);
    arduboy.drawFastHLine(3, 63, 122);


    for (uint8_t i = 0; i < introTextVars.index; i++) {

        uint8_t c = pgm_read_byte(&IntroTexts[introTextVars.panel][i]);

        switch (c) {

            case 0 ... 31:
                line++;
                x = 0 + c;
                break;

            case ' ':
                x = x + 4;
                break;
            
            default:
                Sprites::drawOverwrite(6 + x, 3 + (line * 10), Font::Images, Font::getIndex(c));
                x = x + 5;
                break;

        }
        
    }


    if (introTextVars.index < textLengths[introTextVars.panel] && arduboy.getFrameCount(2)) {

        introTextVars.index++;

    }


    uint8_t justPressed = arduboy.justPressedButtons();

    if (justPressed & A_BUTTON || justPressed & B_BUTTON) {
        
        if (introTextVars.index == textLengths[introTextVars.panel] && introTextVars.skipTypewriter == false) {
            introTextVars.panel++;
            introTextVars.index = 0;
        }
        else {
            if (introTextVars.skipTypewriter) introTextVars.panel++;
            introTextVars.skipTypewriter = true;
            introTextVars.index = textLengths[introTextVars.panel];
        }

        if (introTextVars.panel == 3) {
            gameState = GameState::Seed_Init;
        }

    }

}


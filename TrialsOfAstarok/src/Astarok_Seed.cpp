#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Constants.h"
#include "src/images/Images.h"
#include "Astarok_Seed.h"
#include "Astarok_Title.h"
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

void seed_Init() {

    #ifdef SOUNDS
    tunes.stopScore();
    #endif

    seedVars.reset();
    gameState = GameState::Seed;

}

void seed() {

    drawBackground();
    
    Sprites::drawOverwrite(34, 8, Images::EnterRunes_Text, 0);
    Sprites::drawOverwrite(24, 21, Images::EnterRunes_Frame, 0);
    Sprites::drawOverwrite(24, 47, Images::EnterRunes_Frame, 2);
    Sprites::drawOverwrite(96, 21, Images::EnterRunes_Frame, 1);
    Sprites::drawOverwrite(96, 47, Images::EnterRunes_Frame, 3);

    arduboy.drawFastHLine(24, 21, 72);
    arduboy.drawFastHLine(24, 54, 72);
    arduboy.drawFastHLine(26, 56, 75);

    for (uint8_t i = 0; i < 5; i++) {

        if (i == seedVars.index && seedVars.spinIndex > 0) {
            Sprites::drawOverwrite(30 + (i* 14), 31, Images::Rune_Frame, 16 - seedVars.spinIndex);
        }
        else {
            Sprites::drawOverwrite(30 + (i* 14), 31, Images::Rune_Frame, 0);
            Sprites::drawOverwrite(33 + (i* 14), 35, Images::Runes, seedVars.seed[i]);
        }

    }

    Sprites::drawSelfMasked(33 + (seedVars.index * 14), 26, Images::Arrows, 0);
    Sprites::drawSelfMasked(33 + (seedVars.index* 14), 47, Images::Arrows, 1);

    uint8_t justPressed = arduboy.justPressedButtons();

    if (seedVars.spinIndex == 0) {

        if (justPressed & UP_BUTTON) {
            seedVars.incValue();
            
            #ifdef SOUNDS
            tunes.playScore(Sounds::ButtonPress);
            #endif
        }

        if (justPressed & DOWN_BUTTON) { 
            seedVars.decValue();
            
            #ifdef SOUNDS
            tunes.playScore(Sounds::ButtonPress);
            #endif
        }

        if (justPressed & LEFT_BUTTON) { 
            if (seedVars.decIndex()) {
                #ifdef SOUNDS
                tunes.playScore(Sounds::ButtonPress);
                #endif
            }
        }

        if (justPressed & RIGHT_BUTTON) { 
            if (seedVars.incIndex()) {
                #ifdef SOUNDS
                tunes.playScore(Sounds::ButtonPress);
                #endif
            }
        }

        if ((justPressed & A_BUTTON) || (justPressed & B_BUTTON)) {

            game.seed = seedVars.getSeedValue();
            for (uint8_t a = 0; a < Constants::GameSeeds; a++ ) {
                game.seeds[a] = hash(game.seed);
            }

            #ifdef SOUNDS
            tunes.playScore(Sounds::ButtonPress);
            #endif
            gameState = GameState::Game_Init;

        }

    }

    seedVars.decSpinIndex();

}


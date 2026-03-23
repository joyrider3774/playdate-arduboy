#include "src/utils/Arduboy2Ext.h"
#include "Astarok_Title.h"

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
extern TitleScreenVars titleScreenVars;

void drawBackground() {


    Sprites::drawOverwrite(4, 12, Images::Underground_Brick, 0);
    Sprites::drawOverwrite(105, 12, Images::Underground_Brick, 0);

    Sprites::drawOverwrite(-10, 27, Images::Underground_Brick, 0);
    Sprites::drawOverwrite(119, 27, Images::Underground_Brick, 0);

    Sprites::drawOverwrite(4, 41, Images::Underground_Brick, 0);
    Sprites::drawOverwrite(105, 41, Images::Underground_Brick, 0);

    Sprites::drawOverwrite(11, 27, Images::Torch, arduboy.getFrameCount(16) / 4);
    Sprites::drawOverwrite(111, 27, Images::Torch, arduboy.getFrameCount(16) / 4);

}

void titleScreen() {

    uint8_t justPressed = arduboy.justPressedButtons();

    Sprites::drawOverwrite(24, 4, Images::Title, 0);
    Sprites::drawSelfMasked(35, 0, Images::Underground_Chain, 0);
    Sprites::drawSelfMasked(90, 0, Images::Underground_Chain, 0);    
    drawBackground();

    Sprites::drawOverwrite(titleScreenVars.index == TitleScreenMode::Play ? 35 : 65, 59, Images::Title_Highlight, 0);

    Sprites::drawOverwrite(119, 56, Images::SoundToggle, arduboy.audio.enabled());

    if ((justPressed & A_BUTTON) || (justPressed & B_BUTTON)) {

        switch (titleScreenVars.index) {

            case TitleScreenMode::Play:
                if (introTextVars.seenIntroText)  {
                    gameState = GameState::Seed_Init;
                }
                else {
                    gameState = GameState::IntroText_Init;
                    introTextVars.seenIntroText = true;
                }
                break;

            default:
                gameState = GameState::HighScore_NoFlash;
                break;

        }

    }

    if ((justPressed & UP_BUTTON) || (justPressed & DOWN_BUTTON)) {
        
        if (arduboy.audio.enabled()) {

            arduboy.audio.off(); 
            arduboy.audio.saveOnOff();

        }
        else {

            arduboy.audio.on(); 
            arduboy.audio.saveOnOff();

        }

    }

    if (justPressed & LEFT_BUTTON) {

        titleScreenVars.index = TitleScreenMode::Play;

    }

    if (justPressed & RIGHT_BUTTON) {

        titleScreenVars.index = TitleScreenMode::HighScore;

    }

}


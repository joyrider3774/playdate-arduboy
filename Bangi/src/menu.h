/*
   Copyright (C) 2016 Ignacio Vina (@igvina)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once
#include <Arduboy2.h>
#include "credits.h"

const char PROGMEM STRING_PLAY[] = "PLAY";
const char PROGMEM STRING_CONTINUE[] = "CONTINUE:LVL ";
const char PROGMEM STRING_NEW_GAME[] = "NEW GAME";
const char PROGMEM STRING_SETTINGS[] = "SETTINGS";
const char PROGMEM STRING_SOUND_ON[] = "SOUND:ON";
const char PROGMEM STRING_SOUND_OFF[] = "SOUND:OFF";
const char PROGMEM STRING_AUTOSAVE_ON[] = "AUTOSAVE:ON";
const char PROGMEM STRING_AUTOSAVE_OFF[] = "AUTOSAVE:OFF";
const char PROGMEM STRING_30_FPS[] = "FPS:30";
const char PROGMEM STRING_60_FPS[] = "FPS:60";
const char PROGMEM STRING_SHOW_FPS_ON[] = "SHOW FPS:ON";
const char PROGMEM STRING_SHOW_FPS_OFF[] = "SHOW FPS:OFF";
const char PROGMEM STRING_5_LIVES[] = "LIVES:5";
const char PROGMEM STRING_UNLIMITED_LIVES[] = "LIVES:UNLIMITED";
const char PROGMEM STRING_DIFFICULTY_VERY_EASY[] = "DIFFICULTY:V.EASY";
const char PROGMEM STRING_DIFFICULTY_EASY[] = "DIFFICULTY:EASY";
const char PROGMEM STRING_DIFFICULTY_NORMAL[] = "DIFFICULTY:NORMAL";
const char PROGMEM STRING_DIFFICULTY_HARD[] = "DIFFICULTY:HARD";
const char PROGMEM STRING_DIFFICULTY_VERY_HARD[] = "DIFFICULTY:V.HARD";
const char PROGMEM STRING_DIFFICULTY_ULTRA[] = "DIFFICULTY:ULTRA";
const char PROGMEM STRING_HELP[] = "HELP";
const char PROGMEM STRING_CREDITS[] = "CREDITS";


#define MENU_PLAY         0
#define MENU_NEW_GAME     1
#define MENU_SETTINGS     2
#define MENU_HELP         3
#define MENU_CREDITS      4

#define MENU_ITEMS  5


#define SETTINGS_SETTINGS   0
#define SETTINGS_DIFFICULTY 1
#define SETTINGS_SOUND      2
#define SETTINGS_IMMORTAL   3
#define SETTINGS_AUTOSAVE   4
#define SETTINGS_FPS   5
#define SETTINGS_SHOW_FPS   6

#define SETTINGS_ITEMS  7

unsigned char menuDisplayedOption;
unsigned char settingsDisplayedOption;

int frame;

void playMenu() {


  // Paint centered logo
  helper.drawBitmap((WIDTH - LOGO_WIDTH) / 2, 10, LOGO_BITMAP, LOGO_WIDTH, LOGO_HEIGHT, 0);

  // Paint ball
  helper.drawMaskedBitmap(BALL_STOPS_X, 0,
                          (frame == 0 || frame == 1) ? SMALL_BALL_BITMAP : SMALL_BALL_ROT_BITMAP,
                          SMALL_BALL_MASK_BITMAP, SMALL_BALL_SIZE, SMALL_BALL_SIZE, (frame == 0 || frame == 2) ? 0 : MIRROR);

  // Rotate ball
  if (arduboy.everyXFrames(3)) {
    frame = (frame + 1) % 4;
  }

  const char* displayedOptionString =
    menuDisplayedOption == MENU_PLAY ? (game == GAME_STOP ? STRING_PLAY : STRING_CONTINUE) :
    menuDisplayedOption == MENU_NEW_GAME ? STRING_NEW_GAME :
    menuDisplayedOption == MENU_SETTINGS ?
    settingsDisplayedOption == SETTINGS_SOUND ? (sound == true ? STRING_SOUND_ON : STRING_SOUND_OFF) :
    settingsDisplayedOption == SETTINGS_FPS ? (frameDrop == FPS_60 ? STRING_60_FPS : STRING_30_FPS) :
    settingsDisplayedOption == SETTINGS_IMMORTAL ? (immortal ? STRING_UNLIMITED_LIVES : STRING_5_LIVES) :
    settingsDisplayedOption == SETTINGS_AUTOSAVE ? (autosave ? STRING_AUTOSAVE_ON : STRING_AUTOSAVE_OFF) :
    settingsDisplayedOption == SETTINGS_SHOW_FPS ? (showFPS ? STRING_SHOW_FPS_ON : STRING_SHOW_FPS_OFF) :
    settingsDisplayedOption == SETTINGS_DIFFICULTY ?
    (difficulty == DIFFICULTY_VERY_EASY ? STRING_DIFFICULTY_VERY_EASY :
     difficulty == DIFFICULTY_EASY ? STRING_DIFFICULTY_EASY :
     difficulty == DIFFICULTY_NORMAL ? STRING_DIFFICULTY_NORMAL :
     difficulty == DIFFICULTY_HARD ? STRING_DIFFICULTY_HARD :
     difficulty == DIFFICULTY_VERY_HARD ? STRING_DIFFICULTY_VERY_HARD : STRING_DIFFICULTY_ULTRA) :
    STRING_SETTINGS :
    menuDisplayedOption == MENU_HELP ? STRING_HELP : STRING_CREDITS;

  unsigned int stringLength = strlen_P(displayedOptionString);

  boolean isContinueString = menuDisplayedOption == MENU_PLAY && game == GAME_RUNNING;

  uint8_t extraChars = isContinueString ? getNumberOfChars(level + 1) : 0;

  const unsigned char OFFSET_Y = 44;

  uint8_t offsetX = (WIDTH - (stringLength + extraChars)  * 6 - 1) / 2;

/*
  arduboy.setCursor(offsetX, OFFSET_Y);

  int i;
  char myChar;
  for (i = 0; i < stringLength; i++) {
    myChar =  pgm_read_byte(displayedOptionString + i);
    arduboy.print(myChar);
  }*/
  helper.print(displayedOptionString, offsetX, OFFSET_Y);

  if (menuDisplayedOption == MENU_PLAY && game == GAME_RUNNING) {
    arduboy.print((level + 1));
  }

  // Dynamic arrows position
  unsigned char lateral_arrows_offset = 10;

  if (stringLength + extraChars > 15) lateral_arrows_offset = 4;

  helper.drawBitmap(offsetX - RIGHT_ARROW_WIDTH - lateral_arrows_offset, OFFSET_Y - 1, RIGHT_ARROW_BITMAP, RIGHT_ARROW_WIDTH, RIGHT_ARROW_HEIGHT, MIRROR);
  helper.drawBitmap(offsetX + (stringLength + extraChars) * 6 - 1 + lateral_arrows_offset, OFFSET_Y - 1, RIGHT_ARROW_BITMAP, RIGHT_ARROW_WIDTH, RIGHT_ARROW_HEIGHT, 0);

  if (menuDisplayedOption == MENU_SETTINGS) {
    helper.drawBitmap((WIDTH - DOWN_ARROW_WIDTH) / 2, HEIGHT - DOWN_ARROW_HEIGHT , DOWN_ARROW_BITMAP, DOWN_ARROW_WIDTH, DOWN_ARROW_HEIGHT, 0);
  }
/*
#ifdef BETA
  // Paint beta label
  arduboy.setCursor(75, 56);
  arduboy.print(F("BETA "));
  arduboy.print(STRING_GAME_VERSION);
#endif
*/
  helper.print(STRING_GAME_VERSION, 104, 56);

  //Manage pressed keys
  if (helper.justPressed(RIGHT_BUTTON)) {
    settingsDisplayedOption = SETTINGS_SETTINGS;
    // New game only visible if game running
    if (game == GAME_STOP && menuDisplayedOption == (MENU_NEW_GAME - 1)) menuDisplayedOption++;
    //Reset settings
    settingsDisplayedOption = SETTINGS_SETTINGS;
    menuDisplayedOption = (menuDisplayedOption + 1) % MENU_ITEMS;

  } else if (helper.justPressed(LEFT_BUTTON)) {

    // New game only visible if game running
    if (game == GAME_STOP && menuDisplayedOption == (MENU_NEW_GAME + 1)) menuDisplayedOption--;
    //Reset settings
    settingsDisplayedOption = SETTINGS_SETTINGS;
    menuDisplayedOption = (menuDisplayedOption + MENU_ITEMS - 1) % MENU_ITEMS;

  } else if (helper.justPressed(DOWN_BUTTON)) {

    if (menuDisplayedOption == MENU_SETTINGS) settingsDisplayedOption = (settingsDisplayedOption + 1) % SETTINGS_ITEMS;

    // Also include up opcion (even not draw)
  } else if (helper.justPressed(UP_BUTTON)) {

    if (menuDisplayedOption == MENU_SETTINGS) settingsDisplayedOption = (settingsDisplayedOption + SETTINGS_ITEMS - 1 ) % SETTINGS_ITEMS;

  } else if (helper.justPressed(B_BUTTON)) {


    switch (menuDisplayedOption) {

      case MENU_PLAY:
        if (game == GAME_STOP) {
          resetGame();
        }

        if (levelLoadedFromEEPROM) {
          levelLoadedFromEEPROM = false;
          initBalls();
        }

        configGame();
        mode = MODE_GAME;
        break;

      case MENU_NEW_GAME:
        resetGame();
        configGame();
        mode = MODE_GAME;
        break;

      case MENU_SETTINGS:

        switch (settingsDisplayedOption) {

          case SETTINGS_SETTINGS:
            break;

          case SETTINGS_DIFFICULTY:
            difficulty = (difficulty + 1) % 6;
            EEPROM.write(EEPROM_DIFFICULTY, difficulty);
            break;

          case SETTINGS_SOUND:
            sound = !sound;
            EEPROM.write(EEPROM_SOUND_ON, sound);
            break;

          case SETTINGS_IMMORTAL:
            immortal = !immortal;
            EEPROM.write(EEPROM_IMMORTAL, immortal);
            break;

          case SETTINGS_AUTOSAVE:
            autosave = !autosave;
            EEPROM.write(EEPROM_AUTOSAVE, autosave);
            // Reset last save
            EEPROM.write(EEPROM_LAST_LEVEL, 0);
            break;

          case SETTINGS_FPS:
            //Change between 1 and 2
            frameDrop = 1 + frameDrop % 2;
            EEPROM.write(EEPROM_FPS, frameDrop);
            break;

          case SETTINGS_SHOW_FPS:
            showFPS = !showFPS;
            EEPROM.write(EEPROM_SHOW_FPS, showFPS);
            break;
        }

        break;

      case MENU_CREDITS:
        initCredits();
        mode = MODE_CREDITS;
        break;

      case MENU_HELP:
        mode = MODE_HELP;
        break;
    }
  }
}


void setMenuMode() {
  arduboy.setFrameRate(15);
  mode = MODE_MENU;
  menuDisplayedOption = MENU_PLAY;
}

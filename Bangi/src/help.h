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
#include "globals.h"
#include "gifts.h"

const char PROGMEM STRING_BASICS[] = "BASICS";

const char PROGMEM STRING_MOVE_PLAYER[] = "MOVE PLAYER";
const char PROGMEM STRING_SHOT_RAY[] = "SHOT RAY";
const char PROGMEM STRING_PAUSE[] = "PAUSE";


const char PROGMEM STRING_ITEMS[] = "ITEMS";

const char PROGMEM STRING_LIFE[] = "LIFE";
const char PROGMEM STRING_DEATH[] = "DEATH";
const char PROGMEM STRING_DOUBLE_RAY[] = "2 RAYS";
const char PROGMEM STRING_SHIELD[] = "SHIELD";
const char PROGMEM STRING_SLOW[] = "SLOW";
const char PROGMEM STRING_FAST[] = "FAST";

const char PROGMEM STRING_MIRROR[] = "MIRROR";
const char PROGMEM STRING_NEGATIVE[] = "NEGATIVE";
const char PROGMEM STRING_ZERO_G[] = "ZERO G";



extern void setMenuMode();

uint8_t page;

void playHelp() {

  const unsigned int GAP_Y = 16;


    const uint8_t INIT_X = 12;
    const uint8_t INIT_Y = 4;

    const uint8_t INIT_TEXT_X = 40;


    const uint8_t INIT_X_COL_1 = 2;
    const uint8_t INIT_X_COL_2 = 63;

    const uint8_t INIT_Y_COL = 2;

    const uint8_t GAP_TEXT = 5;    

    helper.print(page == 0 ? STRING_BASICS : STRING_ITEMS, page == 0 ? (WIDTH - 6 * 6) / 2 : (WIDTH - 5 * 6) / 2 , INIT_Y);

  if (page == 0) {
    helper.drawBitmap(INIT_X, INIT_Y + GAP_Y, RIGHT_ARROW_BITMAP, RIGHT_ARROW_WIDTH, RIGHT_ARROW_HEIGHT, MIRROR);
    helper.drawBitmap(INIT_X + RIGHT_ARROW_WIDTH + 4 , INIT_Y + GAP_Y, RIGHT_ARROW_BITMAP, RIGHT_ARROW_WIDTH, RIGHT_ARROW_HEIGHT, 0);
    helper.drawBitmap(INIT_X, INIT_Y + GAP_Y * 2, A_B_BUTTONS_BITMAP, A_B_BUTTONS_WIDTH, A_B_BUTTONS_HEIGHT, MIRROR);
    helper.drawBitmap(INIT_X, INIT_Y + GAP_Y * 3, A_B_BUTTONS_BITMAP, A_B_BUTTONS_WIDTH, A_B_BUTTONS_HEIGHT, 0);

    helper.print(STRING_MOVE_PLAYER, INIT_TEXT_X, INIT_Y + GAP_Y + 1);
    helper.print(STRING_SHOT_RAY,INIT_TEXT_X, INIT_Y + GAP_Y * 2 + 1);
    helper.print(STRING_PAUSE, INIT_TEXT_X, INIT_Y + GAP_Y * 3 + 1);

  } else if (page == 1) {


    //Images

    helper.drawBitmap(INIT_X_COL_1, INIT_Y_COL + GAP_Y, GIFT_LIFE_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    helper.drawBitmap(INIT_X_COL_2, INIT_Y_COL + GAP_Y, GIFT_DEATH_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);

    helper.drawBitmap(INIT_X_COL_1, INIT_Y_COL + GAP_Y * 2, GIFT_DOUBLE_RAY_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    helper.drawBitmap(INIT_X_COL_2, INIT_Y_COL + GAP_Y * 2, GIFT_SHIELD_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);

    helper.drawBitmap(INIT_X_COL_1, INIT_Y_COL + GAP_Y * 3, GIFT_SLOW_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    helper.drawBitmap(INIT_X_COL_2, INIT_Y_COL + GAP_Y * 3, GIFT_SLOW_BITMAP, GIFT_SIZE, GIFT_SIZE, MIRROR);
/*
    helper.drawBitmap(INIT_X_COL_1, INIT_Y_COL + GAP_Y * 3, GIFT_MIRROR_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    helper.drawBitmap(INIT_X_COL_2, INIT_Y_COL + GAP_Y * 3, GIFT_NO_GRAVITY_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
*/

    // Texts

    helper.print(STRING_LIFE, INIT_X_COL_1 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y + 3);
    helper.print(STRING_DEATH, INIT_X_COL_2 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y + 3);

    helper.print(STRING_DOUBLE_RAY, INIT_X_COL_1 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y * 2 + 3);
    helper.print(STRING_SHIELD, INIT_X_COL_2 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y * 2 + 3);

    helper.print(STRING_SLOW, INIT_X_COL_1 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y * 3 + 3);
    helper.print(STRING_FAST, INIT_X_COL_2 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y * 3 + 3);

    //helper.print(STRING_MIRROR, INIT_X_COL_1 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y * 3 + 3);

    //helper.print(STRING_ZERO_G, INIT_X_COL_2 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y * 3 + 3);
  }  else {

    //Images

    helper.drawBitmap(INIT_X_COL_1, INIT_Y_COL + GAP_Y, GIFT_MIRROR_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    helper.drawBitmap(INIT_X_COL_2, INIT_Y_COL + GAP_Y, GIFT_NEGATIVE_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);

    helper.drawBitmap(INIT_X_COL_1, INIT_Y_COL + GAP_Y * 2, GIFT_NO_GRAVITY_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);    
    
    // Texts

    helper.print(STRING_MIRROR, INIT_X_COL_1 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y + 3);
    helper.print(STRING_NEGATIVE, INIT_X_COL_2 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y + 3);

    helper.print(STRING_ZERO_G, INIT_X_COL_1 + GIFT_SIZE + GAP_TEXT , INIT_Y_COL + GAP_Y * 2 + 3);
  }


  // Go to main menu if any key is pressed
  if (helper.justPressed(0xFF)) {
    page++;
    if (page > 2) {
      setMenuMode();
      page = 0;
    }
  }
}



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

#include "Customlib.h"


extern uint8_t rand8();

// store current and previous buttons state for frame based button events
// you should be using nextFrame() in almost all cases, not calling this
// directly
#ifdef ARDUBOY_2
Customlib::Customlib(Arduboy2 &a) {
  arduboy = &a;
  sBuffer = arduboy->getBuffer();
}
#else
Customlib::Customlib(Arduboy &a) {
  arduboy = &a;
  sBuffer = arduboy->getBuffer();
}
#endif


void Customlib::poll() {
  previousButtonState = currentButtonState;
#ifdef ARDUBOY_2
  currentButtonState = arduboy->buttonsState();
#else
  currentButtonState = arduboy->getInput();
#endif
}
// returns true if a button has just been pressed
// if the button has been held down for multiple frames this will return
// false.  You should only use this to poll a single button.
boolean Customlib::justPressed(uint8_t button) {
  return (!(previousButtonState & button) && (currentButtonState & button));
}

boolean Customlib::pressed(uint8_t buttons) {
  return (currentButtonState & buttons) == buttons;
}

boolean Customlib::notPressed(uint8_t buttons) {
  return (currentButtonState & buttons) == 0;
}


void Customlib::drawBitmap(int8_t x, int8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t mirror) {
  drawMaskedBitmap(x,  y, bitmap, NULL, w,  h, mirror);

}

void Customlib::drawBitmap(int8_t x, int8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t maxW, uint8_t mirror) {
  // no need to dar at all of we're offscreen
  drawBitmap(x, y, bitmap, w, h, mirror);
  if (maxW > w) maxW = w;
  arduboy->fillRect(x + maxW, y, w - maxW , h, BLACK);
}

void Customlib::drawMaskedBitmap(int8_t x, int8_t y, const uint8_t *bitmap, const uint8_t *mask, uint8_t w, uint8_t h, uint8_t mirror) {
  if (x + w <= 0 || x > WIDTH - 1 || y + h <= 0 || y > HEIGHT - 1)
    return;

  if (bitmap == NULL)
    return;

  uint16_t xOffset, ofs;
  int8_t yOffset = abs(y) % 8;
  int8_t sRow = y / 8;
  uint8_t loop_h, start_h, rendered_width;

  if (y < 0 && yOffset > 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }

  if (x < 0) {
    xOffset = abs(x);
  } else {
    xOffset = 0;
  }

  if (x + w > WIDTH - 1) {
    rendered_width = ((WIDTH - x) - xOffset);
  } else {
    rendered_width = (w - xOffset);
  }

  if (sRow < -1) {
    start_h = abs(sRow) - 1;
  } else {
    start_h = 0;
  }

  loop_h = h / 8 + (h % 8 > 0 ? 1 : 0);

  if (sRow + loop_h > (HEIGHT / 8)) {
    loop_h = (HEIGHT / 8) - sRow;
  }

  loop_h -= start_h;

  sRow += start_h;
  ofs = (sRow * WIDTH) + x + xOffset;
  if (mirror) ofs += rendered_width - 1;
  uint8_t *bofs = (uint8_t *)bitmap + (start_h * w) + xOffset;
  uint8_t *mask_ofs = NULL;
  if (mask != NULL) {
    mask_ofs = (uint8_t *)mask + (start_h * w) + xOffset;
    if (mirror) mask_ofs += rendered_width - 1;
  }

  uint8_t data;
  uint8_t mul_amt = 1 << yOffset;
  uint16_t mask_data;
  uint16_t bitmap_data;

  for (uint8_t a = 0; a < loop_h; a++) {
    for (uint8_t iCol = 0; iCol < rendered_width; iCol++) {
      if (mask != NULL) {
        mask_data = ~(pgm_read_byte(mask_ofs) * mul_amt);
      } else {
        mask_data = ~(0xFF * mul_amt);
      }
      bitmap_data = pgm_read_byte(bofs) * mul_amt;

      if (sRow >= 0) {
        data = sBuffer[ofs];
        data &= (uint8_t)(mask_data);
        data |= (uint8_t)(bitmap_data);
        sBuffer[ofs] = data;
      }
      if (yOffset != 0 && sRow >= 0 && sRow < 7) {
        data = sBuffer[ofs + WIDTH];
        data &= (*((unsigned char *) (&mask_data) + 1));
        data |= (*((unsigned char *) (&bitmap_data) + 1));
        sBuffer[ofs + WIDTH] = data;
      }
      if (mirror) {
        ofs--;
        if (mask != NULL) mask_ofs--;
      } else {
        ofs++;
        if (mask != NULL) mask_ofs++;
      }
      bofs++;
    }
    sRow++;
    bofs += w - rendered_width;

    if (mirror) {
      if (mask != NULL) mask_ofs += w + rendered_width;
      ofs += WIDTH + rendered_width;
    } else {
      if (mask != NULL) mask_ofs += w - rendered_width;
      ofs += WIDTH - rendered_width;
    }
  }
}



void Customlib::drawNoiseCircle(uint8_t x0, uint8_t y0, uint8_t r)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  uint8_t x = 0;
  uint8_t y = r;

  const uint8_t SHIELD_NOISE = 2;

  arduboy->drawPixel(x0, y0-r, WHITE);
/*
  arduboy->drawPixel(x0, y0+r, WHITE);
  arduboy->drawPixel(x0, y0-r, WHITE);
  arduboy->drawPixel(x0+r, y0, WHITE);
  arduboy->drawPixel(x0-r, y0, WHITE);
  */

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;
/*
    arduboy->drawPixel(x0 + x, y0 + y, color);
    arduboy->drawPixel(x0 - x, y0 + y, color);
    arduboy->drawPixel(x0 + x, y0 - y, color);
    arduboy->drawPixel(x0 - x, y0 - y, color);
    arduboy->drawPixel(x0 + y, y0 + x, color);
    arduboy->drawPixel(x0 - y, y0 + x, color);
    arduboy->drawPixel(x0 + y, y0 - x, color);
    arduboy->drawPixel(x0 - y, y0 - x, color);
    */
/*
    int8_t xMod = x;
    int8_t yMod = y;
    for (uint8_t i = 0; i < 2; i++){

    if (i == 1){
      xMod = -x;
      yMod = -y;
    }
    arduboy->drawPixel(x0 + xMod + rand8()%SHIELD_NOISE, y0 + y + rand8()%SHIELD_NOISE, WHITE);
    //arduboy->drawPixel(x0 - x + rand8()%SHIELD_NOISE, y0 + y + rand8()%SHIELD_NOISE, WHITE);
    arduboy->drawPixel(x0 + xMod + rand8()%SHIELD_NOISE, y0 - y + rand8()%SHIELD_NOISE, WHITE);
    //arduboy->drawPixel(x0 - x + rand8()%SHIELD_NOISE, y0 - y + rand8()%SHIELD_NOISE, WHITE);
    arduboy->drawPixel(x0 + yMod + rand8()%SHIELD_NOISE, y0 + x + rand8()%SHIELD_NOISE, WHITE);
    //arduboy->drawPixel(x0 - y + rand8()%SHIELD_NOISE, y0 + x + rand8()%SHIELD_NOISE, WHITE);
    arduboy->drawPixel(x0 + yMod + rand8()%SHIELD_NOISE, y0 - x + rand8()%SHIELD_NOISE, WHITE);
    //arduboy->drawPixel(x0 - y + rand8()%SHIELD_NOISE, y0 - x + rand8()%SHIELD_NOISE, WHITE);
      
  }*/

    char noise[4];
  
    for (uint8_t i = 0; i < 4; i++){

    for (uint8_t j = 0; j < 4; j++){
            noise[j] = - SHIELD_NOISE / 2 + rand8()%SHIELD_NOISE;  
    }

    int8_t xMod = x;
    int8_t yMod = y;

    int8_t xMod2 = x;
    int8_t yMod2 = y;

    if (i > 1){
      xMod = -x;
      yMod = -y;
    }

    
    if (i%2 == 0){
      xMod2 = -x;
      yMod2 = -y;
    }

    
    arduboy->drawPixel(x0 + xMod + noise[0], y0 + yMod2 + noise[1], WHITE);
    arduboy->drawPixel(x0 + yMod + noise[2], y0 + xMod2 + noise[3], WHITE);
      
  }
  /*
    for (uint8_t i = 0; i < 2; i++){

    if (i == 1){
      xMod = -x;
      yMod = -y;
    }
    arduboy->drawPixel(x0 + xMod + rand8()%SHIELD_NOISE, y0 + y + rand8()%SHIELD_NOISE, WHITE);
    //arduboy->drawPixel(x0 - x + rand8()%SHIELD_NOISE, y0 + y + rand8()%SHIELD_NOISE, WHITE);
    arduboy->drawPixel(x0 + xMod + rand8()%SHIELD_NOISE, y0 - y + rand8()%SHIELD_NOISE, WHITE);
    //arduboy->drawPixel(x0 - x + rand8()%SHIELD_NOISE, y0 - y + rand8()%SHIELD_NOISE, WHITE);
    arduboy->drawPixel(x0 + yMod + rand8()%SHIELD_NOISE, y0 + x + rand8()%SHIELD_NOISE, WHITE);
    //arduboy->drawPixel(x0 - y + rand8()%SHIELD_NOISE, y0 + x + rand8()%SHIELD_NOISE, WHITE);
    arduboy->drawPixel(x0 + yMod + rand8()%SHIELD_NOISE, y0 - x + rand8()%SHIELD_NOISE, WHITE);
    //arduboy->drawPixel(x0 - y + rand8()%SHIELD_NOISE, y0 - x + rand8()%SHIELD_NOISE, WHITE);
      
  }  */
  }
}

void Customlib::display() {

  arduboy->paintScreen(sBuffer);
#ifdef SCREEN_CAST
  Serial.write(sBuffer, 128 * 64 / 8);
#endif
}

void Customlib::print (const char* text, uint8_t x, uint8_t y) {

  arduboy->setCursor(x, y);
  
  int textLength = strlen_P(text);
  uint8_t i;
  char myChar;
  for (i = 0; i < textLength; i++) {
    myChar =  pgm_read_byte(text + i);
    arduboy->print(myChar);
  }
}


void Customlib::addNoise(int noise) {

  createNoiseArray(noise);
  int i;
  for (int i = 0 ; i < WIDTH * HEIGHT / 8 ; i++) {
    sBuffer[i] ^= noiseArray[rand8() % NOISE_SIZE];
  }

}

void Customlib::createNoiseArray(int noise) {

  if (noise == noiseLevel) return;

  // Recreate noise array if noise level changes
  int i, j;
  noiseLevel = noise;
  memset(noiseArray, 0, sizeof noiseArray);
  for (i = 0; i < NOISE_SIZE; i++) {
    for (j = 0; j < 8; j++ ) {
      noiseArray[i] |= ((rand8() % noise == 0) ? (B00000001 << j) : 0);
    }
  }
}

void Customlib::mirrorScreen() {

  uint16_t pos1, pos2;
  uint8_t temp;
  uint16_t i;

  for (i = 0 ; i < WIDTH / 2 * HEIGHT / 8 ; i++) {
    pos1 = (((2 * i) / WIDTH) * WIDTH) + (uint8_t) i % (WIDTH / 2);
    pos2 = (((2 * i) / WIDTH) * WIDTH) + (uint8_t)(WIDTH - 1 - i % (WIDTH / 2));
    temp = sBuffer[pos1];
    sBuffer[pos1] = sBuffer[pos2];
    sBuffer[pos2] = temp;
  }
}
/*
//Fast pseudoo-random number generator, it's ok for noise
#define STATE_BYTES 7
#define MULT 0x13B /* for STATE_BYTES==6 only
#define MULT_LO (MULT & 255)
#define MULT_HI (MULT & 256)

static uint8_t rand8() {
  static uint8_t state[STATE_BYTES] = { 0x87, 0xdd, 0xdc, 0x10, 0x35, 0xbc, 0x5c };
  static uint16_t c = 0x42;
  static int i = 0;
  uint16_t t;
  uint8_t x;

  x = state[i];
  t = (uint16_t)x * MULT_LO + c;
  c = t >> 8;

#if MULT_HI
  c += x;
#endif

  x = t & 255;
  state[i] = x;
  if (++i >= sizeof(state))
    i = 0;
  return x;
}*/

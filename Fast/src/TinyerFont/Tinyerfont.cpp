#include "Tinyerfont.h"
#include "TinyerfontSprite.c"

#define TINYFONT_WIDTH 4
#define TINYFONT_HEIGHT 4

Tinyerfont::Tinyerfont(uint8_t *screenBuffer, int16_t width, int16_t height){
  sBuffer = screenBuffer;
  sWidth = width;
  sHeight = height;

  lineHeight = TINYFONT_HEIGHT + 1;
  letterSpacing = 1;

  cursorX = cursorY = baseX = 0;
  textColor = 1;

  maskText = false;
}


size_t Tinyerfont::write(uint8_t c) {
  if(c == '\n'){
    cursorX = baseX;
    cursorY += lineHeight;
  }
  else if(c == '\t'){
    cursorX += TINYFONT_WIDTH + 5;
  }
  else{
    cursorX -= _printChar(c, cursorX, cursorY);
    cursorX += TINYFONT_WIDTH + letterSpacing;
  }
    return 1;
}


uint8_t Tinyerfont::_printChar(char c, int8_t x, int8_t y) {
  if (x + TINYFONT_WIDTH <= 0 || x > sWidth - 1 || y + TINYFONT_HEIGHT <= 0 || y > sHeight - 1)
    return 0;

  uint8_t kern = 0;

  if (((uint8_t) c) < 32 || ((uint8_t) c) > 127) c = (char)127;

  uint8_t cval = ((uint8_t) c) - 32;

  if (cval >= 65 && cval <= 90) y++;
  if (cval == 12 || cval == 27) y++;

  const uint8_t *letter = TINYERFONT_SPRITE + ((cval>>1) * 4);

  for (uint8_t i = 0; i < 4; i++ ) {

    uint8_t colData = pgm_read_byte(letter++);
    if (c & 0x01) {
      colData >>= 4;
    }
    else{
      colData &= 0x0f;
    }

    if (maskText) {
      drawByte(x+i, y, 0x0f, (textColor == 0)?1:0);
    }

    if (colData) {
        kern = 0;
    } else {
        kern++;
    }

    drawByte(x+i, y, colData, textColor);

  }
  return kern;
}

void Tinyerfont::printChar(char c, int8_t x, int8_t y) {
    _printChar(c, x, y);
}

void Tinyerfont::drawByte(int8_t x, int8_t y, uint8_t pixels, uint8_t color){
  if ((((uint8_t)y)&0x07) == 0) {
    uint8_t row = (uint8_t)y >>3;
    uint8_t col = ((uint8_t)x) & (sWidth-1);
    if (color == 0)
      sBuffer[col + row*sWidth] &= ~pixels;
    else
      sBuffer[col + row*sWidth] |= pixels;
  }
  else{
    uint8_t d = ((uint8_t)y)&0x7;
    drawByte(x, y&0xF8, pixels << d, color);
    drawByte(x, (y&0xF8)+8, pixels >> (8-d), color);
  }
}

void Tinyerfont::setCursor(int8_t x, int8_t y){
  cursorX = baseX = x;
  cursorY = y;
}

int16_t Tinyerfont::getCursorX() const {
  return cursorX;
}

int16_t Tinyerfont::getCursorY() const {
  return cursorY;
}

void Tinyerfont::setTextColor(uint8_t color){
  textColor = color;
}

uint8_t Tinyerfont::getTextColor() const {
  return textColor;
}

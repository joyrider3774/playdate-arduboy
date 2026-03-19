#ifndef TINYERFONT_H
#define TINYERFONT_H

#ifdef _PLAYDATE
#include "Arduboy2Common.h"
#include "Print.h"
#else
#include <Arduino.h>
#include <Print.h>
#endif

/**
 * Tinyerfont uses a 4x4 font to print text.
 * It conforms do standard ASCII letters in range from 32 to 126.
 * Even tough the lowercase letters are available, the best readability
 * is given with the Uppercase letters.
 *
 * Usage:
 * Call the print() method of a Tinyerfont instance.
 *
 * Special letters:
 * \n makes a linebreak in print().
 * \t makes a 9 px long jump in print().
 */
class Tinyerfont : public Print {
  public:
    Tinyerfont(uint8_t *screenBuffer, int16_t width, int16_t height);
    virtual size_t write(uint8_t);

    void printChar(char c, int8_t x, int8_t y);
    uint8_t _printChar(char c, int8_t x, int8_t y);

    void setCursor(int8_t x, int8_t y);
    int16_t getCursorX() const;
    int16_t getCursorY() const;
    void setTextColor(uint8_t color);
    uint8_t getTextColor() const;

    bool maskText;

  private:
    void drawByte(int8_t x, int8_t y, uint8_t pixels, uint8_t color);

    uint8_t *sBuffer;
    int16_t sWidth;
    int16_t sHeight;

    int8_t cursorX;
    int16_t baseX;
    int8_t cursorY;

    uint8_t textColor;
    uint8_t letterSpacing;
    uint8_t lineHeight;
};

#endif

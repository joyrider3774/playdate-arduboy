#pragma once
#include <Arduboy2.h>

class FatFont : public Arduboy2 {
  public:
    virtual size_t write(uint8_t); // used by the Arduino Print class
    void printChar(const char c, const int8_t x, int8_t y);
    void setCursor(const int8_t x, const int8_t y);

  private:
    const uint8_t LINE_HEIGHT = 8;
    const uint8_t LETTER_WIDTH = 6;
    const uint8_t LETTER_SPACING = 1;
    int8_t cursorX;
    int8_t baseX;
    int8_t cursorY;
    uint8_t * screenBuffer = Arduboy2Base::sBuffer;
};

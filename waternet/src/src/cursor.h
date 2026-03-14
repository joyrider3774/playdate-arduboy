#ifndef CURSOR_H
#define CURSOR_H

#include <stdint.h>

uint8_t  updateCursorFrame();
void drawCursors();
void hideCursors();
void showCursors();
void setCursorPos(uint8_t cursorNr, uint8_t xPos, uint8_t yPos);
void initCursors();

#endif
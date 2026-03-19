#include <Arduboy2.h>

extern Arduboy2 arduboy;

uint8_t _get_quadrants(uint8_t start_percentage, uint8_t stop_percentage) {
  uint8_t mask = 0;

  uint8_t cur_mask=0x01;
  for(uint8_t i=0;i<8;i++) {
    if (start_percentage < stop_percentage) {
      if (start_percentage == 0 && stop_percentage >=32) {
        mask |= cur_mask;
      }
    } else {
      if (stop_percentage >= 32 && start_percentage >=32) {
        mask |= cur_mask;
      }
    }
    cur_mask<<=1;
    start_percentage -= 32;
    stop_percentage -= 32;
  }

  return mask;
}

bool is_draw(uint8_t count, uint8_t dither, uint8_t r) {
    switch (dither) {
        case 0: return true;
        case 1: return count % 3;
        case 2: return count % 2;
        case 3: return (count % 3)==2;
        case 4: return (count % 6)==(r*r)%6;
        default:
            return false;
    }
}


uint8_t drawArcHelper(int8_t x0, int8_t y0, uint8_t r, uint8_t color, uint8_t mask, uint8_t max, uint8_t min, uint8_t dither=0)
{
  uint8_t y = r;
  uint8_t x = 0;
  int8_t d = 1 - r;

  while(x < y) {
    x++;
    if(x >= min && x <= max && is_draw(x, dither, r)) {
      if (mask & 0b00000001) arduboy.drawPixel(x0 + y, y0 - x, color);
      if (mask & 0b00000010) arduboy.drawPixel(x0 + x, y0 - y, color);
      if (mask & 0b00000100) arduboy.drawPixel(x0 - x, y0 - y, color);
      if (mask & 0b00001000) arduboy.drawPixel(x0 - y, y0 - x, color);
      if (mask & 0b00010000) arduboy.drawPixel(x0 - y, y0 + x, color);
      if (mask & 0b00100000) arduboy.drawPixel(x0 - x, y0 + y, color);
      if (mask & 0b01000000) arduboy.drawPixel(x0 + x, y0 + y, color);
      if (mask & 0b10000000) arduboy.drawPixel(x0 + y, y0 + x, color);
    }
    if (d<0) {
      d += (x<<1) + 1;
    } else {
      y = y - 1;
      d += ((x-y)<<1) + 1;
    }
  }
  return x;
}


void drawArc(int8_t x0, int8_t y0, uint8_t r, uint8_t color, uint8_t start_percentage, uint8_t stop_percentage, uint8_t dither) {

  if (dither >= 5) return;
  uint8_t quadrants_mask = _get_quadrants(start_percentage, stop_percentage);
  uint8_t count = drawArcHelper(x0, y0, r, color, quadrants_mask, 255, 0, dither);

  uint8_t start = ((uint16_t)start_percentage%32)*count/32;
  uint8_t start_mask = start_percentage/32;
  uint8_t stop = ((uint16_t)stop_percentage%32)*count/32;
  uint8_t stop_mask = stop_percentage/32;

  quadrants_mask |= ((1<<start_mask) | (1<<stop_mask));

  if(dither < 6) {
    if ((quadrants_mask & 0b01100000) == 0b01100000) arduboy.drawPixel(x0, y0+r, color);
    if ((quadrants_mask & 0b00000110) == 0b00000110) arduboy.drawPixel(x0, y0-r, color);
    if ((quadrants_mask & 0b10000001) == 0b10000001) arduboy.drawPixel(x0+r, y0, color);
    if ((quadrants_mask & 0b00011000) == 0b00011000) arduboy.drawPixel(x0-r, y0, color);
  }

  if (start_mask == stop_mask) {
    if (stop > start) {
      if(start_mask % 2) {
        drawArcHelper(x0, y0, r, color, 1<<start_mask, count-start, count-stop, dither);
      } else {
        drawArcHelper(x0, y0, r, color, 1<<start_mask, stop, start, dither);
      }
    } else if (start > stop) {
      if(start_mask % 2) {
        drawArcHelper(x0, y0, r, color, 1<<start_mask, count-start, 0, dither);
        drawArcHelper(x0, y0, r, color, 1<<stop_mask, 255, count-stop, dither);
      } else {
        drawArcHelper(x0, y0, r, color, 1<<start_mask, 255, start, dither);
        drawArcHelper(x0, y0, r, color, 1<<stop_mask, stop, 0, dither);
      }
    } else {
      if(start_percentage < stop_percentage) {
        drawArcHelper(x0, y0, r, color, 1<<stop_mask, start+1, start, dither);
      } else {
        drawArcHelper(x0, y0, r, color, 1<<stop_mask, 255, 0, dither);
      }
    }
  } else
  {
    if(start_mask % 2) {
      drawArcHelper(x0, y0, r, color, 1<<start_mask, count-start, 0, dither);
    } else {
      drawArcHelper(x0, y0, r, color, 1<<start_mask, 255, start, dither);
    }

    if (start_mask != stop_mask || start>stop) {
      if(stop_mask % 2) {
        drawArcHelper(x0, y0, r, color, 1<<stop_mask, 255, count-stop, dither);
      } else {
        drawArcHelper(x0, y0, r, color, 1<<stop_mask, stop, 0, dither);
      }
    }
  }
}

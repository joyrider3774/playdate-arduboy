#ifndef SCORE_H
#define SCORE_H


//define score font size
#define SCORE_SMALL_FONT          0
#define SCORE_BIG_FONT            1

void drawScore(uint8_t scoreX, uint8_t scoreY, uint8_t fontType)
{
  char buf[10];
  //scorePlayer = arduboy.cpuLoad();
  itoa(scorePlayer, buf, 10);
  char charLen = strlen(buf);
  char pad = 5 - charLen;

  //draw 0 padding
  for (uint8_t i = 0; i < pad; i++)
  {
    switch (fontType)
    {
      case SCORE_SMALL_FONT:
        sprites.drawSelfMasked(scoreX + (4 * i), scoreY, numbersSmall, 0);
        break;
      case SCORE_BIG_FONT:
        sprites.drawSelfMasked(scoreX + (20 * i), scoreY, numbersBig, 0);
        break;
    }
  }

  for (uint8_t i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    uint8_t j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (uint8_t z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    switch (fontType)
    {
      case SCORE_SMALL_FONT:
        sprites.drawSelfMasked(scoreX + (pad * 4) + (4 * i), scoreY, numbersSmall, digit);
        break;
      case SCORE_BIG_FONT:
        sprites.drawSelfMasked(scoreX + (pad * 20) + (20 * i), scoreY, numbersBig, digit);
        break;
    }
  }
}

#endif

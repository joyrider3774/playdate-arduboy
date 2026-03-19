#ifdef USE_LARGE_MAP
#include <Arduboy2.h>
#include "Dark-And-Under.h"
/* -----------------------------------------------------------------------------------------------------------------------------
 *  Display Large Map.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void displayLargeMap() {

  drawFrames();
  drawMapAndStatistics(&myHero, &myLevel, false);
  uint8_t buttons = arduboy.justPressedButtons();

  if (buttons & BACK_BUTTON_MASK) { gameState = savedState; }

}
#endif

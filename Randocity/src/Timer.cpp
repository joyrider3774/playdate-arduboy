#include "Timer.h"

void Timer::startCountDown(uint8_t minutes, uint8_t seconds) {
  timerMinutes = minutes;
  timerSeconds = seconds;
  timerMode = timerCountDown;
}

void Timer::startCountUp() {
  timerMinutes = 0;
  timerSeconds = 0;
  timerMode = timerCountUp;
}

void Timer::tick() {
  if (timerMode == timerCountDown) {
    if (timerSeconds == 0) {
      if (timerMinutes == 0) {
        timerMode = timerDone;
      }
      else {
        timerMinutes--;
        timerSeconds = 59;
      }
    }
    else {
      timerSeconds--;
    }
  }
  else if (timerMode == timerCountUp) {
    timerSeconds++;
    if (timerSeconds >= 60) {
      timerSeconds = 0;
      timerMinutes++;
      if (timerMinutes == 99) {
        timerMode = timerDone;
      }
    }
  }
}

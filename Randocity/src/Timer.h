#pragma once
#include <Arduboy2.h>

class Timer {
  public:
  
    uint8_t timerSeconds;
    uint8_t timerMinutes;

    void startCountDown(uint8_t minutes, uint8_t seconds);
    void startCountUp();
    void tick();
    void setDone() { timerMode = timerDone; }
    bool isDone()  { return(timerMode == timerDone); }
    
private:
    enum TimerMode : uint8_t {timerCountDown, timerCountUp, timerDone}; 
    TimerMode timerMode = timerDone;
};

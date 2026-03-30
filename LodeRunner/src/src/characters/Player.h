#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../utils/Enums.h"

class Player {

  public: 

    Player() {};
  
    uint8_t getX();
    uint8_t getY();
    int8_t getXDelta();
    int8_t getYDelta();
    uint16_t getScore();
    uint8_t getMen();
    PlayerStance getStance();
    GameState getNextState();
    
    void setX(uint8_t val);
    void setY(uint8_t val);
    void setXDelta(int8_t val);
    void setYDelta(int8_t val);
    void setScore(uint16_t val);
    void setMen(uint8_t val);
    void setStance(const PlayerStance val);
    void setNextState(const GameState val);
    void incrementMen();

  private:

    uint8_t _x;
    uint8_t _y;
    int8_t _xDelta;
    int8_t _yDelta;
    uint16_t _score;
    uint8_t _men;
    PlayerStance _stance;
    GameState _nextState;

};

//--------------------------------------------------------------------------------------------------------------------------

inline uint8_t Player::getX() {
  return _x;
}

inline uint8_t Player::getY() {
  return _y;
}

inline int8_t Player::getXDelta() {
  return _xDelta;
}

inline int8_t Player::getYDelta() {
  return _yDelta;
}

inline uint16_t Player::getScore() {
  return _score;
}

inline uint8_t Player::getMen() {
  return _men;
}

inline PlayerStance Player::getStance() {
  return _stance;
}

inline GameState Player::getNextState() {
  return _nextState;
}
    
inline void Player::setX(uint8_t val) {
  _x = val;
}

inline void Player::setY(uint8_t val) {
  _y = val;
}

inline void Player::setXDelta(int8_t val) {
  _xDelta = val;
}

inline void Player::setYDelta(int8_t val) {
  _yDelta = val;
}

inline void Player::setScore(uint16_t val) {
  _score = val;
}

inline void Player::setMen(uint8_t val) {
  _men = val;
}

inline void Player::setStance(const PlayerStance val) {
  _stance = val;
}

inline void Player::setNextState(const GameState val) {
  _nextState = val;
}

inline void Player::incrementMen() {
  if (_men < 10)  _men++;
}
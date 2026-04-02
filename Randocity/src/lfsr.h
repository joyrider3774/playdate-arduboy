#pragma once
#include <Arduboy2.h>

class LFSR {
  public:
    uint32_t  lfsr;
    void      forward();
    void      backward();
    void      forward(uint16_t n);
    void      backward(uint16_t n);
    void      forward1024();
    void      backward1024();

    static const uint32_t degree        = 20;
    static const uint32_t mask          = 0xFFFFF;
    static const uint32_t forwardPoly   = 0x80004;
    static const uint32_t backwardPoly  = 0x00009; // 4*2+1
    static const uint32_t lsbBit        = 0x00001;
    static const uint32_t msbBit        = 0x80000;
    
};

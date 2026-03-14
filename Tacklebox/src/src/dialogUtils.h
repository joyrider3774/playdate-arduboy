#pragma once

#include <Arduboy2Common.h>
#include "baitType.h"

struct DialogUtils {
    static void reset();
    static void renderFrame(const uint8_t* bmp);
    static void renderMoneyInCorner();
};


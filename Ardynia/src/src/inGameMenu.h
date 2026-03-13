#ifndef ingamemenu_h
#define ingamemenu_h

#include "entityTypes.h"
#include "entities/player.h"
#include <Arduboy2Common.h>
typedef uint8_t Decision;

class InGameMenu {
    public:
        uint8_t row;
        EntityType chosenItem;

        InGameMenu():
            row(0),
            chosenItem(UNSET)
        {}

        void update(byte frame);
        void render(Player& player, uint8_t frame);
};

#endif


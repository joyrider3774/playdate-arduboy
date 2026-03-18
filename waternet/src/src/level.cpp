#include <string.h>
#include <stdint.h>

#include "level.h"
#include "commonvars.h"


void moveBlockDown(uint8_t aTile)
{
    uint8_t tmp = level[aTile + boardSize - boardWidth]; 
    for (uint8_t i= boardSize - boardWidth; i != 0 ; i -= boardWidth)
        level[aTile + i] = level[aTile + i -boardWidth];
    level[aTile] = tmp;
}

void moveBlockUp(uint8_t aTile)
{
    uint8_t tmp = level[aTile - boardSize + boardWidth]; 
    for (uint8_t i= boardSize - boardWidth; i != 0; i -= boardWidth)
        level[aTile - i] = level[aTile - i + boardWidth];
    level[aTile] = tmp;
}

void moveBlockRight(uint8_t aTile)
{
    uint8_t tmp = level[aTile + boardWidth - 1]; 
    memmove(&level[aTile+1], &level[aTile] , boardWidth -1);
    level[aTile] = tmp;
}

void moveBlockLeft(uint8_t aTile)
{
    uint8_t tmp = level[aTile - boardWidth + 1]; 
    memmove(&level[aTile - boardWidth + 1], &level[aTile - boardWidth + 1 + 1], boardWidth -1);
    level[aTile] = tmp;
}

//rotates a tile by change the tilenr in the level
//there are 16 tiles per set and there are 3 sets no water, water filled, and special start tiles
void rotateBlock(uint8_t aTile)
{
    switch (level[aTile])
    {
        case 1:
        case 17:
        case 33:
            level[aTile] = 2;
            break;
        case 2:
        case 18:
        case 34:
            level[aTile] = 4;
            break;
        case 3:
        case 19:
        case 35:
            level[aTile] = 6;
            break;
        case 4:
        case 20:
        case 36:
            level[aTile] = 8;
            break;
        case 5:
        case 21:
        case 37:
            level[aTile] = 10;
            break;
        case 6:
        case 22:
        case 38:
            level[aTile] = 12;
            break;
        case 7:
        case 23:
        case 39:
            level[aTile] = 14;
            break;
        case 8:
        case 24:
        case 40:
            level[aTile] = 1;
            break;
        case 9:
        case 25:
        case 41:
            level[aTile] = 3;
            break;
        case 10:
        case 26:
        case 42:
            level[aTile] = 5;
            break;
        case 11:
        case 27:
        case 43:
            level[aTile] = 7;
            break;
        case 12:
        case 28:
        case 44:
            level[aTile] = 9;
            break;
        case 13:
        case 29:
        case 45:
            level[aTile] = 11;
            break;
        case 14:
        case 30:
        case 46:
            level[aTile] = 13;
            break;
        default:
            break;
    }
}

void shuffleSlide(uint8_t aTile)
{
    uint8_t rnd = random(4);
    switch (rnd)
    {
        case 0:
            moveBlockUp((aTile % boardWidth) + boardSize - boardWidth);
            break;
        case 1:
            moveBlockDown((aTile % boardWidth));
            break;
        case 2:
            moveBlockLeft(boardWidth - 1 + aTile -(aTile % boardWidth));
            break;
        case 3:
            moveBlockRight(aTile - (aTile % boardWidth));
            break;
    }
}

void shuffleRotate(uint8_t aTile)
{
    uint8_t rnd = random(4);
    for (uint8_t i = 0; i < rnd; i++)
        rotateBlock(aTile);
}

void shuffleLevel()
{
    uint8_t rnd;
    uint8_t j = 0;
    while(j < boardSize)
    {
        switch(gameMode)
        {
            case gmRotate:
                shuffleRotate(j);
                j++;
                break;
            case gmSlide:
                shuffleSlide(j);
                //for speed up it should be fine as all slide levels are uneven in width / height (except random)
                j+=2;
                break;
            case gmRotateSlide:
                rnd = random(2);
                if(rnd == 0)
                {
                    shuffleSlide(j);
                    //for speed up
                    j+=2;
                }
                else
                {
                    shuffleRotate(j);
                    j++;
                }
                break;
        }
    }
}

void handleConnectPoint(uint8_t currentPoint, uint8_t* cellStack, uint8_t* cc)
{
    uint8_t lookUpX = currentPoint % boardWidth;
    uint8_t lookUpY = currentPoint / boardWidth;
    
    if ((lookUpY> 0) && (!(level[currentPoint] & 1)))
    {
        uint8_t tmp = currentPoint - boardWidth;
        uint8_t tmp2 = level[tmp];
        if (((tmp2 < 16) && (!(tmp2 & 4)) ) || 
        ((tmp2 > 15) && (!((tmp2 - 16) & 4))))
        {
            //adapt tile to filled tile
            if(level[currentPoint] < 16)
            { 
                level[currentPoint] += 16;
            }

            //add neighbour to cellstack of to handle tiles
            if (tmp2 < 16)
            {
                cellStack[(*cc)++] = tmp;
            }
        }
        
    }

    //if tile has passage to the east and east neigbour passage to the west 
    if  ((lookUpX  + 1 < boardWidth) && (!(level[currentPoint] & 2)))
    {
        uint8_t tmp = currentPoint + 1;
        uint8_t tmp2 = level[tmp];
        if (((tmp2 < 16) && (!(tmp2 & 8))) || 
        ((tmp2 > 15) && (!((tmp2 - 16) & 8))))
        {
            //adapt tile to filled tile
            if(level[currentPoint] < 16)
            { 
                level[currentPoint] += 16;
            }

            //add neighbour to cellstack of to handle tiles
            if (tmp2 < 16)
            {
                cellStack[(*cc)++] = tmp;
            }

        }
    }

    //if tile has passage to the south and south neigbour passage to the north 
    if ((lookUpY + 1 < boardHeight) && (!(level[currentPoint] & 4 )))
    {
        uint8_t tmp = currentPoint + boardWidth;
        uint8_t tmp2 = level[tmp];
        if (((tmp2 < 16) && (!(tmp2 & 1))) || 
        ((tmp2 > 15) && (!((tmp2 - 16) & 1))))
        {
            //adapt tile to filled tile
            if(level[currentPoint] < 16)
            { 
                level[currentPoint] += 16;
            }

            //add neighbour to cellstack of to handle tiles
            if (tmp2 < 16)
            {
                cellStack[(*cc)++] = tmp; 
            }
        } 
    }

    //if tile has passage to the west and west neigbour passage to the east 
    if  ((lookUpX > 0) && (!(level[currentPoint] & 8)))
    {
        uint8_t tmp = currentPoint - 1;
        uint8_t tmp2 = level[tmp];
        if (((tmp2 < 16) && (!(tmp2 & 2))) ||
        ((tmp2 > 15) && (!((tmp2 - 16) & 2))))
        {
            //adapt tile to filled tile
            if(level[currentPoint] < 16)
            { 
                level[currentPoint] += 16;
            }

            //add neighbour to cellstack of to handle tiles
            if(tmp2 < 16)
            {
                cellStack[(*cc)++] = tmp;
            }
        }  
    }

}

void updateConnected()
{
    uint8_t cellStack[maxBoardSize+1];
    //reset all tiles to default not filled one
    for (uint8_t i= 0; i != boardSize; i++)
    {
        if (level[i] > 31)
        {
            level[i] -= 32;
        }
        else
        {
            if (level[i] > 15)
            {
                level[i] -= 16;
            }
        }
    } 

    //start with start tile
    uint8_t cc = 1;
    handleConnectPoint(startPos, cellStack, &cc);
    while(--cc > 0)
    {
        //if tile is bigger then 15 we already handled this one, continue with next one
        if ((level[cellStack[cc]] < 16))
        {
            handleConnectPoint(cellStack[cc], cellStack, &cc);
        }
    }

    //add start pos special tile
    if (level[startPos] > 15)
       level[startPos] += 16;
    else 
        if (level[startPos] < 16)
            level[startPos] += 32;
}

void generateLevel()
{
    uint8_t neighbours[4];
    uint8_t cellStack[maxBoardSize+1];
    uint8_t cc = 0;
    int8_t currentPoint = 0;
    uint8_t visitedRooms = 1;
    uint8_t tmp, tmp2;
    uint8_t selectedNeighbour;
    uint8_t neighboursFound;
    uint8_t lookUpX, lookUpY;
    uint8_t rnd;
    //generate a lookup table so we don't have to use modulus or divide constantly
    //generateLookupTable(boardWidth, boardHeight);
    
    //intial all walls value in every room we will remove bits of this value to remove walls
    memset(level, 0xfu, boardSize);

    while (visitedRooms != boardSize)
    {
        neighboursFound = 0;
        lookUpX = currentPoint % boardWidth;
        lookUpY = currentPoint / boardWidth;

        tmp  = currentPoint+1; 
        //tile has neighbour to the right which we did not handle yet
        if ((level[tmp] == 0xfu) && ( lookUpX + 1 < boardWidth))
            neighbours[neighboursFound++] = tmp;
    
        tmp = currentPoint-1; 
        //tile has neighbour to the left which we did not handle yet
        if ((level[tmp] == 0xfu) && (lookUpX > 0))
            neighbours[neighboursFound++] = tmp;

        tmp = currentPoint - boardWidth; 
        //tile has neighbour the north which we did not handle yet
        if ((level[tmp] == 0xfu) && (lookUpY > 0))
            neighbours[neighboursFound++] = tmp;

        tmp = currentPoint + boardWidth; 
        //tile has neighbour the south which we did not handle yet
        if ((level[tmp] == 0xfu) && (lookUpY + 1 < boardHeight))
            neighbours[neighboursFound++] = tmp;

        switch (neighboursFound)
        {
            case 0:
                currentPoint = cellStack[--cc];
                continue;
                break;
            default:
                rnd = random(neighboursFound);
                break;
        }
        selectedNeighbour = neighbours[rnd];      
        tmp = (selectedNeighbour % boardWidth);
        //tile has neighbour to the east
        if(tmp > lookUpX)
        {
            //remove west wall neighbour
            level[selectedNeighbour] &= ~(8);
            //remove east wall tile
            level[currentPoint] &= ~(2);
        }
        else // tile has neighbour to the west
        {
            if(tmp < lookUpX)
            {
                //remove east wall neighbour
                level[selectedNeighbour] &= ~(2);
                //remove west wall tile
                level[currentPoint] &= ~(8);
            }
            else // tile has neighbour to the north
            {
                tmp2 = selectedNeighbour / boardWidth;
                if(tmp2 < lookUpY)
                {
                    //remove south wall neighbour
                    level[selectedNeighbour] &= ~(4);
                    //remove north wall tile
                    level[currentPoint] &= ~(1);
                }
                else // tile has neighbour to the south
                {
                    if(tmp2 > lookUpY)
                    {
                        //remove north wall neighbour
                        level[selectedNeighbour] &= ~(1);
                        //remove south wall tile
                        level[currentPoint] &= ~(4);
                    }
                }
            }
        }
        
        //add tile to the cellstack
        if(neighboursFound > 1)
        {
            cellStack[cc++] = currentPoint;
        } 
        //set tile to the neighbour   
        currentPoint = selectedNeighbour;
        visitedRooms++;
    }
}

//when all board tiles are not below 16, the level is cleared
//as there are 16 tiles per tilegroup (no water, water, special start with water) 
uint8_t isLevelDone()
{
    for (uint8_t i=0; i != boardSize; i++)
        if(level[i] < 16)
            return 0;

    return 1;
}

void initLevel(unsigned long aRandomSeed)
{
    levelDone = 0;
    moves = 0;
    if(difficulty != diffRandom)
        //use level number + fixed value based on difficulty as seed for the random function
        //this makes sure every level from a difficulty will remain the same
        randomSeed(selectedLevel + (difficulty * 500) + (gameMode * 50));
    else
        randomSeed(aRandomSeed); 

    maxLevel = levelCount;
    //set boardsize and max level based on difficulty
    switch (difficulty)
    {
        case diffVeryEasy:
            boardWidth = 5;
            boardHeight = 5;
            break;
        case diffEasy:
            boardWidth = 6;
            boardHeight = 6;
            break;
        case diffNormal:
            boardWidth = 7;
            boardHeight = 7;
            break;
        case diffHard:
            boardWidth = 8;
            boardHeight = 8;
            break;
        case diffVeryHard:
            boardWidth = 10;
            boardHeight = 8;
            break;    
        case diffRandom:
            uint8_t rnd = random(255);
            boardWidth = 5 + (rnd % (maxBoardWidth - 5 + 1)); //5 is smallest level width from very easy
            rnd = random(255);
            boardHeight = 5 + (rnd % (maxBoardHeight - 5 + 1)); //5 is smallest level height from very easy
            maxLevel = 0; //special value with random
            break;
    }
    //add space for arrows based on same posadd value (1 or 0 depending if sliding is allowed)
    boardWidth -= posAdd + posAdd;
    boardHeight -= posAdd + posAdd;
    boardSize = boardWidth * boardHeight;
    //generate the level
    generateLevel();
    //startpoint of of level in center of screen
    boardX = (maxBoardBgWidth - boardWidth) >> 1;
    boardY = (maxBoardBgHeight  - boardHeight) >> 1;
    startPos = (boardWidth >> 1) + (boardHeight >> 1) * (boardWidth);
    //startpoint of tile with water and our cursor
    selectionX = boardWidth >> 1;
    selectionY = boardHeight >> 1;

    //level is currently the solution so we still need to shuffle it
    shuffleLevel();
    //update possibly connected tiles already starting from startpoint
    updateConnected();
}

/**
 * @file Arduboy2Core.cpp
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#include "Arduboy2Common.h"
#include "Arduboy2Core.h"

//========================================
//========== class Arduboy2Core ==========
//========================================

// Playdate
LCDBitmap *Arduboy2Core::screenBitmap = nullptr;
uint8_t* Arduboy2Core::buffer = nullptr;
int Arduboy2Core::rowBytes;
int Arduboy2Core::dummy;
uint8_t* Arduboy2Core::dummy2 = nullptr;
bool arduboyFullscreenEnabled = false;
bool arduboyFpsEnabled = false;
PDMenuItem* arduboyFullScreenMenuItem = nullptr;
PDMenuItem* arduboyFpsMenuItem = nullptr;

// Commands sent to the OLED display to initialize it
const PROGMEM uint8_t Arduboy2Core::lcdBootProgram[] = {
        // boot defaults are commented out but left here in case they
        // might prove useful for reference
        //
        // Further reading: https://www.adafruit.com/datasheets/SSD1306.pdf
        //
        // Display Off
        // 0xAE,

        // Set Display Clock Divisor v = 0xF0
        // default is 0x80
        0xD5, 0xF0,

        // Set Multiplex Ratio v = 0x3F
        // 0xA8, 0x3F,

        // Set Display Offset v = 0
        // 0xD3, 0x00,

        // Set Start Line (0)
        // 0x40,

        // Charge Pump Setting v = enable (0x14)
        // default is disabled
        0x8D, 0x14,

        // Set Segment Re-map (A0) | (b0001)
        // default is (b0000)
        0xA1,

        // Set COM Output Scan Direction
        0xC8,

        // Set COM Pins v
        // 0xDA, 0x12,

        // Set Contrast v = 0xCF
        0x81, 0xCF,

        // Set Precharge = 0xF1
        0xD9, 0xF1,

        // Set VCom Detect
        // 0xDB, 0x40,

        // Entire Display ON
        // 0xA4,

        // Set normal/inverse display
        // 0xA6,

        // Display On
        0xAF,

        // set display mode = horizontal addressing mode (0x00)
        0x20, 0x00,

        // set col address range
        // 0x21, 0x00, COLUMN_ADDRESS_END,

        // set page address range
        // 0x22, 0x00, PAGE_ADDRESS_END
};

// prevBuffer tracks last painted state for dirty detection (markUpdatedRows only)
static uint8_t prevBuffer[(HEIGHT * WIDTH) / 8] = {0};

// Fullscreen src->dst range LUTs: for each src pixel, first and last dst pixel it maps to
#define FS_DST_W  398
#define FS_DST_H  236
#define FS_X_OFF  1
#define FS_Y_OFF  2
static uint16_t fsColDstStart[WIDTH];
static uint16_t fsColDstEnd[WIDTH];
static uint8_t fsRowDstStart[HEIGHT];
static uint8_t fsRowDstEnd[HEIGHT];
static bool fsLutBuilt = false;
static bool forceFullRedraw = false;

static void buildFullscreenLut()
{
    for (int sc = 0; sc < WIDTH; sc++) {
        int start = (int)(sc * 3.11f);
        int end   = (int)((sc + 1) * 3.11f) - 1;
        if (end >= FS_DST_W) end = FS_DST_W - 1;
        fsColDstStart[sc] = (uint16_t)start;
        fsColDstEnd[sc]   = (uint16_t)end;
    }
    for (int sr = 0; sr < HEIGHT; sr++) {
        int start = (int)(sr * 3.7f);
        int end   = (int)((sr + 1) * 3.7f) - 1;
        if (end >= FS_DST_H) end = FS_DST_H - 1;
        fsRowDstStart[sr] = (uint8_t)start;
        fsRowDstEnd[sr]   = (uint8_t)end;
    }
    fsLutBuilt = true;
}

void toggleFullscreen(void *userdata) {
    arduboyFullscreenEnabled = pd->system->getMenuItemValue(arduboyFullScreenMenuItem);
    if(!arduboyFullscreenEnabled)
       pd->graphics->clear(kColorBlack);
    // Force full redraw after mode switch
    forceFullRedraw = true;
}

void toggleFps(void *userdata) {
    arduboyFpsEnabled = pd->system->getMenuItemValue(arduboyFpsMenuItem);
    //clear left over fps display
    if(!arduboyFpsEnabled)
       pd->graphics->clear(kColorBlack);
    forceFullRedraw = true;
}


void Arduboy2Core::boot()
{
#ifdef ARDUBOY_SET_CPU_8MHZ
    // ARDUBOY_SET_CPU_8MHZ will be set by the IDE using boards.txt
  setCPUSpeed8MHz();
#endif

    bootPins();
    bootSPI();
    bootOLED();
    bootPowerSaving();

    arduboyFullScreenMenuItem = pd->system->addCheckmarkMenuItem("Fullscreen", arduboyFullscreenEnabled, toggleFullscreen, NULL);
    arduboyFpsMenuItem  = pd->system->addCheckmarkMenuItem("Show FPS", arduboyFpsEnabled, toggleFps, NULL);
}

#ifdef ARDUBOY_SET_CPU_8MHZ
// If we're compiling for 8MHz we need to slow the CPU down because the
// hardware clock on the Arduboy is 16MHz.
// We also need to readjust the PLL prescaler because the Arduino USB code
// likely will have incorrectly set it for an 8MHz hardware clock.
void Arduboy2Core::setCPUSpeed8MHz()
{
  uint8_t oldSREG = SREG;
  cli();                // suspend interrupts
  PLLCSR = _BV(PINDIV); // dissable the PLL and set prescale for 16MHz)
  CLKPR = _BV(CLKPCE);  // allow reprogramming clock
  CLKPR = 1;            // set clock divisor to 2 (0b0001)
  PLLCSR = _BV(PLLE) | _BV(PINDIV); // enable the PLL (with 16MHz prescale)
  SREG = oldSREG;       // restore interrupts
}
#endif

// Pins are set to the proper modes and levels for the specific hardware.
// This routine must be modified if any pins are moved to a different port
void Arduboy2Core::bootPins()
{
}

void Arduboy2Core::bootOLED()
{
    // Playdate
    pd->display->setRefreshRate(0);
    if (screenBitmap != nullptr) {
        pd->graphics->freeBitmap(screenBitmap);
    }
    screenBitmap = pd->graphics->newBitmap(WIDTH, HEIGHT, kColorBlack);
    pd->graphics->clear(kColorBlack);
    // Reset high-res timer so getElapsedTime() starts from a known zero point
    pd->system->resetElapsedTime();
}

void Arduboy2Core::LCDDataMode()
{
}

void Arduboy2Core::LCDCommandMode()
{
}

// Initialize the SPI interface for the display
void Arduboy2Core::bootSPI()
{
}

// Write to the SPI bus (MOSI pin)
void Arduboy2Core::SPItransfer(uint8_t data)
{
}

// Write to and read from the SPI bus (out to MOSI pin, in from MISO pin)
uint8_t Arduboy2Core::SPItransferAndRead(uint8_t data)
{
    return 0;
}

void Arduboy2Core::safeMode()
{
    if (buttonsState() == UP_BUTTON)
    {
        digitalWriteRGB(RED_LED, RGB_ON);
        while (true) { }
    }
}


/* Power Management */

void Arduboy2Core::idle()
{
}

void Arduboy2Core::bootPowerSaving()
{
}

// Shut down the display
void Arduboy2Core::displayOff()
{
}

// Restart the display after a displayOff()
void Arduboy2Core::displayOn()
{
    bootOLED();
}


/* Drawing */

// Playdate
void Arduboy2Core::putPixel(uint8_t x, uint8_t y, uint8_t color)
{
    uint8_t* block = buffer + (y * rowBytes) + (x / 8);
    uint8_t data = 0x80 >> (x % 8);
    *block = color ? *block | data : *block & ~data;
}

void Arduboy2Core::paint8Pixels(uint8_t pixels)
{
    // TODO: paint 8 pixels? (weird)
    pd->system->logToConsole("paint8Pixels");
}

void Arduboy2Core::paintScreen(const uint8_t *image)
{
    paintScreen((uint8_t *)image, false);
}

// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
//
// The following assembly code runs "open loop". It relies on instruction
// execution times to allow time for each byte of data to be clocked out.
// It is specifically tuned for a 16MHz CPU clock and SPI clocking at 8MHz.

void Arduboy2Core::paintScreen(uint8_t image[], bool clear)
{
    uint8_t* frame = pd->graphics->getFrame();

    LCDBitmap* dbg = NULL;
    if (pd->graphics->getDebugBitmap != NULL)
    {
        dbg = pd->graphics->getDebugBitmap();
        pd->graphics->pushContext(dbg);
        pd->graphics->clear(kColorBlack);
        pd->graphics->popContext();
    }

    if (!arduboyFullscreenEnabled)
    {
        const int pdRowBytes = LCD_ROWSIZE;
        const int xOffset = 9;
        const int yOffset = 25;

        int minDirtyRow = 240, maxDirtyRow = -1;

        for (int page = 0; page < HEIGHT / 8; page++)
        {
            int baseRow = page * 8;
            for (int col = 0; col < WIDTH; col++)
            {
                uint8_t byte = image[page * WIDTH + col];
                uint8_t prev = prevBuffer[page * WIDTH + col];
                bool dirty = (byte != prev);
                if (dirty)
                    prevBuffer[page * WIDTH + col] = byte;

                int px0 = xOffset + col * 3;
                int b0 = px0 / 8, b1 = (px0 + 1) / 8, b2 = (px0 + 2) / 8;
                uint8_t m0 = 0x80 >> (px0 % 8);
                uint8_t m1 = 0x80 >> ((px0 + 1) % 8);
                uint8_t m2 = 0x80 >> ((px0 + 2) % 8);
                uint8_t c0 = ~m0, c1 = ~m1, c2 = ~m2;

                for (int bit = 0; bit < 8; bit++)
                {
                    bool on = byte & (1 << bit);
                    int srcRow = baseRow + bit;
                    int pdY0 = yOffset + srcRow * 3;

                    for (int dy = 0; dy < 3; dy++)
                    {
                        int rowStart = (pdY0 + dy) * pdRowBytes;
                        if (on) {
                            frame[rowStart + b0] |= m0;
                            frame[rowStart + b1] |= m1;
                            frame[rowStart + b2] |= m2;
                        } else {
                            frame[rowStart + b0] &= c0;
                            frame[rowStart + b1] &= c1;
                            frame[rowStart + b2] &= c2;
                        }
                        if (dirty) {
                            if ((pdY0 + dy) < minDirtyRow) minDirtyRow = pdY0 + dy;
                            if ((pdY0 + dy) > maxDirtyRow) maxDirtyRow = pdY0 + dy;
                        }
                    }
                }
            }
        }

        if (forceFullRedraw)
        {
            minDirtyRow = yOffset;
            maxDirtyRow = yOffset + HEIGHT * 3 - 1;
        }

        if (maxDirtyRow >= 0)
        {
            pd->graphics->markUpdatedRows(minDirtyRow, maxDirtyRow);
            if (dbg)
            {
                pd->graphics->pushContext(dbg);
                pd->graphics->fillRect(0, minDirtyRow, 400, maxDirtyRow - minDirtyRow + 1, kColorWhite);
                pd->graphics->popContext();
            }
        }
        forceFullRedraw = false;
    }
    else
    {
        if (!fsLutBuilt)
            buildFullscreenLut();

        const int pdRowBytes = LCD_ROWSIZE;
        int minDirtyRow = 240, maxDirtyRow = -1;

        for (int page = 0; page < HEIGHT / 8; page++)
        {
            int baseRow = page * 8;
            for (int col = 0; col < WIDTH; col++)
            {
                uint8_t byte = image[page * WIDTH + col];
                uint8_t prev = prevBuffer[page * WIDTH + col];
                bool dirty = (byte != prev);
                if (dirty || forceFullRedraw)
                    prevBuffer[page * WIDTH + col] = byte;

                uint8_t changed = (forceFullRedraw || dirty) ? (forceFullRedraw ? 0xFF : (byte ^ prev)) : 0;
                if (!changed)
                    continue;

                int dxStart = FS_X_OFF + fsColDstStart[col];
                int dxEnd   = FS_X_OFF + fsColDstEnd[col];

                for (int bit = 0; bit < 8; bit++)
                {
                    if (!(changed & (1 << bit)))
                    {
                        continue;
                    }

                    bool on = byte & (1 << bit);
                    int srcRow = baseRow + bit;
                    int dyStart = FS_Y_OFF + fsRowDstStart[srcRow];
                    int dyEnd   = FS_Y_OFF + fsRowDstEnd[srcRow];

                    for (int pdY = dyStart; pdY <= dyEnd; pdY++)
                    {
                        int rowStart = pdY * pdRowBytes;
                        for (int pdX = dxStart; pdX <= dxEnd; pdX++)
                        {
                            uint8_t* dst = frame + rowStart + (pdX / 8);
                            uint8_t  mask = 0x80 >> (pdX % 8);
                            if (on) *dst |= mask;
                            else    *dst &= ~mask;
                        }
                        if (pdY < minDirtyRow) minDirtyRow = pdY;
                        if (pdY > maxDirtyRow) maxDirtyRow = pdY;
                    }
                }
            }
        }
        
        if (maxDirtyRow >= 0)
        {
            pd->graphics->markUpdatedRows(minDirtyRow, maxDirtyRow);
            if (dbg)
            {
                pd->graphics->pushContext(dbg);
                pd->graphics->fillRect(0, minDirtyRow, 400, maxDirtyRow - minDirtyRow + 1, kColorWhite);
                pd->graphics->popContext();
            }
        }
        forceFullRedraw = false;
    }

    if (clear)
        memset(image, 0, (HEIGHT * WIDTH) / 8);
}

void Arduboy2Core::blank()
{
    // TODO: implement?
}

void Arduboy2Core::sendLCDCommand(uint8_t command)
{
}

// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void Arduboy2Core::invert(bool inverse)
{
    pd->display->setInverted(inverse);
}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void Arduboy2Core::allPixelsOn(bool on)
{
    // TODO: implement?
}

// flip the display vertically or set to normal
void Arduboy2Core::flipVertical(bool flipped)
{
    pd->display->setFlipped(false, flipped);
}

// flip the display horizontally or set to normal
void Arduboy2Core::flipHorizontal(bool flipped)
{
    pd->display->setFlipped(flipped, true);
}

/* RGB LED */

void Arduboy2Core::setRGBled(uint8_t red, uint8_t green, uint8_t blue)
{
    // TODO: implement
}

void Arduboy2Core::setRGBled(uint8_t color, uint8_t val)
{
    // TODO: implement
}

void Arduboy2Core::freeRGBled()
{
    // TODO: implement
}

void Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    // TODO: implement
}

void Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val)
{
    // TODO: implement
}

/* Buttons */
uint8_t Arduboy2Core::buttonsState()
{
    PDButtons buttons;
    pd->system->getButtonState(&buttons, NULL, NULL);

    uint8_t result = 0;
    if (buttons & kButtonLeft)  result |= LEFT_BUTTON;   // PD bit0 → Arduboy bit5
    if (buttons & kButtonRight) result |= RIGHT_BUTTON;  // PD bit1 → Arduboy bit6
    if (buttons & kButtonUp)    result |= UP_BUTTON;     // PD bit2 → Arduboy bit7
    if (buttons & kButtonDown)  result |= DOWN_BUTTON;   // PD bit3 → Arduboy bit4
    if (buttons & kButtonB)     result |= B_BUTTON;      // PD bit4 → Arduboy bit2
    if (buttons & kButtonA)     result |= A_BUTTON;      // PD bit5 → Arduboy bit3

    return result;
}

unsigned long Arduboy2Core::generateRandomSeed()
{
    return rand();
}

// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms)
{
    delay((unsigned long) ms);
}

void Arduboy2Core::exitToBootloader()
{
    while (true) { }
}


//=========================================
//========== class Arduboy2NoUSB ==========
//=========================================

void Arduboy2NoUSB::mainNoUSB()
{
    loop();
}

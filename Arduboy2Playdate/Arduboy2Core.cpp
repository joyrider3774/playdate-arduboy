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

void toggleFullscreen(void *userdata) {
    arduboyFullscreenEnabled = pd->system->getMenuItemValue(arduboyFullScreenMenuItem);
    //clear left over stuff from going to fullscreen to none fullscreen
    if(!arduboyFullscreenEnabled)
       pd->graphics->clear(kColorBlack);
}

void toggleFps(void *userdata) {
    arduboyFpsEnabled = pd->system->getMenuItemValue(arduboyFpsMenuItem);
    //clear left over fps display
    if(!arduboyFpsEnabled)
       pd->graphics->clear(kColorBlack);
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
static uint8_t prevBuffer[(HEIGHT * WIDTH) / 8] = {0};

void Arduboy2Core::paintScreen(uint8_t image[], bool clear)
{
    pd->graphics->getBitmapData(screenBitmap, &dummy, &dummy, &rowBytes, &dummy2, &buffer);

    int minDirtyRow = HEIGHT, maxDirtyRow = -1;

    for (int page = 0; page < HEIGHT / 8; page++)
    {
        int baseRow = page * 8;
        for (int col = 0; col < WIDTH; col++)
        {
            uint8_t byte = image[page * WIDTH + col];
            uint8_t prev = prevBuffer[page * WIDTH + col];
            if (byte == prev)
                continue;  // skip unchanged column slices

            prevBuffer[page * WIDTH + col] = byte;
            uint8_t changed = byte ^ prev;  // only bits that changed

            int dstByteCol = col / 8;
            uint8_t setMask = 0x80 >> (col % 8);
            uint8_t clrMask = ~setMask;

            for (int bit = 0; bit < 8; bit++)
            {
                if (!(changed & (1 << bit)))
                    continue;  // skip unchanged pixels
                int row = baseRow + bit;
                if (row < minDirtyRow) minDirtyRow = row;
                if (row > maxDirtyRow) maxDirtyRow = row;
                uint8_t* dst = buffer + row * rowBytes + dstByteCol;
                if (byte & (1 << bit))
                    *dst |= setMask;
                else
                    *dst &= clrMask;
            }
        }
    }

    if (maxDirtyRow >= 0)
    {
        if (arduboyFullscreenEnabled)
            pd->graphics->drawScaledBitmap(screenBitmap, 0, 0, 3.11f, 3.7f);
        else
            pd->graphics->drawScaledBitmap(screenBitmap, 9, 25, 3.0f, 3.0f);
    }

    if (clear) {
        pd->graphics->clearBitmap(screenBitmap, kColorBlack);
        memset(image, 0, (HEIGHT * WIDTH) / 8);
        memset(prevBuffer, 0, sizeof(prevBuffer));
    }
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

/**
 * @file ArduboyTonesExt.cpp
 * \brief An Arduino library for playing tones and tone sequences, 
 * intended for the Arduboy game system.
 */

/*****************************************************************************
  ArduboyTones

An Arduino library to play tones and tone sequences.

Specifically written for use by the Arduboy miniature game system
https://www.arduboy.com/
but could work with other Arduino AVR boards that have 16 bit timer 3
available, by changing the port and bit definintions for the pin(s)
if necessary.

Copyright (c) 2017 Scott Allen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*****************************************************************************/

#include "ArduboyTonesExt.h"

// pointer to a function that indicates if sound is enabled
static bool (*outputEnabled)();

static volatile bool tonesPlaying = false;
static volatile uint16_t *tonesStart = nullptr;
static volatile uint16_t *tonesIndex = nullptr;
static volatile uint16_t toneSequence[MAX_TONES * 2 + 1];

// On Playdate there is no PROGMEM distinction; inProgmem is kept for API
// compatibility but both RAM and "PROGMEM" pointers are read the same way.
static volatile bool inProgmem;

PDSynth* chan3ext = nullptr;

volatile int32_t tones_duration_ext = 0;
volatile unsigned int playtones_prev_ext = 0;
volatile unsigned int playtones_accum_ext = 0;

void ArduboyTonesExt::callback()
{
    if (tonesIndex == nullptr) return;
    unsigned int curr = pd->system->getCurrentTimeMilliseconds();
    playtones_accum_ext += (curr - playtones_prev_ext) * 1024;
    playtones_prev_ext = curr;

    unsigned int elapsed = playtones_accum_ext / 1000;
    playtones_accum_ext %= 1000;

    if (elapsed == 0) return;

    if (tones_duration_ext > 0)
    {
        tones_duration_ext = tones_duration_ext - elapsed;
        if (tones_duration_ext <= 0) {
            ArduboyTonesExt::nextTone();
        }
    }
}

ArduboyTonesExt::ArduboyTonesExt()
{
    toneSequence[MAX_TONES * 2] = TONES_END;
}

void ArduboyTonesExt::setOutputEnabled(bool (*outEn)())
{
    outputEnabled = outEn;
}

void ArduboyTonesExt::begin()
{
    if (chan3ext == nullptr) {
        chan3ext = pd->sound->synth->newSynth();
        pd->sound->synth->setWaveform(chan3ext, kWaveformSquare);
    }
}

void ArduboyTonesExt::tone(uint16_t freq, uint16_t dur)
{
    if (!outputEnabled())
        return;
    inProgmem = false;
    tonesStart = tonesIndex = toneSequence; // set to start of sequence array
    toneSequence[0] = freq;
    toneSequence[1] = dur;
    toneSequence[2] = TONES_END; // set end marker
    nextTone(); // start playing
}

void ArduboyTonesExt::tone(uint16_t freq1, uint16_t dur1,
                            uint16_t freq2, uint16_t dur2)
{
    if (!outputEnabled())
        return;
    inProgmem = false;
    tonesStart = tonesIndex = toneSequence; // set to start of sequence array
    toneSequence[0] = freq1;
    toneSequence[1] = dur1;
    toneSequence[2] = freq2;
    toneSequence[3] = dur2;
    toneSequence[4] = TONES_END; // set end marker
    nextTone(); // start playing
}

void ArduboyTonesExt::tone(uint16_t freq1, uint16_t dur1,
                            uint16_t freq2, uint16_t dur2,
                            uint16_t freq3, uint16_t dur3)
{
    if (!outputEnabled())
        return;
    inProgmem = false;
    tonesStart = tonesIndex = toneSequence; // set to start of sequence array
    toneSequence[0] = freq1;
    toneSequence[1] = dur1;
    toneSequence[2] = freq2;
    toneSequence[3] = dur2;
    toneSequence[4] = freq3;
    toneSequence[5] = dur3;
    // end marker was set in the constructor and will never change
    nextTone(); // start playing
}

void ArduboyTonesExt::tones(const uint16_t *tones)
{
    inProgmem = true;
    tonesStart = tonesIndex = (uint16_t *)tones; // set to start of sequence array
    nextTone(); // start playing
}

void ArduboyTonesExt::tonesInRAM(uint16_t *tones)
{
    inProgmem = false;
    tonesStart = tonesIndex = tones; // set to start of sequence array
    nextTone(); // start playing
}

void ArduboyTonesExt::noTone()
{
    // stops sequence
    tonesIndex = nullptr;
    tonesStart = nullptr;
    tonesPlaying = false;
    // stops synth also
    pd->sound->synth->stop(chan3ext);
}

void ArduboyTonesExt::volumeMode(uint8_t mode)
{
    // Volume mode is not supported on the Playdate; kept for API compatibility.
    (void)mode;
}

bool ArduboyTonesExt::playing()
{
    return tonesPlaying;
}

void ArduboyTonesExt::nextTone()
{
    if (!outputEnabled())
        return;

    uint16_t freq;
    freq = getNext(); // get tone frequency

    if (freq == TONES_END) { // if freq is actually an "end of sequence" marker
        noTone(); // stop playing
        return;
    }

    tonesPlaying = true;

    if (freq == TONES_REPEAT) { // if frequency is actually a "repeat" marker
        tonesIndex = tonesStart; // reset to start of sequence
        freq = getNext();
    }

    freq &= ~TONE_HIGH_VOLUME; // strip volume indicator from frequency

    tones_duration_ext = getNext(); // get tone duration (in ~milliseconds / 1024ths of a second)

    if (freq == 0) {
        // Silent tone / rest: stop the synth but keep the timer running so
        // the sequence advances after the rest duration elapses.
        pd->sound->synth->stop(chan3ext);
    } else {
        // playNote: note expressed as a float frequency, volume 1.0, length -1
        // (let the callback/noTone stop it), offset 0.
        pd->sound->synth->playNote(chan3ext, freq, 1, -1, 0);
    }

    // Initialise the accumulator so the first callback delta is measured from now.
    playtones_prev_ext = pd->system->getCurrentTimeMilliseconds();
    playtones_accum_ext = 0;
}

uint16_t ArduboyTonesExt::getNext()
{
    // On Playdate, PROGMEM is a no-op macro; both paths read the same way.
    return *(tonesIndex++);
}

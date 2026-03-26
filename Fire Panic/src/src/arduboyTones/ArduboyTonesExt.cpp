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
static SoundSource* tonesSourceExt = nullptr;

// tones_duration_ext counts down in 1024ths of a second (Arduboy duration units).
// Fixed-point sample accumulator: accumulate (len * 1024), subtract 44100 per tick.
volatile int32_t tones_duration_ext = 0;
static volatile int32_t sample_accum_ext = 0;

static int tonesTimingCallbackExt(void* context, int16_t* left, int16_t* right, int len)
{
    if (tonesIndex == nullptr || tones_duration_ext <= 0)
        return 1;

    sample_accum_ext += len * 1024;
    while (sample_accum_ext >= 44100) {
        sample_accum_ext -= 44100;
        tones_duration_ext--;
        if (tones_duration_ext <= 0) {
            ArduboyTonesExt::nextTone();
            break;
        }
    }
    return 1;
}

// callback() kept as no-op for API compatibility.
void ArduboyTonesExt::callback() {}

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
        pd->sound->synth->setAttackTime(chan3ext, 0);
        pd->sound->synth->setDecayTime(chan3ext, 0);
        pd->sound->synth->setSustainLevel(chan3ext, 1.0f);
        pd->sound->synth->setReleaseTime(chan3ext, 0);
    }
    if (tonesSourceExt == nullptr) {
        tonesSourceExt = pd->sound->addSource(tonesTimingCallbackExt, nullptr, 0);
    }
}

void ArduboyTonesExt::tone(uint16_t freq, uint16_t dur)
{
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
    tones_duration_ext = 0;
    sample_accum_ext = 0;
    // stops synth also
    pd->sound->synth->noteOff(chan3ext, 0);
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

    tones_duration_ext = getNext(); // duration in 1024ths of a second (tick count)
    sample_accum_ext = 0; // reset sub-tick accumulator for the new tone

    // Stop the previous note cleanly before starting the next one.
    pd->sound->synth->noteOff(chan3ext, 0);

    // Only drive the synth when sound is enabled and the tone is not a rest.
    // When muted, the sequence still advances on schedule via callback() —
    // exactly as the AVR ISR kept decrementing durationToggleCount with
    // toneSilent = true, just without toggling the pin.
    if (freq == 0 || !outputEnabled()) {
        // Rest, or muted: keep timer running but produce no audio.
    } else {
        // playNote: frequency in Hz, volume 1.0, length -1 (noteOff stops it), offset 0.
        pd->sound->synth->playNote(chan3ext, freq, 1, -1, 0);
    }
}

uint16_t ArduboyTonesExt::getNext()
{
    // On Playdate, PROGMEM is a no-op macro; both paths read the same way.
    return *(tonesIndex++);
}

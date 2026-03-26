/**
 * @file ArduboyTones.cpp
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

#include "ArduboyTones.h"

// pointer to a function that indicates if sound is enabled
static bool (*outputEnabled)();

static volatile bool tonesPlaying = false;
static volatile uint16_t *tonesStart = nullptr;
static volatile uint16_t *tonesIndex = nullptr;
static volatile uint16_t toneSequence[MAX_TONES * 2 + 1];

PDSynth* chan3 = nullptr;
static SoundSource* tonesSource = nullptr;

// tones_duration counts down in 1024ths of a second (Arduboy duration units).
// sample_accum accumulates audio samples; every 44100/1024 ≈ 43.07 samples
// one tick elapses. We use fixed-point: accumulate (len * 1024) and subtract
// 44100 per tick, so no floating point and no drift.
volatile int32_t tones_duration = 0;
static volatile int32_t sample_accum = 0;

// Audio render callback — fires every audio block (~512 samples at 44100 Hz,
// roughly 86 times/sec) completely independent of the game loop and delay().
// We return 1 (silent source) every time; the PDSynth handles actual output.
static int tonesTimingCallback(void* context, int16_t* left, int16_t* right, int len)
{
    if (tonesIndex == nullptr || tones_duration <= 0)
        return 1;

    sample_accum += len * 1024;
    while (sample_accum >= 44100) {
        sample_accum -= 44100;
        tones_duration = tones_duration -1;
        if (tones_duration <= 0) {
            ArduboyTones::nextTone();
            break; // nextTone() resets tones_duration; stop consuming ticks
        }
    }
    return 1;
}

ArduboyTones::ArduboyTones(boolean (*outEn)())
{
    outputEnabled = outEn;
    toneSequence[MAX_TONES * 2] = TONES_END;
}

void ArduboyTones::tone(uint16_t freq, uint16_t dur)
{
    tonesStart = tonesIndex = toneSequence; // set to start of sequence array
    toneSequence[0] = freq;
    toneSequence[1] = dur;
    toneSequence[2] = TONES_END; // set end marker
    nextTone(); // start playing
}

void ArduboyTones::tone(uint16_t freq1, uint16_t dur1,
                        uint16_t freq2, uint16_t dur2)
{
    tonesStart = tonesIndex = toneSequence; // set to start of sequence array
    toneSequence[0] = freq1;
    toneSequence[1] = dur1;
    toneSequence[2] = freq2;
    toneSequence[3] = dur2;
    toneSequence[4] = TONES_END; // set end marker
    nextTone(); // start playing
}

void ArduboyTones::tone(uint16_t freq1, uint16_t dur1,
                        uint16_t freq2, uint16_t dur2,
                        uint16_t freq3, uint16_t dur3)
{
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

void ArduboyTones::tones(const uint16_t *tones)
{
    tonesStart = tonesIndex = (uint16_t *)tones; // set to start of sequence array
    nextTone(); // start playing
}

void ArduboyTones::tonesInRAM(uint16_t *tones)
{
    tonesStart = tonesIndex = tones; // set to start of sequence array
    nextTone(); // start playing
}

void ArduboyTones::noTone()
{
    //stops sequence
    tonesIndex = nullptr;
    tonesStart = nullptr;
    tonesPlaying = false;
    tones_duration = 0;
    sample_accum = 0;
    //stops synth also
    pd->sound->synth->noteOff(chan3, 0);
}

void ArduboyTones::volumeMode(uint8_t mode) {}

bool ArduboyTones::playing()
{
    return tonesPlaying;
}

void ArduboyTones::nextTone()
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

    // Duration is in 1024ths of a second — stored directly as tick count.
    // The audio timing callback decrements tones_duration once per tick.
    tones_duration = getNext();
    sample_accum = 0; // reset sub-tick accumulator for the new tone

    // Stop previous note cleanly before starting the next one.
    pd->sound->synth->noteOff(chan3, 0);

    // Only drive the synth when sound is enabled and the tone is not a rest.
    // When muted the sequence still advances via the audio callback — mirroring
    // AVR behaviour where toneSilent suppressed pin toggles but the timer ran.
    if (freq == 0 || !outputEnabled()) {
        // Rest or muted: keep timing running but produce no audio.
    } else {
        // playNote: frequency in Hz, volume 1.0, length -1 (noteOff stops it).
        pd->sound->synth->playNote(chan3, freq, 1, -1, 0);
    }
}

uint16_t ArduboyTones::getNext()
{
    return *(tonesIndex++);
}

void ArduboyTones::begin() {
    if (chan3 == nullptr) {
        chan3 = pd->sound->synth->newSynth();
        pd->sound->synth->setWaveform(chan3, kWaveformSquare);
        pd->sound->synth->setAttackTime(chan3, 0);
        pd->sound->synth->setDecayTime(chan3, 0);
        pd->sound->synth->setSustainLevel(chan3, 1.0f);
        pd->sound->synth->setReleaseTime(chan3, 0);
    }
    
    if (tonesSource == nullptr) {
        tonesSource = pd->sound->addSource(tonesTimingCallback, nullptr, 0);
    }
}
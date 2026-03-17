/**
 * @file Arduboy2Beep.cpp
 * \brief
 * Classes to generate simple square wave tones on the Arduboy speaker pins.
 *
 * Ported to Playdate SDK: hardware timer/counter replaced by Playdate synth API.
 * - count values are converted back to Hz using the original Arduboy prescaler formulas:
 *     BeepPin1: hz = 1000000 / (count + 1)  [Timer 3A, prescaler 8,  16MHz CPU]
 *     BeepPin2: hz = 62500   / (count + 1)  [Timer 4A, prescaler 128, 16MHz CPU]
 * - duration is a frame count; playNote() is always called with -1.0f (indefinite),
 *   and timer() stops the synth after the correct number of frames, exactly as the
 *   original Arduboy2 does. No fps assumption is made.
 * - tone() guards against retriggering: if called every frame with the same count
 *   while a tone is already playing, playNote() is skipped (only duration is updated).
 *   On real Arduboy hardware writing the same value to OCR3A is a no-op; without
 *   this guard, the Playdate synth would restart on every frame, preventing expiry.
 */

#include "Arduboy2Beep.h"

// Speaker pin 1, Timer 3A, Port C bit 6, Arduino pin 5

uint8_t BeepPin1::duration = 0;
static uint16_t beep1CurrentCount = 0xFFFF; // sentinel: no tone playing

static PDSynth* synth1 = nullptr;
static PDSynth* synth2 = nullptr;

void BeepPin1::begin()
{
    if (synth1 != nullptr)
        pd->sound->synth->freeSynth(synth1);

    synth1 = pd->sound->synth->newSynth();
    //pd->sound->source->setVolume((SoundSource*)synth1, 0.3, 0.3);
    pd->sound->synth->setWaveform(synth1, kWaveformSquare);
    beep1CurrentCount = 0xFFFF;
}

void BeepPin1::tone(uint16_t count)
{
    tone(count, 0);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
    // On real Arduboy hardware, writing to OCR3A while the timer is already
    // running at the same count is a no-op. Replicate that here: only call
    // playNote() when the count (i.e. frequency) actually changes, preventing
    // games that call tone() every frame from continuously restarting the synth.
    if (count == beep1CurrentCount && duration != 0)
    {
        duration = dur; // still update duration so timer() uses the new value
        return;
    }
    beep1CurrentCount = count;
    duration = dur;
    float hz = 1000000.0f / (count + 1);
    // Always play indefinitely (-1.0f); timer() stops the synth after the
    // correct number of frames, matching the original Arduboy2 behaviour.
    // Duration is a frame count, not a time value, so we cannot convert it
    // to seconds without knowing the caller's frame rate.
    pd->sound->synth->playNote(synth1, hz, 1.0f, -1.0f, 0);
}

void BeepPin1::timer()
{
    if (duration && (--duration == 0)) {
        beep1CurrentCount = 0xFFFF;
        pd->sound->synth->stop(synth1);
    }
}

void BeepPin1::noTone()
{
    duration = 0;
    beep1CurrentCount = 0xFFFF;
    pd->sound->synth->stop(synth1);
}


// Speaker pin 2, Timer 4A, Port C bit 7, Arduino pin 13

uint8_t BeepPin2::duration = 0;
static uint16_t beep2CurrentCount = 0xFFFF; // sentinel: no tone playing

void BeepPin2::begin()
{
    if (synth2 != nullptr)
        pd->sound->synth->freeSynth(synth2);

    synth2 = pd->sound->synth->newSynth();
    //pd->sound->source->setVolume((SoundSource*)synth2, 0.3, 0.3);
    pd->sound->synth->setWaveform(synth2, kWaveformSquare);
    beep2CurrentCount = 0xFFFF;
}

void BeepPin2::tone(uint16_t count)
{
    tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
    // See BeepPin1::tone() for explanation of the retrigger guard.
    if (count == beep2CurrentCount && duration != 0)
    {
        duration = dur;
        return;
    }
    beep2CurrentCount = count;
    duration = dur;
    float hz = 62500.0f / (count + 1);
    // Always play indefinitely (-1.0f); timer() stops the synth after the
    // correct number of frames, matching the original Arduboy2 behaviour.
    // Duration is a frame count, not a time value, so we cannot convert it
    // to seconds without knowing the caller's frame rate.
    pd->sound->synth->playNote(synth2, hz, 1.0f, -1.0f, 0);
}

void BeepPin2::timer()
{
    if (duration && (--duration == 0)) {
        beep2CurrentCount = 0xFFFF;
        pd->sound->synth->stop(synth2);
    }
}

void BeepPin2::noTone()
{
    duration = 0;
    beep2CurrentCount = 0xFFFF;
    pd->sound->synth->stop(synth2);
}
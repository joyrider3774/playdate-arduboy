#include <stdint.h>
#include <string.h>
#include <ArduboyTonesPitches.h>
#include <Arduboy2Beep.h>

#include "commonvars.h"
#include "sound.h"
#include "savestate.h"

//I (joyrider3774) created the music in this tool : https://onlinesequencer.net

uint8_t prev_music, music_on, sound_on;
BeepPin2 beep;

constexpr uint16_t pause1 = 250U;
constexpr uint16_t pause2 = 100U;
constexpr uint16_t pause3 = 233U;
constexpr uint16_t pause4 = 216U;

//https://onlinesequencer.net/2498607
const uint16_t PROGMEM music_levelsCleared[] ={
    NOTE_D6,pause4,
    NOTE_DS6,pause4,
    NOTE_D6,pause4,
    NOTE_REST,pause4,
    NOTE_D6,pause4,
    NOTE_E6,pause4,
    NOTE_F6,pause4,
    NOTE_E6,pause4,
    NOTE_E6,pause4,
    NOTE_E6,pause4,
    NOTE_D6,pause4,
    NOTE_REST,pause4,
    NOTE_DS6,pause4,
    NOTE_E6,pause4,
    NOTE_DS6,pause4,
    NOTE_REST,pause4,
    NOTE_E6,pause4,
    NOTE_F6,pause4,
    NOTE_E6,pause4,
    NOTE_REST,pause4,
    NOTE_D6,pause4,
    NOTE_DS6,pause4,
    NOTE_D6,pause4,
    NOTE_E6,pause4,
    NOTE_E6,pause4,
    NOTE_DS6,pause4,
    NOTE_DS6,pause4,
    NOTE_D6,pause4,
    NOTE_D6,pause4,
    NOTE_DS6,pause4,
    NOTE_E6,pause4,
    NOTE_DS6,pause4,
    NOTE_REST,pause4,
    NOTE_E6,pause4,
    NOTE_DS6,pause4,
    NOTE_E6,pause4,
    NOTE_DS6,pause4,
    NOTE_DS6,pause4,
    NOTE_E6,pause4,
    NOTE_REST,pause4,
    NOTE_E6,pause4,
    NOTE_F6,pause4,
    NOTE_DS6,pause4,
    NOTE_E6,pause4,
    NOTE_REST,pause4,
    NOTE_F6,pause4,
    NOTE_E6,pause4,
    NOTE_F6,pause4,
    TONES_REPEAT
};

//https://onlinesequencer.net/2484974
const uint16_t PROGMEM music_won[] ={
  NOTE_DS6, pause2,
  NOTE_D6,  pause2,
  NOTE_DS6, pause2,
  NOTE_E6, pause2,
  NOTE_DS6, pause2,
  NOTE_E6, pause2,
  NOTE_F6, pause2 << 2,
  NOTE_REST, pause2,
  TONES_END  
};

//https://onlinesequencer.net/2485064
const uint16_t PROGMEM music_game[] ={ 
    NOTE_REST, pause3,
    NOTE_C4, pause3,
    NOTE_CS4, pause3,
    NOTE_REST, pause3,
    NOTE_C4, pause3,
    NOTE_CS4, pause3,
    NOTE_CS4, pause3,
    NOTE_D4, pause3,
    NOTE_REST, pause3,
    NOTE_DS4, pause3,
    NOTE_D4, pause3,
    NOTE_REST, pause3,
    NOTE_DS4, pause3,
    NOTE_E4, pause3,
    NOTE_REST, pause3,
    NOTE_DS4, pause3,
    NOTE_E4, pause3,
    NOTE_E4, pause3,
    NOTE_F4, pause3,
    NOTE_REST, pause3,

    NOTE_C4, pause3,
    NOTE_CS4, pause3,
    NOTE_REST, pause3,
    NOTE_C4, pause3,
    NOTE_CS4, pause3,
    NOTE_CS4, pause3,
    NOTE_D4, pause3,
    NOTE_REST, pause3,
    NOTE_DS4, pause3,
    NOTE_D4, pause3,
    NOTE_REST, pause3,
    NOTE_DS4, pause3,
    NOTE_E4, pause3,
    NOTE_REST, pause3,
    NOTE_DS4, pause3,
    NOTE_E4, pause3,
    NOTE_E4, pause3,
    NOTE_F4, pause3,
    NOTE_REST, pause3,

    NOTE_D4, pause3,
    NOTE_C4, pause3,
    NOTE_E4, pause3,
    NOTE_D4, pause3,
    NOTE_REST,  pause3,
    NOTE_E4, pause3,
    NOTE_REST,  pause3,
    NOTE_E4, pause3,
    NOTE_D4, pause3,
    NOTE_REST,  pause3,
    NOTE_DS4, pause3,
    NOTE_DS4, pause3,
    NOTE_E4, pause3,
    NOTE_REST, pause3,
    NOTE_E4, pause3,
    NOTE_DS4, pause3,
    NOTE_D4, pause3,
    NOTE_C4, pause3,
    NOTE_DS4, pause3,
    NOTE_E4,pause3,
    NOTE_D4,pause3,
    NOTE_REST,pause3,
    NOTE_DS4,pause3,
    NOTE_D4,pause3,
    NOTE_C4,pause3,
    NOTE_REST,pause3,
    NOTE_D4,pause3,
    NOTE_C4,pause3,
    NOTE_E4,pause3,
    NOTE_D4,pause3,
    NOTE_REST,pause3,
    NOTE_E4,pause3,
    NOTE_REST,pause3,
    NOTE_E4,pause3,
    NOTE_D4,pause3,
    NOTE_REST,pause3,
    NOTE_DS4,pause3,
    NOTE_DS4,pause3,
    NOTE_E4,pause3,
    NOTE_REST,pause3,
    NOTE_E4,pause3,
    NOTE_D4,pause3,
    NOTE_C4,pause3,
    NOTE_CS4,pause3,
    NOTE_DS4,pause3,
    NOTE_F4,pause3,
    NOTE_E4,pause3,
    NOTE_D4,pause3,
    NOTE_DS4,pause3,
    NOTE_DS4,pause3,
    NOTE_REST, pause3,
    NOTE_E4,pause3,
    NOTE_D4,pause3,
    NOTE_D4,pause3,
    NOTE_REST, pause3,
    NOTE_DS4,pause3,
    NOTE_CS4,pause3,
    NOTE_CS4,pause3,
    NOTE_REST, pause3,
    NOTE_D4,pause3,
    TONES_REPEAT
};

//https://onlinesequencer.net/2484977
const uint16_t PROGMEM music_intro[] = {
    NOTE_C4,  pause1,
    NOTE_C4,  pause1 * 2,
    NOTE_D4,  pause1,
    NOTE_DS4, pause1,
    NOTE_C4,  pause1,
    NOTE_REST, pause1 * 8,
    
    NOTE_C4,  pause1,
    NOTE_C4,  pause1 * 2,
    NOTE_D4,  pause1,
    NOTE_DS4, pause1,
    NOTE_C4,  pause1,
    NOTE_D4,  pause1,
    NOTE_C4,  pause1,
    NOTE_CS4, pause1,
    NOTE_REST, pause1 * 4,

    NOTE_C4,  pause1,
    NOTE_C4,  pause1 * 2,
    NOTE_D4,  pause1,
    NOTE_DS4, pause1,
    NOTE_C4,  pause1,
    NOTE_REST, pause1 * 8,
    
    NOTE_C4,  pause1,
    NOTE_C4,  pause1 * 2,
    NOTE_D4,  pause1,
    NOTE_DS4, pause1,
    NOTE_C4,  pause1,
    NOTE_D4,  pause1,
    NOTE_C4,  pause1,
    NOTE_CS4, pause1,
    NOTE_REST, pause1 * 4,

    NOTE_D4,  pause1,
    NOTE_D4,  pause1, 
    NOTE_CS4, pause1,
    NOTE_C4,  pause1,
    NOTE_DS4, pause1,
    NOTE_CS4, pause1,
    NOTE_D4,  pause1,
    NOTE_DS4, pause1,
    NOTE_CS4, pause1,
    NOTE_DS4, pause1,
    NOTE_D4,  pause1,
    NOTE_D4,  pause1,
    NOTE_D4,  pause1,
    NOTE_E4, pause1,
    NOTE_C4,  pause1,
    NOTE_CS4, pause1,
    NOTE_E4,  pause1,
    NOTE_D4,  pause1 * 3,
    NOTE_REST, pause1 * 4,
    TONES_REPEAT 
};

void setMusicOn(uint8_t value)
{
    music_on = value;
    if(music_on)
    {
        arduboy.audio.on();
        arduboy.audio.saveOnOff();
        if (prev_music != 0)
            SelectMusic(prev_music, 1);
        else
        {
            //this can only happen on title screen
            //normally this does not belong here
            //but if music was off in eeprom no music was playing
            //before and you can only enable it in the main - options - menu
            //where title music plays
            if (gameState == gsTitle)
                SelectMusic(musTitle, 1);
        }
    }
    else
    {
        arduboySound.noTone();
        //both sound and music disabled turn audio off
        //and save the state to global flag
        if (!sound_on)
        {
            arduboy.audio.off();
            arduboy.audio.saveOnOff();
        }
    }
}

void setSoundOn(uint8_t value)
{
    sound_on = value;
    if(sound_on)
    {
        arduboy.audio.on();
        arduboy.audio.saveOnOff();
    }
    else
    {
        //both sound and music disabled turn audio off
        //and save the state to global flag
        if (!music_on)
        {
            arduboy.audio.off();
            arduboy.audio.saveOnOff();
        }
    }
}

uint8_t isMusicOn()
{
    return music_on;
}

uint8_t isSoundOn()
{
    return sound_on;
}

void initSound()
{
    sound_on = isSoundOnSaveState();
    beep.begin();
}

void soundTimer()
{
    beep.timer();
}

void SelectMusic(uint8_t musicFile, uint8_t force)
{
    if (((prev_music != musicFile) || force) && music_on)
    {
        prev_music = musicFile;
        switch (musicFile) 
        {
            case musTitle:
                arduboySound.tones(music_intro);
                break;
            case musLevelClear:
                arduboySound.tones(music_won);
                break;
            case musAllLevelsClear:
                arduboySound.tones(music_levelsCleared);
                break;
            case musGame:
                arduboySound.tones(music_game);
                break;
        }
    }
}

void playSound(uint16_t count)
{
    if(!sound_on)
        return;
    beep.tone(count, sfxSustain);
}

void initMusic()
{
    music_on = isMusicOnSaveState();
    prev_music = 0;
}

void playGameMoveSound()
{
    playSound(beep.freq(750));
}

void playErrorSound()
{
   playSound(beep.freq(300));
}

void playMenuSelectSound()
{
    playSound(beep.freq(1250));
}

void playMenuBackSound()
{
   playSound(beep.freq(1000));
}

void playMenuAcknowlege()
{
    playSound(beep.freq(900));
}

void playGameAction()
{
    playSound(beep.freq(600));
}

#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
extern Mix_Music *BackgroundMusic[];
extern Mix_Chunk *SoundEffects[];
extern Mix_Chunk *BackgroundSoundtracks[];
extern int PlayingChannel;
extern int CurrentMusic;
extern bool BackgroundMusicIsPlaying;
extern int BackgroundMusicChannel;
extern int BackGroundMusicPlaying;
void LoadSounds();
void PlayMusic();

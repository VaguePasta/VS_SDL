#include "sounds.h"
#include "gameobjects.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#define NUM_OF_BACKGROUND_MUSIC 4
#define NUM_OF_SOUNDTRACK 4
Mix_Music* BackgroundMusic[NUM_OF_BACKGROUND_MUSIC];
Mix_Chunk* SoundEffects[12];
Mix_Chunk* BackgroundSoundtracks[NUM_OF_SOUNDTRACK];
int PlayingChannel;
int CurrentMusic = -1;
bool BackgroundMusicIsPlaying = false;
int BackgroundMusicChannel = -1;
int BackGroundMusicPlaying = -1;
void LoadSounds()
{
	for (int i = 0; i < NUM_OF_BACKGROUND_MUSIC; i++) BackgroundMusic[i] = Mix_LoadMUS(("resources/sounds/music/BackgroundMusic" + std::to_string(i) + ".mp3").c_str());
	SoundEffects[0] = Mix_LoadWAV("resources/sounds/gunfire/AK47_sound.mp3");
	SoundEffects[1] = Mix_LoadWAV("resources/sounds/gunfire/Assault_Rifle_sound.mp3");
	SoundEffects[2] = Mix_LoadWAV("resources/sounds/gunfire/Launcher_sound.mp3");
	SoundEffects[3] = Mix_LoadWAV("resources/sounds/gunfire/Explosion_sound.mp3");
	SoundEffects[4] = Mix_LoadWAV("resources/sounds/effects/PlayerHurt.WAV");
	SoundEffects[5] = Mix_LoadWAV("resources/sounds/effects/Stepsound1.WAV");
	SoundEffects[6] = Mix_LoadWAV("resources/sounds/effects/Stepsound2.WAV");
	SoundEffects[7] = Mix_LoadWAV("resources/sounds/effects/PlayerDash.WAV");
	SoundEffects[8] = Mix_LoadWAV("resources/sounds/effects/FireSpell.WAV");
	SoundEffects[9] = Mix_LoadWAV("resources/sounds/effects/LightningSpell.WAV");
	SoundEffects[10] = Mix_LoadWAV("resources/sounds/effects/Bowshot.mp3");
	SoundEffects[11] = Mix_LoadWAV("resources/sounds/effects/Slash.mp3");
	for (int i = 0; i < NUM_OF_SOUNDTRACK; i++) BackgroundSoundtracks[0] = Mix_LoadWAV(("resources/sounds/backgroundsoundtracks/Soundtrack" + std::to_string(i) + ".mp3").c_str());
	Mix_AllocateChannels(32);
}
void PlayMusic()
{
	if (!Mix_PlayingMusic())
	{
		int random = CurrentMusic;
		while (random == CurrentMusic)
		{
			random = rand() % NUM_OF_BACKGROUND_MUSIC;
		}
		Mix_PlayMusic(BackgroundMusic[random], 1);
		CurrentMusic = random;
	}
	if (!gamestate.pause && gamestate.start && !BackgroundMusicIsPlaying)
	{
		if (rand() % 10000 <= 5)
		{
			BackgroundMusicIsPlaying = true;
			int random = BackGroundMusicPlaying;
			while (random == BackGroundMusicPlaying)
			{
				random = rand() % NUM_OF_SOUNDTRACK;
			}
			BackgroundMusicChannel = Mix_PlayChannel(-1, BackgroundSoundtracks[random], 1);
			BackGroundMusicPlaying = random;
		}
	}
	else if (!Mix_Playing(BackgroundMusicChannel)) BackgroundMusicIsPlaying = false;
}

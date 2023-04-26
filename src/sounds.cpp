#include "sounds.h"
#include "gameobjects.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#define NUM_OF_BACKGROUND_MUSIC 4
#define NUM_OF_SOUNDTRACK 15
Mix_Music* BackgroundMusic[NUM_OF_BACKGROUND_MUSIC];
Mix_Chunk* SoundEffects[19];
Mix_Chunk* BackgroundSoundtracks[NUM_OF_SOUNDTRACK];
int CurrentMusic = -1;
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
	SoundEffects[12] = Mix_LoadWAV("resources/sounds/effects/SwordHitShield.mp3");
	SoundEffects[13] = Mix_LoadWAV("resources/sounds/effects/SwordHit.mp3");
	SoundEffects[14] = Mix_LoadWAV("resources/sounds/effects/LaserBeam.mp3");
	SoundEffects[15] = Mix_LoadWAV("resources/sounds/effects/LaserCharging.mp3");
	SoundEffects[16] = Mix_LoadWAV("resources/sounds/effects/SwordDraw.mp3");
	SoundEffects[17] = Mix_LoadWAV("resources/sounds/effects/GunCocking.mp3");
	SoundEffects[18] = Mix_LoadWAV("resources/sounds/effects/ArrowShower.mp3");
	for (int i = 0; i < NUM_OF_SOUNDTRACK; i++) BackgroundSoundtracks[i] = Mix_LoadWAV(("resources/sounds/backgroundsoundtracks/Soundtrack" + std::to_string(i) + ".mp3").c_str());
	Mix_AllocateChannels(48);
	Mix_ReserveChannels(2);
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
	if (!gamestate.pause && gamestate.start && !Mix_Playing(0) &&!player1.isDead)
	{
		if (rand() % 10000 <= 4)
		{
			int random = BackGroundMusicPlaying;
			while (random == BackGroundMusicPlaying)
			{
				random = rand() % NUM_OF_SOUNDTRACK;
			}
			Mix_PlayChannel(0, BackgroundSoundtracks[random], 1);
			BackGroundMusicPlaying = random;
		}
	}
}

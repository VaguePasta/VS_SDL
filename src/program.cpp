#include "program.h"
#include "sounds.h"
#include "player.h"
#include "enemies.h"
#include "elementals.h"
#include "weapons.h"
#include "global.h"
#include "gameobjects.h"
#include "gamelogic.h"
#include "inputprocessing.h"
#include "rendering.h"
#include "menu.h"
#include "background.h"
#include <cstdlib>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
void init()
{
	InitSDL();
	InitWindow();
	LoadSprites();
	LoadSounds();
	InitObjects();
}
void InitSDL()
{
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitor");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	IMG_Init(IMG_INIT_PNG);
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
}
void InitWindow()
{
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LOGICAL_WIDTH, LOGICAL_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_GetRendererOutputSize(renderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
}
void LoadSprites()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	LoadBackgroundSpritesFromDisk();
	LoadPlayerSpritesFromDisk();
	LoadWeaponSpritesFromDisk();
	LoadBulletSpritesFromDisk();
	LoadEnemiesSpritesFromDisk();
	LoadElementalSpritesFromDisk();
	LoadEnemiesProjectileSpritesFromDisk();
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	LoadFont();
	LoadButtons();
}
void InitObjects()
{
	srand(time(NULL));
	screentexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, LEVEL_WIDTH, LEVEL_HEIGHT);
	CamTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, LOGICAL_WIDTH, LOGICAL_HEIGHT);
	SDL_SetTextureBlendMode(screentexture, SDL_BLENDMODE_BLEND);
	Drawbackground();
	for (int i = 0; i < Current_max_enemies; i++)
	{
		enemy.push_back(new Enemies());
		Projectiles.push_back(new EnemyProjectiles());
	}
	for (int i = 0; i < Current_max_elementals; i++)
	{
		elemental.push_back(new Elementals());
	}
	HealthBar.Init("resources/UI/bars/HealthBarBorder.PNG", "resources/UI/bars/HealthBar.PNG", 100);
	ShieldHealthBar.Init("resources/UI/bars/ShieldHealthBarBorder.PNG", "resources/UI/bars/ShieldHealthBar.PNG", 500);
	StaminaBar.Init("resources/UI/bars/StaminaBarBorder.PNG", "resources/UI/bars/StaminaBar.PNG", 100);
	MusicVolumeBar.Init("resources/UI/bars/VolumeBarBorder.PNG", "resources/UI/bars/VolumeBar.PNG", 100);
	SoundEffectVolumeBar.Init("resources/UI/bars/VolumeBarBorder.PNG", "resources/UI/bars/VolumeBar.PNG", 100);
	DashIcon.Init("resources/UI/icons/DashIcon.PNG");
	player1.PlayerShield.ShieldIcon.Init("resources/UI/icons/ShieldIcon.PNG");
	Cursor[0] = IMG_LoadTexture(renderer, "resources/UI/crosshairs/CrossHair1.PNG");
	GameButtons[0].Init(0);
	GameButtons[1].Init(1);
	GameButtons[2].Init(2);
	GameButtons[3].Init(3);
	GameButtons[4].Init(4);
	GameButtons[5].Init(5);
	GameButtons[6].Init(6);
	GameButtons[7].Init(7);
	GameButtons[8].Init(8);
	player1.Init(0, 0);
	SlashEffect.texture = EffectSprites[1];
	SDL_QueryTexture(SlashEffect.texture, NULL, NULL, &SlashEffect.texturesize.x, &SlashEffect.texturesize.y);
	SlashEffect.framecalc();
	SlashEffect.SpriteSize = { 300,300 };
}
void GameProcessing()
{
	if (player1.Health <= 0 && !player1.isDead) player1.Death();
	GameLogic();
	if (!player1.isDead) inputprocessing();
	combinetexture();
}
void GameLoop()
{
	GetMouseClick();
	PlayMusic();
	SDL_RenderClear(renderer);
	if (gamestate.settings)
	{
		Settings();
		rendergame();
		return;
	}
	if (!gamestate.start)
	{
		StartMenu();
		rendergame();
		return;
	}
	Pause();
	if (gamestate.pause)
    {
        PauseMenu();
        rendergame();
        return;
    }
    if (player1.isDead && player1.CurrentSprite >= player1.NumOfSprites - 1)
    {
        GameOver();
        rendergame();
        return;
    }
    if (!gamestate.game_is_over)
    {
        GameProcessing();
        rendergame();
        LoopTime = SDL_GetPerformanceCounter() * freq - GetTime;
        GetTime = SDL_GetPerformanceCounter() * freq;
        DeltaTime = 1.0 * (LoopTime) / 16666666;
    }
}
void Pause()
{
	static Timer PauseDelay(1);
	if (!PauseDelay.Started && !gamestate.pause) PauseDelay.Start();
	if (Keyboard[SDL_SCANCODE_ESCAPE] && !gamestate.pause && !player1.isDead && PauseDelay.GetTime() >= 500)
	{
		gamestate.pause = true;
		MouseLeftDown = false;
		MouseRightDown = false;
		if (BackgroundMusicIsPlaying) Mix_HaltChannel(BackgroundMusicChannel);
		Mix_ResumeMusic();
		SDL_ShowCursor(SDL_ENABLE);
		SDL_SetTextureColorMod(CamTexture, 192, 192, 192);
		player1.DashCooldown.Pause();
		player1.PlayerShield.Time.Pause();
		player1.PlayerWeapon.ShootingDelay.Pause();
		for (int i = 0; i < Current_max_enemies; i++)
			if (enemy[i]->isSpawn)
			{
				enemy[i]->Cooldown.Pause();
				enemy[i]->MovingCounter.Pause();
			}
		for (int i = 0; i < Current_max_elementals; i++)
			if (elemental[i]->isSpawn)
			{
				elemental[i]->Cooldown.Pause();
			}
		FPSCounter.Pause();
		PauseDelay.Reset();
	}
}

#include "menu.h"
#include "gameobjects.h"
#include "sounds.h"
#include "texturerendering.h"
#include "background.h"
#include "rendering.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <SDL_ttf.h>
#include "UI.h"
void StartMenu()
{
	if (!gamestate.start)
	{
		SDL_ShowCursor(SDL_ENABLE);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		GameButtons[0].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[0].Size.x / 2 / 2,500 }, 2);
		GameButtons[3].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[1].Size.x / 2 / 2,650 }, 2);
		GameButtons[4].DrawButton({ 1480,780 }, 2);
		GameButtons[5].DrawButton({ 30,780 }, 2);
		if (MouseDown)
		{
			if (SDL_PointInRect(&MousePosition, &GameButtons[0].ButtonRect) && (MouseState & SDL_BUTTON(1)))
			{
				Start_Button();
			}
			else if (SDL_PointInRect(&MousePosition, &GameButtons[3].ButtonRect) && (MouseState & SDL_BUTTON(1)))
			{
				gamestate.quit = true;
			}
			else if (SDL_PointInRect(&MousePosition, &GameButtons[4].ButtonRect) && (MouseState & SDL_BUTTON(1)))
			{
				Mix_HaltMusic();
			}
			else if (SDL_PointInRect(&MousePosition, &GameButtons[5].ButtonRect) && (MouseState & SDL_BUTTON(1)))
			{
				gamestate.settings = true;
			}
		}
	}
}
void PauseMenu()
{
	if (Keyboard[SDL_SCANCODE_ESCAPE] && !gamestate.pause && !player1.isDead)
	{
		gamestate.pause = true;
		MouseDown = false;
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
		FPSCounter.Pause();
	}
	if (gamestate.pause)
	{
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, CamTexture, NULL, NULL);
		GameButtons[1].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[0].Size.x / 2 / 2,250 }, 2);
		GameButtons[2].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[0].Size.x / 2 / 2,400 }, 2);
		GameButtons[3].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[1].Size.x / 2 / 2,550 }, 2);
		GameButtons[4].DrawButton({ 1480,780 }, 2);
		GameButtons[5].DrawButton({ 30,780 }, 2);
		if (MouseDown)
		{
			if (SDL_PointInRect(&MousePosition, &GameButtons[1].ButtonRect))
			{
				Back_Button();
			}
			else if (SDL_PointInRect(&MousePosition, &GameButtons[2].ButtonRect))
			{
				Menu_Button();
			}
			else if (SDL_PointInRect(&MousePosition, &GameButtons[3].ButtonRect))
			{
				gamestate.quit = true;
			}
			else if (SDL_PointInRect(&MousePosition, &GameButtons[4].ButtonRect))
			{
				Mix_HaltMusic();
			}
			else if (SDL_PointInRect(&MousePosition, &GameButtons[5].ButtonRect))
			{
				gamestate.settings = true;
			}
		}
	}
}
void GameOver()
{
	if (BackgroundMusicIsPlaying)
	{
		Mix_HaltChannel(BackgroundMusicChannel);
		BackgroundMusicIsPlaying = false;
	}
	static SDL_Rect ScoreRect[3];
	static SDL_Texture* ScoreAnnounce[3] = { NULL,NULL,NULL };
	if (ScoreAnnounce[0] == NULL)
	{
		SDL_Surface* TempSurface = TTF_RenderText_Solid(GlobalFont, "You died.", { 178,34,34 });
		ScoreAnnounce[0] = SDL_CreateTextureFromSurface(renderer, TempSurface);
		ScoreRect[0] = { LOGICAL_WIDTH / 2 - 2 * (TempSurface->w),100,TempSurface->w * 4,TempSurface->h * 4 };
		SDL_FreeSurface(TempSurface);
	}
	if (!gamestate.game_is_over)
	{
		gamestate.game_is_over = true;
		SDL_SetRenderTarget(renderer, NULL);
		SDL_ShowCursor(SDL_ENABLE);
		SDL_Surface* TempSurface = TTF_RenderText_Solid(GlobalFont, std::to_string(SCORE).c_str(), { 178,34,34 });
		ScoreAnnounce[1] = SDL_CreateTextureFromSurface(renderer, TempSurface);
		ScoreRect[1] = { LOGICAL_WIDTH / 2 - 2 * (TempSurface->w),200,TempSurface->w * 4,TempSurface->h * 4 };
		SDL_FreeSurface(TempSurface);
		std::string HighscoreString;
		if (SCORE > HIGH_SCORE)
		{
			HighscoreString = "New high score!";
			HIGH_SCORE = SCORE;
		}
		else HighscoreString = "Highest score: " + std::to_string(HIGH_SCORE);
		TempSurface = TTF_RenderText_Solid(GlobalFont, HighscoreString.c_str(), { 178,34,34 });
		ScoreAnnounce[2] = SDL_CreateTextureFromSurface(renderer, TempSurface);
		ScoreRect[2] = { LOGICAL_WIDTH / 2 - 2 * (TempSurface->w),300,TempSurface->w * 4,TempSurface->h * 4 };
		SDL_FreeSurface(TempSurface);
	}
	DrawScore(ScoreAnnounce, ScoreRect);
	GameButtons[2].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[0].Size.x / 2 / 2,400 }, 2);
	GameButtons[3].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[1].Size.x / 2 / 2,550 }, 2);
	GameButtons[5].DrawButton({ 30,780 }, 2);
	if (MouseDown)
	{
		if (SDL_PointInRect(&MousePosition, &GameButtons[2].ButtonRect))
		{
			Menu_Button();
			SDL_DestroyTexture(ScoreAnnounce[1]);
			SDL_DestroyTexture(ScoreAnnounce[2]);
		}
		else if (SDL_PointInRect(&MousePosition, &GameButtons[3].ButtonRect))
		{
			gamestate.quit = true;
		}
		else if (SDL_PointInRect(&MousePosition, &GameButtons[5].ButtonRect))
		{
			gamestate.settings = true;
		}
	}

}
void Settings()
{
	static std::string contents[] = { "Music","Sound effects","Resolution","Fullscreen","1280x720","1366x768","1600x900","1920x1080" };
	static SDL_Texture* StringContent[] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	static Buttons SettingButtons[5];
	if (StringContent[0] == NULL)
	{
		for (int i = 0; i < 8; i++)
		{
			SDL_Surface* TempSurface = TTF_RenderText_Solid(GlobalFont, contents[i].c_str(), { 255,255,255 });
			StringContent[i] = SDL_CreateTextureFromSurface(renderer, TempSurface);
			SDL_FreeSurface(TempSurface);
		}
		for (int i = 0; i < 5; i++) SettingButtons[i] = GameButtons[7];
	}
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderClear(renderer);
	GameButtons[6].DrawButton({ 30,30 }, 2);
	MusicVolumeBar.DrawBar({ 30,200 });
	SoundEffectVolumeBar.DrawBar({ 30,250 });
	DrawLetter(StringContent[0], { 320,200 });
	DrawLetter(StringContent[1], { 320,250 });
	DrawLetter(StringContent[2], { 80,350 });
	DrawLetter(StringContent[3], { 30,300 }); SettingButtons[0].DrawButton({ 150,295 }, 6); //Fullscreen
	DrawLetter(StringContent[4], { 30,390 }); SettingButtons[1].DrawButton({ 150,385 }, 6); //1280x720
	DrawLetter(StringContent[5], { 30,440 }); SettingButtons[2].DrawButton({ 150,435 }, 6); //1366x768
	DrawLetter(StringContent[6], { 30,490 }); SettingButtons[3].DrawButton({ 150,485 }, 6); //1600x900
	DrawLetter(StringContent[7], { 30,540 }); SettingButtons[4].DrawButton({ 150,535 }, 6); //1920x1080
	if (MouseDown)
	{
		if (SDL_PointInRect(&MousePosition, &GameButtons[6].ButtonRect)) gamestate.settings = false;
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[0].ButtonRect) && !isFullScreen)
		{
			isFullScreen = true;
			MouseDown = false;
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
			SDL_DestroyTexture(GameBackground);
			Drawbackground();
			combinetexture();
			WrapCamera();
		}
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[1].ButtonRect) && SCREEN_WIDTH != 1280)
		{
			ChangeResolution(1280, 720);
		}
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[2].ButtonRect) && SCREEN_WIDTH != 1366)
		{
			ChangeResolution(1366, 768);
		}
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[3].ButtonRect) && SCREEN_WIDTH != 1600)
		{
			ChangeResolution(1600, 900);
		}
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[4].ButtonRect) && SCREEN_WIDTH != 1920)
		{
			ChangeResolution(1920, 1080);
		}
	}
	if (SDL_PointInRect(&MousePosition, &MusicVolumeBar.BarRect) && MouseState & SDL_BUTTON(1))
	{
		ChangeVolume(MusicVolumeBar);
		Mix_VolumeMusic(MusicVolumeBar.Value * 1.0 / 100 * MIX_MAX_VOLUME);
	}
	else if (SDL_PointInRect(&MousePosition, &SoundEffectVolumeBar.BarRect) && MouseState & SDL_BUTTON(1))
	{
		ChangeVolume(SoundEffectVolumeBar);
		Mix_MasterVolume(SoundEffectVolumeBar.Value * 1.0 / 100 * MIX_MAX_VOLUME);
	}
}
void Start_Button()
{
	gamestate.start = true;
	Mix_PauseMusic();
	SDL_ShowCursor(SDL_DISABLE);
	UpdateCurrentScore();
	FPSCounter.Restart();
}
void Menu_Button()
{
	gamestate.pause = false;
	gamestate.game_is_over = false;
	Mix_ResumeMusic();
	SCORE = 0;
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetTextureColorMod(CamTexture, 255, 255, 255);
	ClearTexture(screentexture);
	ClearTexture(CamTexture);
	gamestate.start = false;
	FPSTexture = nullptr;
	for (int i = 0; i < Max_Bullets; i++)
	{
		delete player1.PlayerWeapon.bullets[i];
		player1.PlayerWeapon.bullets[i] = nullptr;
	}
	for (int i = 0; i < Current_max_enemies; i++)
	{
		delete enemy[i];
		enemy[i] = new Enemies();
		delete Projectiles[i];
		Projectiles[i] = new EnemyProjectiles();
	}
	SDL_DestroyTexture(GameBackground);
	Drawbackground();
	player1.Init(0, 0);
}
void Back_Button()
{
	gamestate.pause = false;
	Mix_PauseMusic();
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetTextureColorMod(CamTexture, 255, 255, 255);
	player1.DashCooldown.Unpause();
	player1.PlayerShield.Time.Unpause();
	player1.PlayerWeapon.ShootingDelay.Unpause();
	for (int i = 0; i < Current_max_enemies; i++)
		if (enemy[i]->isSpawn)
		{
			enemy[i]->Cooldown.Unpause();
			enemy[i]->MovingCounter.Unpause();
		}
	FPSCounter.Unpause();
}
void ChangeResolution(int _Width, int _Height)
{
	SCREEN_WIDTH = _Width;
	SCREEN_HEIGHT = _Height;
	SDL_SetWindowFullscreen(window, 0);
	SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_DestroyTexture(GameBackground);
	Drawbackground(); combinetexture();
	WrapCamera();
	isFullScreen = false;
	MouseDown = false;
}
void ChangeVolume(Bars& VolumeBar)
{
	VolumeBar.Value = ((MousePosition.x - VolumeBar.BarRect.x + 1) * 1.0 / VolumeBar.BarRect.w) * 100;
	if (VolumeBar.Value < 0) VolumeBar.Value = 0;
	else if (VolumeBar.Value > VolumeBar.MaxValue) VolumeBar.Value = VolumeBar.MaxValue;
}

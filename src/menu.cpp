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
	if (gamestate.start) return;
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderClear(renderer);
	GameButtons[0].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[0].Size.x / 2 / 2,500 }, 2);
	GameButtons[3].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[1].Size.x / 2 / 2,650 }, 2);
	GameButtons[4].DrawButton({ 1480,780 }, 2);
	GameButtons[5].DrawButton({ 30,780 }, 2);
	if (MouseLeftDown)
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
void PauseMenu()
{
    static Timer PauseDelay(1);
	static bool HideHUD = false;
    if (!PauseDelay.Started) PauseDelay.Start();
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, CamTexture, NULL, NULL);
	if (!HideHUD)
	{
		GameButtons[1].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[0].Size.x / 2 / 2,250 }, 2);
		GameButtons[2].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[0].Size.x / 2 / 2,400 }, 2);
		GameButtons[3].DrawButton({ LOGICAL_WIDTH / 2 - GameButtons[1].Size.x / 2 / 2,550 }, 2);
		GameButtons[4].DrawButton({ 1480,780 }, 2);
		GameButtons[5].DrawButton({ 30,780 }, 2);
	}
	if (Keyboard[SDL_SCANCODE_ESCAPE] && PauseDelay.GetTime() >= 500)
    {
        Back_Button();
		HideHUD = false;
        PauseDelay.Reset();
        return;
    }
	if (MouseRightDown)
	{
		HideHUD = !HideHUD;
		if (HideHUD) SDL_SetTextureColorMod(CamTexture, 255, 255, 255);
		else SDL_SetTextureColorMod(CamTexture, 192, 192, 192);
	}
    if (MouseLeftDown)
    {
		if (HideHUD) return;
        if (SDL_PointInRect(&MousePosition, &GameButtons[1].ButtonRect))
        {
            Back_Button();
			HideHUD = false;
            PauseDelay.Reset();
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
	if (MouseLeftDown)
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
	static bool VSync = false;
	static std::string contents[] = { "Music","Sound effects","Resolution","Fullscreen","1280x720","1366x768","1600x900","1920x1080","VSync" };
	static SDL_Texture* StringContent[] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	static Buttons SettingButtons[6];
	if (StringContent[0] == NULL)
	{
		for (int i = 0; i < 9; i++)
		{
			SDL_Surface* TempSurface = TTF_RenderText_Solid(GlobalFont, contents[i].c_str(), { 255,255,255 });
			StringContent[i] = SDL_CreateTextureFromSurface(renderer, TempSurface);
			SDL_FreeSurface(TempSurface);
		}
		SettingButtons[0] = GameButtons[7]; SDL_SetTextureColorMod(StringContent[3], 255, 255, 255);
		for (int i = 1; i < 5; i++)
		{
			SettingButtons[i] = GameButtons[8];
			SDL_SetTextureColorMod(StringContent[i+3], 192, 192, 192);
		}
		SettingButtons[5] = GameButtons[8];
		SDL_SetTextureColorMod(StringContent[8], 192, 192, 192);
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
	DrawLetter(StringContent[8], { 30,590 }); SettingButtons[5].DrawButton({ 150,585 }, 6); //VSync
	if (MouseLeftDown)
	{
		if (SDL_PointInRect(&MousePosition, &GameButtons[6].ButtonRect)) gamestate.settings = false;
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[0].ButtonRect) && !isFullScreen)
		{
			ChangeButton(SettingButtons, StringContent, 0, 8, 7, 5);
			isFullScreen = true;
			MouseLeftDown = false;
			MouseRightDown = false;
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
			ChangeButton(SettingButtons, StringContent, 1, 8, 7, 5);
		}
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[2].ButtonRect) && SCREEN_WIDTH != 1366)
		{
			ChangeResolution(1366, 768);
			ChangeButton(SettingButtons, StringContent, 2, 8, 7, 5);
		}
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[3].ButtonRect) && SCREEN_WIDTH != 1600)
		{
			ChangeResolution(1600, 900);
			ChangeButton(SettingButtons, StringContent, 3, 8, 7, 5);
		}
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[4].ButtonRect) && SCREEN_WIDTH != 1920)
		{
			ChangeResolution(1920, 1080);
			ChangeButton(SettingButtons, StringContent, 4, 8, 7, 5);
		}
		else if (SDL_PointInRect(&MousePosition, &SettingButtons[5].ButtonRect))
		{
			VSync = !VSync;
			SettingButtons[5] = GameButtons[VSync ? 7 : 8];
			SDL_SetTextureColorMod(StringContent[8], VSync ? 255 : 192, VSync ? 255 : 192, VSync ? 255 : 192);
			SDL_RenderSetVSync(renderer, VSync);
			MouseLeftDown = false;
			MouseRightDown = false;
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
	GetTime = SDL_GetPerformanceCounter() * freq;
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
	for (int i = 0; i < Current_max_minions; i++)
	{
		delete minion[i];
		minion[i] = new Minions();
		delete Projectiles[i];
		Projectiles[i] = new MinionProjectiles();
	}
	for (int i = 0; i < Current_max_elementals; i++)
	{
		delete elemental[i];
		elemental[i] = new Elementals();
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
	for (int i = 0; i < Current_max_minions; i++)
		if (minion[i]->isSpawn)
		{
			minion[i]->Cooldown.Unpause();
			minion[i]->MovingCounter.Unpause();
		}
	for (int i = 0; i < Current_max_elementals; i++)
	{
		if (elemental[i]->isSpawn)
		{
			elemental[i]->Cooldown.Unpause();
		}
	}
	FPSCounter.Unpause();
	GetTime = SDL_GetPerformanceCounter() *freq;
}
void ChangeResolution(int _Width, int _Height)
{
	SCREEN_WIDTH = _Width;
	SCREEN_HEIGHT = _Height;
	SDL_SetWindowFullscreen(window, 0);
	SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
	isFullScreen = false;
	MouseLeftDown = false;
	MouseRightDown = false;
}
void ChangeVolume(Bars& VolumeBar)
{
	VolumeBar.Value = ((MousePosition.x - VolumeBar.BarRect.x + 1) * 1.0 / VolumeBar.BarRect.w) * 100;
	if (VolumeBar.Value < 0) VolumeBar.Value = 0;
	else if (VolumeBar.Value > VolumeBar.MaxValue) VolumeBar.Value = VolumeBar.MaxValue;
}
void ChangeButton(Buttons SettingButton[],SDL_Texture *StringContent[],int Num_Change, int Change_from, int Change_to, int Num_of_buttons)
{
	for (int i = 0; i < Num_of_buttons; i++)
	{
		if (i == Num_Change)
		{
			SettingButton[i] = GameButtons[Change_to];
			SDL_SetTextureColorMod(StringContent[i + 3], 255, 255, 255);
		}
		else
		{
			SettingButton[i] = GameButtons[Change_from];
			SDL_SetTextureColorMod(StringContent[i + 3], 192, 192, 192);
		}
	}
}
#include "rendering.h"
#include "effects.h"
#include "gameobjects.h"
#include "texturerendering.h"
#include <cmath>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
void combinetexture()
{
	camera[0].CameraCalculation();
	SDL_SetRenderTarget(renderer, screentexture);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, GameBackground, nullptr, nullptr);
	DrawPlayer();
	DrawMinions();
	DrawEffects();
	SDL_SetRenderTarget(renderer, nullptr);
}
void rendergame()
{
	if (!gamestate.pause && gamestate.start && !gamestate.game_is_over && !gamestate.settings)
	{
		WrapCamera();
		SDL_RenderCopy(renderer, CamTexture, nullptr, nullptr);
		if (!player1.isDead) DrawUI();
	}
	if (SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE) DrawCursor();
	SDL_RenderPresent(renderer);
}
void DrawPlayer()
{
	if (!player1.Dashing)
	{
		player1.draw();
		player1.PlayerWeapon.flip = player1.flip;
		if (!player1.isDead)
		{
			player1.PlayerWeapon.draw();
		}
		if (player1.PlayerShield.isOn) player1.PlayerShield.draw();
	}
}
void DrawEffects()
{
	for (int i = 0; i < Max_Bullets; i++)
		if (player1.PlayerWeapon.bullets[i]->isShot && player1.PlayerWeapon.bullets[i]->DistanceTraveled > 30)
		{
			player1.PlayerWeapon.bullets[i]->draw();
		}
	if (player1.MeleeAttacking && !player1.MeleeAttacked)
	{
		SlashEffectDraw();
	}
}
void DrawMinions()
{
	for (int i = 0; i < Current_max_minions; i++)
	{
		if (minion[i]->isSpawn)
		{
			minion[i]->draw();
		}
	}
	for (int i = 0; i < Current_max_elementals; i++)
	{
		if (elemental[i]->isSpawn)
		{
			elemental[i]->draw();
		}
	}
	for (int i = 0; i < Current_max_enemies; i++)
	{
		if (Projectiles[i]->isShot && Projectiles[i]->Arrived)
		{
			Projectiles[i]->draw();
		}
	}
}
void DrawUI()
{
	HealthBar.DrawBar({ 40,50 });
	StaminaBar.DrawBar({ 40,85 });
	if (player1.PlayerShield.isOn) ShieldHealthBar.DrawBar({ 40,120 });
	player1.PlayerShield.ShieldIcon.DrawIcon({ 40,800 }, player1.PlayerShield.isOn);
	DashIcon.DrawIcon({ 100,800 }, player1.Dashing);
	DrawCurrentScore();
	DrawFPS();
}
void DrawCursor()
{
	SDL_Rect MouseRect = { MousePosition.x - 30,MousePosition.y - 30,60,60 };
	SDL_RenderCopy(renderer, Cursor[0], nullptr, &MouseRect);
}

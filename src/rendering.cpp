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
	SDL_RenderCopy(renderer, GameBackground, NULL, NULL);
	DrawEnemies();
	DrawPlayer();
	DrawEffects();
	SDL_SetRenderTarget(renderer, NULL);
}
void rendergame()
{
	if (!gamestate.pause && gamestate.start && !gamestate.game_is_over && !gamestate.settings)
	{
		WrapCamera();
		SDL_RenderCopy(renderer, CamTexture, NULL, NULL);
		if (!player1.isDead) DrawUI();
	}
	if (SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE) DrawCursor();
	SDL_RenderPresent(renderer);
}
void DrawPlayer()
{
	player1.animation();
	if (!player1.Dashing)
	{
		SDL_FRect WeaponRect = {player1.position.x + player1.PlayerWeapon.Position.x,player1.position.y + player1.PlayerWeapon.Position.y,1.0*player1.PlayerWeapon.WeaponSize / 12,1.0*player1.PlayerWeapon.WeaponSize / 12 };
		PlayerDrawEdge(WeaponRect);
		PlayerDraw(player1.SpriteBox, WeaponRect, player1.PlayerShield.ShieldHitBox);
	}
	else TrailDrawing();
}
void DrawEffects()
{
	for (int i = 0; i < Max_Bullets; i++)
		if (player1.PlayerWeapon.bullets[i]->isShot && player1.PlayerWeapon.bullets[i]->DistanceTraveled > 30)
		{
			if (!player1.PlayerWeapon.bullets[i]->isAnimated)
			{
				SDL_RenderCopyExF(renderer, player1.PlayerWeapon.bullets[i]->bullettexture, NULL, &(player1.PlayerWeapon.bullets[i]->BulletSprite), player1.PlayerWeapon.bullets[i]->angle, NULL, SDL_FLIP_NONE);
				BulletDrawEdge(nullptr, i);
			}
			else
			{
				player1.PlayerWeapon.bullets[i]->BulletAnimation.animation();
				SDL_RenderCopyExF(renderer, player1.PlayerWeapon.bullets[i]->bullettexture, &player1.PlayerWeapon.bullets[i]->BulletAnimation.frame, &player1.PlayerWeapon.bullets[i]->BulletSprite, player1.PlayerWeapon.bullets[i]->angle, NULL, SDL_FLIP_NONE);
				BulletDrawEdge(&player1.PlayerWeapon.bullets[i]->BulletAnimation.frame, i);
			}
		}
	if (player1.MeleeAttacking && !player1.MeleeAttacked)
	{
		SlashEffectDraw();
	}
}
void DrawEnemies()
{
	for (int i = 0; i < Current_max_enemies; i++)
	{
		if (enemy[i]->isSpawn)
		{
			enemy[i]->animation();
			SDL_RenderCopyExF(renderer, enemy[i]->texture, &enemy[i]->frame, &enemy[i]->SpriteBox, 0, NULL, enemy[i]->flip);
			EnemyDrawEdge(i);
		}
		if (Projectiles[i]->isShot)
		{
			if (!Projectiles[i]->isAnimated)
			{
				DrawEnemyProjectiles(nullptr, i);
			}
			else
			{
				DrawEnemyProjectiles(&Projectiles[i]->Projectile.frame, i);
			}
		}
	}
	for (int i = 0; i < Current_max_elementals; i++)
	{
		if (elemental[i]->isSpawn)
		{
			elemental[i]->animation();
			SDL_RenderCopyExF(renderer, elemental[i]->texture, &elemental[i]->frame, &elemental[i]->SpriteBox, 0, NULL, elemental[i]->flip);
			ElementalDrawEdge(i);
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
	SDL_RenderCopy(renderer, Cursor[0], NULL, &MouseRect);
}

#include "skills.h"
#include "global.h"
#include "enemies.h"
#include "gameobjects.h"
#include "sounds.h"
#include "texturerendering.h"
#include "UI.h"
#include "bullets.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
Shield::Shield()
{
	ShieldTexture = NULL;
	Size.x = 100;
	Size.y = 100;
	Position.x = 0;
	Position.y = 0;
	Health = 500;
	isOn = false;
	Damaged = false;
	DamageDelay = 0;
	Cooldown = 2000;
	LiveTime = 2000;
	Time.Restart();
}
Shield::~Shield() {};
void Shield::ActivateShield(SDL_FPoint PlayerPosition)
{
	if (ShieldTexture == NULL) ShieldTexture = IMG_LoadTexture(renderer, "resources/skills/Shield.PNG");
	if (!ShieldIcon.isOnCooldown)
	{
		isOn = true;
		Health = 500;
		Position.x = round(PlayerPosition.x);
		Position.y = round(PlayerPosition.y);
		ShieldHitBox = { (int)round(PlayerPosition.x),(int)round(PlayerPosition.y),Size.x,Size.y };
		isOn = true;
		Time.Restart();
	}
}
void Shield::DecayShield()
{
	ShieldHitBox = { (int)round(player1.position.x),(int)round(player1.position.y),Size.x,Size.y };
	if (!Damaged && DamageDelay >= 10)
	{
		SDL_SetTextureColorMod(ShieldTexture, 255, 255, 255);
		DamageDelay = 0;
	}
	if (Damaged) Damaged = false;
	DamageDelay++;
	if (isOn && (Time.GetTime() >= LiveTime || Health <= 0))
	{
		Position.x = 0;
		Position.y = 0;
		ShieldHitBox = { 0,0,0,0 };
		isOn = false;
		ShieldIcon.isOnCooldown = true;
		Time.Restart();
	}
}
void Shield::ShieldDamage(int damage)
{
	Damaged = true;
	DamageDelay = 0;
	Health -= damage;
	SDL_SetTextureColorMod(ShieldTexture, 255, 0, 0);
}
void PlayerDash(SDL_FPoint& TempPos)
{
	if (player1.Dashing)
	{
		if (player1.DashCooldown.GetTime() >= 300)
		{
			player1.Dashing = false;
			DashIcon.isOnCooldown = true;
			player1.DashCooldown.Restart();
			return;
		}
		else
		{
			TempPos.x += 10 * player1.DashDirection.x * DeltaTime;
			TempPos.y += 10 * player1.DashDirection.y * DeltaTime;
		}
	}
	else
	{
		if (Keyboard[SDL_SCANCODE_SPACE] && DashIcon.isOnCooldown == false)
		{
			if (Keyboard[SDL_SCANCODE_W] - Keyboard[SDL_SCANCODE_S] != 0 || Keyboard[SDL_SCANCODE_A] - Keyboard[SDL_SCANCODE_D] != 0)
			{
				Mix_PlayChannel(-1, SoundEffects[7], 0);
				player1.Dashing = true;
				player1.DashPosition = player1.position;
				player1.DashDirection = player1.Direction;
				player1.DashCooldown.Restart();
			}
		}
	}
}
void TrailDrawing()
{
	player1.Hitbox = { 0,0,0,0 };
}
void Sprint()
{
	int KeyHold = SDL_GetModState();
	KeyHold = KeyHold & KMOD_LSHIFT;
	player1.isSprinting = false;
	static bool Holding = false;
	if (KeyHold == KMOD_LSHIFT && !player1.isAttacking)
	{
		if (player1.Stamina > 0 && Holding)
		{
			player1.isSprinting = true;
			player1.speed = 7;
			player1.Stamina -= 1.2 * DeltaTime;
		}
		else Holding = false;
	}
	else if (!Holding) Holding = true;
	if (!player1.isSprinting)
	{
		if (player1.Stamina < 100) player1.Stamina += 0.4 * DeltaTime;
	}
}
void GunShoot(bool& Recoil, Timer& RecoilTimer, float& ShootAngle)
{
	SDL_FPoint BulletOrigin;
	if (player1.PlayerWeapon.ShootingDelay.GetTime() < 1000 / player1.PlayerWeapon.shootingspeed) return;
	BulletOrigin = CalculateOrigin();
	int x_weapon = (player1.flip == SDL_FLIP_NONE) ? (19) : (-19);
	int y_weapon = 16;
	float vec_x = (player1.position.x + player1.SpriteSize / 2 + x_weapon) - (MousePosition.x + camera[0].CameraPosition.x);
	float vec_y = (player1.position.y + player1.SpriteSize / 2 + y_weapon) - (MousePosition.y + camera[0].CameraPosition.y);
	ShootAngle = atan2(vec_y, vec_x) * 180.000 / 3.14159265 + 180;
	for (int i = 0; i < Max_Bullets; i++)
	{
		if (!player1.PlayerWeapon.bullets[i]->isShot)
		{
			player1.PlayerWeapon.bullets[i]->isShot = true;
			player1.PlayerWeapon.bullets[i]->origin = BulletOrigin;
			player1.PlayerWeapon.bullets[i]->BulletPositionFloat.x = BulletOrigin.x - player1.PlayerWeapon.bullets[i]->BulletSize / 2;
			player1.PlayerWeapon.bullets[i]->BulletPositionFloat.y = BulletOrigin.y - player1.PlayerWeapon.bullets[i]->BulletSize / 2;
			player1.PlayerWeapon.bullets[i]->BulletPosition.x = (int)player1.PlayerWeapon.bullets[i]->BulletPositionFloat.x;
			player1.PlayerWeapon.bullets[i]->BulletPosition.y = (int)player1.PlayerWeapon.bullets[i]->BulletPositionFloat.y;
			player1.PlayerWeapon.bullets[i]->angle = ShootAngle;
			player1.PlayerWeapon.bullets[i]->BulletSpeed = player1.PlayerWeapon.bulletspeed;
			player1.PlayerWeapon.bullets[i]->BulletRange = player1.PlayerWeapon.range;
			player1.PlayerWeapon.ShootingDelay.Restart();
			if (!Recoil)
			{
				RecoilTimer.Start();
				Recoil = true;
			}
			if (player1.CurrentWeapon == 2) PlayingChannel = Mix_PlayChannel(-1, player1.PlayerWeapon.GunSound, 0);
			else Mix_PlayChannel(-1, player1.PlayerWeapon.GunSound, 0);
			break;
		}
	}
}
void Slash()
{
	player1.MeleeAttacking = true;
	player1.MeleeAttacked = false;
	if (MousePosition.x + camera[0].CameraPosition.x < player1.position.x + player1.SpriteSize / 2) player1.flip = SDL_FLIP_HORIZONTAL; else player1.flip = SDL_FLIP_NONE;
	player1.PlayerWeapon.angle = (atan2(player1.position.y + player1.SpriteSize / 2 - (MousePosition.y + camera[0].CameraPosition.y), player1.position.x + player1.SpriteSize / 2 - (MousePosition.x + camera[0].CameraPosition.x))) * 180.000 / 3.14159265 + 180;
	if (player1.flip == SDL_FLIP_NONE) player1.PlayerWeapon.angle -= 45;
	else player1.PlayerWeapon.angle += 225;
	SlashEffect.SpriteAngle = player1.PlayerWeapon.angle;
	SlashEffect.flip = player1.flip;
	Mix_PlayChannel(-1, player1.PlayerWeapon.GunSound, 0);
}
void SlashRecover()
{
	bool Check = false;
	if (player1.flip == SDL_FLIP_NONE)
	{
		if (player1.PlayerWeapon.angle > 360) player1.PlayerWeapon.angle -= 360;
		if (abs(15 - player1.PlayerWeapon.angle) >= 7)
		{
			if (player1.PlayerWeapon.angle > 270)
			{
				player1.PlayerWeapon.angle += 5 * DeltaTime;
				Check = true;
			}
			else if (player1.PlayerWeapon.angle > 15)
			{
				player1.PlayerWeapon.angle -= 5 * DeltaTime;
				Check = true;
			}
		}
	}
	else
	{
		if (abs(345 - player1.PlayerWeapon.angle) >= 7)
		{
			if (player1.PlayerWeapon.angle < 0) player1.PlayerWeapon.angle += 360;
			if (player1.PlayerWeapon.angle > 180 && player1.PlayerWeapon.angle < 345)
			{
				player1.PlayerWeapon.angle += 5 * DeltaTime;
				Check = true;
			}
			else
			{
				player1.PlayerWeapon.angle -= 5 * DeltaTime;
				Check = true;
			}
		}
	}
	if (!Check)
	{
		player1.MeleeAttacking = false;
		player1.MeleeAttacked = false;
		player1.PlayerWeapon.ShootingDelay.Restart();
		SlashEffect.CurrentSprite = 0;
		SlashEffect.tempframe = 1;
	}
}
void SlashDamage(bool isDamaged[])
{
	for (int i = 0; i < Current_max_enemies; i++)
	{
		if (enemy[i]->isSpawn && !enemy[i]->isDead)
		{
			SDL_FPoint PlayerCenter = player1.position; PlayerCenter.x += 50; PlayerCenter.y += 50;
			SDL_FPoint EnemyCenter = enemy[i]->position; EnemyCenter.x += enemy[i]->SpriteSize / 2; EnemyCenter.y += enemy[i]->SpriteSize / 2;
			float enemyAngle = AngleCalculation(EnemyCenter, PlayerCenter, &player1.flip);
			float enemyDistance = DistanceCalculation(EnemyCenter, PlayerCenter);
			if (abs(enemyAngle - player1.PlayerWeapon.angle) <= 5 && enemyDistance <= player1.PlayerWeapon.range && !isDamaged[i])
			{
				enemy[i]->Health -= player1.PlayerWeapon.damage;
				if (enemy[i]->Health <= 0) enemy[i]->Death();
				else
				{
					enemy[i]->isAttacking = false;
					enemy[i]->Hurt();
				}
				isDamaged[i] = true;
			}
		}
	}
}

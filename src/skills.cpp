#include "skills.h"
#include "global.h"
#include "minions.h"
#include "minionAI.h"
#include "gameobjects.h"
#include "sounds.h"
#include "texturerendering.h"
#include "UI.h"
#include "bullets.h"
#include "elementalAI.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
Shield::Shield()
{
	texture = nullptr;
	SpriteSize = { 100,100 };
	position = { 0,0 };
	Health = 500;
	isOn = false;
	Damaged = false;
	DamageDelay = 0;
	Cooldown = 2000;
	LiveTime = 2000;
	Time.Restart();
	delete frame;
	frame = nullptr;
}
Shield::~Shield() {};
void Shield::ActivateShield(SDL_FPoint PlayerPosition)
{
	if (texture == nullptr) texture = IMG_LoadTexture(renderer, "resources/skills/Shield.PNG");
	if (!ShieldIcon.isOnCooldown)
	{
		isOn = true;
		Health = 500;
		position = PlayerPosition;
		hitbox = { PlayerPosition.x,PlayerPosition.y,SpriteSize.x,SpriteSize.y };
		isOn = true;
		Time.Restart();
	}
}
void Shield::DecayShield()
{
	position = player1.position;
	if (!Damaged && DamageDelay >= 10)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		DamageDelay = 0;
	}
	if (Damaged) Damaged = false;
	DamageDelay++;
	if (isOn && (Time.GetTime() >= LiveTime || Health <= 0))
	{
		position = { 0,0 };
		hitbox = { 0,0,0,0 };
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
	SDL_SetTextureColorMod(texture, 255, 0, 0);
}
void PlayerDash(SDL_FPoint& TempPos)
{
	static SDL_FPoint DashPosition;
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
			player1.isParalysed = false;
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
				player1.isParalysed = false;
				DashPosition = player1.position;
				player1.DashDirection = player1.Direction;
				player1.DashCooldown.Restart();
			}
		}
	}
}
void Sprint()
{
	int KeyHold = SDL_GetModState();
	KeyHold = KeyHold & KMOD_LSHIFT;
	player1.isSprinting = false;
	static bool Holding = false;
	if (KeyHold == KMOD_LSHIFT && !player1.isAttacking )
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
	if (player1.PlayerWeapon.ShootingDelay.GetTime() < 1000 / player1.PlayerWeapon.shootingspeed) return;
	SDL_FPoint BulletOrigin;
	BulletOrigin = CalculateOrigin();
	int x_weapon = (player1.flip == SDL_FLIP_NONE) ? (19) : (-19);
	int y_weapon = 16;
	float vec_x = (player1.position.x + player1.SpriteSize.x / 2 + x_weapon) - (MousePosition.x + camera[0].CameraPosition.x);
	float vec_y = (player1.position.y + player1.SpriteSize.y / 2 + y_weapon) - (MousePosition.y + camera[0].CameraPosition.y);
	ShootAngle = atan2(vec_y, vec_x) * 180.000 / 3.14159265 + 180;
	for (int i = 0; i < Max_Bullets; i++)
	{
		if (!player1.PlayerWeapon.bullets[i]->isShot)
		{
			player1.PlayerWeapon.bullets[i]->isShot = true;
			player1.PlayerWeapon.bullets[i]->origin = BulletOrigin;
			player1.PlayerWeapon.bullets[i]->position.x = BulletOrigin.x - player1.PlayerWeapon.bullets[i]->SpriteSize.x / 2;
			player1.PlayerWeapon.bullets[i]->position.y = BulletOrigin.y - player1.PlayerWeapon.bullets[i]->SpriteSize.y / 2;
			player1.PlayerWeapon.bullets[i]->Damage = player1.PlayerWeapon.damage;
			player1.PlayerWeapon.bullets[i]->angle = ShootAngle;
			player1.PlayerWeapon.bullets[i]->BulletSpeed = player1.PlayerWeapon.bulletspeed;
			player1.PlayerWeapon.bullets[i]->BulletRange = player1.PlayerWeapon.range;
			player1.PlayerWeapon.ShootingDelay.Restart();
			if (!Recoil)
			{
				RecoilTimer.Start();
				Recoil = true;
			}
			if (player1.CurrentWeapon == 2) Mix_PlayChannel(1, player1.PlayerWeapon.GunSound, 0);
			else Mix_PlayChannel(-1, player1.PlayerWeapon.GunSound, 0);
			break;
		}
	}
}
void Slash()
{
	player1.MeleeAttacking = true;
	player1.MeleeAttacked = false;
	if (MousePosition.x + camera[0].CameraPosition.x < player1.position.x + player1.SpriteSize.x / 2) player1.flip = SDL_FLIP_HORIZONTAL; else player1.flip = SDL_FLIP_NONE;
	player1.PlayerWeapon.angle = (atan2(player1.position.y + player1.SpriteSize.y / 2 - (MousePosition.y + camera[0].CameraPosition.y), player1.position.x + player1.SpriteSize.x / 2 - (MousePosition.x + camera[0].CameraPosition.x))) * 180.000 / 3.14159265 + 180;
	if (player1.flip == SDL_FLIP_NONE) player1.PlayerWeapon.angle -= 45;
	else player1.PlayerWeapon.angle += 225;
	SlashEffect.angle = player1.PlayerWeapon.angle;
	SlashEffect.flip = player1.flip;
	Mix_PlayChannel(1, player1.PlayerWeapon.GunSound, 0);
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
void SlashDamage(bool MinionDamaged[],bool ElementalDamaged[])
{
	for (int i = 0; i < Current_max_minions; i++) if (minion[i]->isSpawn && !minion[i]->isDead)
			MinionSlashDamage(minion[i], MinionDamaged[i]);
	for (int i = 0; i < Current_max_elementals; i++) if (elemental[i]->isSpawn && !elemental[i]->isDead)
		ElementalSlashDamage(elemental[i],ElementalDamaged[i]);
}

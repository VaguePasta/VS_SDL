#include <SDL.h>
#include <cmath>
#include "player.h"
#include "global.h"
#include "weapons.h"
#include "bullets.h"
#include "skills.h"
#include "gameobjects.h"
#include "sounds.h"
#include "texturerendering.h"
#include <SDL_mixer.h>
SDL_FPoint CalculateTarget(Camera camera)
{
	SDL_FPoint target;
	target.x = MousePosition.x + camera.CameraPosition.x;
	target.y = MousePosition.y + camera.CameraPosition.y;
	return target;
}
void ChangeWeapon()
{
	static Timer Delay(1);
	if (Delay.GetTime() <= 1000) return;
	if (!player1.isAttacking && !player1.MeleeAttacking)
	{
		if (Keyboard[SDL_SCANCODE_1] != 0 && player1.CurrentWeapon != 0)
		{
			player1.CurrentWeapon = 0;
			player1.PlayerWeapon.LoadWeapon(player1.CurrentWeapon);
			Mix_PlayChannel(-1, SoundEffects[17], 0);
			Delay.Restart();
		}
		else if (Keyboard[SDL_SCANCODE_2] != 0 && player1.CurrentWeapon != 1)
		{
			player1.CurrentWeapon = 1;
			player1.PlayerWeapon.LoadWeapon(player1.CurrentWeapon);
			Mix_PlayChannel(-1, SoundEffects[17], 0);
			Delay.Restart();
		}
		else if (Keyboard[SDL_SCANCODE_3] != 0 && player1.CurrentWeapon != 2)
		{
			player1.CurrentWeapon = 2;
			player1.PlayerWeapon.LoadWeapon(player1.CurrentWeapon);
			Mix_PlayChannel(-1, SoundEffects[17], 0);
			Delay.Restart();
		}
		else if (Keyboard[SDL_SCANCODE_4] != 0 && player1.CurrentWeapon != 3)
		{
			player1.CurrentWeapon = 3;
			player1.PlayerWeapon.LoadWeapon(player1.CurrentWeapon);
			Mix_PlayChannel(-1, SoundEffects[16], 0);
			Delay.Restart();
		}
	}
}
void CalculateWeaponAngle()
{
	player1.isAttacking = false;
	if (MouseState & SDL_BUTTON(1))
		if ((MousePosition.x + camera[0].CameraPosition.x - (player1.position.x + player1.SpriteSize.x / 2)) * (MousePosition.x + camera[0].CameraPosition.x - (player1.position.x + player1.SpriteSize.x / 2)) + (MousePosition.y + camera[0].CameraPosition.y - (player1.position.y + player1.SpriteSize.y / 2)) * (MousePosition.y + camera[0].CameraPosition.y - (player1.position.y + player1.SpriteSize.y / 2)) > 6400)
			player1.isAttacking = true;
	if (!player1.isAttacking)
	{
		if (player1.CurrentWeapon != 3)
		{
			if (player1.flip == SDL_FLIP_NONE) player1.PlayerWeapon.angle = 25;
			else player1.PlayerWeapon.angle = -25;
		}
		else if (player1.CurrentWeapon == 3 && !player1.MeleeAttacking)
		{
			if (player1.flip == SDL_FLIP_NONE) player1.PlayerWeapon.angle = 15;
			else player1.PlayerWeapon.angle = 345;
		}
	}
}
void WeaponPosition()
{
	player1.PlayerWeapon.position = player1.position;
	if (player1.CurrentWeapon == 2)
	{
		player1.PlayerWeapon.position.x += (player1.flip == SDL_FLIP_NONE) ? (15) : (-10);
		player1.PlayerWeapon.position.y += 25;
	}
	else if (player1.CurrentWeapon == 3)
	{
		if (player1.flip == SDL_FLIP_NONE)
		{
			player1.PlayerWeapon.position.x += 35;
			player1.PlayerWeapon.position.y += 23;
			*player1.PlayerWeapon.Center = { 2, 30 };
		}
		else
		{
			player1.PlayerWeapon.position.x += -36;
			player1.PlayerWeapon.position.y += 23;
			*player1.PlayerWeapon.Center = { 98, 30 };
		}
	}
	else
	{
		player1.PlayerWeapon.position.x += (player1.flip == SDL_FLIP_NONE) ? (20) : (-5);
		player1.PlayerWeapon.position.y += 33;
	}
}
void PlayerAttacking()
{
	static bool Recoil = false;
	static Timer RecoilTimer;
	static float ShootAngle;
	switch (player1.CurrentWeapon)
	{
	case 0:case 1:case 2:
		if (!player1.isAttacking) break;
		if (player1.flip == SDL_FLIP_HORIZONTAL)
		{
			player1.PlayerWeapon.angle = (atan2(player1.position.y + player1.SpriteSize.y / 2 - (MousePosition.y + camera[0].CameraPosition.y), player1.position.x + player1.SpriteSize.x/ 2 - (MousePosition.x + camera[0].CameraPosition.x))) * 180.000 / 3.14159265;
			if (player1.PlayerWeapon.angle < -90 || player1.PlayerWeapon.angle>90) player1.flip = SDL_FLIP_NONE;
		}
		if (player1.flip == SDL_FLIP_NONE)
		{
			player1.PlayerWeapon.angle = (atan2(player1.position.y + player1.SpriteSize.y / 2 - (MousePosition.y + camera[0].CameraPosition.y), player1.position.x + player1.SpriteSize.x / 2 - (MousePosition.x + camera[0].CameraPosition.x))) * 180.000 / 3.14159265 - 180;
			if (player1.PlayerWeapon.angle > -270 && player1.PlayerWeapon.angle < -90) player1.flip = SDL_FLIP_HORIZONTAL;
		}
		player1.BulletOrigin = CalculateOrigin();
		GunShoot(Recoil, RecoilTimer, ShootAngle);
		if (Recoil)
		{
			player1.PlayerWeapon.position.x += -5 * cos(ShootAngle * 0.017);
			player1.PlayerWeapon.position.y += -5 * sin(ShootAngle * 0.017);
			if (RecoilTimer.GetTime() >= 30)
			{
				Recoil = false;
				RecoilTimer.Reset();
			}
		}
		break;
	case 3:
		static bool* MinionDamaged = nullptr;
		static bool* ElementalDamaged = nullptr;
		if (player1.isAttacking)
		{
			if (player1.PlayerWeapon.ShootingDelay.GetTime() >= 1000 / player1.PlayerWeapon.shootingspeed && !player1.MeleeAttacking)
			{
				Slash();
				if (MinionDamaged != nullptr) delete[] MinionDamaged;
				MinionDamaged = new bool[Current_max_minions];
				for (int i = 0; i < Current_max_minions; i++) MinionDamaged[i] = false;
				if (ElementalDamaged != nullptr) delete[] ElementalDamaged;
				ElementalDamaged = new bool[Current_max_elementals];
				for (int i = 0; i < Current_max_elementals; i++) ElementalDamaged[i] = false;
			}
		}
		if (player1.MeleeAttacking)
		{
			if (abs(SlashEffect.angle - player1.PlayerWeapon.angle) <= 90)
			{
				if (!player1.MeleeAttacked)
				{
					if (player1.flip == SDL_FLIP_NONE) player1.PlayerWeapon.angle += 7 * DeltaTime;
					else player1.PlayerWeapon.angle -= 7 * DeltaTime;
					if (player1.PlayerWeapon.angle > 360)
					{
						player1.PlayerWeapon.angle -= 360;
						SlashEffect.angle -= 360;
					}
					else if (player1.PlayerWeapon.angle < 0)
					{
						player1.PlayerWeapon.angle += 360;
						SlashEffect.angle += 360;
					}
					SlashDamage(MinionDamaged,ElementalDamaged);
				}
			}
			else if (!player1.MeleeAttacked)
			{
				player1.MeleeAttacked = true;
				delete[] MinionDamaged;
				MinionDamaged = nullptr;
				delete[] ElementalDamaged;
				ElementalDamaged = nullptr;
			}
			if (player1.MeleeAttacked)
			{
				SlashRecover();
			}
		}
		break;
	}
}
void PlayerCollisions(SDL_FPoint& TempPos)
{
	if (player1.isHurt && player1.CurrentSprite >= player1.NumOfSprites - 1)
	{
		player1.isHurt = false;
		player1.Idle();
	}
	if (TempPos.x < -player1.SpriteSize.x) TempPos.x = LEVEL_WIDTH - player1.SpriteSize.x;
	if (TempPos.y < -player1.SpriteSize.y) TempPos.y = LEVEL_HEIGHT - player1.SpriteSize.y;
	if (TempPos.x + player1.SpriteSize.x > LEVEL_WIDTH) TempPos.x -= LEVEL_WIDTH;
	if (TempPos.y + player1.SpriteSize.y > LEVEL_HEIGHT) TempPos.y -= LEVEL_HEIGHT;
	SDL_FRect Futurehitbox = Playerhitbox(player1.flip, TempPos);
	if (!player1.Dashing)
	{
		for (int i = 0; i < Current_max_minions; i++)
		{
			if (SDL_HasIntersectionF(&player1.hitbox, &minion[i]->hitbox)) return;
			else if (SDL_HasIntersectionF(&Futurehitbox, &minion[i]->hitbox)) TempPos = player1.position;
		}
		for (int i = 0; i < Current_max_elementals; i++)
		{
			if (SDL_HasIntersectionF(&player1.hitbox, &elemental[i]->hitbox)) return;
			else if (SDL_HasIntersectionF(&Futurehitbox, &elemental[i]->hitbox)) TempPos = player1.position;
		}
	}
}
void Moving()
{
	bool Moved = player1.isMoving;
	player1.isMoving = false;
	Sprint();
	if (!player1.isSprinting)
	{
		if (player1.CurrentWeapon == 2) player1.speed = 3.5;
		else player1.speed = 5;
	}
	SDL_FPoint TempPos = player1.position;
	player1.PlayerDirection();
	if (!player1.isHurt)
	{
		PlayerDash(TempPos);
		if (!player1.Dashing && !player1.isParalysed)
		{
			TempPos.x += player1.Direction.x * player1.speed * DeltaTime;
			TempPos.y += player1.Direction.y * player1.speed * DeltaTime;
		}
		static Timer MovingSound(1);
		static bool Step = false;
		if (player1.isMoving)
		{
			if (MovingSound.GetTime() >= 1000 / player1.speed)
			{
				Mix_PlayChannel(-1, SoundEffects[5 + Step], 0);
				MovingSound.Restart();
				Step = !Step;
			}
		}
		if (Moved != player1.isMoving)
		{
			if (player1.isMoving == true) player1.Run();
			else player1.Idle();
		}
	}
	PlayerCollisions(TempPos);
	player1.position = TempPos;
	player1.hitbox = Playerhitbox(player1.flip, player1.position);
}
void Skills()
{
	PlayerShield();
}
void PlayerLogic()
{
	if (player1.Health <= 0 && !player1.isDead)
	{
		player1.Death();
		return;
	}
	Moving();
	ChangeWeapon();
	WeaponPosition(); 
	CalculateWeaponAngle(); 
	PlayerAttacking();
	Skills();
}
void GetMousePosition()
{
	float ScaleX = LOGICAL_WIDTH * 1.0 / SCREEN_WIDTH;
	float ScaleY = LOGICAL_HEIGHT * 1.0 / SCREEN_HEIGHT;
	MouseState = SDL_GetMouseState(&MousePosition.x, &MousePosition.y);
	MousePosition.x = MousePosition.x * (ScaleX);
	MousePosition.y = MousePosition.y * (ScaleY);
	MouseRelativePosition.x = MousePosition.x + camera[0].CameraPosition.x;
	MouseRelativePosition.y = MousePosition.y + camera[0].CameraPosition.y;
	if (MouseRelativePosition.x < 0) MouseRelativePosition.x += LEVEL_WIDTH;
	if (MouseRelativePosition.y < 0) MouseRelativePosition.y += LEVEL_HEIGHT;
	if (MouseRelativePosition.x > LEVEL_WIDTH) MouseRelativePosition.x -= LEVEL_WIDTH;
	if (MouseRelativePosition.y > LEVEL_HEIGHT) MouseRelativePosition.y -= LEVEL_HEIGHT;
}
void GetMouseClick()
{
	static bool Down1l, Down1r, Down2l = false, Down2r = false;
	Down1l = false;
	if (MouseState & SDL_BUTTON(1))
	{
		Down1l = true;
		if (Down1l != Down2l)
		{
			MouseLeftDown = true;
		}
		else MouseLeftDown = false;
	}
	Down1r = false;
	if (MouseState & SDL_BUTTON(3))
	{
		Down1r = true;
		if (Down1r != Down2r)
		{
			MouseRightDown = true;
		}
		else MouseRightDown = false;
	}
	Down2l = Down1l;
	Down2r = Down1r;
}

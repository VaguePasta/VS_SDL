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
	if (!player1.isAttacking && !player1.MeleeAttacking)
	{
		if (Keyboard[SDL_SCANCODE_1] != 0 && player1.CurrentWeapon != 0)
		{
			player1.CurrentWeapon = 0;
			player1.PlayerWeapon.LoadWeapon(player1.CurrentWeapon);
		}
		else if (Keyboard[SDL_SCANCODE_2] != 0 && player1.CurrentWeapon != 1)
		{
			player1.CurrentWeapon = 1;
			player1.PlayerWeapon.LoadWeapon(player1.CurrentWeapon);
		}
		else if (Keyboard[SDL_SCANCODE_3] != 0 && player1.CurrentWeapon != 2)
		{
			player1.CurrentWeapon = 2;
			player1.PlayerWeapon.LoadWeapon(player1.CurrentWeapon);
		}
		else if (Keyboard[SDL_SCANCODE_4] != 0 && player1.CurrentWeapon != 3)
		{
			player1.CurrentWeapon = 3;
			player1.PlayerWeapon.LoadWeapon(player1.CurrentWeapon);
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
	if (player1.CurrentWeapon == 2)
	{
		player1.PlayerWeapon.Position.x = (player1.flip == SDL_FLIP_NONE) ? (15) : (-10);
		player1.PlayerWeapon.Position.y = 25;
	}
	else if (player1.CurrentWeapon == 3)
	{
		if (player1.flip == SDL_FLIP_NONE)
		{
			player1.PlayerWeapon.Position.x = 35;
			player1.PlayerWeapon.Position.y = 23;
			*player1.PlayerWeapon.Center = { 2,30 };
		}
		else
		{
			player1.PlayerWeapon.Position.x = -36;
			player1.PlayerWeapon.Position.y = 23;
			*player1.PlayerWeapon.Center = { 98,30 };
		}
	}
	else
	{
		player1.PlayerWeapon.Position.x = (player1.flip == SDL_FLIP_NONE) ? (20) : (-5);
		player1.PlayerWeapon.Position.y = 33;
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
			player1.PlayerWeapon.Position.x += -5 * cos(ShootAngle * 0.017);
			player1.PlayerWeapon.Position.y += -5 * sin(ShootAngle * 0.017);
			if (RecoilTimer.GetTime() >= 30)
			{
				Recoil = false;
				RecoilTimer.Reset();
			}
		}
		break;
	case 3:
		static bool* EnemyDamaged = nullptr;
		static bool* ElementalDamaged = nullptr;
		if (player1.isAttacking)
		{
			if (player1.PlayerWeapon.ShootingDelay.GetTime() >= 1000 / player1.PlayerWeapon.shootingspeed && !player1.MeleeAttacking)
			{
				Slash();
				if (EnemyDamaged != nullptr) delete[] EnemyDamaged;
				EnemyDamaged = new bool[Current_max_enemies];
				for (int i = 0; i < Current_max_enemies; i++) EnemyDamaged[i] = false;
				if (ElementalDamaged != nullptr) delete[] ElementalDamaged;
				ElementalDamaged = new bool[Current_max_elementals];
				for (int i = 0; i < Current_max_elementals; i++) ElementalDamaged[i] = false;
			}
		}
		if (player1.MeleeAttacking)
		{
			if (abs(SlashEffect.SpriteAngle - player1.PlayerWeapon.angle) <= 90)
			{
				if (!player1.MeleeAttacked)
				{
					if (player1.flip == SDL_FLIP_NONE) player1.PlayerWeapon.angle += 7 * DeltaTime;
					else player1.PlayerWeapon.angle -= 7 * DeltaTime;
					if (player1.PlayerWeapon.angle > 360)
					{
						player1.PlayerWeapon.angle -= 360;
						SlashEffect.SpriteAngle -= 360;
					}
					else if (player1.PlayerWeapon.angle < 0)
					{
						player1.PlayerWeapon.angle += 360;
						SlashEffect.SpriteAngle += 360;
					}
					SlashDamage(EnemyDamaged,ElementalDamaged);
				}
			}
			else if (!player1.MeleeAttacked)
			{
				player1.MeleeAttacked = true;
				delete[] EnemyDamaged;
				EnemyDamaged = nullptr;
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
	if (TempPos.x > LEVEL_WIDTH) TempPos.x = 0;
	if (TempPos.y > LEVEL_HEIGHT) TempPos.y = 0;
	SDL_FRect FutureHitbox = PlayerHitbox(player1.flip, TempPos);
	for (int i = 0; i < Current_max_enemies; i++)
	{
		if (SDL_HasIntersectionF(&FutureHitbox, &enemy[i]->Hitbox)) TempPos = player1.position;
	}
	for (int i = 0; i < Current_max_elementals; i++)
	{
		if (SDL_HasIntersectionF(&FutureHitbox, &elemental[i]->Hitbox)) TempPos = player1.position;
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
		if (!player1.Dashing)
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
	player1.Hitbox = PlayerHitbox(player1.flip, player1.position);
}
void Skills()
{
	PlayerShield();
}
void inputprocessing()
{
	Moving();
	ChangeWeapon();
	WeaponPosition();
	CalculateWeaponAngle();
	PlayerAttacking();
	Skills();
}
void GetMouseClick()
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

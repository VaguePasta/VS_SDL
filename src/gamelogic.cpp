#include <SDL.h>
#include <SDL_mixer.h>
#include "enemies.h"
#include "player.h"
#include "weapons.h"
#include "global.h"
#include "enemyAI.h"
#include "gameobjects.h"
#include "sounds.h"
#include <cmath>
using namespace std;
void BulletCollision()
{

	for (int i = 0; i < Max_Bullets; i++)
	{
		if (player1.PlayerWeapon.bullets[i]->isShot && !player1.PlayerWeapon.bullets[i]->Decayed)
		{
			if (player1.PlayerWeapon.bullets[i]->Decay())
			{
				delete player1.PlayerWeapon.bullets[i];
				player1.PlayerWeapon.bullets[i] = new bullet(player1.CurrentWeapon);
			}
			else player1.PlayerWeapon.bullets[i]->Update();
		}
		if (player1.PlayerWeapon.bullets[i]->isShot)
		{
			if (!player1.PlayerWeapon.bullets[i]->Decayed)
			{
				for (int j = 0; j < Current_max_enemies; j++)
				{
					if (enemy[j]->isSpawn && !enemy[j]->isDead)
					{
						if (SDL_HasIntersection(&player1.PlayerWeapon.bullets[i]->Hitbox, &enemy[j]->Hitbox))
						{
							if (player1.PlayerWeapon.bullets[i]->CurrentWeapon != 2)
							{
								enemy[j]->Health -= player1.PlayerWeapon.damage;
								if (!enemy[j]->isDead)
								{
									if (enemy[j]->Health <= 0) enemy[j]->Death();
									else if (!enemy[j]->isAttacking) enemy[j]->Hurt();
								}
								delete player1.PlayerWeapon.bullets[i];
								player1.PlayerWeapon.bullets[i] = new bullet(player1.CurrentWeapon);
							}
							else
							{
								SDL_Rect DamageSite = { player1.PlayerWeapon.bullets[i]->BulletPosition.x - 120,player1.PlayerWeapon.bullets[i]->BulletPosition.y - 120,240,240 };
								for (int k = 0; k < Current_max_enemies; k++) if (enemy[k]->isSpawn && !enemy[k]->isDead)
								{
									if (SDL_HasIntersection(&enemy[k]->Hitbox, &DamageSite))
									{
										enemy[k]->Health -= player1.PlayerWeapon.damage;
										if (!enemy[k]->isDead)
										{
											if (enemy[k]->Health <= 0) enemy[k]->Death();
											else if (!enemy[k]->isAttacking) enemy[k]->Hurt();
										}
									}

								}
								player1.PlayerWeapon.bullets[i]->Explosion(8, 8);
								Mix_HaltChannel(PlayingChannel);
								Mix_PlayChannel(-1, SoundEffects[3], 0);
							}
						}
					}
				}
			}
			else if (player1.PlayerWeapon.bullets[i]->BulletAnimation.CurrentSprite >= player1.PlayerWeapon.bullets[i]->BulletAnimation.NumOfSprites - 1)
			{
				delete player1.PlayerWeapon.bullets[i];
				player1.PlayerWeapon.bullets[i] = new bullet(player1.CurrentWeapon);
			}
		}
	}
}
void SpawnAndDeadEnemies()
{
	for (int i = 0; i < Current_max_enemies; i++)
	{
		if (!enemy[i]->isSpawn)
		{
			if (!player1.isDead) enemy[i]->Spawn(player1.position);
		}
		else if (!enemy[i]->isDead && enemy[i]->isHurt)
		{
			if (enemy[i]->CurrentSprite >= enemy[i]->NumOfSprites - 1)
			{
				enemy[i]->isHurt = false;
			}
		}
		else if (enemy[i]->isDead)
		{
			if (!enemy[i]->Decayed)
			{
				if (enemy[i]->CurrentSprite >= enemy[i]->NumOfSprites - 1)
				{
					enemy[i]->Decayed = true;
					enemy[i]->Cooldown.Restart();
				}
			}
			if (enemy[i]->Decayed)
			{
				if (enemy[i]->Cooldown.GetTime() <= 2000)
				{
					enemy[i]->CurrentSprite = enemy[i]->NumOfSprites - 1;
					enemy[i]->MovingCounter.Reset();
				}
				else
				{
					delete enemy[i];
					enemy[i] = new Enemies();
				}
			}
		}
		if (enemy[i]->isSpawn && enemy[i]->MovingCounter.GetTime() >= 6000)
		{
			delete enemy[i];
			enemy[i] = new Enemies();
		}
	}
}
void UpdateUI()
{
	if (player1.Health < 0) player1.Health = 0;
	HealthBar.Value = player1.Health;
	StaminaBar.Value = int(round(player1.Stamina));
	if (player1.PlayerShield.isOn) ShieldHealthBar.Value = player1.PlayerShield.Health;
	if (player1.PlayerShield.ShieldIcon.isOnCooldown)
	{
		if (player1.PlayerShield.Time.GetTime() >= player1.PlayerShield.Cooldown) player1.PlayerShield.ShieldIcon.isOnCooldown = false;
		player1.PlayerShield.ShieldIcon.Value = player1.PlayerShield.Cooldown / 1000 - player1.PlayerShield.Time.GetTime() / 1000;
	}
	if (DashIcon.isOnCooldown)
	{
		if (player1.DashCooldown.GetTime() >= player1.DashCooldownTime) DashIcon.isOnCooldown = false;
		DashIcon.Value = player1.DashCooldownTime / 1000 - player1.DashCooldown.GetTime() / 1000;
	}
}
void GameLogic()
{
	SpawnAndDeadEnemies();
	if (!player1.isDead) UpdateUI();
	BulletCollision();
	EnemyLogics();
	FrameCount++;
}

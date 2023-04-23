#include <SDL.h>
#include <SDL_mixer.h>
#include "enemies.h"
#include "player.h"
#include "weapons.h"
#include "global.h"
#include "enemyAI.h"
#include "elementals.h"
#include "elementalAI.h"
#include "gameobjects.h"
#include "sounds.h"
#include <cmath>
void BulletUpdate()
{
	for (int i = 0; i < Max_Bullets; i++)
	{
	    if (!player1.PlayerWeapon.bullets[i]->isShot) continue;
		if (!player1.PlayerWeapon.bullets[i]->Decayed)
		{
			if (player1.PlayerWeapon.bullets[i]->Decay())
			{
				delete player1.PlayerWeapon.bullets[i];
				player1.PlayerWeapon.bullets[i] = new bullet(player1.CurrentWeapon);
				return;
			}
			else
            {
                player1.PlayerWeapon.bullets[i]->Update();
            }
		}
		else if (player1.PlayerWeapon.bullets[i]->BulletAnimation.CurrentSprite >= player1.PlayerWeapon.bullets[i]->BulletAnimation.NumOfSprites - 1)
		{
			delete player1.PlayerWeapon.bullets[i];
			player1.PlayerWeapon.bullets[i] = new bullet(player1.CurrentWeapon);
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
void SpawnAndDeadElementals()
{
	for (int i = 0; i < Current_max_elementals; i++)
	{
		if (!elemental[i]->isSpawn)
		{
			if (!player1.isDead) elemental[i]->Spawn(player1.position);
		}
		else if (!elemental[i]->isDead && elemental[i]->isHurt)
		{
			if (elemental[i]->CurrentSprite >= elemental[i]->NumOfSprites - 1)
			{
				elemental[i]->isHurt = false;
			}
		}
		else if (elemental[i]->isDead)
		{
			if (!elemental[i]->Decayed)
			{
				if (elemental[i]->CurrentSprite >= elemental[i]->NumOfSprites - 1)
				{
					elemental[i]->Decayed = true;
					elemental[i]->Cooldown.Restart();
				}
			}
			if (elemental[i]->Decayed)
			{
				if (elemental[i]->Cooldown.GetTime() <= 2000)
				{
					elemental[i]->CurrentSprite = elemental[i]->NumOfSprites - 1;
				}
				else
				{
					delete elemental[i];
					elemental[i] = new Elementals();
				}
			}
		}
	}
}
void GameLogic()
{
	SpawnAndDeadEnemies();
	SpawnAndDeadElementals();
	if (!player1.isDead) UpdateUI();
	BulletUpdate();
	EnemyLogics();
	ElementalLogics();
	FrameCount++;
}

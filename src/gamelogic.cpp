#include <SDL.h>
#include <SDL_mixer.h>
#include "minions.h"
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
void SpawnAndDeadMinions()
{
	for (int i = 0; i < Current_max_minions; i++)
	{
		if (!minion[i]->isSpawn)
		{
			if (!player1.isDead) minion[i]->Spawn(player1.position);
		}
		else if (!minion[i]->isDead && minion[i]->isHurt)
		{
			if (minion[i]->CurrentSprite >= minion[i]->NumOfSprites - 1)
			{
				minion[i]->isHurt = false;
			}
		}
		else if (minion[i]->isDead)
		{
			if (!minion[i]->Decayed)
			{
				if (minion[i]->CurrentSprite >= minion[i]->NumOfSprites - 1)
				{
					minion[i]->Decayed = true;
					minion[i]->Cooldown.Restart();
				}
			}
			if (minion[i]->Decayed)
			{
				if (minion[i]->Cooldown.GetTime() <= 2000)
				{
					minion[i]->CurrentSprite = minion[i]->NumOfSprites - 1;
					minion[i]->MovingCounter.Reset();
				}
				else
				{
					delete minion[i];
					minion[i] = new Minions();
				}
			}
		}
		if (minion[i]->isSpawn && minion[i]->MovingCounter.GetTime() >= 6000)
		{
			delete minion[i];
			minion[i] = new Minions();
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
	SpawnAndDeadMinions();
	SpawnAndDeadElementals();
	if (!player1.isDead) UpdateUI();
	BulletUpdate();
	MinionLogics();
	ElementalLogics();
	FrameCount++;
}

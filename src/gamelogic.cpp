#include <SDL.h>
#include <SDL_mixer.h>
#include "minions.h"
#include "global.h"
#include "minionAI.h"
#include "elementals.h"
#include "elementalAI.h"
#include "gameobjects.h"
#include "sounds.h"
#include <cmath>
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
				}
				else
				{
					delete minion[i];
					minion[i] = new Minions();
				}
			}
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
void SpawnEnemies()
{
	SpawnAndDeadMinions();
	SpawnAndDeadElementals();
}
void EnemyLogic()
{
	MinionLogics();
	ElementalLogics();
}
void GameLogic()
{
	SpawnEnemies();
	if (!player1.isDead) UpdateUI();
	BulletUpdate();
	EnemyLogic();
	EnemyProjectilesProcessing();
}

#include <SDL.h>
#include <cmath>
#include "global.h"
#include "timer.h"
#include "minions.h"
#include "player.h"
#include "minionAI.h"
#include "gameobjects.h"
#include "enemyprojectiles.h"
#include "sounds.h"
using namespace std;
void MinionLogics()
{
	for (int i = 0; i < Current_max_minions; i++)
	{
		if (!player1.isDead)
		{
			if (minion[i]->isSpawn && !minion[i]->isDead)
			{
				minion[i]->CalculateBoxes();
				MinionPathfinding(minion[i]);
				MinionAttack(minion[i]);
			}
		}
		else if (minion[i]->isSpawn && !minion[i]->isDead && !minion[i]->isIdling)
		{
			minion[i]->Idle();
		}
		if (minion[i]->isSpawn && !minion[i]->isDead) MinionBulletCollision(minion[i]);
	}
}
SDL_FPoint PlayerRelativeMinion(SDL_FPoint MinionCenter)
{
	SDL_FPoint PlayerCenter = { player1.position.x + player1.SpriteSize.x / 2,player1.position.y + player1.SpriteSize.y / 2 };
	if (abs(PlayerCenter.x - MinionCenter.x) > LEVEL_WIDTH / 2)
	{
		if (PlayerCenter.x < LEVEL_WIDTH / 2)
		{
			PlayerCenter.x += LEVEL_WIDTH;
		}
		else
		{
			PlayerCenter.x -= LEVEL_WIDTH;
		}
	}
	if (abs(PlayerCenter.y - MinionCenter.y) > LEVEL_HEIGHT / 2)
	{
		if (PlayerCenter.y < LEVEL_HEIGHT / 2)
		{
			PlayerCenter.y += LEVEL_HEIGHT;
		}
		else
		{
			PlayerCenter.y -= LEVEL_HEIGHT;
		}
	}
	return PlayerCenter;
}
void MinionPathfinding(Minions* minion)
{
	if (minion->isAttacking) return;
	SDL_FPoint MinionCenter = { minion->position.x + minion->SpriteSize.x / 2,minion->position.y + minion->SpriteSize.y / 2 };
	SDL_FPoint PlayerCenter = PlayerRelativeMinion(MinionCenter);
	SDL_FPoint PlayerRelativePosition = { PlayerCenter.x - 50,PlayerCenter.y - 50 };
	SDL_FRect hitbox = Playerhitbox(player1.flip, PlayerRelativePosition);
	if (PlayerCenter.x < MinionCenter.x - 5 && minion->flip == SDL_FLIP_NONE)
	{
		minion->flip = SDL_FLIP_HORIZONTAL;
		minion->CalculateBoxes();
	}
	else if (PlayerCenter.x > MinionCenter.x + 5 && minion->flip == SDL_FLIP_HORIZONTAL)
	{
		minion->flip = SDL_FLIP_NONE;
		minion->CalculateBoxes();
	}
	if (!minion->isHurt && !SDL_HasIntersectionF(&hitbox, &minion->AttackBox))
	{
		if (!minion->isMoving) minion->Run();
		SDL_FPoint TempPos = minion->position;
		if (!minion->isRanged) MinionMoving(minion, MinionCenter, PlayerCenter);
		else
		{
			SDL_FPoint Target = { (PlayerCenter.x - MinionCenter.x < 0) ? (PlayerCenter.x + 700) : (PlayerCenter.x - 700),PlayerCenter.y };
			MinionMoving(minion, MinionCenter, Target);
		}
		minion->position.x = MinionCenter.x - minion->SpriteSize.x / 2;
		minion->position.y = MinionCenter.y - minion->SpriteSize.y / 2;
		if (minion->position.x < -minion->SpriteSize.x) minion->position.x = LEVEL_WIDTH - minion->SpriteSize.x;
		if (minion->position.y < -minion->SpriteSize.y) minion->position.y = LEVEL_HEIGHT - minion->SpriteSize.y;
		if (minion->position.x > LEVEL_WIDTH) minion->position.x = 0;
		if (minion->position.y > LEVEL_HEIGHT) minion->position.y = 0;
		minion->CalculateBoxes();
		if (SDL_HasIntersectionF(&minion->hitbox, &hitbox))
		{
			minion->position = TempPos;
			minion->CalculateBoxes();
		}
	}
	else
	{
		minion->isMoving = false;
	}
}
void MinionMoving(Minions* minion, SDL_FPoint& MinionCenter, SDL_FPoint& Target)
{
	if (MinionCenter.x != Target.x && MinionCenter.y != Target.y)
	{
		MinionCenter.x += minion->speed * DeltaTime * ((Target.x - MinionCenter.x < 0) ? (-0.707) : (0.707));
		MinionCenter.y += minion->speed * DeltaTime * ((Target.y - MinionCenter.y < 0) ? (-0.707) : (0.707));
	}
	else if (MinionCenter.x != Target.x) MinionCenter.x += minion->speed * DeltaTime * ((Target.x - MinionCenter.x < 0) ? (-1) : (1));
	else if (MinionCenter.y != Target.y) MinionCenter.y += minion->speed * DeltaTime * ((Target.y - MinionCenter.y < 0) ? (-1) : (1));
}
void MinionAttack(Minions* minion)
{
	SDL_FPoint MinionCenter = { minion->position.x + minion->SpriteSize.x / 2,minion->position.y + minion->SpriteSize.y / 2 };
	SDL_FPoint PlayerCenter = PlayerRelativeMinion(MinionCenter);
	SDL_FPoint PlayerRelativePosition = { PlayerCenter.x - 50,PlayerCenter.y - 50 };
	SDL_FRect hitbox = Playerhitbox(player1.flip, PlayerRelativePosition);
	if (!minion->isAttacking && SDL_HasIntersectionF(&hitbox, &minion->AttackBox))
	{
		if (minion->Cooldown.GetTime() > 1000 / minion->AttackSpeed)
		{
			minion->Attack();
			return;
		}
		else if (!minion->isIdling && !minion->isHurt)
		{
			minion->Idle();
			return;
		}
	}
	if (minion->isAttacking)
	{
		Minion_isAttacking(minion, MinionCenter, PlayerCenter, hitbox);
		if (minion->CurrentSprite >= minion->NumOfSprites - 1)
		{
			minion->isAttacking = false;
			minion->Idle();
		}
	}
}
void Minion_isAttacking(Minions* minion, SDL_FPoint& MinionCenter, SDL_FPoint& PlayerCenter, SDL_FRect& hitbox)
{
	switch (minion->CurrentMinionType)
	{
	case 0: case 2:
		if (minion->CurrentSprite >= minion->NumOfSprites - 1 && SDL_HasIntersectionF(&minion->AttackBox, &hitbox))
		{
			player1.Hurt(minion->Damage);
		}
		break;
	case 1:
		if (minion->CurrentSprite == 3 && SDL_HasIntersectionF(&minion->AttackBox, &hitbox))
		{
			minion->CurrentSprite++;
			player1.Hurt(minion->Damage);
		}
		break;
	case 3: case 4:
		if (minion->CurrentMinionType == 3 && minion->CurrentSprite == 2)
		{
			Mix_PlayChannel(-1, SoundEffects[10], 0);
			minion->CurrentSprite++;
		}
		if (minion->CurrentSprite >= minion->NumOfSprites - 1)
			for (int i = 0; i < Current_max_enemies; i++) if (!Projectiles[i]->isShot)
			{
				Projectiles[i]->Origin = MinionCenter;
				Projectiles[i]->ChooseType(minion->CurrentMinionType);
				Projectiles[i]->Target = MinionAiming(PlayerCenter, MinionCenter, minion->CurrentMinionType, Projectiles[i]->type);
				Projectiles[i]->Shoot();
				break;
			}
		break;
	}
}
SDL_FPoint MinionAiming(SDL_FPoint& PlayerCenter, SDL_FPoint& MinionCenter, int CurrentMinionType, int ProjectileType)
{
	if (SCORE < 50) return PlayerCenter;
	float Distance = DistanceCalculation(PlayerCenter, MinionCenter);
	float ScaleDistance;
	switch (CurrentMinionType)
	{
	case 3:
		ScaleDistance = Distance * 0.45;
		break;
	case 4:
		if (ProjectileType == 1) ScaleDistance = 245;
		if (ProjectileType == 2) ScaleDistance = 172;
		break;
	}
	SDL_FPoint Target = { PlayerCenter.x + player1.Direction.x * ScaleDistance,PlayerCenter.y + player1.Direction.y * ScaleDistance };
	return Target;
}
void MinionBulletCollision(Minions* minion)
{
	for (int i = 0; i < Max_Bullets; i++) if (player1.PlayerWeapon.bullets[i]->isShot && !player1.PlayerWeapon.bullets[i]->Decayed)
	{
		if (SDL_HasIntersectionF(&player1.PlayerWeapon.bullets[i]->hitbox, &minion->hitbox))
		{
			if (player1.PlayerWeapon.bullets[i]->CurrentWeapon != 2)
			{
				minion->Health -= player1.PlayerWeapon.bullets[i]->Damage;
				if (!minion->isDead)
				{
					if (minion->Health <= 0) minion->Death();
					else if (!minion->isAttacking) minion->Hurt();
				}
				delete player1.PlayerWeapon.bullets[i];
				player1.PlayerWeapon.bullets[i] = new bullet(player1.CurrentWeapon);
			}
			else
			{
				BulletExplosion(player1.PlayerWeapon.bullets[i]);
			}
		}
	}
}
void MinionSlashDamage(Minions* minion, bool& MinionDamaged)
{
	SDL_FPoint PlayerCenter = player1.position; PlayerCenter.x += 50; PlayerCenter.y += 50;
	SDL_FPoint MinionCenter = minion->position; MinionCenter.x += minion->SpriteSize.x / 2; MinionCenter.y += minion->SpriteSize.y / 2;
	float enemyAngle = AngleCalculation(MinionCenter, PlayerCenter, &player1.flip);
	float enemyDistance = DistanceCalculation(MinionCenter, PlayerCenter);
	if (abs(enemyAngle - player1.PlayerWeapon.angle) <= 5 && enemyDistance <= player1.PlayerWeapon.range && !MinionDamaged)
	{
		Mix_HaltChannel(1);
		Mix_PlayChannel(-1, SoundEffects[13], 0);
		minion->Health -= player1.PlayerWeapon.damage;
		if (minion->Health <= 0) minion->Death();
		else
		{
			minion->isAttacking = false;
			minion->Hurt();
		}
		MinionDamaged = true;
	}
}
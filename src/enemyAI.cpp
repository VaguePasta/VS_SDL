#include <SDL.h>
#include <cmath>
#include "global.h"
#include "timer.h"
#include "enemies.h"
#include "player.h"
#include "enemyAI.h"
#include "gameobjects.h"
#include "enemyprojectiles.h"
#include "sounds.h"
using namespace std;
void EnemyLogics()
{
	for (int i = 0; i < Current_max_enemies; i++)
	{
		if (!player1.isDead)
		{
			if (enemy[i]->isSpawn && !enemy[i]->isDead) enemy[i]->CalCulateBoxes();
			EnemyPathfinding(enemy[i]);
			EnemyAttacking(enemy[i]);
		}
		else if (enemy[i]->isSpawn && !enemy[i]->isDead && !enemy[i]->isIdling)
		{
			enemy[i]->Idle();
		}
	}
	EnemyProjectilesProcessing();
}
void EnemyPathfinding(Enemies* enemy)
{
	if (enemy->isSpawn && !enemy->isDead)
	{
		SDL_FPoint PlayerCenter = { player1.position.x + player1.SpriteSize / 2,player1.position.y + player1.SpriteSize / 2 };
		SDL_FPoint EnemyCenter = { enemy->position.x + enemy->SpriteSize / 2,enemy->position.y + enemy->SpriteSize / 2 };
		if (abs(PlayerCenter.x - EnemyCenter.x) > LEVEL_WIDTH / 2)
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
		if (abs(PlayerCenter.y - EnemyCenter.y) > LEVEL_HEIGHT / 2)
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
		SDL_FPoint Pos = { PlayerCenter.x - 50,PlayerCenter.y - 50 };
		SDL_Rect Hitbox = PlayerHitbox(player1.flip, Pos);
		if (PlayerCenter.x < EnemyCenter.x - 5 && enemy->flip == SDL_FLIP_NONE)
		{
			enemy->flip = SDL_FLIP_HORIZONTAL;
			enemy->CalCulateBoxes();
		}
		else if (PlayerCenter.x > EnemyCenter.x + 5 && enemy->flip == SDL_FLIP_HORIZONTAL)
		{
			enemy->flip = SDL_FLIP_NONE;
			enemy->CalCulateBoxes();
		}
		if (enemy->isAttacking) return;
		if (!enemy->isHurt && !SDL_HasIntersection(&Hitbox, &enemy->AttackBox))
		{
			if (!enemy->isRunning) enemy->Run();
			SDL_FPoint TempPos = enemy->position;
			if (!enemy->isRanged) EnemyMoving(enemy, EnemyCenter, PlayerCenter);
			else
			{
				SDL_FPoint Target = { (PlayerCenter.x - EnemyCenter.x < 0) ? (PlayerCenter.x + 700) : (PlayerCenter.x - 700),PlayerCenter.y };
				EnemyMoving(enemy, EnemyCenter, Target);
			}
			enemy->position.x = EnemyCenter.x - enemy->SpriteSize / 2;
			enemy->position.y = EnemyCenter.y - enemy->SpriteSize / 2;
			if (enemy->position.x < -enemy->SpriteSize) enemy->position.x = LEVEL_WIDTH - enemy->SpriteSize;
			if (enemy->position.y < -enemy->SpriteSize) enemy->position.y = LEVEL_HEIGHT - enemy->SpriteSize;
			if (enemy->position.x > LEVEL_WIDTH) enemy->position.x = 0;
			if (enemy->position.y > LEVEL_HEIGHT) enemy->position.y = 0;
			enemy->CalCulateBoxes();
			if (SDL_HasIntersection(&enemy->Hitbox, &Hitbox))
			{
				enemy->position = TempPos;
				enemy->CalCulateBoxes();
			}
			enemy->MovingCounter.Restart();
		}
		else
		{
			enemy->isRunning = false;
		}
	}
}
void EnemyMoving(Enemies* enemy, SDL_FPoint& EnemyCenter, SDL_FPoint& Target)
{
	if (EnemyCenter.x != Target.x && EnemyCenter.y != Target.y)
	{
		EnemyCenter.x += enemy->speed * 0.707 * DeltaTime * ((Target.x - EnemyCenter.x < 0) ? (-1) : (1));
		EnemyCenter.y += enemy->speed * 0.707 * DeltaTime * ((Target.y - EnemyCenter.y < 0) ? (-1) : (1));
	}
	else if (EnemyCenter.x != Target.x) EnemyCenter.x += 1.0 * enemy->speed * DeltaTime * ((Target.x - EnemyCenter.x < 0) ? (-1) : (1));
	else if (EnemyCenter.y != Target.y) EnemyCenter.y += 1.0 * enemy->speed * DeltaTime * ((Target.y - EnemyCenter.y < 0) ? (-1) : (1));
}
void EnemyAttacking(Enemies* enemy)
{
	if (enemy->isSpawn && !enemy->isDead)
	{
		SDL_FPoint PlayerCenter = { player1.position.x + player1.SpriteSize / 2,player1.position.y + player1.SpriteSize / 2 };
		SDL_FPoint EnemyCenter = { enemy->position.x + enemy->SpriteSize / 2,enemy->position.y + enemy->SpriteSize / 2 };
		if (abs(PlayerCenter.x - EnemyCenter.x) > LEVEL_WIDTH / 2)
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
		if (abs(PlayerCenter.y - EnemyCenter.y) > LEVEL_HEIGHT / 2)
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
		SDL_FPoint Pos = { PlayerCenter.x - 50,PlayerCenter.y - 50 };
		SDL_Rect Hitbox = PlayerHitbox(player1.flip, Pos);
		if (!enemy->isAttacking && SDL_HasIntersection(&Hitbox, &enemy->AttackBox))
		{
			if (enemy->Cooldown.GetTime() > 1000 / enemy->AttackSpeed)
			{
				enemy->Attack();
				return;
			}
			else if (!enemy->isIdling && !enemy->isHurt)
			{
				enemy->Idle();
				return;
			}
		}
		if (enemy->isAttacking)
		{
			switch (enemy->CurrentEnemyType)
			{
			case 0: case 2:
				if (enemy->CurrentSprite >= enemy->NumOfSprites - 1 && SDL_HasIntersection(&enemy->AttackBox, &Hitbox))
				{
					if (player1.PlayerShield.isOn) player1.PlayerShield.ShieldDamage(enemy->Damage);
					else player1.Hurt(enemy->Damage);
				}
				break;
			case 1:
				if (enemy->CurrentSprite == 3 && SDL_HasIntersection(&enemy->AttackBox, &Hitbox))
				{
					enemy->CurrentSprite++;
					if (player1.PlayerShield.isOn) player1.PlayerShield.ShieldDamage(enemy->Damage);
					else player1.Hurt(enemy->Damage);
				}
				break;
			case 3: case 4:
				if (enemy->CurrentEnemyType == 3 && enemy->CurrentSprite == 2)
				{
					Mix_PlayChannel(-1, SoundEffects[10], 0);
					enemy->CurrentSprite++;
				}
				if (enemy->CurrentSprite >= enemy->NumOfSprites - 1)
					for (int i = 0; i < Current_max_enemies; i++) if (!Projectiles[i]->isShot)
					{
						Projectiles[i]->Origin = EnemyCenter;
						Projectiles[i]->ChooseType(enemy->CurrentEnemyType);
						Projectiles[i]->Target = EnemyAiming(PlayerCenter, EnemyCenter, enemy->CurrentEnemyType, Projectiles[i]->type);
						Projectiles[i]->Shoot();
						break;
					}
				break;
			}
			if (enemy->CurrentSprite >= enemy->NumOfSprites - 1)
			{
				enemy->isAttacking = false;
				enemy->Idle();
			}
		}
	}
}
void EnemyProjectilesProcessing()
{
	for (int i = 0; i < Current_max_enemies; i++) if (Projectiles[i]->isShot)
	{
		Projectiles[i]->Update();
		Projectiles[i]->Decay();
		if (!Projectiles[i]->Decayed)
		{
			if (!Projectiles[i]->isAnimated)
			{
				if (SDL_HasIntersection(&Projectiles[i]->Hitbox, &player1.Hitbox))
				{
					Projectiles[i]->Decayed = true;
					if (player1.PlayerShield.isOn) player1.PlayerShield.ShieldDamage(Projectiles[i]->damage);
					else
					{
						player1.Hurt(Projectiles[i]->damage);
					}
				}
			}
			else
			{
				int DamageFrame;
				switch (Projectiles[i]->type)
				{
				case 1:
					DamageFrame = 10;
					break;
				case 2:
					DamageFrame = 4;
					break;
				}
				if (Projectiles[i]->Projectile.CurrentSprite == DamageFrame)
				{
					if (SDL_HasIntersection(&Projectiles[i]->Hitbox, &player1.Hitbox))
					{
						if (player1.PlayerShield.isOn) player1.PlayerShield.ShieldDamage(Projectiles[i]->damage);
						else
						{
							player1.Hurt(Projectiles[i]->damage);
						}
					}
					Projectiles[i]->Projectile.CurrentSprite++;
				}
			}
		}
		if (Projectiles[i]->Decayed)
		{
			delete Projectiles[i];
			Projectiles[i] = new EnemyProjectiles();
		}
	}
}
SDL_FPoint EnemyAiming(SDL_FPoint& PlayerCenter, SDL_FPoint& EnemyCenter, int CurrentEnemyType, int ProjectileType)
{
	if (SCORE < 50) return PlayerCenter;
	float Distance = sqrt((PlayerCenter.x - EnemyCenter.x) * (PlayerCenter.x - EnemyCenter.x) + (PlayerCenter.y - EnemyCenter.y) + (PlayerCenter.y - EnemyCenter.y));
	float ScaleDistance;
	switch (CurrentEnemyType)
	{
	case 3:
		ScaleDistance = Distance * 0.45;
		break;
	case 4:
		if (ProjectileType == 1) ScaleDistance = 245;
		if (ProjectileType == 2) ScaleDistance = 172;
		break;
	}
	SDL_FPoint Target = { PlayerCenter.x + (int)player1.Direction.x * int(round(ScaleDistance)),PlayerCenter.y + (int)player1.Direction.y * int(round(ScaleDistance)) };
	return Target;
}

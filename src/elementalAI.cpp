#include <SDL.h>
#include "elementalAI.h"
#include "global.h"
#include "gameobjects.h"
#include "player.h"
#include "sounds.h"
#include <cmath>
#include <SDL_mixer.h>
void ElementalLogics()
{
	for (int i = 0; i < Current_max_elementals; i++)
	{
		if (!player1.isDead)
		{
			if (elemental[i]->isSpawn && !elemental[i]->isDead) elemental[i]->CalculateBoxes();
			ElementalDodge(elemental[i]);
			ElementalPathfinding(elemental[i]);
			ElementalAttacking(elemental[i]);
		}
		else if (elemental[i]->isSpawn && !elemental[i]->isDead && !elemental[i]->isIdling)
		{
			if (elemental[i]->isAttacking)
			{
				if (elemental[i]->CurrentSprite >= elemental[i]->NumOfSprites - 1) elemental[i]->Idle();
			}
			else elemental[i]->Idle();
		}
		if (elemental[i]->isSpawn && !elemental[i]->isDead) ElementalBulletCollision(elemental[i]);
	}
}
void ElementalPathfinding(Elementals* elemental)
{
	if (elemental->isSpawn && !elemental->isDead)
	{
		if (elemental->isDodging)
		{
			if (elemental->CurrentSprite >= elemental->NumOfSprites - 1)
			{
				elemental->isDodging = false;
				elemental->DodgeCooldown.Restart();
				elemental->speed = ElementalSpritesInfo[elemental->CurrentElementalType][0][3];
			}
		}
		if (elemental->isDefending)
		{
			if (elemental->CurrentSprite >= elemental->NumOfSprites - 1)
			{
				elemental->isDefending = false;
				elemental->BlockCooldown.Restart();
			}
			return;
		}
		SDL_FPoint PlayerCenter = { player1.position.x + player1.SpriteSize.x / 2,player1.position.y + player1.SpriteSize.y / 2 };
		SDL_FPoint ElementalCenter = { elemental->hitbox.x + elemental->hitbox.w / 2,elemental->hitbox.y + elemental->hitbox.h / 2 };
		elemental->ElementalCenter = ElementalCenter;
		if (abs(PlayerCenter.x - ElementalCenter.x) > LEVEL_WIDTH / 2)
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
		if (abs(PlayerCenter.y - ElementalCenter.y) > LEVEL_HEIGHT / 2)
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
		SDL_FRect hitbox = Playerhitbox(player1.flip, Pos);
		if (PlayerCenter.x < ElementalCenter.x - 5 && elemental->flip == SDL_FLIP_NONE && !elemental->isAttacking)
		{
			elemental->flip = SDL_FLIP_HORIZONTAL;
			elemental->CalculateBoxes();
		}
		else if (PlayerCenter.x > ElementalCenter.x + 5 && elemental->flip == SDL_FLIP_HORIZONTAL && !elemental->isAttacking)
		{
			elemental->flip = SDL_FLIP_NONE;
			elemental->CalculateBoxes();
		}
		if (elemental->isAttacking) return;
		if (!elemental->isHurt && !SDL_HasIntersectionF(&hitbox, &elemental->AttackBox))
		{
			SDL_FPoint TempPos = elemental->position;
			SDL_FPoint Target = { (ElementalCenter.x - PlayerCenter.x < 0) ? (PlayerCenter.x - player1.SpriteSize.x / 2 - elemental->AttackBox.w / 3) : (PlayerCenter.x + player1.SpriteSize.x / 2 + elemental->AttackBox.w / 3),PlayerCenter.y };
			if (!elemental->isMoving && !elemental->isDodging) elemental->Run();
			if (elemental->CurrentElementalType == 5) (ElementalCenter.x - PlayerCenter.x < 0) ? (Target.x -= 400) : (Target.x += 400);
			ElementalMoving(elemental, ElementalCenter, Target);
			elemental->position.x = ElementalCenter.x - elemental->hitbox.w / 2 - ElementalSpritesInfo[elemental->CurrentElementalType][11][0];
			elemental->position.y = ElementalCenter.y - elemental->hitbox.h / 2 - ElementalSpritesInfo[elemental->CurrentElementalType][11][1];
			if (elemental->position.x < -elemental->SpriteSize.x) elemental->position.x = LEVEL_WIDTH - elemental->SpriteSize.x;
			if (elemental->position.y < -elemental->SpriteSize.y) elemental->position.y = LEVEL_HEIGHT - elemental->SpriteSize.y;
			if (elemental->position.x > LEVEL_WIDTH) elemental->position.x = 0;
			if (elemental->position.y > LEVEL_HEIGHT) elemental->position.y = 0;
			elemental->CalculateBoxes();
			if (SDL_HasIntersectionF(&elemental->hitbox, &hitbox))
			{
				elemental->position = TempPos;
				elemental->CalculateBoxes();
			}
		}
		else
		{
			elemental->isMoving = false;
		}
	}
}
void ElementalMoving(Elementals* elemental, SDL_FPoint& ElementalCenter, SDL_FPoint Target)
{
	if (ElementalCenter.x != Target.x && ElementalCenter.y != Target.y)
	{
		ElementalCenter.x += elemental->speed * DeltaTime * ((Target.x - ElementalCenter.x < 0) ? (-0.707) : (0.707));
		ElementalCenter.y += elemental->speed * DeltaTime * ((Target.y - ElementalCenter.y < 0) ? (-0.707) : (0.707));
	}
	else if (ElementalCenter.x != Target.x) ElementalCenter.x += elemental->speed * DeltaTime * ((Target.x - ElementalCenter.x < 0) ? (-1) : (1));
	else if (ElementalCenter.y != Target.y) ElementalCenter.y += elemental->speed * DeltaTime * ((Target.y - ElementalCenter.y < 0) ? (-1) : (1));
}
void ElementalAttacking(Elementals* elemental)
{
	if (!elemental->isSpawn || elemental->isDead || elemental->isDodging || elemental->isDefending) return;
	SDL_FPoint PlayerCenter = { player1.position.x + player1.SpriteSize.x / 2,player1.position.y + player1.SpriteSize.y / 2 };
	SDL_FPoint ElementalCenter = { elemental->hitbox.x + elemental->hitbox.w / 2,elemental->hitbox.y + elemental->hitbox.h / 2 };
	if (abs(PlayerCenter.x - ElementalCenter.x) > LEVEL_WIDTH / 2)
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
	if (abs(PlayerCenter.y - ElementalCenter.y) > LEVEL_HEIGHT / 2)
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
	SDL_FRect hitbox = Playerhitbox(player1.flip, Pos);
	if (!elemental->isAttacking && SDL_HasIntersectionF(&hitbox, &elemental->AttackBox))
	{
		if (elemental->Cooldown.GetTime() > 1000 / elemental->AttackSpeed)
		{
			if (elemental->CurrentElementalType != 5)
			{
				elemental->Attack();
				elemental->ChooseAttack();
			}
			else
			{
				if (DistanceCalculation(ElementalCenter, PlayerCenter) <= 170)
				{
					elemental->AttackType = 3;
				}
				else do { elemental->ChooseAttack(); } while (elemental->AttackType == 3);
				if (elemental->AttackType == 6) Mix_PlayChannel(-1, SoundEffects[15], 0);
				elemental->Attack();
			}
			return;
		}
		else if (!elemental->isIdling && !elemental->isHurt)
		{
			elemental->Idle();
			return;
		}
	}
	if (elemental->isAttacking)
	{
		switch (elemental->CurrentElementalType)
		{
		case 0:
			if (!SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox)) break;
			switch (elemental->AttackType)
			{
			case 3:
				if (elemental->CurrentSprite == 4)
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				break;
			case 4:
				if (elemental->CurrentSprite == 4)
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite >= 11 && elemental->CurrentSprite <= 17)
				{
					static int Current;
					if (elemental->CurrentSprite == 11) Current = 10;
					if (Current != elemental->CurrentSprite && elemental->CurrentSprite % 2 == 0)
					{
						player1.Hurt(15);
						Current = elemental->CurrentSprite;
					}
				}
				break;
			case 5:
				if (elemental->CurrentSprite == 4)
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite >= 11 && elemental->CurrentSprite <= 17)
				{
					static int Current;
					if (elemental->CurrentSprite == 11) Current = 10;
					if (Current != elemental->CurrentSprite && elemental->CurrentSprite % 2 == 0)
					{
						player1.Hurt(15);
						Current = elemental->CurrentSprite;
					}
				}
				if (elemental->CurrentSprite == 23)
				{
					player1.Hurt(80);
					elemental->CurrentSprite++;
				}
				break;
			case 6:
				if (elemental->CurrentSprite == 12)
				{
					player1.Hurt(120);
					elemental->CurrentSprite++;
				}
			}
			break;
		case 1:
			if (!SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox)) break;
			switch (elemental->AttackType)
			{
			case 3:
				if (elemental->CurrentSprite == 3)
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				break;
			case 4:
				if (elemental->CurrentSprite == 3)
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite == 14)
				{
					player1.Hurt(60);
					elemental->CurrentSprite++;
				}
				break;
			case 5:
				if (elemental->CurrentSprite == 3)
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite == 14)
				{
					player1.Hurt(60);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite == 22)
				{
					player1.Hurt(80);
					elemental->CurrentSprite++;
				}
				break;
			case 6:
				if (elemental->CurrentSprite == 13)
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite == 23)
				{
					player1.Hurt(120);
					elemental->CurrentSprite++;
				}
				break;
			}
			break;
		case 2:
			switch (elemental->AttackType)
			{
			case 3:
				if (elemental->CurrentSprite == 2 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				break;
			case 4:
				if (elemental->CurrentSprite == 2 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite >= 7 && elemental->CurrentSprite <= 11 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					static int Current;
					if (elemental->CurrentSprite == 7) Current = 6;
					if (Current != elemental->CurrentSprite && elemental->CurrentSprite % 2 == 0)
					{
						player1.Hurt(15);
						Current = elemental->CurrentSprite;
					}
				}
				break;
			case 5:
				if (elemental->CurrentSprite == 2 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					player1.Hurt(40);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite >= 7 && elemental->CurrentSprite <= 11 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					static int Current;
					if (elemental->CurrentSprite == 7) Current = 6;
					if (Current != elemental->CurrentSprite && elemental->CurrentSprite % 2 == 0)
					{
						player1.Hurt(15);
						Current = elemental->CurrentSprite;
					}
				}
				if (elemental->CurrentSprite == 19 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					SDL_FRect AttackBox = elemental->AttackBox;
					if (elemental->flip == SDL_FLIP_NONE) AttackBox.w += 130;
					if (SDL_HasIntersectionF(&AttackBox, &player1.hitbox))
					{
						player1.Hurt(60);
						elemental->CurrentSprite++;
					}
				}
				break;
			case 6:
				if (elemental->CurrentSprite == 11 || elemental->CurrentSprite == 17 || elemental->CurrentSprite == 19 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					player1.Hurt(60);
					elemental->CurrentSprite++;
				}
				break;
			}
			break;
		case 3:
			switch (elemental->AttackType)
			{
			case 3:
				if (elemental->CurrentSprite == 2 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					player1.Hurt(20);
					elemental->CurrentSprite++;
				}
				break;
			case 4:
				if (elemental->CurrentSprite == 2 || elemental->CurrentSprite == 5 || elemental->CurrentSprite == 8)
				{
					if (SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
					{
						player1.Hurt(20);
						elemental->CurrentSprite++;
					}
				}
				break;
			case 5:
				if (elemental->CurrentSprite == 2 || elemental->CurrentSprite == 5 || elemental->CurrentSprite == 8)
				{
					if (SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
					{
						player1.Hurt(20);
						elemental->CurrentSprite++;
					}
				}
				if (elemental->CurrentSprite == 18)
				{
					SDL_FRect AttackBox = elemental->AttackBox;
					if (elemental->flip == SDL_FLIP_NONE) AttackBox.w += 130;
					else
					{
						AttackBox.x -= 130;
						AttackBox.w += 130;
					}
					if (SDL_HasIntersectionF(&AttackBox, &player1.hitbox))
					{
						player1.Hurt(60);
						elemental->CurrentSprite++;
					}
				}
				break;
			case 6:
				if (elemental->CurrentSprite == 7)
				{
					if (SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
					{
						player1.isParalysed = true;
						elemental->CurrentSprite++;
					}
				}
				if (elemental->CurrentSprite == 19)
				{
					if (SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
					{
						player1.Hurt(120);
						player1.isParalysed = false;
						elemental->CurrentSprite++;
					}
				}
				break;
			}
			break;
		case 4:
			switch (elemental->AttackType)
			{
			case 3:
				if (elemental->CurrentSprite == 1 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					player1.Hurt(35);
					elemental->CurrentSprite++;
				}
				break;
			case 4:
				if ((elemental->CurrentSprite == 1 || elemental->CurrentSprite == 4) && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					player1.Hurt(35);
					elemental->CurrentSprite++;
				}
				break;
			case 5:
				if ((elemental->CurrentSprite == 1 || elemental->CurrentSprite == 4 || elemental->CurrentSprite == 8) && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					player1.Hurt(35);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite >= 11 && elemental->CurrentSprite <= 17 && SDL_HasIntersectionF(&elemental->AttackBox, &player1.hitbox))
				{
					static int Current;
					if (elemental->CurrentSprite == 11) Current = 10;
					if (Current != elemental->CurrentSprite && elemental->CurrentSprite % 2 == 0)
					{
						player1.Hurt(20);
						Current = elemental->CurrentSprite;
					}
				}
				break;
			case 6:
				if (elemental->CurrentSprite == 5)
				{
					SDL_FPoint ElementalCenter = { elemental->hitbox.x + elemental->hitbox.w / 2,elemental->hitbox.y + elemental->hitbox.h / 2 };
					SDL_FRect AttackBox = { ElementalCenter.x - 160,elemental->hitbox.y,320,elemental->hitbox.h };
					if (SDL_HasIntersectionF(&AttackBox, &player1.hitbox))
					{
						player1.Hurt(120);
						elemental->CurrentSprite++;
					}
				}
				break;
			}
			break;
		case 5:
			switch (elemental->AttackType)
			{
			case 3:
				{
					SDL_FRect AttackBox = elemental->AttackBox;
					if (elemental->flip == SDL_FLIP_NONE) AttackBox.w = 170;
					else
					{
						AttackBox.x += AttackBox.w - 170;
						AttackBox.w = 170;
					}
					if (elemental->CurrentSprite == 5 && SDL_HasIntersectionF(&AttackBox, &player1.hitbox))
					{
						player1.Hurt(25);
						elemental->CurrentSprite++;
					}
				}
				break;
			case 4:
				if (elemental->CurrentSprite == 4) 
				{
					Mix_PlayChannel(-1, SoundEffects[10], 0);
					elemental->CurrentSprite++;
				}
				if (elemental->CurrentSprite == 9)
				{
					for (int i = 0; i < Current_max_enemies; i++) if (!Projectiles[i]->isShot)
					{
						Projectiles[i]->Origin = ElementalCenter;
						Projectiles[i]->type = 3;
						Projectiles[i]->Target = ElementalAiming(PlayerCenter,ElementalCenter,3);
						Projectiles[i]->Shoot();
						break;
					}
					elemental->CurrentSprite++;
				}
				break;
			case 5:
				if (elemental->CurrentSprite == 7)
				{
					for (int i = 0; i < Current_max_enemies; i++) if (!Projectiles[i]->isShot)
					{
						Projectiles[i]->isShot = true;
						Projectiles[i]->type = 5;
						Projectiles[i]->Target = ElementalAiming(PlayerCenter, ElementalCenter, 5);
						Projectiles[i]->Origin = ElementalCenter;
						Projectiles[i]->Delay.Start();
						break;
					}
					elemental->CurrentSprite++;
				}
				break;
			case 6:
				static bool Shot = false;
				if (elemental->CurrentSprite == 9 && !Shot)
				{
					Shot = true;
					for (int i = 0; i < Current_max_enemies; i++) if (!Projectiles[i]->isShot)
					{
						Projectiles[i]->type = 6;
						Projectiles[i]->Target = PlayerCenter;
						Projectiles[i]->Origin = elemental->position;
						Projectiles[i]->flip = elemental->flip;
						Projectiles[i]->Shoot();
						break;
					}
				}
				if (elemental->CurrentSprite == 13 && Shot) Shot = false;
				break;
			}
			break;
		}
		if (elemental->CurrentSprite >= elemental->NumOfSprites - 1)
		{
			elemental->isAttacking = false;
			elemental->Idle();
		}
	}
}
void ElementalBulletCollision(Elementals* elemental)
{
	if (elemental->isDodging) return;
	for (int i = 0; i < Max_Bullets; i++) if (player1.PlayerWeapon.bullets[i]->isShot && !player1.PlayerWeapon.bullets[i]->Decayed)
	{
		if (SDL_HasIntersectionF(&player1.PlayerWeapon.bullets[i]->hitbox, &elemental->hitbox))
		{
			if (player1.PlayerWeapon.bullets[i]->CurrentWeapon != 2)
			{
				elemental->Health -= player1.PlayerWeapon.bullets[i]->Damage;
				if (!elemental->isDead)
				{
					if (elemental->Health <= 0) elemental->Death();
					else if (!elemental->isAttacking && !elemental->isDodging) elemental->Hurt();
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
void ElementalDodge(Elementals* elemental)
{
	if (elemental->isDead || elemental->isHurt || elemental->isAttacking || elemental->isDodging || elemental->DodgeCooldown.GetTime() < 2500) return;
	for (int i = 0; i < Max_Bullets; i++)
	{
		if (!player1.PlayerWeapon.bullets[i]->isShot || player1.PlayerWeapon.bullets[i]->Decayed) return;
		if (abs((atan2(player1.PlayerWeapon.bullets[i]->origin.y - elemental->ElementalCenter.y, player1.PlayerWeapon.bullets[i]->origin.x - elemental->ElementalCenter.x) * 180.000 / 3.14159265 + 180) - player1.PlayerWeapon.bullets[i]->angle) <= 10)
		{
			if (DistanceCalculation(player1.PlayerWeapon.bullets[i]->position, elemental->ElementalCenter) <= 400)
			{
				elemental->Dodge();
				return;
			}
		}
	}
}
void ElementalBlockMelee(Elementals* elemental)
{
	if (elemental->isHurt || elemental->isDefending || elemental->isDodging) return;
	if (elemental->BlockCooldown.GetTime() >= 5000)
	{
		elemental->Defend();
		return;
	}
}
void ElementalSlashDamage(Elementals* elemental, bool &ElementalDamaged)
{
	SDL_FPoint PlayerCenter = player1.position; PlayerCenter.x += 50; PlayerCenter.y += 50;
	float enemyAngle = AngleCalculation(elemental->ElementalCenter, PlayerCenter, &player1.flip);
	float enemyDistance = DistanceCalculation(elemental->ElementalCenter, PlayerCenter);
	if (abs(enemyAngle - player1.PlayerWeapon.angle) <= 5 && enemyDistance <= player1.PlayerWeapon.range && !ElementalDamaged)
	{
		ElementalBlockMelee(elemental);
		if (elemental->isDefending)
		{
			Mix_HaltChannel(1);
			Mix_PlayChannel(1, SoundEffects[12], 0);
		}
		if (!elemental->isDefending && !elemental->isDodging)
		{
			Mix_HaltChannel(1);
			Mix_PlayChannel(-1, SoundEffects[13], 0);
			elemental->Health -= player1.PlayerWeapon.damage;
			if (elemental->Health <= 0) elemental->Death();
			else
			{
				elemental->Hurt();
			}
			ElementalDamaged = true;
		}
	}
}
SDL_FPoint ElementalAiming(SDL_FPoint PlayerCenter,SDL_FPoint ElementalCenter,int type)
{
	if (SCORE < 50) return PlayerCenter;
	float Distance = DistanceCalculation(PlayerCenter, ElementalCenter);
	float ScaleDistance;
	if (type == 3) ScaleDistance = Distance * 0.45;
	else if (type == 5) ScaleDistance = 550;
	SDL_FPoint Target = { PlayerCenter.x + player1.Direction.x * ScaleDistance ,PlayerCenter.y + player1.Direction.y * ScaleDistance };
	return Target;
}
#include <SDL.h>
#include "elementalAI.h"
#include "global.h"
#include "gameobjects.h"
#include "player.h"
#include <cmath>
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
		SDL_FPoint ElementalCenter = { elemental->Hitbox.x + elemental->Hitbox.w / 2,elemental->Hitbox.y + elemental->Hitbox.h / 2 };
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
		SDL_FRect Hitbox = PlayerHitbox(player1.flip, Pos);
		if (PlayerCenter.x < ElementalCenter.x - 5 && elemental->flip == SDL_FLIP_NONE)
		{
			elemental->flip = SDL_FLIP_HORIZONTAL;
			elemental->CalculateBoxes();
		}
		else if (PlayerCenter.x > ElementalCenter.x + 5 && elemental->flip == SDL_FLIP_HORIZONTAL)
		{
			elemental->flip = SDL_FLIP_NONE;
			elemental->CalculateBoxes();
		}
		if (elemental->isAttacking) return;
		if (!elemental->isHurt && !SDL_HasIntersectionF(&Hitbox, &elemental->AttackBox))
		{
			SDL_FPoint Target = { PlayerCenter.x + (ElementalCenter.x - PlayerCenter.x < 0) ? (PlayerCenter.x - elemental->AttackBox.w) : (PlayerCenter.x + elemental->AttackBox.w),PlayerCenter.y };
			if (!elemental->isMoving && !elemental->isDodging) elemental->Run();
			SDL_FPoint TempPos = elemental->position;
			ElementalMoving(elemental, ElementalCenter, Target);
			elemental->position.x = ElementalCenter.x - elemental->Hitbox.w / 2 - ElementalSpritesInfo[elemental->CurrentElementalType][11][0];
			elemental->position.y = ElementalCenter.y - elemental->Hitbox.h / 2 - ElementalSpritesInfo[elemental->CurrentElementalType][11][1];
			if (elemental->position.x < -elemental->SpriteSize.x) elemental->position.x = LEVEL_WIDTH - elemental->SpriteSize.x;
			if (elemental->position.y < -elemental->SpriteSize.y) elemental->position.y = LEVEL_HEIGHT - elemental->SpriteSize.y;
			if (elemental->position.x > LEVEL_WIDTH) elemental->position.x = 0;
			if (elemental->position.y > LEVEL_HEIGHT) elemental->position.y = 0;
			elemental->CalculateBoxes();
			if (SDL_HasIntersectionF(&elemental->Hitbox, &Hitbox))
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
void ElementalMoving(Elementals* elemental, SDL_FPoint& ElementalCenter, SDL_FPoint& Target)
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
	SDL_FPoint ElementalCenter = { elemental->Hitbox.x + elemental->Hitbox.w / 2,elemental->Hitbox.y + elemental->Hitbox.h / 2 };
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
	SDL_FRect Hitbox = PlayerHitbox(player1.flip, Pos);
	if (!elemental->isAttacking && SDL_HasIntersectionF(&Hitbox, &elemental->AttackBox))
	{
		if (elemental->Cooldown.GetTime() > 1000 / elemental->AttackSpeed)
		{
			elemental->Attack();
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
		if (SDL_HasIntersectionF(&elemental->AttackBox, &player1.Hitbox))
			switch (elemental->CurrentElementalType)
			{
			case 0:
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
		if (SDL_HasIntersectionF(&player1.PlayerWeapon.bullets[i]->Hitbox, &elemental->Hitbox))
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
			if (DistanceCalculation(player1.PlayerWeapon.bullets[i]->BulletPosition, elemental->ElementalCenter) <= 500)
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
	if (elemental->BlockCooldown.GetTime() >=5000)
	{
		elemental->Defend();
		return;
	}
}
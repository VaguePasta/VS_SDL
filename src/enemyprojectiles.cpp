#include "enemyprojectiles.h"
#include "gameobjects.h"
#include "global.h"
#include "sounds.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <cmath>
using namespace std;
void LoadEnemiesProjectileSpritesFromDisk()
{
	EnemyProjectileSprites[0] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/Arrow.PNG");
	EnemyProjectileSprites[1] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/FireBomb.PNG");
	EnemyProjectileSprites[2] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/DarkBolt.PNG");
}
EnemyProjectiles::EnemyProjectiles()
{
	isShot = false;
	Decayed = false;
}
EnemyProjectiles::~EnemyProjectiles()
{
};
void EnemyProjectiles::ChooseType(int CurrentEnemy)
{
	switch (CurrentEnemy)
	{
	case 3:
		type = 0;
		break;
	case 4:
		type = rand() % 2 + 1;
		break;
	}
}
void EnemyProjectiles::Shoot()
{
	isShot = true;
	ProjectileTexture = EnemyProjectileSprites[type];
	ProjectileSize.x = EnemyProjectilesInfo[type][1];
	ProjectileSize.y = EnemyProjectilesInfo[type][2];
	isAnimated = EnemyProjectilesInfo[type][6];
	damage = EnemyProjectilesInfo[type][5];
	if (isAnimated)
	{
		Projectile.CurrentSprite = 0;
		Projectile.NumOfSprites = EnemyProjectilesInfo[type][7];
		Projectile.framespeed = EnemyProjectilesInfo[type][8];
		Projectile.texture = ProjectileTexture;
		Projectile.tempframe = 1;
		SDL_QueryTexture(Projectile.texture, NULL, NULL, &Projectile.texturesize.x, &Projectile.texturesize.y);
		Projectile.framecalc();
	}
	switch (type)
	{
	case 0:
		distancetraveled = 0;
		speed = EnemyProjectilesInfo[type][3];
		range = EnemyProjectilesInfo[type][4];
		damage = EnemyProjectilesInfo[type][5];
		angle = atan2(Target.y - Origin.y, Target.x - Origin.x);
		FPosition.x = Origin.x;
		FPosition.y = Origin.y;
		Position.x = Origin.x;
		Position.y = Origin.y;
		break;
	case 1:
		if (Target.x < -192) Target.x += LEVEL_WIDTH;
		if (Target.x > LEVEL_WIDTH) Target.x -= LEVEL_WIDTH;
		if (Target.y < -192) Target.y += LEVEL_HEIGHT;
		if (Target.x > LEVEL_HEIGHT) Target.y -= LEVEL_HEIGHT;
		Position.x = Target.x;
		Position.y = Target.y;
		Hitbox = { Position.x - 96,Position.y - 96,192,192 };
		angle = 0;
		damage = EnemyProjectilesInfo[type][5];
		ProjectileSprite = { Position.x - 96,Position.y - 96,192,192 };
		Mix_PlayChannel(-1, SoundEffects[8], 0);
		break;
	case 2:
		if (Target.x < -60) Target.x += LEVEL_WIDTH;
		if (Target.x > LEVEL_WIDTH) Target.x -= LEVEL_WIDTH;
		if (Target.y < -60) Target.y += LEVEL_HEIGHT;
		if (Target.x > LEVEL_HEIGHT) Target.y -= LEVEL_HEIGHT;
		Position.x = Target.x;
		Position.y = Target.y;
		Hitbox = { Position.x - 35,Position.y - 25,60,60 };
		angle = 0;
		damage = EnemyProjectilesInfo[type][5];
		ProjectileSprite = { Position.x - 64,Position.y - 130,128,176 };
		Mix_PlayChannel(-1, SoundEffects[9], 0);
		break;
	}
}
void EnemyProjectiles::Update()
{
	if (type == 0)
	{
		distancetraveled += speed * DeltaTime;
		FPosition.x = Origin.x + distancetraveled * cos(angle) - ProjectileSize.x / 2;
		FPosition.y = Origin.y + distancetraveled * sin(angle) - ProjectileSize.y / 2;
		Position.x = int(round(FPosition.x));
		Position.y = int(round(FPosition.y));
		if (Position.x < -ProjectileSize.x)
		{
			Origin.x += LEVEL_WIDTH;
			Position.x = LEVEL_WIDTH;
		}
		if (Position.x > LEVEL_WIDTH)
		{
			Origin.x -= LEVEL_WIDTH;
			Position.x = 0;
		}
		if (Position.y < -ProjectileSize.y)
		{
			Origin.y += LEVEL_HEIGHT;
			Position.y = LEVEL_HEIGHT;
		}
		if (Position.y > LEVEL_HEIGHT)
		{
			Origin.y -= LEVEL_HEIGHT;
			Position.y = 0;
		}
		ProjectileSprite = { Position.x,Position.y,ProjectileSize.x,ProjectileSize.y };
		if (angle >= 1.57)
			Hitbox = { (int)round(Position.x + 17 * sin(angle)),(int)round(Position.y + 10 * sin(angle)),12,12 };
		if (angle >= -3.14 && angle <= -1.57)
			Hitbox = { (int)round(Position.x - 15 * sin(angle)),(int)round(Position.y + 10 * sin(angle)),12,12 };
		if (angle >= 0 && angle < 1.57)
			Hitbox = { (int)round(Position.x + 35 - 15 * sin(angle)),(int)round(Position.y + 10 * sin(angle)),12,12 };
		if (angle > -1.57 && angle < 0)
			Hitbox = { (int)round(Position.x + 35 + 15 * sin(angle)),(int)round(Position.y + 10 * sin(angle)),12,12 };
	}
	else if (type == 1)
	{
		Projectile.animation();
		if (Projectile.CurrentSprite == 8) Projectile.framespeed = 11;
	}
	else if (type == 2)
	{
		Projectile.animation();
		if (Projectile.CurrentSprite == 4) Projectile.framespeed = 11;
	}
}
void EnemyProjectiles::Decay()
{
	if (!isAnimated)
	{
		if (distancetraveled > range)
			Decayed = true;
	}
	else if (Projectile.CurrentSprite >= Projectile.NumOfSprites - 1) Decayed = true;
}
void DrawEnemyProjectiles(SDL_Rect* ProjectileFrame, int i)
{
	SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &Projectiles[i]->ProjectileSprite, (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
	SDL_Rect TempRect[3];
	TempRect[0] = Projectiles[i]->ProjectileSprite;
	TempRect[1] = Projectiles[i]->ProjectileSprite;
	TempRect[2] = Projectiles[i]->ProjectileSprite;
	if (Projectiles[i]->Position.x - player1.SpriteSize < 0)
	{
		TempRect[0].x += LEVEL_WIDTH;
		if (Projectiles[i]->Position.y - player1.SpriteSize < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].y += LEVEL_HEIGHT;
			SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[1], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[2], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
		}
		SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[0], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
	}
	if (Projectiles[i]->Position.x > LEVEL_WIDTH - Projectiles[i]->ProjectileSprite.w)
	{
		TempRect[0].x -= LEVEL_WIDTH;
		if (Projectiles[i]->Position.y > LEVEL_HEIGHT - Projectiles[i]->ProjectileSprite.h)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[1], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[2], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
		}
		SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[0], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
	}
	if (Projectiles[i]->Position.y - player1.SpriteSize < 0)
	{
		TempRect[0].y += LEVEL_HEIGHT;
		if (Projectiles[i]->Position.x > LEVEL_WIDTH - Projectiles[i]->ProjectileSprite.w)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].x -= LEVEL_WIDTH;
			SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[1], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[2], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
		}
		SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[0], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
	}
	if (Projectiles[i]->Position.y > LEVEL_HEIGHT - Projectiles[i]->ProjectileSprite.h)
	{
		TempRect[0].y -= LEVEL_HEIGHT;
		if (Projectiles[i]->Position.x - player1.SpriteSize < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[1], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[2], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
		}
		SDL_RenderCopyEx(renderer, Projectiles[i]->ProjectileTexture, ProjectileFrame, &TempRect[0], (Projectiles[i]->angle) * 57.2958, NULL, SDL_FLIP_NONE);
	}
}

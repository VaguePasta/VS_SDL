#include "enemyprojectiles.h"
#include "gameobjects.h"
#include "global.h"
#include "sounds.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <cmath>
using namespace std;
void LoadEnemyProjectileSpritesFromDisk()
{
	EnemyProjectileSprites[0] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/Arrow.PNG");
	EnemyProjectileSprites[1] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/FireBomb.PNG");
	EnemyProjectileSprites[2] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/DarkBolt.PNG");
	EnemyProjectileSprites[3] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/LeafArrow.PNG");
	EnemyProjectileSprites[4] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/ArrowEntangle.PNG");
	EnemyProjectileSprites[5] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/ArrowShower.PNG");
	EnemyProjectileSprites[6] = IMG_LoadTexture(renderer, "resources/enemyprojectiles/LaserBeam.PNG");
}
std::vector<std::vector<float>> EnemyProjectilesInfo =   //STT, kich co_x, kich co_y, toc do, tam bay, sat thuong, co animation?,so frame,toc do chuyen frame
{
	{0,48,10,11,1000,25,0,0,0},
	{1,192,192,0,0,50,1,14,5},
	{2,128,176,0,0,50,1,11,7},
	{3,57.6,12,12,1100,30,0,0,0},
	{4,256,128,0,0,0,1,8,8},
	{5,256,128,0,0,30,1,18,5},
	{6,480,240,0,0,120,1,5,8},
};
EnemyProjectiles::EnemyProjectiles()
{
	isShot = false;
	Arrived = false;
	Decayed = false;
	flip = SDL_FLIP_NONE;
}
EnemyProjectiles::~EnemyProjectiles()
{
}
void EnemyProjectiles::ChooseType(int CurrentMinion)
{
	switch (CurrentMinion)
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
	texture = EnemyProjectileSprites[type];
	SpriteSize.x = EnemyProjectilesInfo[type][1];
	SpriteSize.y = EnemyProjectilesInfo[type][2];
	isAnimated = EnemyProjectilesInfo[type][6];
	damage = EnemyProjectilesInfo[type][5];
	Arrived = true;
	if (isAnimated)
	{
		CurrentSprite = 0;
		NumOfSprites = EnemyProjectilesInfo[type][7];
		framespeed = EnemyProjectilesInfo[type][8];
		texture = texture;
		tempframe = 1;
		SDL_QueryTexture(texture, NULL, NULL, &texturesize.x, &texturesize.y);
		framecalc();
	}
	else
	{
		delete frame;
		frame = nullptr;
	}
	switch (type)
	{
	case 0: case 3:
		distancetraveled = 0;
		speed = EnemyProjectilesInfo[type][3];
		range = EnemyProjectilesInfo[type][4];
		damage = EnemyProjectilesInfo[type][5];
		angle = atan2(Target.y - Origin.y, Target.x - Origin.x) * 180 / 3.14159;
		position = Origin;
		break;
	case 1:
		if (Target.x < -192) Target.x += LEVEL_WIDTH;
		else if (Target.x > LEVEL_WIDTH - 192) Target.x -= LEVEL_WIDTH;
		if (Target.y < -192) Target.y += LEVEL_HEIGHT;
		else if (Target.y > LEVEL_HEIGHT - 192) Target.y -= LEVEL_HEIGHT;
		position = Target;
		hitbox = { position.x - 96,position.y - 96,192,192 };
		angle = 0;
		damage = EnemyProjectilesInfo[type][5];
		position.x -= 96;
		position.y -= 96;
		SpriteSize = { 192,192 };
		Mix_PlayChannel(-1, SoundEffects[8], 0);
		break;
	case 2:
		if (Target.x < -60) Target.x += LEVEL_WIDTH;
		else if (Target.x > LEVEL_WIDTH -60) Target.x -= LEVEL_WIDTH;
		if (Target.y < -60) Target.y += LEVEL_HEIGHT;
		else if (Target.y > LEVEL_HEIGHT -60) Target.y -= LEVEL_HEIGHT;
		position = Target;
		hitbox = { position.x - 35,position.y - 25,60,60 };
		angle = 0;
		damage = EnemyProjectilesInfo[type][5];
		position.x -= 64;
		position.y -= 130;
		SpriteSize = { 128,176 };
		Mix_PlayChannel(-1, SoundEffects[9], 0);
		break;
	case 4:
		position = Target;
		hitbox = { position.x , position.y - 64, 128, 128 };
		angle = 0;
		damage = EnemyProjectilesInfo[type][5];
		if (flip == SDL_FLIP_NONE) position.x -= 235; else position.x -= 225;
		position.y -= 120;
		SpriteSize = { 512,256 };
		break;
	case 5:
		if (Target.x < -150) Target.x += LEVEL_WIDTH;
		else if (Target.x > LEVEL_WIDTH - 150) Target.x -= LEVEL_WIDTH;
		if (Target.y < -60) Target.y += LEVEL_HEIGHT;
		else if (Target.y > LEVEL_HEIGHT - 60) Target.y -= LEVEL_HEIGHT;
		position = Target;
		hitbox = { position.x - 80, position.y - 10,150,60 };
		angle = 0;
		damage = EnemyProjectilesInfo[type][5];
		position.x -= 260;
		position.y -= 200;
		SpriteSize = { 512,256 };
		break;
	case 6:
		Target = { player1.position.x + 50,player1.position.y + 50 };
		if (abs(Target.x - Origin.x) > LEVEL_WIDTH / 2)
		{
			if (Target.x < Origin.x) Origin.x -= LEVEL_WIDTH;
			else Origin.x += LEVEL_WIDTH;
		}
		if (abs(Target.y - Origin.y) > LEVEL_HEIGHT / 2)
		{
			if (Target.y < Origin.y) Origin.y -= LEVEL_HEIGHT;
			else Origin.y += LEVEL_HEIGHT;
		}
		position.y = Origin.y;
		angle = 0;
		SpriteSize = { 480,240 };
		if (flip == SDL_FLIP_HORIZONTAL)
		{
			position.x = Origin.x - 480;
			hitbox = { position.x,position.y + 172,762,30};
		}
		else
		{
			position.x = Origin.x + 540;
			hitbox = { position.x - 278,position.y +172,762,30 };
		}
		position.y += 67;
		break;
	}
}
void EnemyProjectiles::Update()
{
	if (type == 0 || type == 3)
	{
		distancetraveled += speed * DeltaTime;
		position.x = Origin.x + distancetraveled * cos(angle * 3.14159 / 180) -SpriteSize.x / 2;
		position.y = Origin.y + distancetraveled * sin(angle * 3.14159 / 180) -SpriteSize.y / 2;
		if (position.x < -SpriteSize.x)
		{
			Origin.x += LEVEL_WIDTH;
			position.x += LEVEL_WIDTH;
		}
		if (position.x > LEVEL_WIDTH - SpriteSize.x)
		{
			Origin.x -= LEVEL_WIDTH;
			position.x -= LEVEL_WIDTH;
		}
		if (position.y < -SpriteSize.y)
		{
			Origin.y += LEVEL_HEIGHT;
			position.y += LEVEL_HEIGHT;
		}
		if (position.y > LEVEL_HEIGHT - SpriteSize.y)
		{
			Origin.y -= LEVEL_HEIGHT;
			position.y -= LEVEL_HEIGHT;
		}
		SpriteBox = { position.x,position.y,float(SpriteSize.x),float(SpriteSize.y) };
		if (angle >= 90)
			hitbox = { position.x + 17 * float(sin(angle * 3.14159 / 180)),position.y + 10 * float(sin(angle * 3.14159 / 180)),12,12 };
		if (angle >= -180 && angle <= -90)
			hitbox = { position.x - 15 * float(sin(angle * 3.14159 / 180)),position.y + 10 * float(sin(angle * 3.14159 / 180)),12,12 };
		if (angle >= 0 && angle < 90)
			hitbox = { position.x + 35 - 15 * float(sin(angle * 3.14159 / 180)),position.y + 10 * float(sin(angle * 3.14159 / 180)),12,12 };
		if (angle > -90 && angle < 0)
			hitbox = { position.x + 35 + 15 * float(sin(angle * 3.14159 / 180)),position.y + 10 * float(sin(angle * 3.14159 / 180)),12,12 };
	}
	else if (type == 1)
	{
		if (CurrentSprite == 8) framespeed = 11;
	}
	else if (type == 2)
	{
		if (CurrentSprite == 4) framespeed = 11;
	}
}
void EnemyProjectiles::Decay()
{
	if (!isAnimated)
	{
		if (distancetraveled > range)
		{
			Decayed = true;
		}
	}
	else if (CurrentSprite >= NumOfSprites - 1)
	{
		Decayed = true;
		if (type == 4) player1.isParalysed = false;
	}
}
void EnemyProjectilesProcessing()
{
	for (int i = 0; i < Current_max_enemies; i++) if (Projectiles[i]->isShot)
	{
		if (Projectiles[i]->type == 5 && !Projectiles[i]->Arrived)
		{
			if (Projectiles[i]->Delay.GetTime() >= 1500) Projectiles[i]->Shoot();
			else continue;
		}
		Projectiles[i]->Update();
		Projectiles[i]->Decay();
		if (!Projectiles[i]->Decayed)
		{
			switch (Projectiles[i]->type)
			{
			case 0:
				if (SDL_HasIntersectionF(&Projectiles[i]->hitbox, &player1.hitbox))
				{
					Projectiles[i]->Decayed = true;
					player1.Hurt(Projectiles[i]->damage);
				}
				break;
			case 1: case 2:
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
				if (Projectiles[i]->CurrentSprite == DamageFrame)
				{
					if (SDL_HasIntersectionF(&Projectiles[i]->hitbox, &player1.hitbox))
					{
						player1.Hurt(Projectiles[i]->damage);
					}
					Projectiles[i]->CurrentSprite++;
				}
				break;
			case 3:
				if (SDL_HasIntersectionF(&Projectiles[i]->hitbox, &player1.hitbox))
				{
					if (rand() % 10 + 1 <= 7)
					{
						Projectiles[i]->Decayed = true;
						player1.Hurt(Projectiles[i]->damage);
					}
					else
					{
						SDL_FPoint TempTarget = Projectiles[i]->position;
						SDL_RendererFlip  flip = (player1.position.x > Projectiles[i]->Origin.x) ? (SDL_FLIP_NONE) : (SDL_FLIP_HORIZONTAL);
						delete Projectiles[i];
						Projectiles[i] = new EnemyProjectiles();
						Projectiles[i]->Target = TempTarget;
						Projectiles[i]->flip = flip;
						Projectiles[i]->type = 4;
						Projectiles[i]->Shoot();
						player1.isParalysed = true;
					}
				}
				break;
			case 5:
				static int Current;
				if (Projectiles[i]->CurrentSprite == 1) Current = 0;
				if (!SDL_HasIntersectionF(&Projectiles[i]->hitbox, &player1.hitbox)) break;
				if (Current != Projectiles[i]->CurrentSprite && Projectiles[i]->CurrentSprite >= 1 && Projectiles[i]->CurrentSprite <= 9 && Projectiles[i]->CurrentSprite % 2 == 0)
				{
					player1.Hurt(10);
					Current = Projectiles[i]->CurrentSprite;
				}
				break;
			case 6:
				static bool Check = false;
				if (Projectiles[i]->CurrentSprite == 3) Check = false;
				if (SDL_HasIntersectionF(&Projectiles[i]->hitbox, &player1.hitbox) && Projectiles[i]->CurrentSprite == 0 && !Check)
				{
					player1.Hurt(120);
					Check = true;
				}
				break;
			}
		}
		if (Projectiles[i]->Decayed)
		{
			delete Projectiles[i];
			Projectiles[i] = new EnemyProjectiles();
		}
	}
}
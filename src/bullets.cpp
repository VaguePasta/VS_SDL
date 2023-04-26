#include <SDL.h>
#include <SDL_image.h>
#include "bullets.h"
#include "global.h"
#include "gameobjects.h"
#include "sounds.h"
#include <cmath>
SDL_FPoint CalculateOrigin()
{
	SDL_FPoint BulletOrigin;
	int x_weapon = (player1.flip == SDL_FLIP_NONE) ? (19) : (-19);
	int y_weapon = 16;
	BulletOrigin.x = player1.position.x + player1.SpriteSize.x / 2 + x_weapon;
	BulletOrigin.y = player1.position.y + player1.SpriteSize.y / 2 + y_weapon;
	if (BulletOrigin.x < 0) BulletOrigin.x += LEVEL_WIDTH;
	if (BulletOrigin.y < 0) BulletOrigin.y += LEVEL_HEIGHT;
	if (BulletOrigin.x > LEVEL_WIDTH) BulletOrigin.x -= LEVEL_WIDTH;
	if (BulletOrigin.y > LEVEL_HEIGHT) BulletOrigin.y -= LEVEL_HEIGHT;
	return BulletOrigin;
}
void LoadBulletSpritesFromDisk()
{
	BulletSprites[0] = IMG_LoadTexture(renderer, "resources/bullets/AK47_Bullets.PNG");
	BulletSprites[1] = IMG_LoadTexture(renderer, "resources/bullets/Assault_Rifle_Bullets.PNG");
	BulletSprites[2] = IMG_LoadTexture(renderer, "resources/bullets/Rocket.PNG");
	EffectSprites[0] = IMG_LoadTexture(renderer, "resources/effects/Explosion.PNG");
	EffectSprites[1] = IMG_LoadTexture(renderer, "resources/effects/Slash.PNG");
}
std::vector <std::vector<float>> BulletSpritesList =                //So thu tu, Kich co dan, co animation?,so frame,toc do frame
{
	{0,50,0},
	{1,50,0},
	{2,60,1,8,4},
	{3,0,0},
};
bullet::bullet(int Weapon)
{
	CurrentWeapon = Weapon;
	isShot = false;
	Decayed = false;
	texture = BulletSprites[CurrentWeapon];
	SpriteSize = { BulletSpritesList[CurrentWeapon][1],BulletSpritesList[CurrentWeapon][1] };
	isAnimated = BulletSpritesList[CurrentWeapon][2];
	angle = 0;
	DistanceTraveled = 0;
	if (isAnimated)
	{
		NumOfSprites = BulletSpritesList[CurrentWeapon][3];
		framespeed = BulletSpritesList[CurrentWeapon][4];
		tempframe = 1;
		CurrentSprite = 0;
		texture = BulletSprites[CurrentWeapon];
		SDL_QueryTexture(texture, nullptr, nullptr, &texturesize.x, &texturesize.y);
		framecalc();
	}
	else
	{
		delete frame;
		frame = nullptr;
	}
	angle = 0;
	hitbox = { 0,0,0,0 };
}
bullet::~bullet()
{}
bool bullet::Decay()
{
	return ((position.x + SpriteSize.x) < 0 || (position.y + SpriteSize.y) < 0 || position.x > LEVEL_WIDTH || position.y > LEVEL_HEIGHT || DistanceTraveled >= BulletRange);
}
void bullet::Update()
{
	DistanceTraveled += BulletSpeed * DeltaTime;
	position.x = origin.x + DistanceTraveled * cos(angle * 0.017453) - SpriteSize.x / 2;
	position.y = origin.y + DistanceTraveled * sin(angle * 0.017453) - SpriteSize.y / 2;
	if (position.x < -SpriteSize.x)
	{
		origin.x += LEVEL_WIDTH;
		position.x = LEVEL_WIDTH - SpriteSize.x;
	}
	if (position.y < -SpriteSize.y)
	{
		origin.y += LEVEL_HEIGHT;
		position.y = LEVEL_HEIGHT - SpriteSize.y;
	}
	if (position.x > LEVEL_WIDTH)
	{
		origin.x -= LEVEL_WIDTH;
		position.x = 0;
	}
	if (position.y > LEVEL_HEIGHT)
	{
		origin.y -= LEVEL_HEIGHT;
		position.y = 0;
	}
	hitbox = { position.x + SpriteSize.x / 3,position.y + SpriteSize.y / 3, SpriteSize.x / 3, SpriteSize.y };
	SpriteBox = { position.x,position.y,SpriteSize.x,SpriteSize.y };
}
void BulletExplosion(bullet* CurrentBullet)
{
	CurrentBullet->Decayed = true;
	SDL_FRect DamageSite = { CurrentBullet->position.x - 120,CurrentBullet->position.y - 120,240,240 };
	for (int j = 0; j < Current_max_minions; j++) if (SDL_HasIntersectionF(&minion[j]->hitbox, &DamageSite))
	{
		minion[j]->Health -= CurrentBullet->Damage;
		if (!minion[j]->isDead)
		{
			if (minion[j]->Health <= 0) minion[j]->Death();
			else if (!minion[j]->isAttacking) minion[j]->Hurt();
		}
	}
	for (int j = 0; j < Current_max_elementals; j++) if (SDL_HasIntersectionF(&elemental[j]->hitbox, &DamageSite))
	{
		elemental[j]->Health -= CurrentBullet->Damage;
		if (!elemental[j]->isDead)
		{
			if (elemental[j]->Health <= 0) elemental[j]->Death();
			else if (!elemental[j]->isAttacking) elemental[j]->Hurt();
		}
	}
	CurrentBullet->Explosion(8, 8);
	Mix_HaltChannel(1);
	Mix_PlayChannel(1, SoundEffects[3], 0);
}
void bullet::Explosion(int NumOfFrame, int _framespeed)
{
	Decayed = true;
	NumOfSprites = NumOfFrame;
	framespeed = _framespeed;
	tempframe = 1;
	CurrentSprite = 0;
	angle = 0;
	texture = EffectSprites[0];
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SpriteSize = { 240,240 };
	position.x -= 120;
	position.y -= 120;
	SDL_QueryTexture(texture, nullptr, nullptr, &texturesize.x, &texturesize.y);
	framecalc();
}

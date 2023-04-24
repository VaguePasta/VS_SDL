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
std::vector <std::vector<int>> BulletSpritesList =                //So thu tu, Kich co dan, co animation?,so frame,toc do frame
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
	bullettexture = BulletSprites[CurrentWeapon];
	BulletSize = BulletSpritesList[CurrentWeapon][1];
	isAnimated = BulletSpritesList[CurrentWeapon][2];
	angle = 0;
	DistanceTraveled = 0;
	if (isAnimated)
	{
		BulletAnimation.NumOfSprites = BulletSpritesList[CurrentWeapon][3];
		BulletAnimation.framespeed = BulletSpritesList[CurrentWeapon][4];
		BulletAnimation.tempframe = 1;
		BulletAnimation.CurrentSprite = 0;
		BulletAnimation.texture = BulletSprites[CurrentWeapon];
		SDL_QueryTexture(BulletAnimation.texture, NULL, NULL, &BulletAnimation.texturesize.x, &BulletAnimation.texturesize.y);
		BulletAnimation.framecalc();
	}
	angle = 0;
	Hitbox = { 0,0,0,0 };
}
bullet::~bullet()
{}
bool bullet::Decay()
{
	return ((BulletPosition.x + BulletSize) < 0 || (BulletPosition.y + BulletSize) < 0 || BulletPosition.x > LEVEL_WIDTH || BulletPosition.y > LEVEL_HEIGHT || DistanceTraveled >= BulletRange);
}
void bullet::Update()
{
	DistanceTraveled += BulletSpeed * DeltaTime;
	BulletPosition.x = origin.x + DistanceTraveled * cos(angle * 0.017453) - BulletSize / 2;
	BulletPosition.y = origin.y + DistanceTraveled * sin(angle * 0.017453) - BulletSize / 2;
	if (BulletPosition.x < -BulletSize)
	{
		origin.x += LEVEL_WIDTH;
		BulletPosition.x = LEVEL_WIDTH - BulletSize;
	}
	if (BulletPosition.y < -BulletSize)
	{
		origin.y += LEVEL_HEIGHT;
		BulletPosition.y = LEVEL_HEIGHT - BulletSize;
	}
	if (BulletPosition.x > LEVEL_WIDTH)
	{
		origin.x -= LEVEL_WIDTH;
		BulletPosition.x = 0;
	}
	if (BulletPosition.y > LEVEL_HEIGHT)
	{
		origin.y -= LEVEL_HEIGHT;
		BulletPosition.y = 0;
	}
	Hitbox = { BulletPosition.x + BulletSize / 3,BulletPosition.y + BulletSize / 3,float(BulletSize / 3),float(BulletSize / 3) };
	BulletSprite = { BulletPosition.x,BulletPosition.y,float(BulletSize),float(BulletSize) };
}
void BulletExplosion(bullet* CurrentBullet)
{
	CurrentBullet->Decayed = true;
	SDL_FRect DamageSite = { CurrentBullet->BulletPosition.x - 120,CurrentBullet->BulletPosition.y - 120,240,240 };
	for (int j = 0; j < Current_max_minions; j++) if (SDL_HasIntersectionF(&minion[j]->Hitbox, &DamageSite))
	{
		minion[j]->Health -= CurrentBullet->Damage;
		if (!minion[j]->isDead)
		{
			if (minion[j]->Health <= 0) minion[j]->Death();
			else if (!minion[j]->isAttacking) minion[j]->Hurt();
		}
	}
	for (int j = 0; j < Current_max_elementals; j++) if (SDL_HasIntersectionF(&elemental[j]->Hitbox, &DamageSite))
	{
		elemental[j]->Health -= CurrentBullet->Damage;
		if (!elemental[j]->isDead)
		{
			if (elemental[j]->Health <= 0) elemental[j]->Death();
			else if (!elemental[j]->isAttacking) elemental[j]->Hurt();
		}
	}
	CurrentBullet->Explosion(8, 8);
	Mix_HaltChannel(PlayingChannel);
	Mix_PlayChannel(-1, SoundEffects[3], 0);
}
void bullet::Explosion(int NumOfFrame, int framespeed)
{
	Decayed = true;
	BulletAnimation.NumOfSprites = NumOfFrame;
	BulletAnimation.framespeed = framespeed;
	BulletAnimation.tempframe = 1;
	BulletAnimation.CurrentSprite = 0;
	angle = 0;
	bullettexture = EffectSprites[0];
	SDL_SetTextureBlendMode(bullettexture, SDL_BLENDMODE_BLEND);
	BulletSprite = { BulletPosition.x - 120,BulletPosition.y - 120,240,240 };
	SDL_QueryTexture(bullettexture, NULL, NULL, &BulletAnimation.texturesize.x, &BulletAnimation.texturesize.y);
	BulletAnimation.framecalc();
}
void BulletDrawCorner(SDL_FRect TempRect[], SDL_Rect* BulletFrame, int i)
{
	SDL_RenderCopyExF(renderer, player1.PlayerWeapon.bullets[i]->bullettexture, BulletFrame, &TempRect[1], player1.PlayerWeapon.bullets[i]->angle, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyExF(renderer, player1.PlayerWeapon.bullets[i]->bullettexture, BulletFrame, &TempRect[2], player1.PlayerWeapon.bullets[i]->angle, NULL, SDL_FLIP_NONE);
}
void BulletDrawEdge(SDL_Rect* Frame, int i)
{
	SDL_FRect TempRect[3];
	TempRect[0] = player1.PlayerWeapon.bullets[i]->BulletSprite;
	TempRect[1] = player1.PlayerWeapon.bullets[i]->BulletSprite;
	TempRect[2] = player1.PlayerWeapon.bullets[i]->BulletSprite;
	if (player1.PlayerWeapon.bullets[i]->BulletSprite.x < 0)
	{
		TempRect[0].x += LEVEL_WIDTH;
		if (player1.PlayerWeapon.bullets[i]->BulletSprite.y < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].y += LEVEL_HEIGHT;
			BulletDrawCorner(TempRect, nullptr, i);
		}
		SDL_RenderCopyExF(renderer, player1.PlayerWeapon.bullets[i]->bullettexture, Frame, &TempRect[0], player1.PlayerWeapon.bullets[i]->angle, NULL, SDL_FLIP_NONE);
	}
	if (player1.PlayerWeapon.bullets[i]->BulletSprite.y < 0)
	{
		TempRect[0].y += LEVEL_HEIGHT;
		if (player1.PlayerWeapon.bullets[i]->BulletSprite.x > LEVEL_WIDTH - player1.PlayerWeapon.bullets[i]->BulletSize)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].x -= LEVEL_WIDTH;
			BulletDrawCorner(TempRect, nullptr, i);
		}
		SDL_RenderCopyExF(renderer, player1.PlayerWeapon.bullets[i]->bullettexture, Frame, &TempRect[0], player1.PlayerWeapon.bullets[i]->angle, NULL, SDL_FLIP_NONE);
	}
	if (player1.PlayerWeapon.bullets[i]->BulletSprite.x > LEVEL_WIDTH - player1.PlayerWeapon.bullets[i]->BulletSize)
	{
		TempRect[0].x -= LEVEL_WIDTH;
		if (player1.PlayerWeapon.bullets[i]->BulletSprite.y > LEVEL_HEIGHT - player1.PlayerWeapon.bullets[i]->BulletSize)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			BulletDrawCorner(TempRect, nullptr, i);
		}
		SDL_RenderCopyExF(renderer, player1.PlayerWeapon.bullets[i]->bullettexture, Frame, &TempRect[0], player1.PlayerWeapon.bullets[i]->angle, NULL, SDL_FLIP_NONE);
	}
	if (player1.PlayerWeapon.bullets[i]->BulletSprite.y > LEVEL_HEIGHT - player1.PlayerWeapon.bullets[i]->BulletSize)
	{

		TempRect[0].y -= LEVEL_HEIGHT;
		if (player1.PlayerWeapon.bullets[i]->BulletSprite.x < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;;
			BulletDrawCorner(TempRect, nullptr, i);
		}
		SDL_RenderCopyExF(renderer, player1.PlayerWeapon.bullets[i]->bullettexture, Frame, &TempRect[0], player1.PlayerWeapon.bullets[i]->angle, NULL, SDL_FLIP_NONE);
	}
}

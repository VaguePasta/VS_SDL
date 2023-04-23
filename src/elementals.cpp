#include "elementals.h"
#include <SDL.h>
#include "gameobjects.h"
#include "global.h"
#include <SDL.h>
#include <SDL_image.h>
void LoadElementalSpritesFromDisk()
{
	ElementalSprites[0][0] = IMG_LoadTexture(renderer, "resources/elementals/FireIdle.PNG");
	ElementalSprites[0][1] = IMG_LoadTexture(renderer, "resources/elementals/FireRun.PNG");
	ElementalSprites[0][2] = IMG_LoadTexture(renderer, "resources/elementals/FireRoll.PNG");
	ElementalSprites[0][3] = IMG_LoadTexture(renderer, "resources/elementals/FireAttack1.PNG");
	ElementalSprites[0][4] = IMG_LoadTexture(renderer, "resources/elementals/FireAttack2.PNG");
	ElementalSprites[0][5] = IMG_LoadTexture(renderer, "resources/elementals/FireAttack3.PNG");
	ElementalSprites[0][6] = IMG_LoadTexture(renderer, "resources/elementals/FireSP.PNG");
	ElementalSprites[0][7] = IMG_LoadTexture(renderer, "resources/elementals/FireDefend.PNG");
	ElementalSprites[0][8] = IMG_LoadTexture(renderer, "resources/elementals/FireHurt.PNG");
	ElementalSprites[0][9] = IMG_LoadTexture(renderer, "resources/elementals/FireDeath.PNG");
	ElementalSprites[1][0] = IMG_LoadTexture(renderer, "resources/elementals/WaterIdle.PNG");
	ElementalSprites[1][1] = IMG_LoadTexture(renderer, "resources/elementals/WaterSurf.PNG");
	ElementalSprites[1][2] = IMG_LoadTexture(renderer, "resources/elementals/WaterRoll.PNG");
	ElementalSprites[1][3] = IMG_LoadTexture(renderer, "resources/elementals/WaterAttack1.PNG");
	ElementalSprites[1][4] = IMG_LoadTexture(renderer, "resources/elementals/WaterAttack2.PNG");
	ElementalSprites[1][5] = IMG_LoadTexture(renderer, "resources/elementals/WaterAttack3.PNG");
	ElementalSprites[1][6] = IMG_LoadTexture(renderer, "resources/elementals/WaterSP.PNG");
	ElementalSprites[1][7] = IMG_LoadTexture(renderer, "resources/elementals/WaterDefend.PNG");
	ElementalSprites[1][8] = IMG_LoadTexture(renderer, "resources/elementals/WaterHurt.PNG");
	ElementalSprites[1][9] = IMG_LoadTexture(renderer, "resources/elementals/WaterDeath.PNG");

}
std::vector <std::vector<std::vector<float>>> ElementalSpritesInfo    //{So thu tu, kich co, toc do, toc do danh, mau}, {So frame, toc do chuyen frame},{HitboxNoFlip},{HitboxFlip},{AttackBoxNoFlip},{AttackBoxFlip}
{
	{{0,540,240,5,0.4,700},{8,6},{8,8},{8,8},{11,8},{19,8},{28,8},{18,8},{10,3},{6,3},{13,8}, {235,155,65,80},{235,155,65,65},{275,140,120,100},{150,140,120,100}},
	{{0,540,240,5,0.2,600},{8,6},{8,8},{6,6},{7,8},{21,8},{27,8},{32,8},{12,3},{7,3},{16,8},{235,155,65,80},{235,155,65,65},{275,140,120,100},{150,140,120,100}},
};
Elementals::Elementals()
{
	CurrentElementalType = rand() % 2;
	isSpawn = false;
	isHurt = false;
	isDead = false;
	isAttacking = false;
	isIdling = false;
}
Elementals::~Elementals(){}
void Elementals::Spawn(SDL_FPoint PlayerPosition)
{
	do
	{
		position.x = rand() % LEVEL_WIDTH - 200;
		position.y = rand() % LEVEL_HEIGHT - 200;
	} while (DistanceCalculation(player1.position, position) <= 1500);
	flip = (rand() % 2  == 0) ? (SDL_FLIP_HORIZONTAL) : (SDL_FLIP_NONE);
	LoadTexture(0);
	Cooldown.Start();
	DodgeCooldown.Start();
	BlockCooldown.Start();
	isSpawn = true;
}
void Elementals::LoadTexture(int CurrentState)
{
	texture = ElementalSprites[CurrentElementalType][CurrentState];
	NumOfSprites = ElementalSpritesInfo[CurrentElementalType][CurrentState + 1][0];
	framespeed = ElementalSpritesInfo[CurrentElementalType][CurrentState + 1][1];
	tempframe = 1;
	CurrentSprite = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texturesize.x, &texturesize.y);
	SpriteSize.x = ElementalSpritesInfo[CurrentElementalType][0][1]; 
	SpriteSize.y = ElementalSpritesInfo[CurrentElementalType][0][2];
	framecalc();
	if (!isSpawn)
	{
		Health = ElementalSpritesInfo[CurrentElementalType][0][5];
		AttackSpeed = ElementalSpritesInfo[CurrentElementalType][0][4];
		speed = ElementalSpritesInfo[CurrentElementalType][0][3];
		CalculateBoxes();
	}
}
void Elementals::Run()
{
	isIdling = false;
	isAttacking = false;
	isMoving = true;
	LoadTexture(1);
}
void Elementals::Dodge()
{
	isIdling = false;
	isAttacking = false;
	isMoving = false;
	isDodging = true;
	isDefending = false;
	LoadTexture(2);
	speed *= 1.2;
}
void Elementals::Attack()
{
	isIdling = false;
	isAttacking = true;
	isMoving = false;
	isDodging = false;
	isDefending = false;
	AttackType = rand() % 4 + 3;
	LoadTexture(AttackType);
	Cooldown.Restart();
}
void Elementals::Defend()
{
	isIdling = false;
	isAttacking = false;
	isMoving = false;
	isDodging = false;
	isDefending = true;
	LoadTexture(7);
}
void Elementals::Hurt()
{
	isIdling = false;
	isHurt = true;
	isAttacking = false;
	isMoving = false;
	isDodging = false;
	isDefending = false;
	LoadTexture(8);
}
void Elementals::Death()
{
	isIdling = false;
	isHurt = false;
	isAttacking = false;
	isMoving = false;
	isDodging = false;
	isDefending = false;
	isDead = true;
	LoadTexture(9);
	SCORE++;
	Hitbox = { 0,0,0,0 };
	AttackBox = { 0,0,0,0 };
	UpdateCurrentScore();
}
void Elementals::Idle()
{
	isIdling = true;
	isHurt = false;
	isAttacking = false;
	isMoving = false;
	isDodging = false;
	isDefending = false;
	LoadTexture(0);
}
void Elementals::CalculateBoxes()
{
	if (flip == SDL_FLIP_NONE)
	{
		Hitbox = { position.x + ElementalSpritesInfo[CurrentElementalType][11][0],position.y + ElementalSpritesInfo[CurrentElementalType][11][1],ElementalSpritesInfo[CurrentElementalType][11][2],ElementalSpritesInfo[CurrentElementalType][11][3] };
		AttackBox = { position.x + ElementalSpritesInfo[CurrentElementalType][13][0],position.y + ElementalSpritesInfo[CurrentElementalType][13][1],ElementalSpritesInfo[CurrentElementalType][13][2],ElementalSpritesInfo[CurrentElementalType][13][3] };
	}
	if (flip == SDL_FLIP_HORIZONTAL)
	{
		Hitbox = { position.x + ElementalSpritesInfo[CurrentElementalType][12][0],position.y + ElementalSpritesInfo[CurrentElementalType][12][1],ElementalSpritesInfo[CurrentElementalType][12][2],ElementalSpritesInfo[CurrentElementalType][12][3] };
		AttackBox = { position.x + ElementalSpritesInfo[CurrentElementalType][14][0],position.y + ElementalSpritesInfo[CurrentElementalType][14][1],ElementalSpritesInfo[CurrentElementalType][14][2],ElementalSpritesInfo[CurrentElementalType][14][3] };
	}
	if (Hitbox.x<0 && player1.BulletOrigin.x>LEVEL_WIDTH / 2) Hitbox.x += LEVEL_WIDTH;
	else if (Hitbox.x + Hitbox.w > LEVEL_WIDTH && player1.BulletOrigin.x < LEVEL_WIDTH / 2) Hitbox.x -= LEVEL_WIDTH;
	if (Hitbox.y + Hitbox.h > LEVEL_HEIGHT && player1.BulletOrigin.y < LEVEL_HEIGHT / 2) Hitbox.y -= LEVEL_HEIGHT;
	else if (Hitbox.y<0 && player1.BulletOrigin.y>LEVEL_HEIGHT / 2) Hitbox.y += LEVEL_HEIGHT;
}
void ElementalDrawCorner(SDL_FRect TempRect[], int i)
{
	SDL_RenderCopyExF(renderer, elemental[i]->texture, &elemental[i]->frame, &TempRect[1], 0, NULL, elemental[i]->flip);
	SDL_RenderCopyExF(renderer, elemental[i]->texture, &elemental[i]->frame, &TempRect[2], 0, NULL, elemental[i]->flip);
}
void ElementalDrawEdge(int i)
{
	SDL_FRect TempRect[3];
	TempRect[0] = elemental[i]->SpriteBox;
	TempRect[1] = elemental[i]->SpriteBox;
	TempRect[2] = elemental[i]->SpriteBox;
	if (elemental[i]->position.x < 0)
	{
		TempRect[0].x += LEVEL_WIDTH;
		if (elemental[i]->position.y < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].y += LEVEL_HEIGHT;
			ElementalDrawCorner(TempRect, i);
		}
		SDL_RenderCopyExF(renderer, elemental[i]->texture, &elemental[i]->frame, &TempRect[0], 0, NULL, elemental[i]->flip);
	}
	if (elemental[i]->position.y < 0)
	{
		TempRect[0].y += LEVEL_HEIGHT;
		if (elemental[i]->position.x > LEVEL_WIDTH - elemental[i]->SpriteBox.w)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].x -= LEVEL_WIDTH;
			ElementalDrawCorner(TempRect, i);
		}
		SDL_RenderCopyExF(renderer, elemental[i]->texture, &elemental[i]->frame, &TempRect[0], 0, NULL, elemental[i]->flip);
	}
	if (elemental[i]->position.x > LEVEL_WIDTH - elemental[i]->SpriteBox.w)
	{
		TempRect[0].x -= LEVEL_WIDTH;
		if (elemental[i]->position.y > LEVEL_HEIGHT - elemental[i]->SpriteBox.h)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			ElementalDrawCorner(TempRect, i);
		}
		SDL_RenderCopyExF(renderer, elemental[i]->texture, &elemental[i]->frame, &TempRect[0], 0, NULL, elemental[i]->flip);
	}
	if (elemental[i]->position.y > LEVEL_HEIGHT - elemental[i]->SpriteBox.h)
	{
		TempRect[0].y -= LEVEL_HEIGHT;
		if (elemental[i]->position.x < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			ElementalDrawCorner(TempRect, i);
		}
		SDL_RenderCopyExF(renderer, elemental[i]->texture, &elemental[i]->frame, &TempRect[0], 0, NULL, elemental[i]->flip);
	}
}
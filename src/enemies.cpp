#include <SDL.h>
#include <SDL_image.h>
#include "global.h"
#include "sprites.h"
#include "enemies.h"
#include "global.h"
#include "timer.h"
#include "gameobjects.h"
#include <cstdlib>
#include <cmath>
#include "UI.h"
using namespace std;
vector<vector<vector<float>>> EnemiesInfo =   //{So thu tu, kich co, toc do, toc do danh, mau, gan-xa(0-1),dmg}, {So frame, toc do chuyen frame},{HitboxNoFlip},{HitboxFlip},{AttackBoxNoFlip},{AttackBoxFlip}
{
	{{0,228,5,2,250,0,20},{9,4},{6,8},{12,5},{5,4},{24,7},{80,100,80,80},{70,100,80,80},{90,30,130,170},{10,30,130,170}},    //1:Idle,2:Running,3:Attacking,4:Hurting,5:Dying
	{{1,228,4,0.5,500,0,35},{5,7},{8,8},{9,8},{3,5},{6,13},{75,50,70,100},{85,50,70,100},{50,10,110,150},{28,10,110,150}},
	{{2,228,5,2,300,0,15},{10,5},{8,7},{9,5},{3,6},{11,11},{100,85,55,65},{75,85,55,65},{125,75,80,85},{25,75,80,85}},
	{{3,228,6,0.5,200,1,25},{10,5},{8,6},{6,10},{3,6},{10,11},{80,80,80,75},{80,80,80,75},{130,-190,700,600},{-600,-190,700,600}},
	{{4,400,4,0.2,220,1,50},{8,8},{8,8},{8,8},{3,6},{7,12},{171,190,44,78},{186,190,44,78},{260,-95,700,650},{-570,-95,700,650}},
};
vector<vector<float>> EnemyProjectilesInfo =   //STT, kich co_x, kich co_y, toc do, tam bay, sat thuong, co animation?,so frame,toc do chuyen frame
{
	{0,48,10,11,1000,25,0,0,0},
	{1,192,192,0,0,50,1,14,5},
	{2,128,176,0,0,50,1,11,7},
};
void LoadEnemiesSpritesFromDisk()
{
	EnemySprites[0][0] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneIdle.PNG");
	EnemySprites[0][1] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneRunning.PNG");
	EnemySprites[0][2] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneAttacking.PNG");
	EnemySprites[0][3] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneHurt.PNG");
	EnemySprites[0][4] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneDeath.PNG");
	EnemySprites[1][0] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurIdle.PNG");
	EnemySprites[1][1] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurMoving.PNG");
	EnemySprites[1][2] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurAttacking.PNG");
	EnemySprites[1][3] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurHurt.PNG");
	EnemySprites[1][4] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurDeath.PNG");
	EnemySprites[2][0] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroIdle.PNG");
	EnemySprites[2][1] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroRunning.PNG");
	EnemySprites[2][2] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroAttacking.PNG");
	EnemySprites[2][3] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroHurt.PNG");
	EnemySprites[2][4] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroDeath.PNG");
	EnemySprites[3][0] = IMG_LoadTexture(renderer, "resources/enemies/HuntressIdle.PNG");
	EnemySprites[3][1] = IMG_LoadTexture(renderer, "resources/enemies/HuntressRunning.PNG");
	EnemySprites[3][2] = IMG_LoadTexture(renderer, "resources/enemies/HuntressAttacking.PNG");
	EnemySprites[3][3] = IMG_LoadTexture(renderer, "resources/enemies/HuntressHurt.PNG");
	EnemySprites[3][4] = IMG_LoadTexture(renderer, "resources/enemies/HuntressDeath.PNG");
	EnemySprites[4][0] = IMG_LoadTexture(renderer, "resources/enemies/WizardIdle.PNG");
	EnemySprites[4][1] = IMG_LoadTexture(renderer, "resources/enemies/WizardRunning.PNG");
	EnemySprites[4][2] = IMG_LoadTexture(renderer, "resources/enemies/WizardAttacking.PNG");
	EnemySprites[4][3] = IMG_LoadTexture(renderer, "resources/enemies/WizardHurt.PNG");
	EnemySprites[4][4] = IMG_LoadTexture(renderer, "resources/enemies/WizardDeath.PNG");
}
Enemies::Enemies()
{
	CurrentEnemyType = rand() % 5;
	isSpawn = false;
	isHurt = false;
	isDead = false;
	isIdling = true;
	isAttacking = false;
	isRunning = false;
	isRanged = EnemiesInfo[CurrentEnemyType][0][5];
	Decayed = false;
}
Enemies::~Enemies()
{}
void Enemies::LoadTexture(int CurrentState)
{
	texture = EnemySprites[CurrentEnemyType][CurrentState - 1];
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	CurrentSprite = 0;
	NumOfSprites = EnemiesInfo[CurrentEnemyType][CurrentState][0];
	framespeed = EnemiesInfo[CurrentEnemyType][CurrentState][1];
	SDL_QueryTexture(texture, NULL, NULL, &texturesize.x, &texturesize.y);
	framecalc();
	SpriteSize = EnemiesInfo[CurrentEnemyType][0][1];
	if (!isSpawn)
	{
		Health = EnemiesInfo[CurrentEnemyType][0][4];
		Damage = EnemiesInfo[CurrentEnemyType][0][6];
		speed = EnemiesInfo[CurrentEnemyType][0][2];
		AttackSpeed = EnemiesInfo[CurrentEnemyType][0][3];
		CalCulateBoxes();
	}
}
void Enemies::CalCulateBoxes()
{
	if (flip == SDL_FLIP_NONE)
	{
		Hitbox = { (int)position.x + (int)EnemiesInfo[CurrentEnemyType][6][0],(int)position.y + (int)EnemiesInfo[CurrentEnemyType][6][1],(int)EnemiesInfo[CurrentEnemyType][6][2],(int)EnemiesInfo[CurrentEnemyType][6][3] };
		AttackBox = { (int)position.x + (int)EnemiesInfo[CurrentEnemyType][8][0],(int)position.y + (int)EnemiesInfo[CurrentEnemyType][8][1],(int)EnemiesInfo[CurrentEnemyType][8][2],(int)EnemiesInfo[CurrentEnemyType][8][3] };
	}
	if (flip == SDL_FLIP_HORIZONTAL)
	{
		Hitbox = { (int)position.x + (int)EnemiesInfo[CurrentEnemyType][7][0],(int)position.y + (int)EnemiesInfo[CurrentEnemyType][7][1],(int)EnemiesInfo[CurrentEnemyType][7][2],(int)EnemiesInfo[CurrentEnemyType][7][3] };
		AttackBox = { (int)position.x + (int)EnemiesInfo[CurrentEnemyType][9][0],(int)position.y + (int)EnemiesInfo[CurrentEnemyType][9][1],(int)EnemiesInfo[CurrentEnemyType][9][2],(int)EnemiesInfo[CurrentEnemyType][9][3] };
	}
	if (Hitbox.x<0 && player1.BulletOrigin.x>LEVEL_WIDTH / 2) Hitbox.x += LEVEL_WIDTH;
	else if (Hitbox.x + Hitbox.w > LEVEL_WIDTH && player1.BulletOrigin.x < LEVEL_WIDTH / 2) Hitbox.x -= LEVEL_WIDTH;
	if (Hitbox.y + Hitbox.h > LEVEL_HEIGHT && player1.BulletOrigin.y < LEVEL_HEIGHT / 2) Hitbox.y -= LEVEL_HEIGHT;
	else if (Hitbox.y<0 && player1.BulletOrigin.y>LEVEL_HEIGHT / 2) Hitbox.y += LEVEL_HEIGHT;
}
void Enemies::Death()
{
	SCORE++;
	isDead = true;
	isRunning = false;
	isAttacking = false;
	isIdling = false;
	isHurt = false;
	LoadTexture(5);
	Hitbox = { 0,0,0,0 };
	AttackBox = { 0,0,0,0 };
	MovingCounter.Restart();
	UpdateCurrentScore();
}
void Enemies::Hurt()
{
	isHurt = true;
	isIdling = false;
	LoadTexture(4);
	MovingCounter.Restart();
}
void Enemies::Spawn(SDL_FPoint PlayerPosition)
{
	do
	{
		position.x = rand() % LEVEL_WIDTH - 200;
		position.y = rand() % LEVEL_HEIGHT - 200;
	} while (DistanceCalculation(player1.position,position) <= 1300);
	flip = (rand() % 2 + 1 == 1) ? (SDL_FLIP_HORIZONTAL) : (SDL_FLIP_NONE);
	LoadTexture(1);
	Cooldown.Start();
	isSpawn = true;
	MovingCounter.Start();
}
void Enemies::Attack()
{
	isIdling = false;
	isRunning = false;
	isAttacking = true;
	Cooldown.Restart();
	LoadTexture(3);
	MovingCounter.Restart();
}
void Enemies::Run()
{
	isIdling = false;
	isAttacking = false;
	isRunning = true;
	LoadTexture(2);
}
void Enemies::Idle()
{
	isAttacking = false;
	isRunning = false;
	isIdling = true;
	LoadTexture(1);
}
void EnemyDrawCorner(SDL_Rect TempRect[], int i)
{
	SDL_RenderCopyEx(renderer, enemy[i]->texture, &enemy[i]->frame, &TempRect[1], 0, NULL, enemy[i]->flip);
	SDL_RenderCopyEx(renderer, enemy[i]->texture, &enemy[i]->frame, &TempRect[2], 0, NULL, enemy[i]->flip);
}
void EnemyDrawEdge(int i)
{
	SDL_Rect TempRect[3];
	TempRect[0] = enemy[i]->SpriteBox;
	TempRect[1] = enemy[i]->SpriteBox;
	TempRect[2] = enemy[i]->SpriteBox;
	if (enemy[i]->position.x < 0)
	{
		TempRect[0].x += LEVEL_WIDTH;
		if (enemy[i]->position.y < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].y += LEVEL_HEIGHT;
			EnemyDrawCorner(TempRect, i);
		}
		SDL_RenderCopyEx(renderer, enemy[i]->texture, &enemy[i]->frame, &TempRect[0], 0, NULL, enemy[i]->flip);
	}
	if (enemy[i]->position.y < 0)
	{
		TempRect[0].y += LEVEL_HEIGHT;
		if (enemy[i]->position.x > LEVEL_WIDTH - enemy[i]->SpriteBox.w)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].x -= LEVEL_WIDTH;
			EnemyDrawCorner(TempRect, i);
		}
		SDL_RenderCopyEx(renderer, enemy[i]->texture, &enemy[i]->frame, &TempRect[0], 0, NULL, enemy[i]->flip);
	}
	if (enemy[i]->position.x > LEVEL_WIDTH - enemy[i]->SpriteBox.w)
	{
		TempRect[0].x -= LEVEL_WIDTH;
		if (enemy[i]->position.y > LEVEL_HEIGHT - enemy[i]->SpriteBox.h)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			EnemyDrawCorner(TempRect, i);
		}
		SDL_RenderCopyEx(renderer, enemy[i]->texture, &enemy[i]->frame, &TempRect[0], 0, NULL, enemy[i]->flip);
	}
	if (enemy[i]->position.y > LEVEL_HEIGHT - enemy[i]->SpriteBox.h)
	{
		TempRect[0].y -= LEVEL_HEIGHT;
		if (enemy[i]->position.x < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			EnemyDrawCorner(TempRect, i);
		}
		SDL_RenderCopyEx(renderer, enemy[i]->texture, &enemy[i]->frame, &TempRect[0], 0, NULL, enemy[i]->flip);
	}
}

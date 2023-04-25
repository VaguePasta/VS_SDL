#include <SDL.h>
#include <SDL_image.h>
#include "global.h"
#include "sprites.h"
#include "minions.h"
#include "global.h"
#include "timer.h"
#include "gameobjects.h"
#include <cstdlib>
#include <cmath>
#include "UI.h"
std::vector<std::vector<std::vector<float>>> MinionsInfo =   //{So thu tu, kich co, toc do, toc do danh, mau, gan-xa(0-1),dmg}, {So frame, toc do chuyen frame},{hitboxNoFlip},{hitboxFlip},{AttackBoxNoFlip},{AttackBoxFlip}
{
	{{0,228,5,2,250,0,20},{9,4},{6,8},{12,5},{5,4},{24,7},{80,100,80,80},{70,100,80,80},{90,30,130,170},{10,30,130,170}},    //1:Idle,2:Running,3:Attacking,4:Hurting,5:Dying
	{{1,228,4,0.5,500,0,35},{5,7},{8,8},{9,8},{3,5},{6,13},{75,50,70,100},{85,50,70,100},{50,10,110,150},{28,10,110,150}},
	{{2,228,5,2,300,0,15},{10,5},{8,7},{9,5},{3,6},{11,11},{100,85,55,65},{75,85,55,65},{125,75,80,85},{25,75,80,85}},
	{{3,228,6,0.5,200,1,25},{10,5},{8,6},{6,10},{3,6},{10,11},{80,80,80,75},{80,80,80,75},{130,-190,700,600},{-600,-190,700,600}},
	{{4,400,4,0.2,220,1,50},{8,8},{8,8},{8,8},{3,6},{7,12},{171,190,44,78},{186,190,44,78},{260,-95,700,650},{-570,-95,700,650}},
};
void LoadMinionsSpritesFromDisk()
{
	MinionSprites[0][0] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneIdle.PNG");
	MinionSprites[0][1] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneRunning.PNG");
	MinionSprites[0][2] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneAttacking.PNG");
	MinionSprites[0][3] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneHurt.PNG");
	MinionSprites[0][4] = IMG_LoadTexture(renderer, "resources/enemies/NightBorneDeath.PNG");
	MinionSprites[1][0] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurIdle.PNG");
	MinionSprites[1][1] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurMoving.PNG");
	MinionSprites[1][2] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurAttacking.PNG");
	MinionSprites[1][3] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurHurt.PNG");
	MinionSprites[1][4] = IMG_LoadTexture(renderer, "resources/enemies/MinotaurDeath.PNG");
	MinionSprites[2][0] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroIdle.PNG");
	MinionSprites[2][1] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroRunning.PNG");
	MinionSprites[2][2] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroAttacking.PNG");
	MinionSprites[2][3] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroHurt.PNG");
	MinionSprites[2][4] = IMG_LoadTexture(renderer, "resources/enemies/MartialHeroDeath.PNG");
	MinionSprites[3][0] = IMG_LoadTexture(renderer, "resources/enemies/HuntressIdle.PNG");
	MinionSprites[3][1] = IMG_LoadTexture(renderer, "resources/enemies/HuntressRunning.PNG");
	MinionSprites[3][2] = IMG_LoadTexture(renderer, "resources/enemies/HuntressAttacking.PNG");
	MinionSprites[3][3] = IMG_LoadTexture(renderer, "resources/enemies/HuntressHurt.PNG");
	MinionSprites[3][4] = IMG_LoadTexture(renderer, "resources/enemies/HuntressDeath.PNG");
	MinionSprites[4][0] = IMG_LoadTexture(renderer, "resources/enemies/WizardIdle.PNG");
	MinionSprites[4][1] = IMG_LoadTexture(renderer, "resources/enemies/WizardRunning.PNG");
	MinionSprites[4][2] = IMG_LoadTexture(renderer, "resources/enemies/WizardAttacking.PNG");
	MinionSprites[4][3] = IMG_LoadTexture(renderer, "resources/enemies/WizardHurt.PNG");
	MinionSprites[4][4] = IMG_LoadTexture(renderer, "resources/enemies/WizardDeath.PNG");
}
Minions::Minions()
{
	int Random = rand() % 10 + 1;
	if (Random <= 7)
	{
		CurrentMinionType = rand() % 3;
	}
	else CurrentMinionType = rand() % 2 + 3;
	isSpawn = false;
	isHurt = false;
	isDead = false;
	isIdling = true;
	isAttacking = false;
	isMoving = false;
	isRanged = MinionsInfo[CurrentMinionType][0][5];
	Decayed = false;
}
Minions::~Minions()
{}
void Minions::LoadTexture(int CurrentState)
{
	texture = MinionSprites[CurrentMinionType][CurrentState - 1];
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	CurrentSprite = 0;
	NumOfSprites = MinionsInfo[CurrentMinionType][CurrentState][0];
	framespeed = MinionsInfo[CurrentMinionType][CurrentState][1];
	SDL_QueryTexture(texture, NULL, NULL, &texturesize.x, &texturesize.y);
	framecalc();
	SpriteSize.x = MinionsInfo[CurrentMinionType][0][1];
	SpriteSize.y = MinionsInfo[CurrentMinionType][0][1];
	if (!isSpawn)
	{
		Health = MinionsInfo[CurrentMinionType][0][4];
		Damage = MinionsInfo[CurrentMinionType][0][6];
		speed = MinionsInfo[CurrentMinionType][0][2];
		AttackSpeed = MinionsInfo[CurrentMinionType][0][3];
		CalculateBoxes();
	}
}
void Minions::CalculateBoxes()
{
	if (flip == SDL_FLIP_NONE)
	{
		hitbox = { position.x + MinionsInfo[CurrentMinionType][6][0],position.y + MinionsInfo[CurrentMinionType][6][1],MinionsInfo[CurrentMinionType][6][2],MinionsInfo[CurrentMinionType][6][3] };
		AttackBox = { position.x + MinionsInfo[CurrentMinionType][8][0],position.y + MinionsInfo[CurrentMinionType][8][1],MinionsInfo[CurrentMinionType][8][2],MinionsInfo[CurrentMinionType][8][3] };
	}
	if (flip == SDL_FLIP_HORIZONTAL)
	{
		hitbox = { position.x + MinionsInfo[CurrentMinionType][7][0],position.y + MinionsInfo[CurrentMinionType][7][1],MinionsInfo[CurrentMinionType][7][2],MinionsInfo[CurrentMinionType][7][3] };
		AttackBox = { position.x + MinionsInfo[CurrentMinionType][9][0],position.y + MinionsInfo[CurrentMinionType][9][1],MinionsInfo[CurrentMinionType][9][2],MinionsInfo[CurrentMinionType][9][3] };
	}
	if (hitbox.x<0 && player1.BulletOrigin.x>LEVEL_WIDTH / 2) hitbox.x += LEVEL_WIDTH;
	else if (hitbox.x + hitbox.w > LEVEL_WIDTH && player1.BulletOrigin.x < LEVEL_WIDTH / 2) hitbox.x -= LEVEL_WIDTH;
	if (hitbox.y + hitbox.h > LEVEL_HEIGHT && player1.BulletOrigin.y < LEVEL_HEIGHT / 2) hitbox.y -= LEVEL_HEIGHT;
	else if (hitbox.y<0 && player1.BulletOrigin.y>LEVEL_HEIGHT / 2) hitbox.y += LEVEL_HEIGHT;
}
void Minions::Death()
{
	SCORE++;
	isDead = true;
	isMoving = false;
	isAttacking = false;
	isIdling = false;
	isHurt = false;
	LoadTexture(5);
	hitbox = { 0,0,0,0 };
	AttackBox = { 0,0,0,0 };
	UpdateCurrentScore();
}
void Minions::Hurt()
{
	isHurt = true;
	isIdling = false;
	LoadTexture(4);
}
void Minions::Spawn(SDL_FPoint PlayerPosition)
{
	do
	{
		position.x = rand() % LEVEL_WIDTH - 200;
		position.y = rand() % LEVEL_HEIGHT - 200;
	} while (DistanceCalculation(player1.position,position) <= 1500);
	flip = (rand() % 2 + 1 == 1) ? (SDL_FLIP_HORIZONTAL) : (SDL_FLIP_NONE);
	LoadTexture(1);
	Cooldown.Start();
	isSpawn = true;
}
void Minions::Attack()
{
	isIdling = false;
	isMoving = false;
	isAttacking = true;
	Cooldown.Restart();
	LoadTexture(3);
}
void Minions::Run()
{
	isIdling = false;
	isAttacking = false;
	isMoving = true;
	LoadTexture(2);
}
void Minions::Idle()
{
	isAttacking = false;
	isMoving = false;
	isIdling = true;
	LoadTexture(1);
}
void MinionSlashDamage(Minions* minion, bool &MinionDamaged)
{
	SDL_FPoint PlayerCenter = player1.position; PlayerCenter.x += 50; PlayerCenter.y += 50;
	SDL_FPoint MinionCenter = minion->position; MinionCenter.x += minion->SpriteSize.x / 2; MinionCenter.y += minion->SpriteSize.y / 2;
	float enemyAngle = AngleCalculation(MinionCenter, PlayerCenter, &player1.flip);
	float enemyDistance = DistanceCalculation(MinionCenter, PlayerCenter);
	if (abs(enemyAngle - player1.PlayerWeapon.angle) <= 5 && enemyDistance <= player1.PlayerWeapon.range && !MinionDamaged)
	{
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

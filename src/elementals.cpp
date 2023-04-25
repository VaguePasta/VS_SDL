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
	ElementalSprites[2][0] = IMG_LoadTexture(renderer, "resources/elementals/WindIdle.PNG");
	ElementalSprites[2][1] = IMG_LoadTexture(renderer, "resources/elementals/WindRun.PNG");
	ElementalSprites[2][2] = IMG_LoadTexture(renderer, "resources/elementals/WindRoll.PNG");
	ElementalSprites[2][3] = IMG_LoadTexture(renderer, "resources/elementals/WindAttack1.PNG");
	ElementalSprites[2][4] = IMG_LoadTexture(renderer, "resources/elementals/WindAttack2.PNG");
	ElementalSprites[2][5] = IMG_LoadTexture(renderer, "resources/elementals/WindAttack3.PNG");
	ElementalSprites[2][6] = IMG_LoadTexture(renderer, "resources/elementals/WindSP.PNG");
	ElementalSprites[2][7] = IMG_LoadTexture(renderer, "resources/elementals/WindDefend.PNG");
	ElementalSprites[2][8] = IMG_LoadTexture(renderer, "resources/elementals/WindHurt.PNG");
	ElementalSprites[2][9] = IMG_LoadTexture(renderer, "resources/elementals/WindDeath.PNG");
	ElementalSprites[3][0] = IMG_LoadTexture(renderer, "resources/elementals/GroundIdle.PNG");
	ElementalSprites[3][1] = IMG_LoadTexture(renderer, "resources/elementals/GroundRun.PNG");
	ElementalSprites[3][2] = IMG_LoadTexture(renderer, "resources/elementals/GroundRoll.PNG");
	ElementalSprites[3][3] = IMG_LoadTexture(renderer, "resources/elementals/GroundAttack1.PNG");
	ElementalSprites[3][4] = IMG_LoadTexture(renderer, "resources/elementals/GroundAttack2.PNG");
	ElementalSprites[3][5] = IMG_LoadTexture(renderer, "resources/elementals/GroundAttack3.PNG");
	ElementalSprites[3][6] = IMG_LoadTexture(renderer, "resources/elementals/GroundSP.PNG");
	ElementalSprites[3][7] = IMG_LoadTexture(renderer, "resources/elementals/GroundDefend.PNG");
	ElementalSprites[3][8] = IMG_LoadTexture(renderer, "resources/elementals/GroundHurt.PNG");
	ElementalSprites[3][9] = IMG_LoadTexture(renderer, "resources/elementals/GroundDeath.PNG");
	ElementalSprites[4][0] = IMG_LoadTexture(renderer, "resources/elementals/MetalIdle.PNG");
	ElementalSprites[4][1] = IMG_LoadTexture(renderer, "resources/elementals/MetalRun.PNG");
	ElementalSprites[4][2] = IMG_LoadTexture(renderer, "resources/elementals/MetalRoll.PNG");
	ElementalSprites[4][3] = IMG_LoadTexture(renderer, "resources/elementals/MetalAttack1.PNG");
	ElementalSprites[4][4] = IMG_LoadTexture(renderer, "resources/elementals/MetalAttack2.PNG");
	ElementalSprites[4][5] = IMG_LoadTexture(renderer, "resources/elementals/MetalAttack3.PNG");
	ElementalSprites[4][6] = IMG_LoadTexture(renderer, "resources/elementals/MetalSP.PNG");
	ElementalSprites[4][7] = IMG_LoadTexture(renderer, "resources/elementals/MetalDefend.PNG");
	ElementalSprites[4][8] = IMG_LoadTexture(renderer, "resources/elementals/MetalHurt.PNG");
	ElementalSprites[4][9] = IMG_LoadTexture(renderer, "resources/elementals/MetalDeath.PNG");
	ElementalSprites[5][0] = IMG_LoadTexture(renderer, "resources/elementals/LeafIdle.PNG");
	ElementalSprites[5][1] = IMG_LoadTexture(renderer, "resources/elementals/LeafRun.PNG");
	ElementalSprites[5][2] = IMG_LoadTexture(renderer, "resources/elementals/LeafRoll.PNG");
	ElementalSprites[5][3] = IMG_LoadTexture(renderer, "resources/elementals/LeafAttack1.PNG");
	ElementalSprites[5][4] = IMG_LoadTexture(renderer, "resources/elementals/LeafAttack2.PNG");
	ElementalSprites[5][5] = IMG_LoadTexture(renderer, "resources/elementals/LeafAttack3.PNG");
	ElementalSprites[5][6] = IMG_LoadTexture(renderer, "resources/elementals/LeafSP.PNG");
	ElementalSprites[5][7] = IMG_LoadTexture(renderer, "resources/elementals/LeafDefend.PNG");
	ElementalSprites[5][8] = IMG_LoadTexture(renderer, "resources/elementals/MetalHurt.PNG");
	ElementalSprites[5][9] = IMG_LoadTexture(renderer, "resources/elementals/LeafDeath.PNG");
}
std::vector <std::vector<std::vector<float>>> ElementalSpritesInfo    //{So thu tu, kich co, toc do, toc do danh, mau}, {So frame, toc do chuyen frame},{hitboxNoFlip},{hitboxFlip},{AttackBoxNoFlip},{AttackBoxFlip}
{
	{{0,540,240,5,0.4,700},{8,6},{8,8},{8,8},{11,8},{19,8},{28,8},{18,8},{10,3},{6,3},{13,8}, {235,160,65,80},{235,160,65,80},{275,140,120,100},{150,140,120,100}},
	{{0,540,240,5,0.65,600},{8,6},{8,8},{6,9},{7,8},{21,8},{27,8},{32,8},{12,3},{7,3},{16,8},{235,160,65,80},{235,160,65,80},{275,160,120,80},{150,160,120,80}},
	{{0,540,240,5,1,700},{8,6},{8,8},{6,9},{8,8},{18,8},{26,8},{30,8},{8,3},{6,3},{19,8},{235,160,65,80},{235,160,65,80},{275,160,60,80},{205,160,60,80}},
	{{0,720,320,5,1,650},{6,5},{8,8},{6,9},{6,8},{12,8},{23,8},{25,8},{13,3},{6,3},{18,8},{330,220,60,80},{330,220,60,80},{388,255,45,5},{286,255,45,5}},
	{{0,540,240,5,1,650},{8,6},{8,8},{7,9},{6,8},{8,8},{18,8},{11,8},{12,3},{6,3},{12,8},{235,160,65,80},{235,160,65,80},{275,160,60,80},{205,160,60,80}},
	{{0,540,240,5,0.6,600},{12,6},{10,8},{8,8},{10,8},{15,8},{12,8},{17,8},{19,3},{6,3},{19,8},{235,160,65,80},{235,160,65,80},{420,185,500,5},{-350,185,500,5}}
};
Elementals::Elementals()
{
	CurrentElementalType = rand() % 6;
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
	ChooseAttack();
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
void Elementals::ChooseAttack()
{
	int ChooseAttackType = rand() % 10 + 1;
	if (ChooseAttackType <= 5) AttackType = 3;
	else if (ChooseAttackType <= 7) AttackType = 4;
	else if (ChooseAttackType <= 9) AttackType = 5;
	else AttackType = 6;
}
void Elementals::Attack()
{
	isIdling = false;
	isAttacking = true;
	isMoving = false;
	isDodging = false;
	isDefending = false;
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
	hitbox = { 0,0,0,0 };
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
		hitbox = { position.x + ElementalSpritesInfo[CurrentElementalType][11][0],position.y + ElementalSpritesInfo[CurrentElementalType][11][1],ElementalSpritesInfo[CurrentElementalType][11][2],ElementalSpritesInfo[CurrentElementalType][11][3] };
		AttackBox = { position.x + ElementalSpritesInfo[CurrentElementalType][13][0],position.y + ElementalSpritesInfo[CurrentElementalType][13][1],ElementalSpritesInfo[CurrentElementalType][13][2],ElementalSpritesInfo[CurrentElementalType][13][3] };
	}
	if (flip == SDL_FLIP_HORIZONTAL)
	{
		hitbox = { position.x + ElementalSpritesInfo[CurrentElementalType][12][0],position.y + ElementalSpritesInfo[CurrentElementalType][12][1],ElementalSpritesInfo[CurrentElementalType][12][2],ElementalSpritesInfo[CurrentElementalType][12][3] };
		AttackBox = { position.x + ElementalSpritesInfo[CurrentElementalType][14][0],position.y + ElementalSpritesInfo[CurrentElementalType][14][1],ElementalSpritesInfo[CurrentElementalType][14][2],ElementalSpritesInfo[CurrentElementalType][14][3] };
	}
	if (hitbox.x<0 && player1.BulletOrigin.x>LEVEL_WIDTH / 2) hitbox.x += LEVEL_WIDTH;
	else if (hitbox.x + hitbox.w > LEVEL_WIDTH && player1.BulletOrigin.x < LEVEL_WIDTH / 2) hitbox.x -= LEVEL_WIDTH;
	if (hitbox.y + hitbox.h > LEVEL_HEIGHT && player1.BulletOrigin.y < LEVEL_HEIGHT / 2) hitbox.y -= LEVEL_HEIGHT;
	else if (hitbox.y<0 && player1.BulletOrigin.y>LEVEL_HEIGHT / 2) hitbox.y += LEVEL_HEIGHT;
}
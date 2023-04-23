#include <SDL.h>
#include <SDL_image.h>
#include "player.h"
#include "sprites.h"
#include "weapons.h"
#include "global.h"
#include "skills.h"
#include "UI.h"
#include "gameobjects.h"
#include "sounds.h"
#include <cmath>
void LoadPlayerSpritesFromDisk()
{
	PlayerSprites[0] = IMG_LoadTexture(renderer, "resources/player/KnightIdle.PNG");
	PlayerSprites[1] = IMG_LoadTexture(renderer, "resources/player/KnightRunning.PNG");
	PlayerSprites[2] = IMG_LoadTexture(renderer, "resources/player/KnightHurt.PNG");
	PlayerSprites[3] = IMG_LoadTexture(renderer, "resources/player/KnightDeath.PNG");
}
std::vector <std::vector<int>> PlayerSpritesInfo =                           //So thu tu, So frame, Toc do chuyen frame
{
	{0,2,16},   //Knight Idle
	{1,9,4},    //Knight Running
	{2,2,5},    //Knight Hurt
	{3,13,15},   //Knight Death
};
player::player() : PlayerWeapon(0)
{
	speed = 5;
	HitBoxSize = 62;
	SpriteSize = { 100,100 };
	DashCooldownTime = 3000;
}
void player::Init(int CurrentSet, int Using_Weapon)
{
	CurrentSprite = 0;
	tempframe = 1;
	NumOfSprites = PlayerSpritesInfo[0][1];
	framespeed = PlayerSpritesInfo[0][2];
	PlayerWeapon.LoadWeapon(0);
	CurrentWeapon = Using_Weapon;
	NumOfSprites = PlayerSpritesInfo[CurrentSet][1];
	framespeed = PlayerSpritesInfo[CurrentSet][2];
	Health = 100;
	Stamina = 100;
	isMoving = false;
	isDead = false;
	isSprinting = false;
	MeleeAttacking = false;
	MeleeAttacked = false;
	Dashing = false;
	isAttacking = false;
	isHurt = false;
	DashIcon.isOnCooldown = false;
	PlayerShield.ShieldIcon.isOnCooldown = false;
	flip = SDL_FLIP_NONE;
	DashCooldown.Restart();
	position.x = LEVEL_WIDTH / 2;
	position.y = LEVEL_HEIGHT / 2;
	LoadTexture(CurrentSet);
}
player::~player() {};
void player::LoadTexture(int CurrentSet)
{
	CurrentSprite = 0;
	tempframe = 1;
	texture = PlayerSprites[CurrentSet];
	NumOfSprites = PlayerSpritesInfo[CurrentSet][1];
	framespeed = PlayerSpritesInfo[CurrentSet][2];
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_QueryTexture(texture, NULL, NULL, &texturesize.x, &texturesize.y);
	framecalc();
	if (flip == SDL_FLIP_NONE) Hitbox = { float(position.x + 0.5 * (SpriteSize.x - HitBoxSize) + SpriteSize.x / 20) ,float(position.y + 0.5 * (SpriteSize.y - HitBoxSize) + SpriteSize.y / 20) ,float(HitBoxSize),float(HitBoxSize) };
	else Hitbox = { float(position.x + 0.5 * (SpriteSize.x - HitBoxSize) - SpriteSize.x / 20),float(position.y + 0.5 * (SpriteSize.y - HitBoxSize) + SpriteSize.y / 20),float(HitBoxSize),float(HitBoxSize) };
}
void player::Run()
{
	player1.LoadTexture(1);
}
void player::Idle()
{
	player1.LoadTexture(0);
}
void player::Hurt(int Damage)
{
	if (PlayerShield.isOn) PlayerShield.ShieldDamage(Damage);
	else Health -= Damage;
	if (!isHurt)
	{
		Mix_PlayChannel(-1, SoundEffects[4], 0);
		isHurt = true;
		LoadTexture(2);
	}
}
void player::Death()
{
	isDead = true;
	isHurt = false;
	isMoving = false;
	isAttacking = false;
	MeleeAttacking = false;
	if (BackgroundMusicIsPlaying) Mix_HaltChannel(BackgroundMusicChannel);
	LoadTexture(3);
	Hitbox = { 0,0,0,0 };
}
void PlayerShield()
{
	if (Keyboard[SDL_SCANCODE_Q] && !player1.PlayerShield.isOn && !player1.PlayerShield.ShieldIcon.isOnCooldown)
	{
		player1.PlayerShield.ActivateShield(player1.position);
	}
	if (player1.PlayerShield.isOn)
	{
		player1.PlayerShield.DecayShield();
	}
}
void player::PlayerDirection()
{
	Direction = { 0,0 };
	if (isHurt) return;
	if (Keyboard[SDL_SCANCODE_W]) Direction.y -= 1;
	if (Keyboard[SDL_SCANCODE_S]) Direction.y += 1;
	if (Keyboard[SDL_SCANCODE_A]) Direction.x -= 1;
	if (Keyboard[SDL_SCANCODE_D]) Direction.x += 1;
	if (Direction.x == 0 && Direction.y == 0) return;
	player1.isMoving = true;
	if (!isAttacking && !MeleeAttacking)
	{
		if (Direction.x > 0) flip = SDL_FLIP_NONE;
		else if (Direction.x < 0) flip = SDL_FLIP_HORIZONTAL;
	}
	float TempX = Direction.x;
	Direction.x = Direction.x / sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
	Direction.y = Direction.y / sqrt(TempX * TempX + Direction.y * Direction.y);
}
void PlayerDrawCorner(SDL_FRect TempRect[], SDL_FRect TempWeaponRect[], SDL_FRect TempShieldRect[])
{
	SDL_RenderCopyExF(renderer, player1.texture, &player1.frame, &TempRect[1], 0, NULL, player1.flip);
	SDL_RenderCopyExF(renderer, player1.texture, &player1.frame, &TempRect[2], 0, NULL, player1.flip);
	if (!player1.isDead)
	{
		SDL_RenderCopyExF(renderer, player1.PlayerWeapon.weapontexture, NULL, &TempWeaponRect[1], player1.PlayerWeapon.angle, player1.PlayerWeapon.Center, player1.flip);
		SDL_RenderCopyExF(renderer, player1.PlayerWeapon.weapontexture, NULL, &TempWeaponRect[2], player1.PlayerWeapon.angle, player1.PlayerWeapon.Center, player1.flip);
	}
	if (player1.PlayerShield.isOn)
	{
		SDL_RenderCopyF(renderer, player1.PlayerShield.ShieldTexture, NULL, &TempShieldRect[1]);
		SDL_RenderCopyF(renderer, player1.PlayerShield.ShieldTexture, NULL, &TempShieldRect[2]);
	}
}
void PlayerDraw(SDL_FRect TempRect, SDL_FRect TempWeaponRect, SDL_FRect TempShieldRect)
{
	SDL_RenderCopyExF(renderer, player1.texture, &player1.frame, &TempRect, 0, NULL, player1.flip);
	if (!player1.isDead) SDL_RenderCopyExF(renderer, player1.PlayerWeapon.weapontexture, NULL, &TempWeaponRect, player1.PlayerWeapon.angle, player1.PlayerWeapon.Center, player1.flip);
	if (player1.PlayerShield.isOn) SDL_RenderCopyF(renderer, player1.PlayerShield.ShieldTexture, NULL, &TempShieldRect);
}
void PlayerDrawEdge(SDL_FRect WeaponRect)
{
	SDL_FRect TempRect[3];
	SDL_FRect TempWeaponRect[3];
	SDL_FRect TempShieldRect[3];
	TempRect[0] = player1.SpriteBox;
	TempRect[1] = player1.SpriteBox;
	TempRect[2] = player1.SpriteBox;
	if (!player1.isDead)
	{
		TempWeaponRect[0] = WeaponRect;
		TempWeaponRect[1] = WeaponRect;
		TempWeaponRect[2] = WeaponRect;
	}
	if (player1.PlayerShield.isOn)
	{
		TempShieldRect[0] = player1.PlayerShield.ShieldHitBox;
		TempShieldRect[1] = player1.PlayerShield.ShieldHitBox;
		TempShieldRect[2] = player1.PlayerShield.ShieldHitBox;
	}
	if (player1.position.x < 0)
	{
		TempRect[0].x += LEVEL_WIDTH;
		TempWeaponRect[0].x += LEVEL_WIDTH;
		TempShieldRect[0].x += LEVEL_WIDTH;
		if (player1.position.y < 0)
		{
			TempRect[1].x += LEVEL_WIDTH; TempWeaponRect[1].x += LEVEL_WIDTH; TempShieldRect[1].x += LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT; TempWeaponRect[1].y += LEVEL_HEIGHT; TempShieldRect[1].y += LEVEL_HEIGHT;
			TempRect[2].y += LEVEL_HEIGHT; TempWeaponRect[2].y += LEVEL_HEIGHT; TempShieldRect[2].y += LEVEL_HEIGHT;
			PlayerDrawCorner(TempRect, TempWeaponRect, TempShieldRect);
		}
		PlayerDraw(TempRect[0], TempWeaponRect[0], TempShieldRect[0]);
	}
	if (player1.position.y < 0)
	{
		TempRect[0].y += LEVEL_HEIGHT;
		TempWeaponRect[0].y += LEVEL_HEIGHT;
		TempShieldRect[0].y += LEVEL_HEIGHT;
		if (player1.position.x > LEVEL_WIDTH - player1.SpriteSize.x)
		{
			TempRect[1].x -= LEVEL_WIDTH; TempWeaponRect[1].x -= LEVEL_WIDTH; TempShieldRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT; TempWeaponRect[1].y += LEVEL_HEIGHT; TempShieldRect[1].y += LEVEL_HEIGHT;
			TempRect[2].x -= LEVEL_WIDTH; TempWeaponRect[2].x -= LEVEL_WIDTH; TempShieldRect[2].x -= LEVEL_WIDTH;
			PlayerDrawCorner(TempRect, TempWeaponRect, TempShieldRect);
		}
		PlayerDraw(TempRect[0], TempWeaponRect[0], TempShieldRect[0]);
	}
	if (player1.position.x > LEVEL_WIDTH - player1.SpriteSize.x)
	{
		TempRect[0].x -= LEVEL_WIDTH;
		TempWeaponRect[0].x -= LEVEL_WIDTH;
		TempShieldRect[0].x -= LEVEL_WIDTH;
		if (player1.position.y > LEVEL_HEIGHT - player1.SpriteSize.y)
		{
			TempRect[1].x -= LEVEL_WIDTH; TempWeaponRect[1].x -= LEVEL_WIDTH; TempShieldRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT; TempWeaponRect[1].y -= LEVEL_HEIGHT; TempShieldRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT; TempWeaponRect[2].y -= LEVEL_HEIGHT; TempShieldRect[2].y -= LEVEL_HEIGHT;
			PlayerDrawCorner(TempRect, TempWeaponRect, TempShieldRect);
		}
		PlayerDraw(TempRect[0], TempWeaponRect[0], TempShieldRect[0]);
	}
	if (player1.position.y > LEVEL_HEIGHT - player1.SpriteSize.y)
	{

		TempRect[0].y -= LEVEL_HEIGHT;
		TempWeaponRect[0].y -= LEVEL_HEIGHT;
		TempShieldRect[0].y -= LEVEL_HEIGHT;
		if (player1.position.x < 0)
		{
			TempRect[1].x += LEVEL_WIDTH; TempWeaponRect[1].x += LEVEL_WIDTH; TempShieldRect[1].x += LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT; TempWeaponRect[1].y -= LEVEL_HEIGHT; TempShieldRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT; TempWeaponRect[2].y -= LEVEL_HEIGHT; TempShieldRect[2].y -= LEVEL_HEIGHT;
			PlayerDrawCorner(TempRect, TempWeaponRect, TempShieldRect);
		}
		PlayerDraw(TempRect[0], TempWeaponRect[0], TempShieldRect[0]);
	}
}

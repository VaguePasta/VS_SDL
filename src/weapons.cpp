#include "global.h"
#include "gameobjects.h"
#include "weapons.h"
#include "player.h"
#include "sounds.h"
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
void LoadWeaponSpritesFromDisk()
{
	WeaponSprites[0] = IMG_LoadTexture(renderer, "resources/weapons/AK47.PNG");
	WeaponSprites[1] = IMG_LoadTexture(renderer, "resources/weapons/Assault_Rifle.PNG");
	WeaponSprites[2] = IMG_LoadTexture(renderer, "resources/weapons/Launcher.PNG");
	WeaponSprites[3] = IMG_LoadTexture(renderer, "resources/weapons/Sword.PNG");
}
std::vector <std::vector<float>> WeaponSpritesList =                //So thu tu, Kich co, Tam ban, Toc do dan, Toc do ban, sat thuong
{
	{0,83.3,1200,12,8,33},
	{1,83.3,1200,14,10,22},
	{2,100,1800,10,0.5,400},
	{3,100,150,0,2,200},
};
weapon::weapon(int CurrentWeapon)
{
	texture = WeaponSprites[CurrentWeapon];
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	ShootingDelay.Start();
	for (int i = 0; i < Max_Bullets; i++) bullets.push_back(NULL);
	delete frame;
	frame = nullptr;
}
void weapon::LoadWeapon(int CurrentWeapon)
{
	if (CurrentWeapon != 3)
	{
		for (int i = 0; i < Max_Bullets; i++)
		{
			if (bullets[i] == NULL)
			{
				bullets[i] = new bullet(CurrentWeapon);
			}
			else if (!bullets[i]->isShot)
			{
				delete bullets[i];
				bullets[i] = new bullet(CurrentWeapon);
			}
		}
		delete Center;
		Center = nullptr;
		GunSound = SoundEffects[CurrentWeapon];
	}
	else
	{
		GunSound = SoundEffects[11];
		delete Center;
		Center = new SDL_FPoint;
	}
	texture = WeaponSprites[CurrentWeapon];
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SpriteSize = { WeaponSpritesList[CurrentWeapon][1],WeaponSpritesList[CurrentWeapon][1] };
	range = WeaponSpritesList[CurrentWeapon][2];
	bulletspeed = WeaponSpritesList[CurrentWeapon][3];
	shootingspeed = WeaponSpritesList[CurrentWeapon][4];
	damage = WeaponSpritesList[CurrentWeapon][5];
	ShootingDelay.Restart();
}
weapon::~weapon()
{}

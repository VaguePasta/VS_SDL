#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "global.h"
#include "timer.h"
#include "sprites.h"
#include "bullets.h"
void LoadWeaponSpritesFromDisk();
class weapon:public sprite
{
    public:
        int damage;
        float range;
        float bulletspeed;
        float shootingspeed;
        Timer ShootingDelay;
        Mix_Chunk *GunSound;
        weapon(int CurrentWeapon);
        std::vector <bullet*> bullets;
        void LoadWeapon(int);
        ~weapon();
};
extern std::vector <std::vector<float>> WeaponSpritesList;

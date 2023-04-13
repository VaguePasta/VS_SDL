#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "global.h"
#include "timer.h"
#include "sprites.h"
#include "bullets.h"
void LoadWeaponSpritesFromDisk();
class weapon
{
    public:
        int damage;
        SDL_Texture *weapontexture;
        SDL_FPoint Position;
        SDL_FPoint *Center;
        float angle;
        float range;
        float bulletspeed;
        float shootingspeed;
        int WeaponSize;
        Timer ShootingDelay;
        Mix_Chunk *GunSound;
        weapon(int CurrentWeapon);
        std::vector <bullet*> bullets;
        void LoadWeapon(int);
        ~weapon();
};
extern std::vector <std::vector<float>> WeaponSpritesList;
#endif // WEAPON_H_INCLUDED

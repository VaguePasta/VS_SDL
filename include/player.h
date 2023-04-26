#pragma once
#include <SDL.h>
#include <vector>
#include "sprites.h"
#include "weapons.h"
#include "skills.h"
class player:public sprite,public objects
{
    public:
        weapon PlayerWeapon;
        Shield PlayerShield;
        Timer DashCooldown;
        SDL_FPoint Direction;
        SDL_FPoint DashDirection;
        SDL_FPoint BulletOrigin;
        float Stamina;
        bool Dashing;
        bool isParalysed;
        bool isSprinting;
        Uint32 DashCooldownTime;
        int CurrentWeapon;
        bool MeleeAttacking;
        bool MeleeAttacked;
        player();
        void Init(int CurrentSet,int Using_Weapon);
        ~player();
        void Run();
        void Idle();
        void Hurt(int);
        void Death();
        void PlayerDirection();
        void LoadTexture(int);
};
void LoadPlayerSpritesFromDisk();
void PlayerShield();
template <class T> SDL_FRect Playerhitbox(SDL_RendererFlip flip,T Position)
{
    SDL_FRect hitbox;
    if (flip==SDL_FLIP_NONE) hitbox={Position.x+24,Position.y+22,62,62};
    else hitbox = {Position.x+14,Position.y+22,62,62};
    return hitbox;
}
void PlayerDash(SDL_FPoint &TempPos);

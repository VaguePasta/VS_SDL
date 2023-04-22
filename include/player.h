#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
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
        SDL_FPoint BulletOrigin;
        float Stamina;
        bool Dashing;
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
template <class T> SDL_FRect PlayerHitbox(SDL_RendererFlip flip,T Position)
{
    SDL_FRect Hitbox;
    if (flip==SDL_FLIP_NONE) Hitbox={Position.x+24,Position.y+22,62,62};
    else Hitbox = {Position.x+14,Position.y+22,62,62};
    return Hitbox;
}
void PlayerDash(SDL_FPoint &TempPos);
void TrailDrawing();
void PlayerDrawCorner(SDL_FRect[],SDL_FRect[],SDL_FRect[]);
void PlayerDraw(SDL_FRect,SDL_FRect,SDL_FRect);
void PlayerDrawEdge(SDL_FRect);
#endif // PLAYER_H_INCLUDED

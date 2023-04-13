#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <SDL.h>
#include <vector>
#include "sprites.h"
#include "weapons.h"
#include "skills.h"
class player:public sprite
{
    public:
        weapon PlayerWeapon;
        Shield PlayerShield;
        Timer DashCooldown;
        SDL_FPoint DashPosition;
        SDL_FPoint Direction;
        SDL_FPoint DashDirection;
        SDL_FPoint BulletOrigin;
        int Health;
        float Stamina;
        bool isHurt;
        bool Dashing;
        bool isDead;
        bool isSprinting;
        Uint32 DashCooldownTime;
        int CurrentWeapon;
        bool isMoving;
        bool isAttacking;
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
template <class T> SDL_Rect PlayerHitbox(SDL_RendererFlip flip,T Position)
{
    SDL_Rect Hitbox;
    if (flip==SDL_FLIP_NONE) Hitbox={(int)round(Position.x)+24,(int)round(Position.y)+22,62,62};
    else Hitbox = {(int)round(Position.x)+14,(int)round(Position.y)+22,62,62};
    return Hitbox;
}
void PlayerDash(SDL_FPoint &TempPos);
void TrailDrawing();
void PlayerDrawCorner(SDL_Rect[],SDL_FRect[],SDL_Rect[]);
void PlayerDraw(SDL_Rect,SDL_FRect,SDL_Rect);
void PlayerDrawEdge(SDL_FRect);
#endif // PLAYER_H_INCLUDED

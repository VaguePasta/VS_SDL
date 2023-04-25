#pragma once
#include <SDL.h>
#include <vector>
#include "sprites.h"
#include "timer.h"
#include "global.h"
#include <cmath>
extern std::vector<std::vector<std::vector<float>>> MinionsInfo;
void LoadMinionsSpritesFromDisk();
class Minions:public sprite,public objects
{
    public:
        bool isSpawn;
        bool isRanged;
        bool Decayed;
        Timer Cooldown;
        float AttackSpeed;
        int CurrentMinionType;
        int ShootingSlot;
        SDL_FRect AttackBox;
        Minions();
        ~Minions();
        void Death();
        void Hurt();
        void Spawn(SDL_FPoint PlayerPosition);
        void Attack();
        void Run();
        void Idle();
        void CalculateBoxes();
        void LoadTexture(int CurrentState);
};
void MinionDrawCorner(SDL_FRect[],int);
void MinionDrawEdge(int);
void MinionSlashDamage(Minions* minion, bool &MinionDamaged);
template <class T> float AngleCalculation(T pos1,T origin,SDL_RendererFlip *flip)
{
    if (abs(pos1.x-origin.x)>LEVEL_WIDTH/2) (pos1.x<origin.x)?(pos1.x+=LEVEL_WIDTH):(pos1.x-=LEVEL_WIDTH);
    if (abs(pos1.y-origin.y)>LEVEL_HEIGHT/2) (pos1.y<origin.y)?(pos1.y+=LEVEL_HEIGHT):(pos1.y-=LEVEL_HEIGHT);
    if (flip==nullptr)
        return atan2(origin.y-pos1.y,origin.x-pos1.x)*180/3.14159;
    else
    {
        float Angle=(atan2(origin.y-pos1.y,origin.x-pos1.x))*180/3.14159+180;
        if (*flip==SDL_FLIP_HORIZONTAL) Angle+=180;
        if (Angle<0) Angle+=360;
        else if (Angle>360) Angle-=360;
        return Angle;
    }
}
template <class T> float DistanceCalculation(T pos1,T pos2)
{
    if (pos1.x-pos2.x>LEVEL_WIDTH/2) (pos1.x<pos2.x)?(pos1.x+=LEVEL_WIDTH):(pos1.x-=LEVEL_WIDTH);
    if (pos1.y-pos2.y>LEVEL_HEIGHT/2) (pos1.y<pos2.y)?(pos1.y+=LEVEL_HEIGHT):(pos1.y-=LEVEL_HEIGHT);
    return sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-pos2.y)*(pos1.y-pos2.y));
}

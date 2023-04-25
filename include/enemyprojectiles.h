#pragma once
#include <SDL.h>
#include "sprites.h"
#include "timer.h"
#include <vector>
extern std::vector<std::vector<float>> EnemyProjectilesInfo;
class EnemyProjectiles:public sprite
{
    public:
        int type;
        bool isShot;
        bool Arrived;
        bool Decayed;
        bool isAnimated;
        Timer Delay;
        int damage;
        int range;
        int speed;
        float distancetraveled;
        SDL_FPoint Target;
        SDL_FPoint Origin;
        SDL_FRect hitbox;
        EnemyProjectiles();
        ~EnemyProjectiles();
        void Shoot();
        void Update();
        void Decay();
        void ChooseType(int);
};
void LoadEnemyProjectileSpritesFromDisk();
void EnemyProjectilesProcessing();

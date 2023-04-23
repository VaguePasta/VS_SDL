#pragma once
#include <SDL.h>
#include "sprites.h"
#include <vector>
extern std::vector<std::vector<float>> EnemyProjectilesInfo;
class EnemyProjectiles
{
    public:
        int type;
        bool isShot;
        bool Decayed;
        bool isAnimated;
        sprite Projectile;
        int damage;
        SDL_Texture *ProjectileTexture;
        float angle;
        int range;
        int speed;
        float distancetraveled;
        SDL_Point ProjectileSize;
        SDL_FPoint Target;
        SDL_FPoint Origin;
        SDL_FPoint Position;
        SDL_FRect Hitbox;
        SDL_FRect ProjectileSprite;
        EnemyProjectiles();
        ~EnemyProjectiles();
        void Shoot();
        void Update();
        void Decay();
        void ChooseType(int);
};
void LoadEnemiesProjectileSpritesFromDisk();
void DrawEnemyProjectiles(SDL_Rect*,int);

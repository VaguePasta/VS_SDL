#ifndef ENEMYPROJECTILES_H_INCLUDED
#define ENEMYPROJECTILES_H_INCLUDED
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
        SDL_Point Position;
        SDL_FPoint FPosition;
        SDL_Rect Hitbox;
        SDL_Rect ProjectileSprite;
        EnemyProjectiles();
        ~EnemyProjectiles();
        void Shoot();
        void Update();
        void Decay();
        void ChooseType(int);
};
void LoadEnemiesProjectileSpritesFromDisk();
void DrawEnemyProjectiles(SDL_Rect*,int);
#endif // ENEMYPROJECTILES_H_INCLUDED

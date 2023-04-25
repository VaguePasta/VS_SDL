#pragma once
#include "sprites.h"
#include <vector>
SDL_FPoint CalculateOrigin();
void LoadBulletSpritesFromDisk();
class bullet:public sprite
{
    public:
        bool isShot;
        bool isAnimated;
        bool Decayed;
        SDL_FPoint origin;
        int BulletSpeed;
        int BulletRange;
        int Damage;
        int CurrentWeapon;
        float DistanceTraveled;
        SDL_FRect hitbox;
        ~bullet();
        bullet(int);
        bool Decay();
        void Update();
        void Explosion(int,int);
};
extern std::vector <std::vector<float>> BulletSpritesList;
void BulletDrawCorner(SDL_FRect[],SDL_Rect*,int);
void BulletDrawEdge(SDL_Rect*,int);
void BulletExplosion(bullet*);

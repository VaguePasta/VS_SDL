#ifndef BULLETS_H_INCLUDED
#define BULLETS_H_INCLUDED
#include "sprites.h"
#include <vector>
SDL_FPoint CalculateOrigin();
void LoadBulletSpritesFromDisk();
class bullet
{
    public:
        bool isShot;
        bool isAnimated;
        bool Decayed;
        sprite BulletAnimation;
        SDL_Texture *bullettexture;
        SDL_FPoint origin;
        SDL_Point BulletPosition;
        SDL_FPoint BulletPositionFloat;
        int BulletSize;
        int BulletSpeed;
        int BulletRange;
        int CurrentWeapon;
        float DistanceTraveled;
        float angle;
        SDL_Rect Hitbox;
        SDL_Rect BulletSprite;
        ~bullet();
        bullet(int);
        bool Decay();
        void Update();
        void Explosion(int,int);
};
extern std::vector <std::vector<int>> BulletSpritesList;
void BulletDrawCorner(SDL_Rect[],SDL_Rect*,int);
void BulletDrawEdge(SDL_Rect*,int);
#endif // BULLETS_H_INCLUDED

#ifndef SPRITES_H_LOAD
#define SPRITES_H_LOAD
#include <SDL.h>
class sprite
{
    public:
        SDL_Texture *texture;
        int NumOfSprites;
        int CurrentSprite;
        int SpriteSize;
        int tempframe;
        int framespeed;
        float AnimationDelay;
        float SpriteAngle;
        SDL_Rect frame;
        SDL_FRect SpriteBox;
        sprite();
        sprite(int,int);
        ~sprite();
        void framecalc();
        SDL_FPoint position;
        SDL_Point texturesize;
        SDL_RendererFlip flip;
        void animation();

};
class objects
{
    public:
        float speed;
        float Health;
        bool isDead;
        bool isHurt;
        bool isMoving;
        bool isIdling;
        bool isAttacking;
        SDL_FRect Hitbox;
        int HitBoxSize;
};
#endif // SPRITES_H_LOAD

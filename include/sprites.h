#pragma once
#include <SDL.h>
class sprite
{
    public:
        SDL_Texture *texture;
        int NumOfSprites;
        int CurrentSprite;
        SDL_FPoint SpriteSize;
        SDL_FPoint* Center;
        float angle;
        int tempframe;
        int framespeed;
        float AnimationDelay;
        SDL_Rect* frame;
        SDL_FRect SpriteBox;
        sprite();
        sprite(int,int);
        ~sprite();
        void framecalc();
        SDL_FPoint position;
        SDL_Point texturesize;
        SDL_RendererFlip flip;
        void animation();
        void draw();
        void draw_corner(SDL_FRect[]);

};
class objects
{
    public:
        float Damage;
        float speed;
        float Health;
        bool isDead;
        bool isHurt;
        bool isMoving;
        bool isIdling;
        bool isAttacking;
        SDL_FRect hitbox;
        int HitBoxSize;
};

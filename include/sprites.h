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
        Uint32 AnimationDelay;
        float speed;
        float SpriteAngle;
        SDL_Rect frame;
        SDL_Rect Hitbox;
        SDL_Rect SpriteBox;
        int HitBoxSize;
        sprite();
        sprite(int,int);
        ~sprite();
        void framecalc();
        SDL_FPoint position;
        SDL_Point texturesize;
        SDL_RendererFlip flip;
        void animation();

};
SDL_Texture *animation(sprite &CurrentSprite);
#endif // SPRITES_H_LOAD

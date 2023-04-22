#ifndef SKILLS_H_INCLUDED
#define SKILLS_H_INCLUDED
#include <SDL.h>
#include <vector>
#include "timer.h"
#include "UI.h"
extern std::vector <float> Shields;
class Shield
{
    public:
        SDL_Texture *ShieldTexture;
        SDL_FPoint Position;
        Icons ShieldIcon;
        int Health;
        SDL_Point Size;
        Timer Time;
        Uint32 Cooldown;
        int DamageDelay;
        Uint32 LiveTime;
        bool isOn;
        bool Damaged;
        SDL_FRect ShieldHitBox;
        Shield();
        ~Shield();
        void ActivateShield(SDL_FPoint);
        void DecayShield();
        void ShieldDamage(int damage);
};
SDL_FPoint Dash();
void Sprint();
void GunShoot(bool&, Timer&,float&);
void Slash();
void SlashRecover();
void SlashDamage(bool[]);
#endif // SKILLS_H_INCLUDED

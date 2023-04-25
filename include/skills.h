#pragma once
#include <SDL.h>
#include <vector>
#include "timer.h"
#include "UI.h"
#include "sprites.h"
extern std::vector <float> Shields;
class Shield:public sprite
{
    public:
        Icons ShieldIcon;
        int Health;
        Timer Time;
        Uint32 Cooldown;
        int DamageDelay;
        Uint32 LiveTime;
        bool isOn;
        bool Damaged;
        SDL_FRect hitbox;
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
void SlashDamage(bool[], bool[]);

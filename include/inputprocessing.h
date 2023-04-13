#ifndef INPUTPROCESSING_H_INCLUDED
#define INPUTPROCESSING_H_INCLUDED
#include <SDL.h>
void inputprocessing();
void CalculateWeaponAngle();
void WeaponPosition();
void PlayerCollisions();
void Moving();
void GetMouseClick();
SDL_FPoint CalculateTarget();
#endif // INPUTPROCESSING_H_INCLUDED

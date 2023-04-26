#pragma once
#include <SDL.h>
void inputprocessing();
void CalculateWeaponAngle();
void WeaponPosition();
void PlayerCollisions();
void Moving(bool &Moved);
void GetMouseClick();
void PlayerCollisions(SDL_FPoint&);
SDL_FPoint CalculateTarget();

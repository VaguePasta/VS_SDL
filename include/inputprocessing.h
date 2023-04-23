#pragma once
#include <SDL.h>
void inputprocessing();
void CalculateWeaponAngle();
void WeaponPosition();
void PlayerCollisions();
void Moving(bool &Moved);
void GetMouseClick();
SDL_FPoint CalculateTarget();

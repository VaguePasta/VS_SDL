#pragma once
#include "elementals.h"
void ElementalPathfinding(Elementals*);
void ElementalAttacking(Elementals*);
void ElementalMoving(Elementals*,SDL_FPoint&,SDL_FPoint&);
void ElementalBulletCollision(Elementals*);
void ElementalDodge(Elementals*);
void ElementalBlock(Elementals*);
void ElementalBlockMelee(Elementals*);
void ElementalLogics();

#pragma once
#include "elementals.h"
void ElementalPathfinding(Elementals*);
void Elemental_isAttacking(Elementals* elemental, SDL_FPoint& ElementalCenter, SDL_FPoint PlayerCenter);
void ElementalAttack(Elementals*);
void ElementalMoving(Elementals*,SDL_FPoint&,SDL_FPoint);
void ElementalBulletCollision(Elementals*);
void ElementalDodge(Elementals*);
void ElementalBlock(Elementals*);
void ElementalBlockMelee(Elementals*);
void ElementalLogics();
void ElementalSlashDamage(Elementals*, bool&);
SDL_FPoint ElementalAiming(SDL_FPoint, SDL_FPoint,int);
#pragma once
void MinionLogics();
void MinionAttack(Minions*minion);
void Minion_isAttacking(Minions* minion, SDL_FPoint& MinionCenter, SDL_FPoint& PlayerCenter, SDL_FRect& hitbox);
void MinionPathfinding(Minions*minion);
void MinionMoving(Minions*,SDL_FPoint&,SDL_FPoint&);
void MinionBulletCollision(Minions*);
void MinionSlashDamage(Minions* minion, bool& MinionDamaged);
SDL_FPoint MinionAiming(SDL_FPoint&,SDL_FPoint&,int,int);

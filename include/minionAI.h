#pragma once
void MinionLogics();
void MinionAttacking(Minions*minion);
void MinionPathfinding(Minions*minion);
void MinionMoving(Minions*,SDL_FPoint&,SDL_FPoint&);
void MinionBulletCollision(Minions*);
void MinionSlashDamage(Minions* minion, bool& MinionDamaged);
SDL_FPoint MinionAiming(SDL_FPoint&,SDL_FPoint&,int,int);

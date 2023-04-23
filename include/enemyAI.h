#pragma once
void EnemyLogics();
void EnemyAttacking(Enemies *enemy);
void EnemyPathfinding(Enemies *enemy);
void EnemyMoving(Enemies*,SDL_FPoint&,SDL_FPoint&);
void EnemyBulletCollision(Enemies*);
void EnemyProjectilesProcessing();
SDL_FPoint EnemyAiming(SDL_FPoint&,SDL_FPoint&,int,int);

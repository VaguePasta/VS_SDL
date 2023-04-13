#ifndef ENEMY_AI_H_INCLUDED
#define ENEMY_AI_H_INCLUDED
void EnemyLogics();
void EnemyAttacking(Enemies *enemy);
void EnemyPathfinding(Enemies *enemy);
void EnemyMoving(Enemies*,SDL_FPoint&,SDL_FPoint&);
void EnemyProjectilesProcessing();
SDL_FPoint EnemyAiming(SDL_FPoint&,SDL_FPoint&,int,int);
#endif // ENEMY_AI_H_INCLUDED

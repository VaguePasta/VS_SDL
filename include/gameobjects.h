#ifndef GAMEOBJECTS_H_INCLUDED
#define GAMEOBJECTS_H_INCLUDED
#include "enemies.h"
#include "player.h"
#include "enemyprojectiles.h"
#include "UI.h"
#include <vector>
#include <SDL.h>
extern std::vector <Enemies*> enemy;
extern std::vector <EnemyProjectiles*> Projectiles;
extern player player1;
extern sprite SlashEffect;
extern Bars HealthBar;
extern Bars ShieldHealthBar;
extern Bars StaminaBar;
extern Bars MusicVolumeBar;
extern Bars SoundEffectVolumeBar;
extern Icons DashIcon;
extern Buttons GameButtons[];
class GameState
{
    public:
        bool quit;
        bool start;
        bool pause;
        bool game_is_over;
        bool settings;
        GameState();
        ~GameState();
};
extern GameState gamestate;
#endif // GAMEOBJECTS_H_INCLUDED
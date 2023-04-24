#pragma once
#include "minions.h"
#include "elementals.h"
#include "player.h"
#include "enemyprojectiles.h"
#include "UI.h"
#include <vector>
#include <SDL.h>
extern std::vector <Minions*> minion;
extern std::vector <Elementals*> elemental;
extern std::vector <MinionProjectiles*> Projectiles;
extern player player1;
extern sprite SlashEffect;
extern Bars HealthBar;
extern Bars ShieldHealthBar;
extern Bars StaminaBar;
extern Bars MusicVolumeBar;
extern Bars SoundEffectVolumeBar;
extern Icons DashIcon;
extern Buttons GameButtons[];
extern SDL_Rect CurrentScoreRect;
extern SDL_Texture* ScoreTexture;
extern SDL_Rect FPSRect;
extern SDL_Texture* FPSTexture;
extern Timer FPSCounter;
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

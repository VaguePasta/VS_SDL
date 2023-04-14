#include "gameobjects.h"
#include "enemies.h"
#include "player.h"
#include "UI.h"
#include <SDL.h>
#include <vector>
using namespace std;
vector <Enemies*> enemy;
vector <EnemyProjectiles*> Projectiles;
player player1;
sprite SlashEffect(21, 1);
Bars HealthBar;
Bars ShieldHealthBar;
Bars StaminaBar;
Bars MusicVolumeBar;
Bars SoundEffectVolumeBar;
Icons DashIcon;
Buttons GameButtons[8];
GameState::GameState() :quit(false), start(false), pause(false), game_is_over(false), settings(false)
{}
GameState::~GameState() {}
GameState gamestate;
SDL_Rect CurrentScoreRect;
SDL_Texture* ScoreTexture;
SDL_Rect FPSRect;
SDL_Texture* FPSTexture;
Timer FPSCounter;
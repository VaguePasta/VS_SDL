#include "global.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
const int LOGICAL_WIDTH = 1600;
const int LOGICAL_HEIGHT = 900;
const int LEVEL_WIDTH = 4096;
const int LEVEL_HEIGHT = 4096;
bool isFullScreen = true;
int SCORE = 0;
int HIGH_SCORE = 0;
Uint32 FrameCount = 0;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Uint64 GetTime = 0;
float DeltaTime = 0;
Uint32 LoopTime = 0;
Uint16 freq = 1000000000 / SDL_GetPerformanceFrequency();
TTF_Font* GlobalFont;
SDL_Texture* screentexture;
SDL_Texture* CamTexture;
const Uint8* Keyboard = SDL_GetKeyboardState(nullptr);
SDL_Point MousePosition;
SDL_Point MouseRelativePosition;
Uint32 MouseState;
bool MouseLeftDown;
bool MouseRightDown;
SDL_Texture* PlayerSprites[4];
SDL_Texture* WeaponSprites[4];
SDL_Texture* BulletSprites[3];
SDL_Texture* MinionSprites[5][5];
SDL_Texture* ElementalSprites[6][10];
SDL_Texture* EnemyProjectileSprites[7];
SDL_Texture* SkillTextures[1];
SDL_Texture* ButtonSprites[9];
SDL_Texture* EffectSprites[2];
SDL_Texture* Cursor[1];
SDL_Texture* GameBackground;
SDL_Texture* BackgroundSprites[4];
const int Max_Bullets = 40;
int Current_max_enemies = 18;
int Current_max_elementals =  std::ceil(Current_max_enemies / 5);
int Current_max_minions = Current_max_enemies - Current_max_elementals;
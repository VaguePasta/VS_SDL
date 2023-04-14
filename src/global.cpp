#include "global.h"
#include <SDL.h>
#include <SDL_ttf.h>
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
const int LOGICAL_WIDTH = 1600;
const int LOGICAL_HEIGHT = 900;
const int LEVEL_WIDTH = 5120;
const int LEVEL_HEIGHT = 5120;
bool isFullScreen = true;
int SCORE = 0;
int HIGH_SCORE = 0;
Uint32 FrameCount = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
float DeltaTime = 0;
Uint32 LoopTime = 0;
TTF_Font* GlobalFont;
SDL_Texture* screentexture;
SDL_Texture* CamTexture;
const Uint8* Keyboard = SDL_GetKeyboardState(NULL);
SDL_Point MousePosition;
SDL_Point MouseRelativePosition;
Uint32 MouseState;
bool MouseDown;
SDL_Texture* PlayerSprites[4];
SDL_Texture* WeaponSprites[4];
SDL_Texture* BulletSprites[3];
SDL_Texture* EnemySprites[5][5];
SDL_Texture* EnemyProjectileSprites[3];
SDL_Texture* SkillTextures[1];
SDL_Texture* ButtonSprites[8];
SDL_Texture* EffectSprites[2];
SDL_Texture* Cursor[1];
SDL_Texture* GameBackground;
SDL_Texture* BackgroundSprites[4];
const int Max_Bullets = 40;
int Current_max_enemies = 20;

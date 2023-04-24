#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "timer.h"
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern const int LOGICAL_WIDTH;
extern const int LOGICAL_HEIGHT;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;
extern Uint32 FrameCount;
extern Uint64 GetTime;
extern Uint16 freq;
extern float DeltaTime;
extern Uint32 LoopTime;
extern bool isFullScreen;
extern int SCORE;
extern int HIGH_SCORE;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *GlobalFont;
extern SDL_Texture *screentexture;
extern SDL_Texture *CamTexture;
extern SDL_Point MousePosition;
extern SDL_Point MouseRelativePosition;
extern Uint32 MouseState;
extern bool MouseLeftDown;
extern bool MouseRightDown;
extern std::vector <std::vector<int>> PlayerSpritesInfo;
extern SDL_Texture *PlayerSprites[];
extern SDL_Texture *MinionSprites[][5];
extern SDL_Texture* ElementalSprites[][10];
extern SDL_Texture *WeaponSprites[];
extern SDL_Texture *BulletSprites[];
extern SDL_Texture *MinionProjectileSprites[];
extern SDL_Texture *SkillTextures[];
extern SDL_Texture *EffectSprites[];
extern SDL_Texture *Cursor[];
extern SDL_Texture *ButtonSprites[];
extern SDL_Texture *GameBackground;
extern SDL_Texture *BackgroundSprites[];
extern const int Max_Bullets;
extern const Uint8 *Keyboard;
extern int Current_max_minions;
extern int Current_max_elementals;

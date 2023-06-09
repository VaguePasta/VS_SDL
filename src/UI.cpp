#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "UI.h"
#include "gameobjects.h"
#include <cmath>
#include <string>
void LoadButtons()
{
	ButtonSprites[0] = IMG_LoadTexture(renderer, "resources/UI/buttons/Play.PNG");
	ButtonSprites[1] = IMG_LoadTexture(renderer, "resources/UI/buttons/Back.PNG");
	ButtonSprites[2] = IMG_LoadTexture(renderer, "resources/UI/buttons/Menu.PNG");
	ButtonSprites[3] = IMG_LoadTexture(renderer, "resources/UI/buttons/Quit.PNG");
	ButtonSprites[4] = IMG_LoadTexture(renderer, "resources/UI/buttons/NextSong.PNG");
	ButtonSprites[5] = IMG_LoadTexture(renderer, "resources/UI/buttons/Settings.PNG");
	ButtonSprites[6] = IMG_LoadTexture(renderer, "resources/UI/buttons/BackSquare.PNG");
	ButtonSprites[7] = IMG_LoadTexture(renderer, "resources/UI/buttons/Check.PNG");
	ButtonSprites[8] = IMG_LoadTexture(renderer, "resources/UI/buttons/Cross.PNG");
}
void LoadFont()
{
	GlobalFont = TTF_OpenFont("resources/fonts/minecraft_font.ttf", 14);
}
Bars::Bars()
{
	BarBorder = nullptr;
	ProgressBar = nullptr;
}
void Bars::Init(std::string border, std::string progress, int Max_Value)
{
	BarBorder = IMG_LoadTexture(renderer, border.c_str());
	ProgressBar = IMG_LoadTexture(renderer, progress.c_str());
	SDL_SetTextureBlendMode(BarBorder, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(ProgressBar, SDL_BLENDMODE_BLEND);
	SDL_QueryTexture(BarBorder, nullptr, nullptr, &BorderBarSize.x, &BorderBarSize.y);
	SDL_QueryTexture(ProgressBar, nullptr, nullptr, &ProgressBarSize.x, &ProgressBarSize.y);
	MaxValue = Max_Value;
	Value = MaxValue;
	FinalTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, BorderBarSize.x, BorderBarSize.y);
}
Bars::~Bars() {}
void Bars::DrawBar(SDL_FPoint pos)
{
	if (Value < 0) Value = 0;
	else if (Value > MaxValue) Value = MaxValue;
	SDL_Surface* TempSurface = TTF_RenderText_Solid(GlobalFont, std::to_string(Value).c_str(), { 255,255,255 });
	BarValue = SDL_CreateTextureFromSurface(renderer, TempSurface);
	SDL_SetTextureBlendMode(FinalTexture, SDL_BLENDMODE_BLEND);
	BarRect = { (int)round(pos.x),(int)round(pos.y),BorderBarSize.x,BorderBarSize.y };
	SDL_SetRenderTarget(renderer, FinalTexture);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, BarBorder, nullptr, nullptr);
	SDL_Rect ProgressRect = { (int)(BorderBarSize.x - ProgressBarSize.x) / 2,(int)(BorderBarSize.y - ProgressBarSize.y) / 2,(int)round(ProgressBarSize.x * (1.0 * Value / MaxValue)),ProgressBarSize.y };
	SDL_RenderCopy(renderer, ProgressBar, nullptr, &ProgressRect);
	SDL_Rect BarValueRect = { (int)round((BorderBarSize.x - TempSurface->w) / 2),BorderBarSize.y / 10,TempSurface->w,TempSurface->h };
	SDL_RenderCopy(renderer, BarValue, nullptr, &BarValueRect);
	SDL_SetRenderTarget(renderer, nullptr);
	SDL_FreeSurface(TempSurface);
	SDL_DestroyTexture(BarValue);
	SDL_RenderCopy(renderer, FinalTexture, nullptr, &BarRect);
}
Icons::Icons() {}
Icons::~Icons() {}
void Icons::Init(std::string path)
{
	isOnCooldown = false;
	Icon = IMG_LoadTexture(renderer, path.c_str());
	IconValue = nullptr;
	SDL_QueryTexture(Icon, nullptr, nullptr, &IconSize.x, &IconSize.y);
	FinalTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, IconSize.x, IconSize.y);
}
void Icons::DrawIcon(SDL_FPoint pos, bool Using_Skill)
{
	SDL_Surface* TempSurface = TTF_RenderText_Solid(GlobalFont, std::to_string(Value).c_str(), { 255,255,255 });
	IconValue = SDL_CreateTextureFromSurface(renderer, TempSurface);
	if (Using_Skill) SDL_SetTextureColorMod(Icon, 255, 255, 255);
	else SDL_SetTextureColorMod(Icon, 192, 192, 192);
	if (isOnCooldown)
		SDL_SetTextureColorMod(Icon, 105, 105, 105);
	SDL_SetTextureBlendMode(FinalTexture, SDL_BLENDMODE_BLEND);
	IconRect = { pos.x,pos.y,(float)IconSize.x,(float)IconSize.y };
	SDL_SetRenderTarget(renderer, FinalTexture);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, Icon, nullptr, nullptr);
	if (isOnCooldown)
	{
		IconValueRect = { float(IconSize.x - TempSurface->w) / 2,float(IconSize.y - TempSurface->h) / 2,(float)TempSurface->w,(float)TempSurface->h };
		SDL_RenderCopyF(renderer, IconValue, nullptr, &IconValueRect);
	}
	SDL_SetRenderTarget(renderer, nullptr);
	SDL_FreeSurface(TempSurface);
	SDL_DestroyTexture(IconValue);
	SDL_RenderCopyF(renderer, FinalTexture, nullptr, &IconRect);
}
Buttons::Buttons() {}
Buttons::~Buttons() {}
void Buttons::Init(int i)
{
	Button = ButtonSprites[i];
	SDL_QueryTexture(Button, nullptr, nullptr, &Size.x, &Size.y);
}
void Buttons::DrawButton(SDL_Point Position, int Scale)
{
	ButtonRect = { Position.x,Position.y,Size.x / Scale,Size.y / Scale };
	if (!SDL_PointInRect(&MousePosition, &ButtonRect)) SDL_SetTextureColorMod(Button, 192, 192, 192);
	else SDL_SetTextureColorMod(Button, 255, 255, 255);
	SDL_RenderCopy(renderer, Button, nullptr, &ButtonRect);
}
void DrawScore(SDL_Texture* ScoreAnnounce[], SDL_Rect ScoreRect[])
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, ScoreAnnounce[0], nullptr, &ScoreRect[0]);
	SDL_RenderCopy(renderer, ScoreAnnounce[1], nullptr, &ScoreRect[1]);
	SDL_RenderCopy(renderer, ScoreAnnounce[2], nullptr, &ScoreRect[2]);
}
void UpdateCurrentScore()
{
	SDL_Color ScoreColor;
	if (SCORE < 30) ScoreColor = { 0,192,0 };
	else if (SCORE < 50) ScoreColor = { 255,255,0 };
	else ScoreColor = { 255,0,0 };
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_Surface* ScoreTemp = TTF_RenderText_Solid(GlobalFont, std::to_string(SCORE).c_str(), ScoreColor);
	ScoreTexture = SDL_CreateTextureFromSurface(renderer, ScoreTemp);
	CurrentScoreRect = { 1500 - ScoreTemp->w * 2,20,ScoreTemp->w * 4,ScoreTemp->h * 4 };
	SDL_FreeSurface(ScoreTemp);
}
void DrawCurrentScore()
{
	SDL_RenderCopy(renderer, ScoreTexture, nullptr, &CurrentScoreRect);
}
void DrawLetter(SDL_Texture* StringContent, SDL_Point pos)
{
    int w,h;
    SDL_QueryTexture(StringContent,nullptr,nullptr,&w,&h);
	SDL_Rect StringRect = { pos.x,pos.y,w,h };
	SDL_RenderCopy(renderer, StringContent, nullptr, &StringRect);
}
void DrawFPS()
{
	if (FPSCounter.GetTime() >= 1500 || FPSTexture == nullptr)
	{
		int CurrentFPS = (FPSCounter.GetTime()>=1500) ? (FrameCount / int(round(FPSCounter.GetTime() / 1000.0))):(0);
		SDL_Surface* TempSurface = TTF_RenderText_Solid(GlobalFont, std::to_string(CurrentFPS).c_str(), { 255,255,0 });
		FPSRect = { 400,10,TempSurface->w,TempSurface->h };
		FPSTexture = SDL_CreateTextureFromSurface(renderer, TempSurface);
		SDL_FreeSurface(TempSurface);
		FPSCounter.Restart();
		FrameCount = 0;
	}
	SDL_RenderCopy(renderer, FPSTexture, nullptr, &FPSRect);
}

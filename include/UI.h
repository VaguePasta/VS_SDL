#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED
#include <SDL.h>
#include <string>
void LoadFont();
void LoadButtons();
class Bars
{
    public:
        int Value;
        int MaxValue;
        SDL_Point BorderBarSize;
        SDL_Point ProgressBarSize;
        SDL_Texture *BarBorder;
        SDL_Texture *ProgressBar;
        SDL_Texture *BarValue;
        SDL_Texture *FinalTexture;
        SDL_Rect BarRect;
        Bars();
        void Init(std::string,std::string,int);
        ~Bars();
        void DrawBar(SDL_FPoint pos);
};
class Icons
{
    public:
    int Value;
    bool isOnCooldown;
    SDL_FRect IconRect;
    SDL_FRect IconValueRect;
    SDL_Point IconSize;
    SDL_Texture *IconValue;
    SDL_Texture *Icon;
    SDL_Texture *FinalTexture;
    Icons();
    void Init(std::string);
    ~Icons();
    void DrawIcon(SDL_FPoint pos,bool Using_Skill);
};
class Buttons
{
    public:
        SDL_Point Size;
        SDL_Texture *Button;
        SDL_Rect ButtonRect;
        void DrawButton(SDL_Point,int);
        void Init(int);
        Buttons();
        ~Buttons();
};
void DrawScore(SDL_Texture*[],SDL_Rect ScoreRect[]);
void DrawCurrentScore();
void DrawLetter(SDL_Texture*,SDL_Point);
void UpdateCurrentScore();
#endif // UI_H_INCLUDED

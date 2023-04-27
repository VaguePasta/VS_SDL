#pragma once
#include "UI.h"
void StartMenu();
void PauseMenu();
void GameOver();
void Settings();
void Pause();
void Start_Button();
void Menu_Button();
void Back_Button();
void ChangeResolution(int,int);
void ChangeButton(Buttons[], SDL_Texture* [], int, int, int, int);
void ChangeVolume(Bars&);

#pragma once
#include "UI.h"
void StartMenu();
void PauseMenu();
void GameOver();
void Settings();
void Start_Button();
void Menu_Button();
void Quit_Button();
void Back_Button();
void ChangeResolution(int,int);
void ChangeVolume(Bars&);
void ChangeButton(Buttons[],SDL_Texture**, int, int, int, int);

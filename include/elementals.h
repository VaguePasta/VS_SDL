#pragma once
#include "sprites.h"
#include "timer.h"
#include <vector>
#include <SDL.h>
void LoadElementalSpritesFromDisk();
class Elementals :public sprite, public objects
{
	public:
		bool isSpawn;
		bool Decayed;
		bool isDodging;
		bool isDefending;
		Timer Cooldown;
		float AttackSpeed;
		int CurrentElementalType;
		int AttackType;
		Timer DodgeCooldown;
		Timer BlockCooldown;
		SDL_FRect AttackBox;
		SDL_FPoint ElementalCenter;
		Elementals();
		~Elementals();
		void Spawn(SDL_FPoint PlayerPosition);
		void ChooseAttack();
		void Attack();
		void Run();
		void Dodge();
		void Idle();
		void Defend();
		void Hurt();
		void Death();
		void CalculateBoxes();
		void LoadTexture(int CurrentState);
};
extern std::vector <std::vector<std::vector<float>>> ElementalSpritesInfo;
void ElementalDrawEdge(int i);
void ElementalDrawCorner(SDL_Rect[], int);
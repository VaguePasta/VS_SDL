#include "effects.h"
#include "gameobjects.h"
#include "texturerendering.h"
#include <SDL.h>
void SlashEffectDraw()
{
	if (SlashEffect.Center == nullptr) SlashEffect.Center = new SDL_FPoint;
	if (player1.flip == SDL_FLIP_NONE)
	{
		*SlashEffect.Center = { 111,108 };
		if (player1.position.y + player1.SpriteSize.y / 2 < MousePosition.y + camera[0].CameraPosition.y)
		{
			SlashEffect.position.x = player1.position.x - 65;
			SlashEffect.position.y = player1.position.y - 30;
		}
		else
		{
			SlashEffect.position.x = player1.position.x - 75;
			SlashEffect.position.y = player1.position.y - 55;
		}
	}
	else
	{
		*SlashEffect.Center = { 189,108 };
		if (player1.position.y + player1.SpriteSize.y / 2 < MousePosition.y + camera[0].CameraPosition.y)
		{
			SlashEffect.position.x = player1.position.x - 135;
			SlashEffect.position.y = player1.position.y - 30;
		}
		else
		{
			SlashEffect.position.x = player1.position.x - 145;
			SlashEffect.position.y = player1.position.y - 55;
		}
	}
	SlashEffect.draw();
}

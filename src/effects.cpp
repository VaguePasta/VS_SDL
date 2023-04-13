#include "effects.h"
#include "gameobjects.h"
#include "texturerendering.h"
#include <SDL.h>
void SlashEffectDraw()
{
	SDL_Point Center;
	if (player1.flip == SDL_FLIP_NONE)
	{
		Center = { 111,108 };
		if (player1.position.y + player1.SpriteSize / 2 < MousePosition.y + camera[0].CameraPosition.y)
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
		Center = { 189,108 };
		if (player1.position.y + player1.SpriteSize / 2 < MousePosition.y + camera[0].CameraPosition.y)
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
	SlashEffect.animation();
	SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &SlashEffect.SpriteBox, SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
	SlashEffectDrawEdge(Center);
}
void SlashEffectDrawEdge(SDL_Point Center)
{
	SDL_Rect TempRect[3];
	TempRect[0] = SlashEffect.SpriteBox;
	TempRect[1] = SlashEffect.SpriteBox;
	TempRect[2] = SlashEffect.SpriteBox;
	if (SlashEffect.SpriteBox.x < 0)
	{
		TempRect[0].x += LEVEL_WIDTH;
		if (SlashEffect.SpriteBox.y < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].y += LEVEL_HEIGHT;
			SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[1], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
			SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[2], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
		}
		SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[0], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
	}
	if (SlashEffect.SpriteBox.y < 0)
	{
		TempRect[0].y += LEVEL_HEIGHT;
		if (SlashEffect.SpriteBox.x > LEVEL_WIDTH - SlashEffect.SpriteSize)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].x -= LEVEL_WIDTH;
			SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[1], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
			SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[2], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
		}
		SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[0], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
	}
	if (SlashEffect.SpriteBox.x > LEVEL_WIDTH - SlashEffect.SpriteSize)
	{
		TempRect[0].x -= LEVEL_WIDTH;
		if (SlashEffect.SpriteBox.y > LEVEL_HEIGHT - SlashEffect.SpriteSize)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[1], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
			SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[2], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
		}
		SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[0], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
	}
	if (SlashEffect.SpriteBox.y > LEVEL_HEIGHT - SlashEffect.SpriteSize)
	{

		TempRect[0].y -= LEVEL_HEIGHT;
		if (SlashEffect.SpriteBox.x < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[1], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
			SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[2], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
		}
		SDL_RenderCopyEx(renderer, SlashEffect.texture, &SlashEffect.frame, &TempRect[0], SlashEffect.SpriteAngle, &Center, SlashEffect.flip);
	}
}

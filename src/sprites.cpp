#include <SDL.h>
#include "sprites.h"
#include "global.h"
#include <cmath>
sprite::sprite()
{
	flip = SDL_FLIP_NONE;
	AnimationDelay = 0;
	angle = 0;
	frame = new SDL_Rect;
	Center = nullptr;
}
sprite::sprite(int _NumOfFrames, int _FrameSpeed)
{
	frame = new SDL_Rect;
	NumOfSprites = _NumOfFrames;
	framespeed = _FrameSpeed;
	CurrentSprite = 0;
	tempframe = 1;
	flip = SDL_FLIP_NONE;
	AnimationDelay = 0;
	Center = nullptr;
	angle = 0;
}
sprite::~sprite() 
{
	delete frame;
	delete Center;
}
void sprite::framecalc()
{
	int increment = texturesize.x / NumOfSprites;
	*frame = { CurrentSprite * increment,0,increment,texturesize.y };
}
void sprite::animation()
{	
	SpriteBox = { position.x,position.y,SpriteSize.x,SpriteSize.y};
	if (frame == nullptr) return;
	framecalc();
	AnimationDelay += DeltaTime;
	if (AnimationDelay >= 0.9)
	{
		if (tempframe / framespeed >= 1) { CurrentSprite++; tempframe = 1; }
		tempframe++;
		if (CurrentSprite > NumOfSprites - 1) CurrentSprite = 0;
		AnimationDelay = 0;
	}
}
void sprite::draw()
{
	animation();
	SDL_RenderCopyExF(renderer, texture, frame, &SpriteBox, angle, Center, flip);
	SDL_FRect TempRect[3];
	TempRect[0] = SpriteBox;
	TempRect[1] = SpriteBox;
	TempRect[2] = SpriteBox;
	if (position.x < 0)
	{
		TempRect[0].x += LEVEL_WIDTH;
		if (position.y < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].y += LEVEL_HEIGHT;
			draw_corner(TempRect);
		}
		SDL_RenderCopyExF(renderer, texture, frame, &TempRect[0], angle, Center, flip);
	}
	if (position.y < 0)
	{
		TempRect[0].y += LEVEL_HEIGHT;
		if (position.x > LEVEL_WIDTH - SpriteBox.w)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y += LEVEL_HEIGHT;
			TempRect[2].x -= LEVEL_WIDTH;
			draw_corner(TempRect);
		}
		SDL_RenderCopyExF(renderer, texture, frame, &TempRect[0], angle, Center, flip);
	}
	if (position.x > LEVEL_WIDTH - SpriteBox.w)
	{
		TempRect[0].x -= LEVEL_WIDTH;
		if (position.y > LEVEL_HEIGHT - SpriteBox.h)
		{
			TempRect[1].x -= LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			draw_corner(TempRect);
		}
		SDL_RenderCopyExF(renderer, texture, frame, &TempRect[0], angle, Center, flip);
	}
	if (position.y > LEVEL_HEIGHT - SpriteBox.h)
	{
		TempRect[0].y -= LEVEL_HEIGHT;
		if (position.x < 0)
		{
			TempRect[1].x += LEVEL_WIDTH;
			TempRect[1].y -= LEVEL_HEIGHT;
			TempRect[2].y -= LEVEL_HEIGHT;
			draw_corner(TempRect);
		}
		SDL_RenderCopyExF(renderer, texture, frame, &TempRect[0], angle, Center, flip);
	}
}
void sprite::draw_corner(SDL_FRect TempRect[])
{
	SDL_RenderCopyExF(renderer, texture, frame, &TempRect[1], angle, Center, flip);
	SDL_RenderCopyExF(renderer, texture, frame, &TempRect[2], angle, Center, flip);
}
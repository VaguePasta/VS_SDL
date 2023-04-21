#include <SDL.h>
#include "sprites.h"
#include "global.h"
#include <cmath>
sprite::sprite()
{
	flip = SDL_FLIP_NONE;
	AnimationDelay = 0;
}
sprite::sprite(int _NumOfFrames, int _FrameSpeed)
{
	NumOfSprites = _NumOfFrames;
	framespeed = _FrameSpeed;
	CurrentSprite = 0;
	tempframe = 1;
	flip = SDL_FLIP_NONE;
	AnimationDelay = 0;
}
sprite::~sprite() {}
void sprite::framecalc()
{
	int increment = texturesize.x / NumOfSprites;
	frame = { CurrentSprite * increment,0,increment,texturesize.y };
}
void sprite::animation()
{
	framecalc();
	AnimationDelay += DeltaTime;
	if (AnimationDelay >= 0.9)
	{
		if (tempframe / framespeed >= 1) { CurrentSprite++; tempframe = 1; }
		tempframe++;
		if (CurrentSprite > NumOfSprites - 1) CurrentSprite = 0;
		AnimationDelay = 0;
	}
	SpriteBox = { (int)round(position.x),(int)round(position.y),SpriteSize,SpriteSize };
}

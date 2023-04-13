#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "background.h"
#include "global.h"
void LoadBackgroundSpritesFromDisk()
{
	for (int i = 0; i <= 3; i++)
	{
		std::string temp = "resources/background/floor_" + std::to_string(i) + ".PNG";
		BackgroundSprites[i] = IMG_LoadTexture(renderer, temp.c_str());
	}
}
void Drawbackground()
{
	GameBackground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, LEVEL_WIDTH, LEVEL_HEIGHT);
	SDL_SetRenderTarget(renderer, GameBackground);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	for (int i = 0; i <= LEVEL_HEIGHT - 64; i += 64)
		for (int j = 0; j <= LEVEL_WIDTH - 64; j += 64)
		{
			SDL_Rect dsrect = { j,i,64,64 };
			static int floortype = -1;
			int previous = floortype;
			int random;
			do
			{
				random = rand() % 100 + 1;
				if (random <= 94) floortype = 0;
				else if (random > 94 && random <= 96) floortype = 1;
				else if (random > 96 && random <= 98) floortype = 2;
				else if (random > 98 && random <= 100) floortype = 2;
			} while (floortype == previous && floortype != 0);
			SDL_RenderCopy(renderer, BackgroundSprites[floortype], NULL, &dsrect);
		}
	SDL_SetTextureBlendMode(GameBackground, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, NULL);
}

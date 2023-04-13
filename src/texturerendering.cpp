#include "texturerendering.h"
#include "global.h"
#include "gameobjects.h"
#include <SDL.h>
#include <cmath>
Camera::Camera()
{
	CameraSize.x = LOGICAL_WIDTH;
	CameraSize.y = LOGICAL_HEIGHT;
}
Camera::~Camera() {}
void Camera::CameraCalculation()
{
	CameraPosition.x = player1.position.x - 800 + 50;
	CameraPosition.y = player1.position.y - 450 + 50;
	CameraRect = { (int)round(CameraPosition.x),(int)round(CameraPosition.y),LOGICAL_WIDTH,LOGICAL_HEIGHT };
}
void ClearTexture(SDL_Texture* Texture)
{
	SDL_SetRenderTarget(renderer, screentexture);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
}
void Camera::CameraWrapping()
{
	CameraRect = { (int)round(CameraPosition.x),(int)round(CameraPosition.y),CameraSize.x,CameraSize.y };
}
Camera camera[5];
void WrapCamera()
{
	SDL_SetRenderTarget(renderer, CamTexture);
	SDL_RenderClear(renderer);
	SDL_Point Position = { (int)player1.position.x - 800 + 50,(int)player1.position.y - 450 + 50 };
	SDL_Point PlayerCenter = { Position.x + 800,Position.y + 450 };
	if (Position.x < 0)
	{
		camera[1].CameraPosition.x = LEVEL_WIDTH + Position.x;
		camera[1].CameraSize.x = 800 - PlayerCenter.x;
		camera[2].CameraPosition.x = 0;
		camera[2].CameraSize.x = 1600 - camera[1].CameraSize.x;
		camera[3].CameraPosition.x = 0;
		camera[3].CameraPosition.y = 0;
		camera[3].CameraSize.x = camera[2].CameraSize.x;
		camera[4].CameraPosition.x = camera[1].CameraPosition.x;
		camera[4].CameraPosition.y = 0;
		camera[4].CameraSize.x = camera[1].CameraSize.x;
		if (Position.y < 0)
		{
			camera[1].CameraPosition.y = LEVEL_HEIGHT + Position.y;
			camera[1].CameraSize.y = 450 - PlayerCenter.y;
			camera[2].CameraPosition.y = camera[1].CameraPosition.y;
			camera[2].CameraSize.y = camera[1].CameraSize.y;
			camera[3].CameraSize.y = 900 - camera[1].CameraSize.y;
			camera[4].CameraSize.y = camera[3].CameraSize.y;
			SDL_Rect Cam1 = { 0,0,camera[1].CameraSize.x,camera[1].CameraSize.y };
			SDL_Rect Cam2 = { camera[1].CameraSize.x,0,camera[2].CameraSize.x,camera[2].CameraSize.y };
			SDL_Rect Cam3 = { camera[1].CameraSize.x,camera[1].CameraSize.y,camera[3].CameraSize.x,camera[3].CameraSize.y };
			SDL_Rect Cam4 = { 0,camera[1].CameraSize.y,camera[4].CameraSize.x,camera[4].CameraSize.y };
			camera[1].CameraWrapping();
			camera[2].CameraWrapping();
			camera[3].CameraWrapping();
			camera[4].CameraWrapping();
			SDL_RenderCopy(renderer, screentexture, &camera[1].CameraRect, &Cam1);
			SDL_RenderCopy(renderer, screentexture, &camera[2].CameraRect, &Cam2);
			SDL_RenderCopy(renderer, screentexture, &camera[3].CameraRect, &Cam3);
			SDL_RenderCopy(renderer, screentexture, &camera[4].CameraRect, &Cam4);
		}
		else if (Position.y + 900 > LEVEL_HEIGHT)
		{
			camera[1].CameraPosition.y = Position.y;
			camera[1].CameraSize.y = LEVEL_HEIGHT - PlayerCenter.y + 450;
			camera[2].CameraPosition.y = camera[1].CameraPosition.y;
			camera[2].CameraSize.y = camera[1].CameraSize.y;
			camera[3].CameraSize.y = 900 - camera[1].CameraSize.y;
			camera[4].CameraSize.y = camera[3].CameraSize.y;
			SDL_Rect Cam1 = { 0,0,camera[1].CameraSize.x,camera[1].CameraSize.y };
			SDL_Rect Cam2 = { camera[1].CameraSize.x,0,camera[2].CameraSize.x,camera[2].CameraSize.y };
			SDL_Rect Cam3 = { camera[1].CameraSize.x,camera[1].CameraSize.y,camera[3].CameraSize.x,camera[3].CameraSize.y };
			SDL_Rect Cam4 = { 0,camera[1].CameraSize.y,camera[4].CameraSize.x,camera[4].CameraSize.y };
			camera[1].CameraWrapping();
			camera[2].CameraWrapping();
			camera[3].CameraWrapping();
			camera[4].CameraWrapping();
			SDL_RenderCopy(renderer, screentexture, &camera[1].CameraRect, &Cam1);
			SDL_RenderCopy(renderer, screentexture, &camera[2].CameraRect, &Cam2);
			SDL_RenderCopy(renderer, screentexture, &camera[3].CameraRect, &Cam3);
			SDL_RenderCopy(renderer, screentexture, &camera[4].CameraRect, &Cam4);
		}
		else
		{
			camera[1].CameraPosition.y = Position.y;
			camera[1].CameraSize.y = 900;
			camera[2].CameraPosition.y = camera[1].CameraPosition.y;
			camera[2].CameraSize.y = camera[1].CameraSize.y;
			SDL_Rect Cam1 = { 0,0,camera[1].CameraSize.x,camera[1].CameraSize.y };
			SDL_Rect Cam2 = { camera[1].CameraSize.x,0,camera[2].CameraSize.x,camera[2].CameraSize.y };
			camera[1].CameraWrapping();
			camera[2].CameraWrapping();
			SDL_RenderCopy(renderer, screentexture, &camera[1].CameraRect, &Cam1);
			SDL_RenderCopy(renderer, screentexture, &camera[2].CameraRect, &Cam2);
		}
	}
	else if (Position.x + 1600 > LEVEL_WIDTH)
	{
		camera[1].CameraPosition.x = Position.x;
		camera[1].CameraSize.x = LEVEL_WIDTH - PlayerCenter.x + 800;
		camera[2].CameraPosition.x = 0;
		camera[2].CameraSize.x = 1600 - camera[1].CameraSize.x;
		camera[3].CameraPosition.x = 0;
		camera[3].CameraPosition.y = 0;
		camera[3].CameraSize.x = camera[2].CameraSize.x;
		camera[4].CameraPosition.x = camera[1].CameraPosition.x;
		camera[4].CameraPosition.y = 0;
		camera[4].CameraSize.x = camera[1].CameraSize.x;
		if (Position.y < 0)
		{
			camera[1].CameraPosition.y = LEVEL_HEIGHT + Position.y;
			camera[1].CameraSize.y = 450 - PlayerCenter.y;
			camera[2].CameraPosition.y = camera[1].CameraPosition.y;
			camera[2].CameraSize.y = camera[1].CameraSize.y;
			camera[3].CameraSize.y = 900 - camera[1].CameraSize.y;
			camera[4].CameraSize.y = camera[3].CameraSize.y;
			SDL_Rect Cam1 = { 0,0,camera[1].CameraSize.x,camera[1].CameraSize.y };
			SDL_Rect Cam2 = { camera[1].CameraSize.x,0,camera[2].CameraSize.x,camera[2].CameraSize.y };
			SDL_Rect Cam3 = { camera[1].CameraSize.x,camera[1].CameraSize.y,camera[3].CameraSize.x,camera[3].CameraSize.y };
			SDL_Rect Cam4 = { 0,camera[1].CameraSize.y,camera[4].CameraSize.x,camera[4].CameraSize.y };
			camera[1].CameraWrapping();
			camera[2].CameraWrapping();
			camera[3].CameraWrapping();
			camera[4].CameraWrapping();
			SDL_RenderCopy(renderer, screentexture, &camera[1].CameraRect, &Cam1);
			SDL_RenderCopy(renderer, screentexture, &camera[2].CameraRect, &Cam2);
			SDL_RenderCopy(renderer, screentexture, &camera[3].CameraRect, &Cam3);
			SDL_RenderCopy(renderer, screentexture, &camera[4].CameraRect, &Cam4);
		}
		else if (Position.y + 900 > LEVEL_HEIGHT)
		{
			camera[1].CameraPosition.y = Position.y;
			camera[1].CameraSize.y = LEVEL_HEIGHT - PlayerCenter.y + 450;
			camera[2].CameraPosition.y = camera[1].CameraPosition.y;
			camera[2].CameraSize.y = camera[1].CameraSize.y;
			camera[3].CameraSize.y = 900 - camera[1].CameraSize.y;
			camera[4].CameraSize.y = camera[3].CameraSize.y;
			SDL_Rect Cam1 = { 0,0,camera[1].CameraSize.x,camera[1].CameraSize.y };
			SDL_Rect Cam2 = { camera[1].CameraSize.x,0,camera[2].CameraSize.x,camera[2].CameraSize.y };
			SDL_Rect Cam3 = { camera[1].CameraSize.x,camera[1].CameraSize.y,camera[3].CameraSize.x,camera[3].CameraSize.y };
			SDL_Rect Cam4 = { 0,camera[1].CameraSize.y,camera[4].CameraSize.x,camera[4].CameraSize.y };
			camera[1].CameraWrapping();
			camera[2].CameraWrapping();
			camera[3].CameraWrapping();
			camera[4].CameraWrapping();
			SDL_RenderCopy(renderer, screentexture, &camera[1].CameraRect, &Cam1);
			SDL_RenderCopy(renderer, screentexture, &camera[2].CameraRect, &Cam2);
			SDL_RenderCopy(renderer, screentexture, &camera[3].CameraRect, &Cam3);
			SDL_RenderCopy(renderer, screentexture, &camera[4].CameraRect, &Cam4);
		}
		else
		{
			camera[1].CameraPosition.y = Position.y;
			camera[1].CameraSize.y = 900;
			camera[2].CameraPosition.y = camera[1].CameraPosition.y;
			camera[2].CameraSize.y = camera[1].CameraSize.y;
			SDL_Rect Cam1 = { 0,0,camera[1].CameraSize.x,camera[1].CameraSize.y };
			SDL_Rect Cam2 = { camera[1].CameraSize.x,0,camera[2].CameraSize.x,camera[2].CameraSize.y };
			camera[1].CameraWrapping();
			camera[2].CameraWrapping();
			SDL_RenderCopy(renderer, screentexture, &camera[1].CameraRect, &Cam1);
			SDL_RenderCopy(renderer, screentexture, &camera[2].CameraRect, &Cam2);
		}
	}
	else if (Position.y < 0)
	{
		camera[1].CameraPosition.x = Position.x;
		camera[1].CameraPosition.y = Position.y + LEVEL_HEIGHT;
		camera[1].CameraSize.x = 1600;
		camera[1].CameraSize.y = 450 - PlayerCenter.y;
		camera[2].CameraPosition.x = camera[1].CameraPosition.x;
		camera[2].CameraPosition.y = 0;
		camera[2].CameraSize.x = 1600;
		camera[2].CameraSize.y = 900 - camera[1].CameraSize.y;
		SDL_Rect Cam1 = { 0,0,camera[1].CameraSize.x,camera[1].CameraSize.y };
		SDL_Rect Cam2 = { 0,camera[1].CameraSize.y,camera[2].CameraSize.x,camera[2].CameraSize.y };
		camera[1].CameraWrapping();
		camera[2].CameraWrapping();
		SDL_RenderCopy(renderer, screentexture, &camera[1].CameraRect, &Cam1);
		SDL_RenderCopy(renderer, screentexture, &camera[2].CameraRect, &Cam2);
	}
	else if (Position.y + 900 > LEVEL_HEIGHT)
	{
		camera[1].CameraPosition.x = Position.x;
		camera[1].CameraPosition.y = Position.y;
		camera[1].CameraSize.x = 1600;
		camera[1].CameraSize.y = LEVEL_HEIGHT - PlayerCenter.y + 450;
		camera[2].CameraPosition.x = camera[1].CameraPosition.x;
		camera[2].CameraPosition.y = 0;
		camera[2].CameraSize.x = 1600;
		camera[2].CameraSize.y = 900 - camera[1].CameraSize.y;
		SDL_Rect Cam1 = { 0,0,camera[1].CameraSize.x,camera[1].CameraSize.y };
		SDL_Rect Cam2 = { 0,camera[1].CameraSize.y,camera[2].CameraSize.x,camera[2].CameraSize.y };
		camera[1].CameraWrapping();
		camera[2].CameraWrapping();
		SDL_RenderCopy(renderer, screentexture, &camera[1].CameraRect, &Cam1);
		SDL_RenderCopy(renderer, screentexture, &camera[2].CameraRect, &Cam2);
	}
	else
	{
		SDL_RenderCopy(renderer, screentexture, &camera[0].CameraRect, NULL);
	}
	SDL_SetRenderTarget(renderer, NULL);
}

#pragma once
#include <SDL.h>
class Camera
{
    public:
        SDL_FPoint CameraPosition;
        SDL_Point CameraSize;
        SDL_Rect CameraRect;
        void CameraCalculation();
        void CameraWrapping();
        Camera();
        ~Camera();
};
extern Camera camera[];
void ClearTexture (SDL_Texture *Texture);
void WrapCamera();

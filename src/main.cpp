#include "gameobjects.h"
#include "program.h"
#include <SDL.h>
int main(int argc,char*argv[])
{
    init();
    static Uint64 GetTime = 0;
    static auto freq = 1000000000 / SDL_GetPerformanceFrequency();
    while (!gamestate.quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)!=0)
        {
            if (e.type==SDL_QUIT) gamestate.quit=true;
        }
        LoopTime= SDL_GetPerformanceCounter()*freq - GetTime;
        GetTime=SDL_GetPerformanceCounter()*freq;
        DeltaTime = 1.0*(LoopTime)/16666666;
        GameLoop();
    }
    return 0;
}

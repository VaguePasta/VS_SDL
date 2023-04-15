#include "gameobjects.h"
#include "program.h"
#include <SDL.h>
int main(int argc,char*argv[])
{
    init();
    while (!gamestate.quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)!=0)
        {
            if (e.type==SDL_QUIT) gamestate.quit=true;
        }
        GameLoop();
    }
    return 0;
}

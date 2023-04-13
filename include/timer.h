#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED
#include <SDL.h>
class Timer
{
    public:
        bool Started;
        bool Paused;
        Uint32 StartTime;
        Uint32 GetTime();
        float TimeWhenPaused;
        void Start();
        void Pause();
        void Unpause();
        void Reset();
        void Restart();
        Timer();
        Timer(bool);
        ~Timer();
};
#endif // TIMER_H_INCLUDED

#include <SDL.h>
#include "timer.h"
Timer::Timer()
{
	Started = false;
	Paused = false;
	TimeWhenPaused = 0;
	StartTime = 0;
}
Timer::Timer(bool start)
{
	Started = false;
	Paused = false;
	TimeWhenPaused = 0;
	StartTime = 0;
	if (start) Start();
}
Timer::~Timer() {};
void Timer::Start()
{
	Started = true;
	Paused = false;
	StartTime = SDL_GetTicks();
}
void Timer::Reset()
{
	Started = false;
	Paused = false;
	StartTime = 0;
}
void Timer::Pause()
{
	if (Started && !Paused)
	{
		Paused = true;
		TimeWhenPaused = SDL_GetTicks() - StartTime;
		StartTime = 0;
	}
}
void Timer::Unpause()
{
	if (Started && Paused)
	{
		Paused = false;
		StartTime = SDL_GetTicks() - TimeWhenPaused;
		TimeWhenPaused = 0;
	}
}
void Timer::Restart()
{
	Reset();
	Start();
}
Uint32 Timer::GetTime()
{
	if (Started)
	{
		if (Paused) return TimeWhenPaused;
		else return SDL_GetTicks() - StartTime;
	}
	return 0;
}

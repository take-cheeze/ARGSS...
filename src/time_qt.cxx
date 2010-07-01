#include "time_qt.hxx"
#include <SDL.h>

namespace Time
{
	long GetTime()
	{
		return SDL_GetTicks();
	}

	void SleepMs(long ms)
	{
		SDL_Delay(ms);
	}
}

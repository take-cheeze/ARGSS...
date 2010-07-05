#include "time_qt.hxx"
#include <SDL.h>

namespace Time
{
	long getTime()
	{
		return SDL_GetTicks();
	}

	void SleepMs(long ms)
	{
		SDL_Delay(ms);
	}
}

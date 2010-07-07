#include "time.hxx"
#include <SDL.h>

namespace Time
{
	long getTime()
	{
		return SDL_GetTicks();
	}

	void sleepMs(long ms)
	{
		SDL_Delay(ms);
	}
}

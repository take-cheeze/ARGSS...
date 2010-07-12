#include "..//time.hxx"

#include <iostream>

#include <SDL.h>


namespace Time
{
	long getTime()
	{
		return SDL_GetTicks();
	}

	void sleepMs(long ms)
	{
		// std::cout << "Sleeping (ms): " << ms << std::endl;
		SDL_Delay(ms);
	}
}

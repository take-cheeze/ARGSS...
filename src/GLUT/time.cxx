#include "..//time.hxx"

#include <iostream>

#include <boost/thread.hpp>


namespace Time
{
	long getTime()
	{
		return boost::get_system_time().total_milliseconds();
	}

	void sleepMs(long ms)
	{
		boost::this_thread::sleep( boost::posix_time::milliseconds(ms) );
	}
}

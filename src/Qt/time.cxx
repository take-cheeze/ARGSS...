#include "time.hxx"

#include <QtCore/QThread>
#include <QtCore/QTime>

#include <iostream>

namespace Time
{
	namespace
	{
		class SleepAccess : public QThread
		{
		public:
			using QThread::sleep;
			using QThread::msleep;
			using QThread::usleep;
		};
	} // namespace

	long getTime()
	{
		QTime curTime = QTime::currentTime();
		// std::cout << ( ( ( curTime.hour() * 60 + curTime.minute() ) * 60 ) + curTime.second() ) * 1000 + curTime.msec() << std::endl;
		return ( ( ( curTime.hour() * 60 + curTime.minute() ) * 60 ) + curTime.second() ) * 1000 + curTime.msec();
	}

	void sleepMs(long ms)
	{
		// std::cout << "Sleeping (ms): " << ms << std::endl;
		SleepAccess::msleep(ms);
	}
}

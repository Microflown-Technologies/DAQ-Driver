#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include "szevent.h"
#include <thread>

class MyTimer
{
public:
	MyTimer();
	~MyTimer();

	sz::event<> Tick;

	bool Start(unsigned int millisecondsInterval);
	bool Stop();

private:
	void TimerFunc();

	bool m_isRunning;
	unsigned int m_millisecondsInterval;
	std::thread m_timerThread;
};

#endif // __MYTIMER_H__
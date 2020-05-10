#include "Timer.h"
#include <chrono>

namespace mnp
{
	Timer::Timer()
	{
	}

	Timer::~Timer()
	{
	}

	void Timer::set(uint32 interval, EUnit unit)
	{
		_currUnit = unit;

		_interval = interval;
	}

	void Timer::start()
	{
		_startTime = std::chrono::steady_clock::now().time_since_epoch().count();
	}

	bool Timer::tick() const
	{
		uint64 elapsed{ std::chrono::steady_clock::now().time_since_epoch().count() - _startTime };

		switch (_currUnit)
		{
		case mnp::Timer::EUnit::Nano:
			__noop;
			break;
		case mnp::Timer::EUnit::Micro:
			elapsed /= 1'000;
			break;
		case mnp::Timer::EUnit::Milli:
			elapsed /= 1'000'000;
			break;
		case mnp::Timer::EUnit::Sec:
			elapsed /= 1'000'000'000;
			break;
		default:
			break;
		}

		if (elapsed >= _interval)
		{
			return true;
		}
		
		return false;
	}

	void Timer::reset()
	{
		start();
	}
}
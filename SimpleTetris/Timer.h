#pragma once
#include "CommonTypes.h"

//일정 시간이 지났는지 알려주는 함수를 만드는 클래스
namespace mnp
{
	class Timer
	{
	public:
		enum class EUnit
		{
			Nano,
			Micro,
			Milli,
			Sec
		};

	public:
		Timer();
		~Timer();
	
	public:
		void set(uint32 interval, EUnit unit);

		void reset();

		void start();

		bool tick() const;

	private:
		//간격
		uint32 _interval{};
		uint64 _startTime{};

	private:
		EUnit _currUnit{ EUnit::Micro };
	};
}

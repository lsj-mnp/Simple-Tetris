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

		void start();

		void stop();
		//_tickCount를 0으로 만들고 다시 처음부터 시간을 잰다.
		//tick()을 호출할 경우 굳이 이 함수를 호출할 필요가 없다.
		void reset();

		//set()에서 정한 시간이 흐르면 _tickCount가 증가한다.
		//_tickCount가 0보다 클 때 reset()을 호출하고 true를 return한다.
		bool tick();

	private:
		void update();

	private:
		//간격
		uint32 _interval{};
		uint64 _startTime{};
		uint64 _tickCount{};

	private:
		EUnit _currUnit{ EUnit::Micro };

	private:
		bool _isTicking{ false };
	};
}

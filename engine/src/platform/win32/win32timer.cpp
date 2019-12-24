
#ifdef ENGINE_WINDOWS

#include <Windows.h>
#include <profileapi.h>

#include "core/timer.h"
#include "common/assert.h"

namespace prev {

	static pvdouble PC_FREQUENCY = 0.0;
	static pvint64 COUNTER_START = 0;

	TimePoint Timer::GetTime() {
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		TimePoint tp = {};
		tp.Time = static_cast<double>(static_cast<pvint64>(li.QuadPart) - COUNTER_START) / PC_FREQUENCY;
		return tp;
	}

	struct TimerStart {
		TimerStart() {
			LARGE_INTEGER li;
			QueryPerformanceFrequency(&li);
			PC_FREQUENCY = static_cast<double>(li.QuadPart) / 1000000.0;

			QueryPerformanceCounter(&li);
			COUNTER_START = static_cast<pvint64>(li.QuadPart);
		}
	};

	static TimerStart START;

}

#endif
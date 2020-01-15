#pragma once

#include "../common/types.h"

namespace prev {
	
	struct TimePoint {
		friend class Timer;

		inline pvdouble GetS() const { return Time / 1000000.0; }
		inline pvdouble GetMS() const { return Time / 1000.0; }
		inline pvdouble GetUS() const { return Time; }
		inline pvdouble GetNS() const { return Time * 1000.0; }

		TimePoint operator-(const TimePoint & other) {
			return TimePoint(Time - other.Time);
		}
	private:
		TimePoint() : Time(0.0) {}
		TimePoint(pvdouble time) : Time(time) {}

		pvdouble Time;
	};

	class Timer {
	public:
		static void Update();
		inline static TimePoint GetDeltatTime() { return s_DeltaTime; }

		// Platform Specific
		static TimePoint GetTime();
	private:
		static TimePoint s_PrevTime, s_CurrentTime, s_DeltaTime;
	};

}
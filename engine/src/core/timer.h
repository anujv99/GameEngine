#pragma once

#include "../common/types.h"

namespace prev {
	
	struct TimePoint {
		friend class Timer;

		inline pvdouble GetS() const { return Time / 1000000.0; }
		inline pvdouble GetMS() const { return Time / 1000.0; }
		inline pvdouble GetUS() const { return Time; }
		inline pvdouble GetNS() const { return Time * 1000.0; }
	private:
		pvdouble Time;
	};

	class Timer {
	public:
		static TimePoint GetTime();
	};

}
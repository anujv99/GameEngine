#include "timer.h"

namespace prev {

	TimePoint Timer::s_CurrentTime;
	TimePoint Timer::s_DeltaTime;
	TimePoint Timer::s_PrevTime;

	void prev::Timer::Update() {
		s_PrevTime = s_CurrentTime;
		s_CurrentTime = Timer::GetTime();
		s_DeltaTime = s_CurrentTime - s_PrevTime;
	}

}
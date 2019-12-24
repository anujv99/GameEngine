#include "instrumentor.h"

namespace prev {

	std::stack<pvint64> InstrumentationTimer::s_ParentsTimes;
	ProfileResult InstrumentationTimer::s_Results[512];
	pvuint InstrumentationTimer::s_ArrayIndex = 0u;

}
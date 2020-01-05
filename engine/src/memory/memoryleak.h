#pragma once

#ifdef ENGINE_WINDOWS

#include <crtdbg.h>

namespace prev {

	struct MemoryLeakDetector {
		MemoryLeakDetector() {
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			_CrtMemCheckpoint(&m_Start);
		}
		~MemoryLeakDetector() {
			_CrtMemCheckpoint(&m_End);
			_CrtMemDifference(&m_Diff, &m_Start, &m_End);
		}
		void SetBreak(long alloc) {
			_CrtSetBreakAlloc(alloc);
		}
	private:
		_CrtMemState m_Start	= {};
		_CrtMemState m_End		= {};
		_CrtMemState m_Diff		= {};
	};

}

#else

namespace prev {

	struct MemoryLeakDetector {

	};

}

#endif

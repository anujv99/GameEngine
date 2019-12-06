#pragma once

#include <cassert>
#include <csignal>

#include "log.h"

#ifdef PREV_GAME_ENGINE

	#if defined(_MSC_VER)
		#define DEBUG_BREAK() __debugbreak()
	#elif defined(__GNUC__)
		#define DEBUG_BREAK() raise(SIGTRAP)
	#endif

	#if defined(ENGINE_ASSERT) && defined(DEBUG_BREAK)
		#define ASSERT(X) if (!(X)) { LOG_ERROR("Assertion \"%s\" failed, File : %s, Line : %d", #X, __FILE__, __LINE__); DEBUG_BREAK(); }
	#elif defined(ENGINE_ASSERT)
		#define ASSERT(X) assert(X)
	#else
		#define ASSERT(X)
	#endif

#endif
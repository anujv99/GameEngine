#pragma once

#include <cassert>

#ifdef PREV_GAME_ENGINE

	#if defined(ENGINE_ASSERT) && defined(ENGINE_WINDOWS)
		#define ASSERT(X) if (!(X)) { assert(X); __debugbreak(); }
	#elif defined(ENGINE_ASSERT)
		#define ASSERT(X) assert(X)
	#else
		#define ASSERT(X)
	#endif

#endif
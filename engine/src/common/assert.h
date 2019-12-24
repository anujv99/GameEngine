#pragma once

#include <cassert>
#include <csignal>

#include "../core/logger.h"

#if defined(_MSC_VER)
	#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__)
	#define DEBUG_BREAK() raise(SIGTRAP)
#endif

#ifdef ENGINE_LOGGING
	#define ASSERT_LOG(...) prev::Logger::ChangeColor(prev::LoggerColor::COLOR_RED);\
							prev::Logger::Log(__VA_ARGS__);\
							prev::Logger::ChangeColor(prev::LoggerColor::COLOR_ORIGINAL)
#else
	#define ASSERT_LOG(...)
#endif

#if defined(ENGINE_ASSERT) && defined(DEBUG_BREAK)
	#define ASSERTM(X, M) if (!(X)) { ASSERT_LOG("Assertion failed \"%s\", file : %s, line : %d - ", #X, __FILE__, __LINE__); ASSERT_LOG(M); DEBUG_BREAK(); }
	#define ASSERT(X) if (!(X)) { ASSERT_LOG("Assertion failed \"%s\", file : %s, line : %d - ", #X, __FILE__, __LINE__); DEBUG_BREAK(); }
#elif defined(ENGINE_ASSERT)
	#define ASSERTM(X, M) if(!(X)) { ASSERT_LOG(M); } assert(X)
	#define ASSERT(X) assert(X)
#else
	#define ASSERTM(X, M)
	#define ASSERT(X)
#endif
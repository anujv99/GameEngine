#pragma once

#include <cstdio>
#include <cstdarg>

#include "../common/log.h"
#include "../common/types.h"

namespace prev {

	class VMLogger {
	public:
		static void Log(const char * fmt, ...) {
			va_list args;
			va_start(args, fmt);
			pvstring str;
			str.resize(1024);
			vsnprintf(str.data(), str.size(), fmt, args);
			LOG_TRACE("[LUA] %s", str.c_str());
			va_end(args);
		}
	};

}
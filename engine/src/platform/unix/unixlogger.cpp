
#ifdef ENGINE_LINUX && defined(ENGINE_LOGGING)

#include "enginepch.h"

namespace prev {

	static std::unordered_map<LoggerColor, pvstring> LOGGER_COLOR_MAP({
		{ LoggerColor::COLOR_ORIGINAL,	"\033[0m" },
		{ LoggerColor::COLOR_RED,		"\033[1;31m" },
		{ LoggerColor::COLOR_GREEN,		"\033[1;32m" },
		{ LoggerColor::COLOR_YELLOW,	"\033[1;33m" },
		{ LoggerColor::COLOR_BLUE,		"\033[1;34m" },
		{ LoggerColor::COLOR_MAGENTA,	"\033[1;35m" },
		{ LoggerColor::COLOR_CYAN,		"\033[1;36m" },
		{ LoggerColor::COLOR_WHITE,		"\033[1;37m" },
		});

	void Logger::ChangeColor(LoggerColor color) {
		printf(LOGGER_COLOR_MAP[color].c_str());
	}

	void Logger::Log(const pvchar * fmt, ...) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}

}

#endif

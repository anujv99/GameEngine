
#ifdef ENGINE_WINDOWS && defined(ENGINE_LOGGING)

#include <Windows.h>

namespace prev {

	static std::unordered_map<LoggerColor, pvint> LOGGER_COLOR_MAP({
		{ LoggerColor::COLOR_ORIGINAL,	7  },
		{ LoggerColor::COLOR_BLUE,		9  },
		{ LoggerColor::COLOR_GREEN,		10 },
		{ LoggerColor::COLOR_CYAN,		11 },
		{ LoggerColor::COLOR_RED,		12 },
		{ LoggerColor::COLOR_MAGENTA,	13 },
		{ LoggerColor::COLOR_YELLOW,	14 },
		{ LoggerColor::COLOR_WHITE,		15 },
	});

	void Logger::ChangeColor(LoggerColor color) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		if (console) {
			SetConsoleTextAttribute(console, (WORD)LOGGER_COLOR_MAP[color]);
		}
	}

	void Logger::Log(const pvchar * fmt, ...) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}

}

#endif
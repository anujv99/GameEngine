#pragma once

#include "../common/types.h"

namespace prev {

	enum class LoggerColor {
		COLOR_RED,
		COLOR_WHITE,
		COLOR_BLUE,
		COLOR_GREEN,
		COLOR_YELLOW,
		COLOR_CYAN,
		COLOR_MAGENTA,
		COLOR_ORIGINAL
	};

	class Logger {
	public:
		static void ChangeColor(LoggerColor color);
		static void Log(const pvchar * fmt, ...);
	};

}

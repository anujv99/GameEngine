#pragma once

#include <cstdio>
#include <cstdarg>

#include "../common/log.h"
#include "../common/types.h"

#define LUA_LOG(...)	app::core::Logger::ChangeColor(app::core::LoggerColor::RED);\
						app::core::Logger::Log("[LUA]" __VA_ARGS__);\
						app::core::Logger::Log("\n");\
						app::core::Logger::ChangeColor(app::core::LoggerColor::ORIGINAL);

#define LUA_DEBUG_LOG(...)	app::core::Logger::ChangeColor(app::core::LoggerColor::RED);\
							{\
								lua_Debug ar;\
								lua_getstack(L, 1, &ar);\
								lua_getinfo(L, "nSl", &ar);\
								app::core::Logger::Log("[LUA %s:%d]", ar.source, ar.currentline);\
							}\
							app::core::Logger::Log( __VA_ARGS__);\
							app::core::Logger::Log("\n");\
							app::core::Logger::ChangeColor(app::core::LoggerColor::ORIGINAL);

#define LUA_LOG(...)	prev::Logger::ChangeColor(prev::LoggerColor::COLOR_RED);\
						prev::VMLogger::Log(__VA_ARGS__);\
						prev::VMLogger::Log("\n");\
						prev::Logger::ChangeColor(prev::LoggerColor::COLOR_WHITE);

#define LUA_DEBUG_LOG(...)	prev::Logger::ChangeColor(prev::LoggerColor::COLOR_RED);\
							{\
								lua_Debug ar;\
								lua_getstack(L, 1, &ar);\
								lua_getinfo(L, "nSl", &ar);\
								prev::VMLogger::Log("[LUA %s:%d]", ar.source, ar.currentline);\
							}\
							prev::VMLogger::Log(__VA_ARGS__);\
							prev::VMLogger::Log("\n");\
							prev::Logger::ChangeColor(prev::LoggerColor::COLOR_WHITE);

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
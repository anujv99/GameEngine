#pragma once

#ifdef ENGINE_LOGGING

	#include "logger.h"

	#define LOG_CHANGE_COLOR_RED()		prev::Logger::ChangeColor(prev::LoggerColor::COLOR_RED)
	#define LOG_CHANGE_COLOR_GREEN()	prev::Logger::ChangeColor(prev::LoggerColor::COLOR_GREEN)
	#define LOG_CHANGE_COLOR_YELLOW()	prev::Logger::ChangeColor(prev::LoggerColor::COLOR_YELLOW)
	#define LOG_CHANGE_COLOR_BLUE()		prev::Logger::ChangeColor(prev::LoggerColor::COLOR_BLUE)
	#define LOG_CHANGE_COLOR_MAGENTA()	prev::Logger::ChangeColor(prev::LoggerColor::COLOR_MAGENTA)
	#define LOG_CHANGE_COLOR_CYAN()		prev::Logger::ChangeColor(prev::LoggerColor::COLOR_CYAN)
	#define LOG_CHANGE_COLOR_WHITE()	prev::Logger::ChangeColor(prev::LoggerColor::COLOR_WHITE)
	#define LOG_COLOR_RESET()			prev::Logger::ChangeColor(prev::LoggerColor::COLOR_ORIGINAL)

	#define LOG(...) prev::Logger::Log(__VA_ARGS__)

	#ifdef ENGINE_DEBUG
		#define DLOG_TRACE(...)	LOG_CHANGE_COLOR_CYAN();	LOG("[DLOG] "); LOG(__VA_ARGS__); LOG_COLOR_RESET(); LOG("\n")
		#define DLOG_INFO(...)	LOG_CHANGE_COLOR_GREEN();	LOG("[DLOG] "); LOG(__VA_ARGS__); LOG_COLOR_RESET(); LOG("\n")
		#define DLOG_WARN(...)	LOG_CHANGE_COLOR_YELLOW();	LOG("[DLOG] "); LOG(__VA_ARGS__); LOG_COLOR_RESET(); LOG("\n")
		#define DLOG_ERROR(...)	LOG_CHANGE_COLOR_RED();		LOG("[DLOG] "); LOG(__VA_ARGS__); LOG_COLOR_RESET(); LOG("\n")
		#define DLOG_FATAL(...)	LOG_CHANGE_COLOR_MAGENTA(); LOG("[DLOG] "); LOG(__VA_ARGS__); LOG_COLOR_RESET(); LOG("\n")
	#else
		#define DLOG_TRACE(F, ...)
		#define DLOG_INFO(F, ...)
		#define DLOG_WARN(F, ...)
		#define DLOG_ERROR(F, ...)
		#define DLOG_FATAL(F, ...)
	#endif
	
	#define LOG_TRACE(...)	LOG_CHANGE_COLOR_CYAN();	LOG("[LOG] "); LOG(__VA_ARGS__);  LOG_COLOR_RESET(); LOG("\n")
	#define LOG_INFO(...)	LOG_CHANGE_COLOR_GREEN();	LOG("[LOG] "); LOG(__VA_ARGS__);  LOG_COLOR_RESET(); LOG("\n")
	#define LOG_WARN(...)	LOG_CHANGE_COLOR_YELLOW();	LOG("[LOG] "); LOG(__VA_ARGS__);  LOG_COLOR_RESET(); LOG("\n")
	#define LOG_ERROR(...)	LOG_CHANGE_COLOR_RED();		LOG("[LOG] "); LOG(__VA_ARGS__);  LOG_COLOR_RESET(); LOG("\n")
	#define LOG_FATAL(...)	LOG_CHANGE_COLOR_MAGENTA(); LOG("[LOG] "); LOG(__VA_ARGS__);  LOG_COLOR_RESET(); LOG("\n")
#else
	#define DLOG_TRACE(F, ...)
	#define DLOG_INFO(F, ...)
	#define DLOG_WARN(F, ...)
	#define DLOG_ERROR(F, ...)
	#define DLOG_FATAL(F, ...)

	#define LOG_TRACE(F, ...)
	#define LOG_INFO(F, ...)
	#define LOG_WARN(F, ...)
	#define LOG_ERROR(F, ...)
	#define LOG_FATAL(F, ...)
#endif

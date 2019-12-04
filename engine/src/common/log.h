#pragma once

#ifdef PREV_GAME_ENGINE

	#ifdef ENGINE_LOGGING
		#include <cstdio>

		#define LOG_CHANGE_COLOR_RED()		printf("\033[1;31m")
		#define LOG_CHANGE_COLOR_GREEN()	printf("\033[1;32m")
		#define LOG_CHANGE_COLOR_YELLOW()	printf("\033[1;33m")
		#define LOG_CHANGE_COLOR_BLUE()		printf("\033[1;34m")
		#define LOG_CHANGE_COLOR_MAGENTA()	printf("\033[1;35m")
		#define LOG_CHANGE_COLOR_CYAN()		printf("\033[1;36m")
		#define LOG_CHANGE_COLOR_WHITE()	printf("\033[1;37m")
		#define LOG_COLOR_RESET				printf("\033[0;0m")

		#define LOG(F, ...) printf(F, __VA_ARGS__)

		#ifdef ENGINE_DEBUG
			#define DLOG_TRACE(F, ...)	LOG_CHANGE_COLOR_CYAN();	LOG("[DLOG] "); LOG(F, __VA_ARGS__); LOG("\n")
			#define DLOG_INFO(F, ...)	LOG_CHANGE_COLOR_GREEN();	LOG("[DLOG] "); LOG(F, __VA_ARGS__); LOG("\n")
			#define DLOG_WARN(F, ...)	LOG_CHANGE_COLOR_YELLOW();	LOG("[DLOG] "); LOG(F, __VA_ARGS__); LOG("\n")
			#define DLOG_ERROR(F, ...)	LOG_CHANGE_COLOR_RED();		LOG("[DLOG] "); LOG(F, __VA_ARGS__); LOG("\n")
			#define DLOG_FATAL(F, ...)	LOG_CHANGE_COLOR_MAGENTA(); LOG("[DLOG] "); LOG(F, __VA_ARGS__); LOG("\n")
		#else
			#define DLOG_TRACE(F, ...)
			#define DLOG_INFO(F, ...)
			#define DLOG_WARN(F, ...)
			#define DLOG_ERROR(F, ...)
			#define DLOG_FATAL(F, ...)
		#endif
		
		#define LOG_TRACE(F, ...)	LOG_CHANGE_COLOR_CYAN();	LOG("[LOG] "); LOG(F, __VA_ARGS__); LOG("\n")
		#define LOG_INFO(F, ...)	LOG_CHANGE_COLOR_GREEN();	LOG("[LOG] "); LOG(F, __VA_ARGS__); LOG("\n")
		#define LOG_WARN(F, ...)	LOG_CHANGE_COLOR_YELLOW();	LOG("[LOG] "); LOG(F, __VA_ARGS__); LOG("\n")
		#define LOG_ERROR(F, ...)	LOG_CHANGE_COLOR_RED();		LOG("[LOG] "); LOG(F, __VA_ARGS__); LOG("\n")
		#define LOG_FATAL(F, ...)	LOG_CHANGE_COLOR_MAGENTA(); LOG("[LOG] "); LOG(F, __VA_ARGS__); LOG("\n")
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

#endif

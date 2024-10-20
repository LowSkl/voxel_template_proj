#pragma once

#include <spdlog/spdlog.h>

#ifdef NDEBUG
	#define LINFO(...)
	#define LWARN(...)
	#define LERROR(...)
	#define LCRITICAL(...)
#else
	#define LINFO(...)       spdlog::info(__VA_ARGS__)
	#define LWARN(...)       spdlog::warn(__VA_ARGS__)
	#define LERROR(...)		 spdlog::error(__VA_ARGS__)
	#define LCRITICAL(...)   spdlog::critical(__VA_ARGS__)
#endif
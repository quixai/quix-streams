#pragma once

#include "spdlog/spdlog.h"

#include <memory>

#define QUIX_DEFAULT_LOGGER_NAME "quix-streams"

namespace Quix
{
	class Log
	{
	public:
		static void Init();
        inline static std::shared_ptr<spdlog::logger> GetLogger()
		{
			if (spdlog::get(QUIX_DEFAULT_LOGGER_NAME) == nullptr)
			{
				Log::Init();
			}

			return spdlog::get(QUIX_DEFAULT_LOGGER_NAME);
		}

	};
}

#ifndef QUIX_CONFIG_RELEASE
#define LOG_TRACE(...)	::Quix::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)	::Quix::Log::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)	::Quix::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)	::Quix::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)	::Quix::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)	::Quix::Log::GetLogger()->critical(__VA_ARGS__)
#else
// Disable logging for release builds
#define LOG_TRACE(...)	(void)0
#define LOG_DEBUG(...)	(void)0
#define LOG_INFO(...)	(void)0
#define LOG_WARN(...)	(void)0
#define LOG_ERROR(...)	(void)0
#define LOG_FATAL(...)	(void)0
#endif
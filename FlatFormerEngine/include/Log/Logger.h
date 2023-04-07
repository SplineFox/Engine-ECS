#pragma once

#include <memory>
#include "spdlog/spdlog.h"

#if !FFE_DISABLE_LOGGING

namespace FlatFormer { namespace Log {

	class Logger
	{
		std::shared_ptr<spdlog::logger> m_Logger;

		Logger(const Logger&) = delete;
		Logger& operator=(Logger&) = delete;

	public:
		explicit Logger(std::shared_ptr<spdlog::logger> logger);

		~Logger();
		
		template<typename... Args>
		inline void LogTrace(Args... args)
		{
			m_Logger->trace(args...);
		}


		template<typename... Args>
		inline void LogDebug(Args... args)
		{
			m_Logger->debug(args...);
		}


		template<typename... Args>
		inline void LogInfo(Args... args)
		{
			m_Logger->info(args...);
		}


		template<typename... Args>
		inline void LogWarning(Args... args)
		{
			m_Logger->warn(args...);
		}


		template<typename... Args>
		inline void LogError(Args... args)
		{
			m_Logger->error(args...);
		}


		template<typename... Args>
		inline void LogCritical(Args... args)
		{
			m_Logger->critical(args...);
		}
	};

}}

#endif

#include "LoggerMacro.h"
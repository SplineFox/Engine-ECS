#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace FlatFormer { namespace Log {

	#pragma region Forward declarataion
	class Logger;
	#pragma endregion

	class LoggerManager
	{
	#pragma region Using
		using LoggerMap = std::unordered_map<std::string, Logger*>;
	#pragma endregion

	private:
		static constexpr const char* LOG_FILE_NAME = "ENGINE_LOG.log";
		static constexpr const char* DEFAULT_LOGGER_NAME = "FFE_DEFUALT_LOGGER";
		static constexpr const char* LOG_PATTERN = "[%T.%ems] %n: %^%v%$";

		std::shared_ptr<spdlog::logger> m_BaseLogger;
		LoggerMap m_Loggers;

		LoggerManager(const LoggerManager&) = delete;
		LoggerManager& operator=(LoggerManager&) = delete;

	public:
		LoggerManager();
		~LoggerManager();

		// Get the logger with the given name.
		// If there is no object with the same name, then it is created and added to "m_Loggers".
		Logger* GetLogger(const char* loggerName = DEFAULT_LOGGER_NAME);
	};

}}


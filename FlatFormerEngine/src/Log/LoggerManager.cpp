#include "Log/LoggerManager.h"

#include "Log/Logger.h"
#if !FFE_DISABLE_LOGGING

namespace FlatFormer { namespace Log {

	LoggerManager::LoggerManager()
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_FILE_NAME, true));
		
		m_BaseLogger = std::make_shared <spdlog::logger>(DEFAULT_LOGGER_NAME, std::begin(sinks), std::end(sinks));
		m_BaseLogger->set_level(spdlog::level::trace);
		m_BaseLogger->set_pattern(LOG_PATTERN);
	}


	LoggerManager::~LoggerManager()
	{
		for (auto& iterator : this->m_Loggers)
		{
			delete iterator.second;
			iterator.second = nullptr;
		}
	}


	Logger* LoggerManager::GetLogger(const char* loggerName)
	{
		auto iterator = this->m_Loggers.find(loggerName);
		if (iterator == this->m_Loggers.end())
		{
			this->m_Loggers.insert(std::make_pair<std::string, Logger*>(loggerName, new Logger(m_BaseLogger->clone(loggerName))));
		}

		return this->m_Loggers[loggerName];
	}

}}

#endif

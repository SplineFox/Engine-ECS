#if !FFE_DISABLE_LOGGING

#include "Log/Logger.h"

namespace FlatFormer { namespace Log {

	Logger::Logger(std::shared_ptr<spdlog::logger> logger)
	{
		m_Logger = logger;
	}


	Logger::~Logger()
	{}

}}

#endif
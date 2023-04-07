#pragma once
#include "Log/LoggerManager.h"

#if !FFE_DISABLE_LOGGING

namespace FlatFormer { namespace Log { namespace Internal { static LoggerManager s_LoggerManager; }}}
using namespace FlatFormer::Log;

#define FFE_DECLARE_LOGGER					Logger* LOGGER;
#define FFE_DECLARE_STATIC_LOGGER	static	Logger* LOGGER;

#define FFE_DEFINE_LOGGER(name)									LOGGER											=  Internal::s_LoggerManager.GetLogger(name);
#define FFE_DEFINE_STATIC_LOGGER(className, name)				Logger* className::LOGGER						=  Internal::s_LoggerManager.GetLogger(name);
#define FFE_DEFINE_STATIC_LOGGER_TEMPLATE(className, T, name)	template<class T> Logger* className<T>::LOGGER	=  Internal::s_LoggerManager.GetLogger(name);

#define FFE_LOG_TRACE(...)		LOGGER->LogTrace	(__VA_ARGS__);
#define FFE_LOG_DEBUG(...)		LOGGER->LogDebug	(__VA_ARGS__);
#define FFE_LOG_INFO(...)		LOGGER->LogInfo		(__VA_ARGS__);
#define FFE_LOG_WARNING(...)	LOGGER->LogWarning	(__VA_ARGS__);
#define FFE_LOG_ERROR(...)		LOGGER->LogError	(__VA_ARGS__);
#define FFE_LOG_CRITICAL(...)	LOGGER->LogCritical	(__VA_ARGS__);

#define FFE_SPDLOG_DROP_ALL spdlog::drop_all(); 

#else

#define FFE_DECLARE_LOGGER
#define FFE_DEFINE_LOGGER(name)

#define FFE_LOG_TRACE(...)
#define FFE_LOG_DEBUG(...)
#define FFE_LOG_INFO(...)
#define FFE_LOG_WARNING(...)
#define FFE_LOG_ERROR(...)
#define FFE_LOG_CRITICAL(...)

#define FFE_DECLARE_STATIC_LOGGER
#define FFE_DEFINE_STATIC_LOGGER(className, name)	
#define FFE_DEFINE_STATIC_LOGGER_TEMPLATE(className, T, name)
#define FFE_SPDLOG_DROP_ALL
#endif

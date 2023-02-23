#pragma once
#include "CleLogger.h"
#define CLE_LOGGING_LIB __declspec(dllexport)



extern "C" CLE_LOGGING_LIB CleLogger* newWrapLogger()
{
	CleLogger* clelogger = new CleLogger();
	return clelogger;
}

extern "C" CLE_LOGGING_LIB  void setOnOffSinks(CleLogger* logger, bool bCs, bool bF, bool bQt, bool bCb, bool bSql)
{
	logger->setOnOffSink(bCs, bF, bQt, bCb, bSql);
}

extern "C" CLE_LOGGING_LIB  void setLevels(CleLogger * logger, spdlog::level::level_enum lCs, spdlog::level::level_enum lF, spdlog::level::level_enum lQt, spdlog::level::level_enum lCb, spdlog::level::level_enum lSql)
{
	logger->setOnOffSink(lCs, lF, lQt, lCb, lSql);
}

extern "C" CLE_LOGGING_LIB  void createLogger(CleLogger * logger, char* strDir, void* Null, void* func, char* strSqlPw)
{
	logger->initCleLogger(strDir, nullptr, func, strSqlPw);
}


extern "C" CLE_LOGGING_LIB  void freeLogger(CleLogger* clelogger)
{
	clelogger->~CleLogger();
}

extern "C" CLE_LOGGING_LIB  void logTrace(CleLogger * logger, char* msg)
{
	logger->trace(msg);
};
extern "C" CLE_LOGGING_LIB  void logDebug(CleLogger * logger, char* msg)
{
	logger->debug(msg);
};
extern "C" CLE_LOGGING_LIB  void logInfo(CleLogger * logger, char* msg)
{
	logger->info(msg);
};
extern "C" CLE_LOGGING_LIB  void logWarn(CleLogger * logger, char* msg)
{
	logger->warn(msg);
};
extern "C" CLE_LOGGING_LIB  void logError(CleLogger * logger, char* msg)
{
	logger->error(msg);
};
extern "C" CLE_LOGGING_LIB  void logCritical(CleLogger * logger, char* msg)
{
	logger->critical(msg);
};

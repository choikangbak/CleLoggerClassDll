#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/details/log_msg.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/sinks/qt_sinks.h"

#include "spdlog/common.h"
#include "spdlog/logger.h"
#include "spdlog/details/fmt_helper.h"
//#include <spdlog/details/backtracer.h>
#include <stdarg.h>
#include <sstream>

#include <vector>
#include <pqxx/pqxx>
#include "LogDAO.h"


#define DllExport __declspec( dllexport )
typedef void(*CALLBACK_FUNC)(std::string);

spdlog::level::level_enum get_log_level(std::string log_level);

namespace spdlog {

    // callbacks type
    typedef std::function<void(const details::log_msg& msg)> custom_log_callback;

    namespace sinks {
        /*
         * Trivial callback sink, gets a callback function and calls it on each log
         */
        template<typename Mutex>
        class callback_sink final : public base_sink<Mutex>
        {
        public:
            explicit callback_sink(const custom_log_callback& callback)
                : callback_{ callback }
            {
                _jCallBack = NULL;
            }
            explicit callback_sink(CALLBACK_FUNC func)
            {
                _jCallBack = func;
            }

        protected:
            void sink_it_(const details::log_msg& msg) override;
            void flush_() override {};

        private:
            custom_log_callback callback_;
            CALLBACK_FUNC _jCallBack;
        };

        using callback_sink_mt = callback_sink<std::mutex>;
        using callback_sink_st = callback_sink<details::null_mutex>;

    } // namespace sinks

    //
    // factory functions
    //
    template<typename Factory = spdlog::synchronous_factory>
    inline std::shared_ptr<logger> callback_logger_mt(const std::string& logger_name, const custom_log_callback& callback)
    {
        return Factory::template create<sinks::callback_sink_mt>(logger_name, callback);
    }

    template<typename Factory = spdlog::synchronous_factory>
    inline std::shared_ptr<logger> callback_logger_st(const std::string& logger_name, const custom_log_callback& callback)
    {
        return Factory::template create<sinks::callback_sink_st>(logger_name, callback);
    }

} // namespace spdlog


namespace spdlog
{
	namespace sinks 
	{
//		template<typename Mutex>
		class postgresql_sink : public sink// public base_sink<Mutex>
		{
		private:
			CRITICAL_SECTION cs;
			LogDAO m_logDao;
		public:
			void set_pattern(const std::string& pattern) override {};
			void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override {};

			explicit postgresql_sink(const std::string& strPasswd);
			~postgresql_sink();

			void log(const spdlog::details::log_msg& msg) override;

			void flush() override {};
		};
	}
}

class DllExport CleLogger
{
private:
	spdlog::logger* m_pLogger;
	std::ostringstream m_oss;
	spdlog::level::level_enum m_lvl[5];	// 
	bool m_bSink[5];	// bConsole, bFile, bSql, bQtUi, bCallback
public:
	CleLogger();

	~CleLogger();

	void initCleLogger(const std::string& strDir, QTextEdit* pQEdit, void* func, const std::string& strSqlPasswd);
	void setOnOffSink(bool bConsole, bool bFile, bool bQtUi, bool bCallback, bool bSql);
	void setLevels(spdlog::level::level_enum lCs, spdlog::level::level_enum lF, spdlog::level::level_enum lQt, spdlog::level::level_enum lCb, spdlog::level::level_enum lSql);

	template<typename... Args>	void trace(const char* fmt, const Args &...args) { m_pLogger->trace(fmt, args); }
	template<typename... Args>	void debug(const char* fmt, const Args &...args) { m_pLogger->debug(fmt, args); }
	template<typename... Args>	void info(const char* fmt, const Args &...args) { m_pLogger->info(fmt, args); }
	template<typename... Args>	void warn(const char* fmt, const Args &...args) { m_pLogger->warn(fmt, args); }
	template<typename... Args>	void error(const char* fmt, const Args &...args) { m_pLogger->error(fmt, args); }
	template<typename... Args>	void critical(const char* fmt, const Args &...args) { m_pLogger->critical(fmt, args); }

	template<typename T > void 	trace(const T& msg) { m_pLogger->trace(msg); }
	template<typename T > void 	debug(const T& msg) { m_pLogger->debug(msg); }
	template<typename T > void 	info(const T& msg) { m_pLogger->info(msg); }
	template<typename T > void 	warn(const T& msg) { m_pLogger->warn(msg); }
	template<typename T > void 	error(const T& msg) { m_pLogger->error(msg); }
	template<typename T > void 	critical(const T& msg) { m_pLogger->critical(msg); }
};

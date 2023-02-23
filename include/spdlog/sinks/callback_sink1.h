// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/synchronous_factory.h>

#include <mutex>
#include <string>

typedef void(*CALLBACK_FUNC)(std::string);


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
            void sink_it_(const details::log_msg& msg) override
            {
                auto time = std::chrono::system_clock::to_time_t(msg.time);	// timestamp
                char buf[26];
                ctime_s(buf, sizeof(buf), &time);
                std::string str(buf);
                str = str.substr(0, str.length() - 1);

                str += "-";
                str += level::to_short_c_str(msg.level);
                str += "-";

                for (int i = 0; i < msg.payload.size(); i++)	// message
                    str += msg.payload.data()[i];

                _jCallBack(str);
            }
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

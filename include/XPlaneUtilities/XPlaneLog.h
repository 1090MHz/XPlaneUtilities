#ifndef XPLANELOG_H
#define XPLANELOG_H

// Standard Library Headers
#include <string> // For std::string
#include <memory> // For std::shared_ptr
#include <mutex>  // For std::mutex

// Third-Party Library Headers
#include <spdlog/spdlog.h>          // For spdlog logging functions
#include <spdlog/sinks/base_sink.h> // For spdlog::sinks::base_sink
#include <spdlog/details/log_msg.h> // For spdlog::details::log_msg

class XPlaneLog
{
public:
    // Initialize the logger
    static void init(const std::string &plugin_name);

    // Shutdown the logger and clean up resources
    static void shutdown();

    // Log a trace message
    static void trace(const std::string &message);

    // Log a debug message
    static void debug(const std::string &message);

    // Log an info message
    static void info(const std::string &message);

    // Log a warning message
    static void warn(const std::string &message);

    // Log an error message
    static void error(const std::string &message);

    // Log a critical message
    static void critical(const std::string &message);

private:
    // Custom sink for X-Plane
    class Sink : public spdlog::sinks::base_sink<std::mutex>
    {
    protected:
        void sink_it_(const spdlog::details::log_msg &msg) override;
        void flush_() override;
    };

    // Custom formatter for X-Plane
    class Formatter : public spdlog::formatter
    {
    public:
        void format(const spdlog::details::log_msg &msg, spdlog::memory_buf_t &dest) override;
        std::unique_ptr<spdlog::formatter> clone() const override
        {
            return std::make_unique<Formatter>(*this);
        }

        virtual ~Formatter() {} // Required for abstract class
    };

    static std::shared_ptr<spdlog::logger> logger;
};

// Compile-time logging macros that eliminate strings from production binaries
// These macros completely remove debug/trace logging code in NDEBUG builds
// preventing sensitive information from appearing in the binary

#ifdef NDEBUG
    // Production build: strip out TRACE and DEBUG completely
    #define XPLANE_LOG_TRACE(fmt, ...) ((void)0)
    #define XPLANE_LOG_DEBUG(fmt, ...) ((void)0)
    #define XPLANE_LOG_INFO(fmt, ...)  spdlog::info(fmt, ##__VA_ARGS__)
    #define XPLANE_LOG_WARN(fmt, ...)  spdlog::warn(fmt, ##__VA_ARGS__)
    #define XPLANE_LOG_ERROR(fmt, ...) spdlog::error(fmt, ##__VA_ARGS__)
#else
    // Debug build: include all logging levels
    #define XPLANE_LOG_TRACE(fmt, ...) spdlog::trace(fmt, ##__VA_ARGS__)
    #define XPLANE_LOG_DEBUG(fmt, ...) spdlog::debug(fmt, ##__VA_ARGS__)
    #define XPLANE_LOG_INFO(fmt, ...)  spdlog::info(fmt, ##__VA_ARGS__)
    #define XPLANE_LOG_WARN(fmt, ...)  spdlog::warn(fmt, ##__VA_ARGS__)
    #define XPLANE_LOG_ERROR(fmt, ...) spdlog::error(fmt, ##__VA_ARGS__)
#endif

#endif // XPLANELOG_H
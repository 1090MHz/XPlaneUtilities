#pragma once

/**
 * @file XPlaneLog.h
 * @brief Advanced logging system for X-Plane plugins with spdlog integration
 */

// Standard Library Headers
#include <string> // For std::string
#include <memory> // For std::shared_ptr
#include <mutex>  // For std::mutex

// Third-Party Library Headers
#include <spdlog/spdlog.h>          // For spdlog logging functions
#include <spdlog/sinks/base_sink.h> // For spdlog::sinks::base_sink
#include <spdlog/details/log_msg.h> // For spdlog::details::log_msg

/**
 * @brief Advanced logging system for X-Plane plugins
 * 
 * XPlaneLog provides a modern C++ logging interface built on top of spdlog.
 * It automatically creates multiple sinks:
 * - Console output (with colors)
 * - File output (plugin-specific log file)
 * - X-Plane debug output (via XPLMDebugString)
 * 
 * The logger is registered globally as "xplane" so other libraries can use it.
 * 
 * @example
 * ```cpp
 * // Initialize once in your plugin
 * XPlaneLog::init("MyPlugin");
 * 
 * // Use throughout your code
 * XPlaneLog::info("Plugin initialized");
 * XPlaneLog::error("Error occurred: {}", error_message);
 * 
 * // Other libraries can access the same logger
 * if (auto logger = spdlog::get("xplane")) {
 *     logger->info("Message from library");
 * }
 * ```
 */
class XPlaneLog
{
public:
    /**
     * @brief Initialize the logging system
     * @param plugin_name Name of the plugin (used for log file naming)
     * 
     * Creates a multi-sink logger with console, file, and X-Plane debug output.
     * Registers the logger globally as "xplane" for use by other libraries.
     * Safe to call multiple times - subsequent calls are ignored.
     */
    static void init(const std::string &plugin_name);

    /**
     * @brief Log a trace message
     * @param message Message to log
     */
    static void trace(const std::string &message);

    /**
     * @brief Log a debug message
     * @param message Message to log
     */
    static void debug(const std::string &message);

    /**
     * @brief Log an info message
     * @param message Message to log
     */
    static void info(const std::string &message);

    /**
     * @brief Log a warning message
     * @param message Message to log
     */
    static void warn(const std::string &message);

    /**
     * @brief Log an error message
     * @param message Message to log
     */
    static void error(const std::string &message);

    /**
     * @brief Log a critical message
     * @param message Message to log
     */
    static void critical(const std::string &message);

    /**
     * @brief Get the underlying spdlog logger instance
     * @return Shared pointer to the logger, or nullptr if not initialized
     */
    static std::shared_ptr<spdlog::logger> getLogger() { return logger; }

private:
    /**
     * @brief Custom sink for X-Plane debug output
     * 
     * Formats log messages and sends them to X-Plane's Log.txt via XPLMDebugString.
     */
    class Sink : public spdlog::sinks::base_sink<std::mutex>
    {
    protected:
        void sink_it_(const spdlog::details::log_msg &msg) override;
        void flush_() override;
    };

    /**
     * @brief Custom formatter for consistent log formatting
     * 
     * Provides consistent timestamp, logger name, log level, and message formatting
     * across all sinks. Ensures proper newline handling for X-Plane compatibility.
     */
    class Formatter : public spdlog::formatter
    {
    public:
        void format(const spdlog::details::log_msg &msg, spdlog::memory_buf_t &dest) override;
        std::unique_ptr<spdlog::formatter> clone() const override
        {
            return std::make_unique<Formatter>(*this);
        }

        virtual ~Formatter() = default;
    };

    /// Shared logger instance
    static std::shared_ptr<spdlog::logger> logger;
};

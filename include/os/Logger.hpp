/**
 * @file Logger.hpp
 * @brief Logging interface whose public output is currently disabled.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <queue>

/**
 * @class Logger
 * @brief Provides the firmware logging API.
 *
 * The current `log()` implementation drops messages without queuing or
 * emitting them.
 */
class Logger
{
    /** Queue reserved for messages received while the system is offline. */
    static std::queue <String> offlineLogQueue;
    /** Monotonic counter used to track emitted log entries. */
    static long logCounter;
    /** Decodes and dispatches a single message to the active output backend. */
    static void processMessage(String& message);
public:
    /**
        * @brief Accepts a diagnostic message without emitting it.
        * @param message Text payload currently discarded by the disabled logger.
     */
    static void log(String message);

};

#endif LOGGER_H
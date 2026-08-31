/**
 * @file Logger.hpp
 * @brief Central log sink for firmware diagnosis and operational trace output.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <queue>

/**
 * @class Logger
 * @brief Simplifies asynchronous, buffered logging for runtime diagnostics.
 *
 * The logger stores incoming messages in a queue and flushes them to the
 * configured serial interface when the runtime can process them.
 */
class Logger
{
    /** Queue holding messages received while the system is temporarily offline. */
    static std::queue <String> offlineLogQueue;
    /** Monotonic counter used to track emitted log entries. */
    static long logCounter;
    /** Decodes and dispatches a single message to the active output backend. */
    static void processMessage(String& message);
public:
    /**
     * @brief Logs a message to the active diagnostic channel.
     * @param message Text payload to be queued and emitted.
     */
    static void log(String message);

};

#endif LOGGER_H
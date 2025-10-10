#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <queue>



class Logger
{
    static std::queue <String> offlineLogQueue;
    static long logCounter;
    static void processMessage(String& message);
public:
    static void log(String message);

};

#endif LOGGER_H
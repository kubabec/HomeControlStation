#ifndef ERROR_MONITOR_H
#define ERROR_MONITOR_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <array>


class ErrorMonitor 
{
    std::array<uint8_t, ERR_MONT_ERROR_COUNT> errorList;


public:
    static void init();
    static void deinit();
    static void cyclic();        
};


#endif
#ifndef ERROR_MONITOR_H
#define ERROR_MONITOR_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <array>

class ErrorMonitor 
{
    static std::array<SystemErrorType, ERR_MONT_ERROR_COUNT> errorList;

    
    static void errorReport(ERR_MON_ERROR_TYPE errorCode, uint16_t extendedData);
    static void errorClear(ERR_MON_ERROR_TYPE errorCode);

    static void updateSystemErrorSignal();

public:
    static void init();
    static void deinit();
    static void cyclic();        
};


#endif
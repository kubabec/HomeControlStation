#ifndef SAFE_RESET_MANAGER_H
#define SAFE_RESET_MANAGER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>

class SafeResetManager
{
    static const long long RESET_DELAY; // 30h in ms
    static const uint8_t morningStartHour;
    static const uint8_t morningEndHour;

    static bool isMorning();
    static bool localDevicesConditionFilled();

public:
    static void init();
    static void cyclic();
    static void deinit();
};

#endif
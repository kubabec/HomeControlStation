#ifndef SOFTWARE_UPDATER_HPP
#define SOFTWARE_UPDATER_HPP

#include <Arduino.h>

class SoftwareUpdater
{
public:
    static void init();
    static void cyclic();
    static bool isBusy();
    static bool start(const String &version);
    static bool requestRestart();
    static String getVersionsJson();
    static String getStatusJson();
    static const char *target();
};

#endif

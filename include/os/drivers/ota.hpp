#ifndef OTA_H
#define OTA_H


#include <ArduinoOTA.h>
/**
 * @class OTA
 * @brief Manages the over-the-air firmware update flow used by the station.
 */

class OTA
{
public:
    static void init(const String hostname, const String password);
    static void cyclic();
};

#endif 
#ifndef OTA_H
#define OTA_H


#include <ArduinoOTA.h>

class OTA
{
public:
    static void init(const String hostname, const String password);
    static void cyclic();
};

#endif 
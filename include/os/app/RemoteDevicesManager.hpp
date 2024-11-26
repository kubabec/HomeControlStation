#ifndef REMOTE_DEVICES_MANAGER_H
#define REMOTE_DEVICES_MANAGER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <map>


struct RCTranslation {
    /* Remote Node Identifier */
    uint16_t nodeId = 255;
    /* Local device identifier on source node */
    uint8_t onSourceNodeLocalId = 255;

    void print() {
        Serial.println(String(nodeId) + ", " + String((int) onSourceNodeLocalId));
    }
};

class RemoteDevicesManager 
{   
    /* This vector is tunneled from RCServer when handshake is over via SIG_RC_DEVICES_INTERNAL_TUNEL signal */
    static std::vector<OnOffDeviceDescription> remoteDevicesCollection;
    static std::map<uint8_t, RCTranslation> currentIdMapping;
public:
    static void init();
    static void deinit();
    static void cyclic();
    static void printTranslationMap();

    static bool deviceEnable(uint8_t deviceId, bool state);
    static bool deviceBrightnessChange(uint8_t deviceId, uint8_t brightnessLevel);

    static void tunnelDataUpdate(std::any remoteDevices);
    static RCTranslation getTranslationFromUnique(uint8_t uniqueId);

    static bool receiveResponse(RcResponse& response);

};

#endif
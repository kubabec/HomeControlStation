#ifndef REMOTE_DEVICES_MANAGER_H
#define REMOTE_DEVICES_MANAGER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <map>

#define MAX_EXTERNAL_NODES 24
struct RCTranslation {
    /* Remote Node MAC */
    uint64_t mac = 0LL;
    /* Local device identifier on source node */
    uint8_t onSourceNodeLocalId = 255;

    void print() {
        Serial.println("MAC : " + String((int) + mac));
        Serial.println(", " + String((int) onSourceNodeLocalId));
    }

    bool isValid(){
        return (mac != 0LL && (onSourceNodeLocalId != 255));
    }
};

struct ExternalNodeMapping{
    uint8_t isUsed = 0;
    uint64_t mac;
};

class RemoteDevicesManager 
{   
    /* This vector is tunneled from RCServer when handshake is over via SIG_RC_DEVICES_INTERNAL_TUNEL signal */
    static std::vector<DeviceDescription> remoteDevicesCollection;
    static std::map<uint8_t, RCTranslation> currentIdMapping;
    static std::array<ExternalNodeMapping, MAX_EXTERNAL_NODES> mappingSlotsForExternalNodes;
    static uint8_t getMappingOffsetForNode(uint64_t& nodeMAC);
public:
    static void init();
    static void deinit();
    static void cyclic();
    static void printTranslationMap();

    
    static void tunnelDataUpdate(std::any remoteDevices);
    static RCTranslation getTranslationFromUnique(uint8_t uniqueId);

    static bool receiveResponse(RcResponse& response);


    /* TESTCODE */
    static ServiceRequestErrorCode service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
    );
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set1 param
    );
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set2 param
    );
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set3 param
    );
    /* TESTCODE */

};

#endif
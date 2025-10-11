#ifndef REMOTE_DEVICES_MANAGER_H
#define REMOTE_DEVICES_MANAGER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include "os/Logger.hpp"
#include <map>

#define MAX_EXTERNAL_NODES 24
struct RCTranslation {
    /* Remote Node MAC */
    uint64_t mac = 0LL;
    /* Local device identifier on source node */
    uint8_t onSourceNodeLocalId = 255;

    void print() {
        Logger::log("MAC : " + String((int) + mac));
        Logger::log(", " + String((int) onSourceNodeLocalId));
    }

    bool isValid(){
        return (mac != 0LL && (onSourceNodeLocalId != 255));
    }
};

struct __attribute__((packed)) ExternalNodeMapping{
    uint64_t mac;
    uint8_t isUsed = 0;
};

class RemoteDevicesManager 
{   
    enum RDM_RequestProcessingState{
        RDM_NO_REQUEST,
        RDM_REQUEST_IN_PROGRESS,
        RDM_REQUEST_FAILED,
        RDM_REQUEST_COMPLETED
    };

    struct ServiceCallFingerprint {
        uint8_t deviceId = 255;
        DeviceServicesType serviceName = DEVSERVICE_INVALID;
        ServiceOverloadingFunction overloading = serviceCall_INVALID;
        uint8_t memoryRequestDirection = e_UNKNOWN_PARAM_DIRECTION;


    bool operator==(ServiceCallFingerprint& other){
            return (this->deviceId == other.deviceId && this->serviceName == other.serviceName && this->overloading == other.overloading);
        }
    };

    static void handleService3Response(RcResponse& response, DeviceDescription& responseDeviceDescription);

    /* This vector is tunneled from RCServer when handshake is over via SIG_RC_DEVICES_INTERNAL_TUNEL signal */
    static std::vector<DeviceDescription> remoteDevicesCollection;
    static std::map<uint8_t, RCTranslation> currentIdMapping;
    static std::array<ExternalNodeMapping, MAX_EXTERNAL_NODES> mappingSlotsForExternalNodes;
    static uint8_t getMappingOffsetForNode(uint64_t& nodeMAC);

    static RDM_RequestProcessingState requestProcessingState;
    static ServiceCallFingerprint currentRequestFingerprint;
    static ServiceRequestErrorCode currentRequestRespErrorCode;
    static uint8_t awaitingResponseId;
public:
    static void init();
    static void deinit();
    static void cyclic();
    static void printTranslationMap();


    static void tunnelDataUpdate(std::any remoteDevices);
    static RCTranslation getTranslationFromUnique(uint8_t uniqueId);

    static bool receiveResponse(RcResponse& response);


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

};

#endif
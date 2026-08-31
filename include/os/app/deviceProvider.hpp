#ifndef DEVICE_PROVIDER_H
#define DEVICE_PROVIDER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <map>
#include <any>
#include <functional>


/* typ opisujacy czy dane urzadzenie jest local czy remote i jakie ma oryginalne ID */
/**
 * @struct DeviceTranslationDetails
 * @brief Maps a unique runtime device identifier to the original device ID and its origin.
 *
 * This structure is used when the system exposes a combined local/remote device collection but has
 * to keep track whether a logical device belongs to the current node or to a remote node.
 */
struct DeviceTranslationDetails{
    /**
     * Original identifier used by the device on its native node.
     */
    uint8_t originalID = 255;

    /**
     * true when the device is managed locally; false when it belongs to a remote node.
     */
    bool isLocal = true;
};
/**
 * @class DeviceProvider
 * @brief Provides the OS-level device API and translates local/remote device identifiers.
 *
 * The provider exposes the device service entry points, keeps the combined device map synchronized,
 * and forwards remote-control requests to the correct device logic.
 */

class DeviceProvider 
{
    /**
     * Callback used to send a remote-control response back through the active transport layer.
     */
    static std::function<bool(RcResponse&)> sendResponse;

    /**
     * Callback used to handle long responses requiring extended payload handling.
     */
    static std::function<bool(RcResponseLong&)> requestResponseLong;

    /**
     * Queue of room-state device identifiers waiting for a state switch request to be processed.
     */
    static std::queue<uint8_t> roomStateChangeDeviceIdQueue;

    /**
     * Requested target state for the next queued room-state transition.
     */
    static bool requestedRoomState;

    /**
     * true when this node acts as the remote-control server; false when it acts as a terminal node.
     */
    static bool isRCServer;

    /**
     * Translation table that maps a runtime unique ID to the original device identifier and origin.
     */
    static std::map<uint8_t, DeviceTranslationDetails> uniqueDeviceIdToNormalDeviceIdMap;
    
    /**
     * Rebuilds the current device collection and updates the related data-container signals.
     */
    static void updateDeviceDescriptionSignal();

    /**
     * Resolves the original device metadata for a provided unique identifier.
     * @param uniqueId Unique runtime identifier used in the combined device collection.
     * @return Translation details for the corresponding original device; default values when not found.
     */
    static DeviceTranslationDetails getOriginalIdFromUnique(uint8_t uniqueId);
    //static uint8_t findUniqueIdByOriginalId(uint8_t originalId);

    /**
     * Subscribes the provider to local-device collection updates.
     */
    static void initLocalDevicesSetup();

    /**
     * Subscribes the provider to remote-device collection updates when acting as a control server.
     */
    static void initRemoteDevicesSetup();

    /**
     * Appends the selected device description into a service response payload.
     * @param response Response object being filled.
     * @param deviceId Device whose description should be embedded in the payload.
     */
    static void addDeviceDescriptionToResponsePayload(RcResponse& response, uint8_t deviceId);

    /*Complex handlers for service RcRequests*/
    /**
     * Handles a service request that carries a 3-parameter payload and fills the associated response.
     * @param request Incoming request metadata.
     * @param response Response object to be populated.
     * @param param Parameter set used by the requested service.
     * @param payloadSize Size of the received payload in bytes.
     * @return Status describing whether the service call succeeded or why it failed.
     */
    static ServiceRequestErrorCode handelService3Request(RcRequest& request, RcResponse& response, ServiceParameters_set3& param, uint16_t payloadSize);

public:
    /**
     * Initializes the provider, registers the service API, and subscribes to device collection updates.
     */
    static void init();

    /**
     * Releases runtime state created by the device provider.
     */
    static void deinit();

    /**
     * Runs the periodic queue processing for delayed room-state changes.
     */
    static void cyclic();

    /**
     * Triggers a full device reset via the registered reset callback.
     */
    static void deviceReset();

    /**
     * Reacts to changes in the local or remote device collection and re-synchronizes device metadata.
     * @param signal Notification carrying the collection update.
     */
    static void updateDeviceDescriptionSignal_onChange(std::any signal);

    /**
     * Logs the current ID translation map for debugging and troubleshooting.
     */
    static void printIdMap();

    /**
     * Processes an incoming remote-control request and dispatches it to the proper local device service.
     * @param request Incoming request describing the target device and operation.
     * @return true when the request was accepted or routed; false on invalid input.
     */
    static bool receiveRequest(RcRequest& request);
    

    /* TESTCODE */
    /**
     * Executes a service call without structured parameters.
     * @param deviceId Local or mapped device identifier.
     * @param serviceType Requested service type.
     * @return Result code describing the outcome of the operation.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
    );

    /**
     * Executes a service call with a single parameter payload.
     * @param deviceId Local or mapped device identifier.
     * @param serviceType Requested service type.
     * @param param Service parameters packed in the set-1 structure.
     * @return Result code describing the outcome of the operation.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set1 param
    );

    /**
     * Executes a service call with a dual-parameter payload.
     * @param deviceId Local or mapped device identifier.
     * @param serviceType Requested service type.
     * @param param Service parameters packed in the set-2 structure.
     * @return Result code describing the outcome of the operation.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set2 param
    );

    /**
     * Executes a service call with a full triple-parameter payload.
     * @param deviceId Local or mapped device identifier.
     * @param serviceType Requested service type.
     * @param param Service parameters packed in the set-3 structure.
     * @return Result code describing the outcome of the operation.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set3 param
    );
    /* TESTCODE */
        
};


#endif
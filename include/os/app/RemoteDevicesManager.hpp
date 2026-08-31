#ifndef REMOTE_DEVICES_MANAGER_H
#define REMOTE_DEVICES_MANAGER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include "os/Logger.hpp"
#include <map>

#define MAX_EXTERNAL_NODES 24
/**
 * @struct RCTranslation
 * @brief Maps an internal device identifier on the remote node to the target node MAC address.
 */
struct RCTranslation {
    /**
     * MAC address of the remote node that owns the device.
     */
    uint64_t mac = 0LL;

    /**
     * Local device identifier as seen on the remote node.
     */
    uint8_t onSourceNodeLocalId = 255;

    /**
     * Logs the translation in a human-readable format for debugging.
     */
    void print() {
        Logger::log("MAC : " + String((int) + mac));
        Logger::log(", " + String((int) onSourceNodeLocalId));
    }

    /**
     * Checks whether the translation describes a valid remote-device mapping.
     * @return true when both the MAC and the local ID are valid; false otherwise.
     */
    bool isValid(){
        return (mac != 0LL && (onSourceNodeLocalId != 255));
    }
};

/**
 * @struct ExternalNodeMapping
 * @brief Packed storage entry used by the remote-device manager to track node slots.
 */
struct __attribute__((packed)) ExternalNodeMapping{
    /**
     * MAC address of the external node occupying the slot.
     */
    uint64_t mac;

    /**
     * true when the slot is currently in use.
     */
    uint8_t isUsed = 0;
};
/**
 * @class RemoteDevicesManager
 * @brief Maintains the mapping between local device IDs and devices available on remote nodes.
 *
 * The manager tracks remote-node devices, translates identifiers during service calls, and processes
 * the results of requests sent across the network so the local OS can act on remote hardware as if it
 * were local.
 */

class RemoteDevicesManager 
{   
    /**
     * State machine describing the current remote service request lifecycle.
     */
    enum RDM_RequestProcessingState{
        RDM_NO_REQUEST,
        RDM_REQUEST_IN_PROGRESS,
        RDM_REQUEST_FAILED,
        RDM_REQUEST_COMPLETED
    };
/**
 * @struct ServiceCallFingerprint
 * @brief Identifies a unique remote service call so a response can be matched to the correct request.
 */

    struct ServiceCallFingerprint {
        /**
         * Device identifier targeted by the call.
         */
        uint8_t deviceId = 255;

        /**
         * Service type requested from the remote node.
         */
        DeviceServicesType serviceName = DEVSERVICE_INVALID;

        /**
         * Overload variant used by the call.
         */
        ServiceOverloadingFunction overloading = serviceCall_INVALID;

        /**
         * Parameter direction metadata for extended memory requests.
         */
        uint8_t memoryRequestDirection = e_UNKNOWN_PARAM_DIRECTION;

        /**
         * Compares two request fingerprints for equality.
         * @param other Candidate fingerprint.
         * @return true when both requests describe the same service call.
         */
        bool operator==(ServiceCallFingerprint& other){
            return (this->deviceId == other.deviceId && this->serviceName == other.serviceName && this->overloading == other.overloading);
        }
    };

    /**
     * Processes a remote service response and converts it into the appropriate device-description format.
     * @param response Response packet received from the remote node.
     * @param responseDeviceDescription Device description associated with the response payload.
     */
    static void handleService3Response(RcResponse& response, DeviceDescription& responseDeviceDescription);

    /**
     * Vector of remote device descriptions tunneled in from the RC server after handshake completion.
     */
    static std::vector<DeviceDescription> remoteDevicesCollection;

    /**
     * Map of unique local identifiers to their remote-node translation metadata.
     */
    static std::map<uint8_t, RCTranslation> currentIdMapping;

    /**
     * Slots reserved for tracking remote-node mappings and checking their occupancy.
     */
    static std::array<ExternalNodeMapping, MAX_EXTERNAL_NODES> mappingSlotsForExternalNodes;

    /**
     * Finds the mapping slot reserved for the supplied node MAC address.
     * @param nodeMAC MAC address of the remote node to look up.
     * @return Slot offset assigned to that remote node.
     */
    static uint8_t getMappingOffsetForNode(uint64_t& nodeMAC);

    /**
     * Current lifecycle state of the pending remote request.
     */
    static RDM_RequestProcessingState requestProcessingState;

    /**
     * Fingerprint of the request currently awaiting a remote response.
     */
    static ServiceCallFingerprint currentRequestFingerprint;

    /**
     * Result code returned by the most recent remote service call.
     */
    static ServiceRequestErrorCode currentRequestRespErrorCode;

    /**
     * Request ID of the currently awaited response.
     */
    static uint8_t awaitingResponseId;
public:
    /**
     * Initializes the manager and prepares the current mapping state.
     */
    static void init();

    /**
     * Cleans up the manager state when the service is shutting down.
     */
    static void deinit();

    /**
     * Performs periodic maintenance and state checks for remote requests.
     */
    static void cyclic();

    /**
     * Prints the current translation map in a readable form for diagnostics.
     */
    static void printTranslationMap();

    /**
     * Updates the remote-device map with the latest collection received through the RC server tunnel.
     * @param remoteDevices New collection of remote device descriptions.
     */
    static void tunnelDataUpdate(std::any remoteDevices);

    /**
     * Resolves the remote-node translation for a given unique internal device ID.
     * @param uniqueId Unique identifier used in the local translation map.
     * @return Translation metadata for the remote device.
     */
    static RCTranslation getTranslationFromUnique(uint8_t uniqueId);

    /**
     * Handles a remote response packet and updates the pending request state.
     * @param response Response packet received from the remote node.
     * @return true when the response was processed successfully; false when it was invalid.
     */
    static bool receiveResponse(RcResponse& response);

    /**
     * Sends a remote service call with no additional parameters.
     * @param deviceId Remote device identifier.
     * @param serviceType Requested service.
     * @return Result code received from the remote node or the local validation layer.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
    );

    /**
     * Sends a remote service call with set-1 payload parameters.
     * @param deviceId Remote device identifier.
     * @param serviceType Requested service.
     * @param param Packed parameter payload for the service.
     * @return Result code received from the remote node or the local validation layer.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set1 param
    );

    /**
     * Sends a remote service call with set-2 payload parameters.
     * @param deviceId Remote device identifier.
     * @param serviceType Requested service.
     * @param param Packed parameter payload for the service.
     * @return Result code received from the remote node or the local validation layer.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set2 param
    );

    /**
     * Sends a remote service call with set-3 payload parameters.
     * @param deviceId Remote device identifier.
     * @param serviceType Requested service.
     * @param param Packed parameter payload for the service.
     * @return Result code received from the remote node or the local validation layer.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set3 param
    );

};

#endif
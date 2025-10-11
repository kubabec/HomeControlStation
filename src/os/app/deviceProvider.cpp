#include <os/app/DeviceProvider.hpp>
#include <os/Logger.hpp>
#include "esp_heap_caps.h"

#define MINIMAL_PAYLOAD_SIZE 2
// #include "deviceProvider.hpp"
//  mapa do przechowywania unikalnych ID i powiazania lokalnych ID + info ot tym czy jest to urzadzenie lokalne czy zadalne (tzn na slave ESP)
std::map<uint8_t, DeviceTranslationDetails> DeviceProvider::uniqueDeviceIdToNormalDeviceIdMap;
std::queue<uint8_t> DeviceProvider::roomStateChangeDeviceIdQueue;
bool DeviceProvider::requestedRoomState = false;

std::function<bool(RcResponse &)> DeviceProvider::sendResponse;
std::function<bool(RcResponseLong &)> DeviceProvider::requestResponseLong;

bool DeviceProvider::isRCServer = false;

void DeviceProvider::deinit()
{
}

void DeviceProvider::init()
{
    Logger::log("DeviceProvider init ...");

    /*TESTCODE*/
    /* Link service API functions to DeviceProvider function calls */
    DeviceServicesAPI servicesFunctionSet = {
        .serviceCall_NoParams =
            [](uint8_t deviceId, DeviceServicesType request)
        {
            return DeviceProvider::service(deviceId, request);
        },
        .serviceCall_set1 =
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set1 params)
        {
            return DeviceProvider::service(deviceId, request, params);
        },
        .serviceCall_set2 =
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set2 params)
        {
            return DeviceProvider::service(deviceId, request, params);
        },
        .serviceCall_set3 =
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set3 params)
        {
            return DeviceProvider::service(deviceId, request, params);
        }};

    /* Push prepared service API to DataContainer */
    DataContainer::setSignalValue(
        SIG_DEVICE_SERVICES,
        static_cast<DeviceServicesAPI>(servicesFunctionSet));
    /*TESTCODE*/

    std::any rcServerCoding = DataContainer::getSignalValue(SIG_IS_RC_SERVER);
    isRCServer = std::any_cast<bool>(rcServerCoding);

    initLocalDevicesSetup();

    if (isRCServer)
    {
        initRemoteDevicesSetup();
    }
    else
    {
        std::any_cast<std::function<bool(RequestType, std::function<bool(RcRequest &)>)>>(DataContainer::getSignalValue(CBK_REGISTER_REQUEST_RECEIVER))(SERVICE_CALL_REQ, DeviceProvider::receiveRequest);

        std::any responseCBK = DataContainer::getSignalValue(CBK_RESPONSE);
        sendResponse = (std::any_cast<std::function<bool(RcResponse &)>>(responseCBK));
    }

    updateDeviceDescriptionSignal();

    Logger::log("... done");
}

void DeviceProvider::initLocalDevicesSetup()
{
    DataContainer::subscribe(SIG_LOCAL_COLLECTION, DeviceProvider::updateDeviceDescriptionSignal_onChange);
}

void DeviceProvider::initRemoteDevicesSetup()
{
    DataContainer::subscribe(SIG_REMOTE_COLLECTION, DeviceProvider::updateDeviceDescriptionSignal_onChange);
}

void DeviceProvider::cyclic()
{
    // Handle room switch state change if requested
    if (!roomStateChangeDeviceIdQueue.empty())
    {
        ServiceParameters_set1 serviceParameters;
        serviceParameters.a = (uint8_t)requestedRoomState;
        uint8_t deviceId = roomStateChangeDeviceIdQueue.front();
        // Logger::log("DeviceProvider: Processing room state change for device ID: " + String((int)deviceId) + " to state: " + String((int)requestedRoomState));

        ServiceRequestErrorCode serviceErrorCode = service(
            deviceId,
            DEVSERVICE_STATE_SWITCH,
            serviceParameters);
        if (serviceErrorCode != SERV_PENDING && serviceErrorCode != SERV_BUSY)
        {
            roomStateChangeDeviceIdQueue.pop();
        }
    }
}

DeviceTranslationDetails DeviceProvider::getOriginalIdFromUnique(uint8_t uniqueId)
{
    DeviceTranslationDetails retVal; // oryginalId ma default-owo 255

    if (uniqueDeviceIdToNormalDeviceIdMap.find(uniqueId) == uniqueDeviceIdToNormalDeviceIdMap.end())
    {
        // not found
        Logger::log("Device Provider-Translation for invalid ID received: " + String(uniqueId));
        Logger::log("---------------------------------------");
    }
    else
    {
        // found
        retVal = uniqueDeviceIdToNormalDeviceIdMap.find(uniqueId)->second;
    }

    return retVal;
}

void DeviceProvider::updateDeviceDescriptionSignal_onChange(std::any signal)
{
    updateDeviceDescriptionSignal();
}

void DeviceProvider::updateDeviceDescriptionSignal()
{

    uniqueDeviceIdToNormalDeviceIdMap.clear();

    std::vector<DeviceDescription> deviceDescriptionsTotal;

    try
    {
        // Pobieranie i przetwarzanie lokalnych urządzeń

        std::any newLocalDeviceDescriptionVector = DataContainer::getSignalValue(SIG_LOCAL_COLLECTION);
        std::vector<DeviceDescription> localDeviceDescriptionVector = (std::any_cast<std::vector<DeviceDescription>>(newLocalDeviceDescriptionVector));

        /* No need to change unique id for local devices as it is remembered based on pins configuration */
        for (auto device : localDeviceDescriptionVector)
        {
            DeviceTranslationDetails translationDetails = {
                .originalID = device.deviceId,
                .isLocal = true};
            // Logger::log("Local device id: " + String((int)device.deviceId));

            deviceDescriptionsTotal.push_back(device);
            uniqueDeviceIdToNormalDeviceIdMap.insert(std::pair<uint8_t, DeviceTranslationDetails>{device.deviceId, translationDetails});
        }
    }
    catch (const std::bad_any_cast &e)
    {
    }

    if (isRCServer)
    {
        try
        {
            // Pobieranie i przetwarzanie zdalnych urządzeń

            std::any newRemoteDescriptionVector = DataContainer::getSignalValue(SIG_REMOTE_COLLECTION);
            std::vector<DeviceDescription> remoteDescriptionVector = std::any_cast<std::vector<DeviceDescription>>(newRemoteDescriptionVector);

            for (auto device : remoteDescriptionVector)
            {

                DeviceTranslationDetails translationDetails = {
                    .originalID = device.deviceId,
                    .isLocal = false};

                deviceDescriptionsTotal.push_back(device);
                uniqueDeviceIdToNormalDeviceIdMap.insert({device.deviceId, translationDetails});
            }
        }
        catch (const std::bad_any_cast &e)
        {
        }
    }

    DataContainer::setSignalValue(SIG_DEVICE_COLLECTION, deviceDescriptionsTotal);
    std::any_cast<std::function<uint16_t(void)>>(DataContainer::getSignalValue(CBK_CALCULATE_RUNTIME_NODE_HASH))();
}

void DeviceProvider::printIdMap()
{

    Logger::log("=Device Provider - zawartosc mapy ");
    for (const auto &pair : uniqueDeviceIdToNormalDeviceIdMap)
    {
        uint8_t key = pair.first;
        DeviceTranslationDetails value = pair.second;
        Logger::log("Unique ID: " + String(key) + ", Original ID: " + String(value.originalID) + ", Is Local: " + String(value.isLocal ? "true" : "false"));
    }
}

void DeviceProvider::deviceReset()
{

    Logger::log(" -> DeviceProvider.deviceReset");
}

ServiceRequestErrorCode DeviceProvider::handelService3Request(RcRequest &request, RcResponse &response, ServiceParameters_set3 &param, uint16_t payloadSize)
{
    ServiceRequestErrorCode result = SERV_GENERAL_FAILURE;
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(request.getRequestDeviceId());
    response.setResponseType((uint8_t)POSITIVE_RESP);

    if (payloadSize < (sizeof(ServiceParameters_set3) + MINIMAL_PAYLOAD_SIZE))
    {
        Logger::log("Payload size too small for set1");
        response.setResponseType((uint8_t)INVALID_REQ_RESP);
        sendResponse(response);
        return SERV_GENERAL_FAILURE;
    }

    memcpy(&param, &request.getData().at(2), sizeof(ServiceParameters_set3));

    if (param.direction == e_OUT_from_DEVICE)
    { /* we are expecting some data from the device to be provided in the response */

        /* replace remote address of the memory with local RAM */
        uint8_t *memoryAbstraction = (uint8_t *)malloc(param.size);

        if (memoryAbstraction == nullptr)
        {
            Logger::log("Unable to allocate memory for set3");
            response.setResponseType((uint8_t)INVALID_REQ_RESP);
            sendResponse(response);
            return SERV_GENERAL_FAILURE;
        }

        /* change value of the pointer in the request */
        param.buff = memoryAbstraction;

        /* call the service */
        result = (std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_set3(devicedetails.originalID, (DeviceServicesType)request.getData().at(SERVICE_NAME_INDEX), /* TODO negative response*/
                                                                                                                               param);

        if (result == SERV_SUCCESS)
        {
            response.setResponseType((uint8_t)POSITIVE_RESP);
            addDeviceDescriptionToResponsePayload(response, devicedetails.originalID);

            /* we must also add device manipulated information to the response */
            response.pushData(memoryAbstraction, param.size);
            // response.print();
            // Logger::log("Response total size: " + String(response.getData().size()));

            sendResponse(response);
            free(memoryAbstraction);

            return SERV_SUCCESS;
        }
        else
        {
            free(memoryAbstraction);

            Logger::log("Problem with service call");
            response.setResponseType((uint8_t)INVALID_REQ_RESP);
            sendResponse(response);
            return SERV_GENERAL_FAILURE;
        }
    }
    else if (param.direction == e_IN_to_DEVICE)
    { /* we are expecting some data inside of the request to be sent to the device */

        /* we must copy data from the request to the param structure */
        if (payloadSize < (sizeof(ServiceParameters_set3) + param.size + MINIMAL_PAYLOAD_SIZE))
        {
            Logger::log("Payload size too small for set1");
            response.setResponseType((uint8_t)INVALID_REQ_RESP);
            sendResponse(response);
            return SERV_GENERAL_FAILURE;
        }

        param.buff = (uint8_t *)malloc(param.size);
        if (param.buff == nullptr)
        {
            Logger::log("Unable to allocate memory for set3");
            response.setResponseType((uint8_t)INVALID_REQ_RESP);
            sendResponse(response);
            return SERV_GENERAL_FAILURE;
        }
        memcpy(param.buff, &request.getData().at(2 + sizeof(ServiceParameters_set3)), param.size);
        /* call the service */
        result = (std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_set3(devicedetails.originalID, (DeviceServicesType)request.getData().at(SERVICE_NAME_INDEX), /* TODO negative response*/
                                                                                                                               param);

        free(param.buff);

        if (result == SERV_SUCCESS)
        {
            response.setResponseType((uint8_t)POSITIVE_RESP);
            addDeviceDescriptionToResponsePayload(response, devicedetails.originalID);
            sendResponse(response);
            return SERV_SUCCESS;
        }
        else
        {
            Logger::log("Problem with service call");
            response.setResponseType((uint8_t)INVALID_REQ_RESP);
            sendResponse(response);
            return SERV_GENERAL_FAILURE;
        }
    }

    return result;
}

bool DeviceProvider::receiveRequest(RcRequest &request)
{
    // request.print();

    RcResponse response(request.getRequestId(), request.getRequestNodeMAC(), request.getRequestType(), INVALID_REQ_RESP);
    ServiceParameters_set1 params1;
    ServiceParameters_set3 params3;

    uint8_t *memoryAbstraction = nullptr;

    uint16_t payloadSize = request.getData().size();

    if (payloadSize < MINIMAL_PAYLOAD_SIZE)
    {
        Logger::log("Payload size too small");

        sendResponse(response);
        return false;
    }

    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(request.getRequestDeviceId());
    ServiceRequestErrorCode result = SERV_GENERAL_FAILURE;

    if (devicedetails.originalID != 255)
    {
        if (devicedetails.isLocal)
        {

            /* Which function service overloading is received? */
            switch (request.getData().at(SERVICE_OVERLOADING_FUNCTION_INDEX))
            {
            case serviceCall_NoParams:
                result = (std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_NoParams(devicedetails.originalID, (DeviceServicesType)request.getData().at(SERVICE_NAME_INDEX) /* TODO negative response*/
                );
                if (result == SERV_SUCCESS)
                {
                    // Logger::log("Success, adding response metadata");
                    response.setResponseType((uint8_t)POSITIVE_RESP);
                    addDeviceDescriptionToResponsePayload(response, devicedetails.originalID);
                }

                break;

            case serviceCall_1:
                /* Copy function parameter values from the request */

                if (payloadSize < sizeof(ServiceParameters_set1) + MINIMAL_PAYLOAD_SIZE)
                {
                    Logger::log("Payload size too small for set1");
                    response.setResponseType((uint8_t)INVALID_REQ_RESP);
                    sendResponse(response);
                    return false;
                }

                memcpy(&params1, &request.getData().at(2), sizeof(ServiceParameters_set1));

                /* call the service */
                result = (std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_set1(devicedetails.originalID, (DeviceServicesType)request.getData().at(SERVICE_NAME_INDEX), /* TODO negative response*/
                                                                                                                                       params1);
                if (result == SERV_SUCCESS)
                {
                    response.setResponseType((uint8_t)POSITIVE_RESP);
                    addDeviceDescriptionToResponsePayload(response, devicedetails.originalID);
                }

                sendResponse(response);
                break;

                /*TODO  case serviceCall_2: missing  */

            case serviceCall_3:
                /* Copy function parameter values from the request */
                result = handelService3Request(request, response, params3, payloadSize);
                break;

            default:
                break;
            }
        }
        else
        {
            Logger::log("Device id corruption within received request " + String((int)request.getRequestId()));
        }
    }
    else
    {
        Logger::log("No mapping found for received DeviceID (" + String((int)request.getRequestDeviceId()) + ") in request " + String((int)request.getRequestId()));
    }
    if (result == SERV_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
    //
}

ServiceRequestErrorCode DeviceProvider::service(
    uint8_t deviceId,
    DeviceServicesType serviceType)
{
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if (devicedetails.originalID != 255)
    {
        if (devicedetails.isLocal)
        {
            // zawołaj deviceEnable() w device manager
            // deviceManager_DeviceEnable(devicedetails.originalID, state);
            /* TODO */
            return (std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_NoParams(devicedetails.originalID, serviceType);
        }
        else
        {
            // zawołaj deviceEnable() w RC Server
            if (isRCServer)
            {
                // rcServer_DeviceEnable(devicedetails.originalID, state);
                /* TODO */
                return (std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_REMOTE_DEVICE_SERVICES))).serviceCall_NoParams(devicedetails.originalID, serviceType);
            }
        }
    }

    return SERV_GENERAL_FAILURE;
}

ServiceRequestErrorCode DeviceProvider::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set1 param)
{
    if (serviceType == DEVSERVICE_ROOM_STATE_CHANGE)
    {
        Logger::log("DeviceProvider:// serviceType != DEVSERVICE_ROOM_STATE_CHANGE");
        Logger::log("DeviceProvider:// param.a: " + String((int)param.a));
        Logger::log("DeviceProvider:// deviceId: " + String((int)deviceId));

        try
        {
            std::vector<DeviceDescription> devicesVector =
                std::any_cast<std::vector<DeviceDescription>>(
                    DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
            for (auto &device : devicesVector)
            {
                if (device.roomId == deviceId)
                {
                    roomStateChangeDeviceIdQueue.push(device.deviceId);
                }
            }
            requestedRoomState = (bool)param.a;
        }
        catch (std::bad_any_cast ex)
        {
            Logger::log("DeviceProvider:// Error during device collection casting");
        }

        return SERV_SUCCESS;
    }

    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if (devicedetails.originalID != 255)
    {
        if (devicedetails.isLocal)
        {
            // zawołaj deviceEnable() w device manager
            // deviceManager_DeviceEnable(devicedetails.originalID, state);
            /* TODO */
            return (std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_set1(devicedetails.originalID, serviceType, param);
        }
        else
        {
            // zawołaj deviceEnable() w RC Server
            if (isRCServer)
            {
                // rcServer_DeviceEnable(devicedetails.originalID, state);
                /* TODO */
                return (std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_REMOTE_DEVICE_SERVICES))).serviceCall_set1(devicedetails.originalID, serviceType, param);
            }
        }
    }

    return SERV_GENERAL_FAILURE;
}

ServiceRequestErrorCode DeviceProvider::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set2 param)
{
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if (devicedetails.originalID != 255)
    {
        if (devicedetails.isLocal)
        {
            // zawołaj deviceEnable() w device manager
            // deviceManager_DeviceEnable(devicedetails.originalID, state);
            /* TODO */
            return std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES)).serviceCall_set2(devicedetails.originalID, serviceType, param);
        }
        else
        {
            // zawołaj deviceEnable() w RC Server
            if (isRCServer)
            {
                // rcServer_DeviceEnable(devicedetails.originalID, state);
                /* TODO */
                return std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_REMOTE_DEVICE_SERVICES)).serviceCall_set2(devicedetails.originalID, serviceType, param);
            }
        }
    }

    return SERV_GENERAL_FAILURE;
}

ServiceRequestErrorCode DeviceProvider::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set3 param)
{
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if (devicedetails.originalID != 255)
    {
        if (devicedetails.isLocal)
        {
            // zawołaj deviceEnable() w device manager
            // deviceManager_DeviceEnable(devicedetails.originalID, state);
            /* TODO */
            return std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES)).serviceCall_set3(devicedetails.originalID, serviceType, param);
        }
        else
        {
            // zawołaj deviceEnable() w RC Server
            if (isRCServer)
            {
                // rcServer_DeviceEnable(devicedetails.originalID, state);
                /* TODO */
                return std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_REMOTE_DEVICE_SERVICES)).serviceCall_set3(devicedetails.originalID, serviceType, param);
            }
        }
    }
    Logger::log("DeviceProvider:// General failure.");
    return SERV_GENERAL_FAILURE;
}

void DeviceProvider::addDeviceDescriptionToResponsePayload(RcResponse &response, uint8_t deviceId)
{
    std::vector<DeviceDescription> deviceDescriptions = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_LOCAL_COLLECTION));
    for (auto &device : deviceDescriptions)
    {
        if (device.deviceId == deviceId)
        {
            uint8_t *serializedDescription = (uint8_t *)malloc(device.getSize());
            if (serializedDescription != nullptr)
            {
                /*Serialize DeviceDescription to memory buffer*/
                device.toByteArray(serializedDescription, device.getSize());
                /*Add memory buffer to response payload*/
                response.pushData(serializedDescription, device.getSize());
                // Logger::log("serializedDescription size: " + String(device.getSize()));

                free(serializedDescription);
            }
            else
            {
                Logger::log("DeviceProvider:// Error during response memory allocation");
            }
            uint16_t nodeHash = std::any_cast<uint16_t>(DataContainer::getSignalValue(SIG_RUNTIME_NODE_HASH));
            response.pushData((uint8_t *)&nodeHash, sizeof(nodeHash));
            break;
        }
    }
}

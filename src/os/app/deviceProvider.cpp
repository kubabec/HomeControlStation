#include <os/app/DeviceProvider.hpp>

//#include "deviceProvider.hpp"
// mapa do przechowywania unikalnych ID i powiazania lokalnych ID + info ot tym czy jest to urzadzenie lokalne czy zadalne (tzn na slave ESP)
std::map<uint8_t,DeviceTranslationDetails> DeviceProvider::uniqueDeviceIdToNormalDeviceIdMap;


std::function<bool(RcResponse&)> DeviceProvider::sendResponse;
std::function<bool(RcResponseLong&)> DeviceProvider::requestResponseLong;

bool DeviceProvider::isRCServer = false;

void DeviceProvider::deinit() {
    
}

void DeviceProvider::init()
{
    Serial.println("DeviceProvider init ...");
    
    /*TESTCODE*/
    /* Link service API functions to DeviceProvider function calls */
    DeviceServicesAPI servicesFunctionSet = {
        .serviceCall_NoParams = 
            [](uint8_t deviceId, DeviceServicesType request){ 
                return DeviceProvider::service(deviceId, request);
            },
        .serviceCall_set1 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set1 params){
                return DeviceProvider::service(deviceId, request, params);
            },
        .serviceCall_set2 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set2 params){
                return DeviceProvider::service(deviceId, request, params);
            },
        .serviceCall_set3 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set3 params){
                return DeviceProvider::service(deviceId, request, params);
            }
    };

    /* Push prepared service API to DataContainer */
    DataContainer::setSignalValue(
        SIG_DEVICE_SERVICES,
        static_cast<DeviceServicesAPI>(servicesFunctionSet));
    /*TESTCODE*/


    std::any rcServerCoding = DataContainer::getSignalValue(SIG_IS_RC_SERVER);
    isRCServer = std::any_cast<bool> (rcServerCoding);

    

    initLocalDevicesSetup();

    if(isRCServer)  {
        initRemoteDevicesSetup();
    }else {
        std::any_cast<std::function<bool(RequestType, std::function<bool(RcRequest&)>)>> 
        (DataContainer::getSignalValue(CBK_REGISTER_REQUEST_RECEIVER)) (SERVICE_CALL_REQ, DeviceProvider::receiveRequest);
       
                
        std::any responseCBK = DataContainer::getSignalValue(CBK_RESPONSE);
        sendResponse = (std::any_cast<std::function<bool(RcResponse&)>>(responseCBK));
        // requestResponseLong = (std::any_cast<std::function<bool(RcResponseLong&)>>(responseCBK));
    }
    
    updateDeviceDescriptionSignal();

    Serial.println("... done");
}

void DeviceProvider::initLocalDevicesSetup() {
    DataContainer::subscribe(SIG_LOCAL_COLLECTION, DeviceProvider::updateDeviceDescriptionSignal_onChange);
    
}

void DeviceProvider::initRemoteDevicesSetup() {
    DataContainer::subscribe(SIG_REMOTE_COLLECTION, DeviceProvider::updateDeviceDescriptionSignal_onChange);
}


void DeviceProvider::cyclic()
{
    
}

DeviceTranslationDetails DeviceProvider::getOriginalIdFromUnique(uint8_t uniqueId) {
    DeviceTranslationDetails retVal; //oryginalId ma default-owo 255
    
     if (uniqueDeviceIdToNormalDeviceIdMap.find(uniqueId) == uniqueDeviceIdToNormalDeviceIdMap.end()) {
            // not found 
            Serial.println("Device Provider-Translation for invalid ID received: " + String(uniqueId));
            Serial.println("---------------------------------------");
        } else {
            // found            
            retVal = uniqueDeviceIdToNormalDeviceIdMap.find(uniqueId)->second;
        }
        
    return retVal;
}


void DeviceProvider::updateDeviceDescriptionSignal_onChange(std::any signal) {
    updateDeviceDescriptionSignal();
}

void DeviceProvider::updateDeviceDescriptionSignal() {

    uniqueDeviceIdToNormalDeviceIdMap.clear();

    std::vector<DeviceDescription> deviceDescriptionsTotal;

    try {
        // Pobieranie i przetwarzanie lokalnych urządzeń

        std::any newLocalDeviceDescriptionVector = DataContainer::getSignalValue(SIG_LOCAL_COLLECTION);
        std::vector<DeviceDescription> localDeviceDescriptionVector = (std::any_cast<std::vector<DeviceDescription>>(newLocalDeviceDescriptionVector));

        /* No need to change unique id for local devices as it is remembered based on pins configuration */
        for(auto device: localDeviceDescriptionVector) {
            DeviceTranslationDetails translationDetails = {                
                .originalID = device.deviceId, 
                .isLocal = true            
            };
            // Serial.println("Local device id: " + String((int)device.deviceId));

            deviceDescriptionsTotal.push_back(device);
            uniqueDeviceIdToNormalDeviceIdMap.insert(std::pair<uint8_t,DeviceTranslationDetails>{device.deviceId,translationDetails});
        }   
           
    }catch (const std::bad_any_cast& e){ }

    if(isRCServer) {
        try {            
            // Pobieranie i przetwarzanie zdalnych urządzeń

            std::any newRemoteDescriptionVector = DataContainer::getSignalValue(SIG_REMOTE_COLLECTION);
            std::vector<DeviceDescription> remoteDescriptionVector = std::any_cast<std::vector<DeviceDescription>>(newRemoteDescriptionVector);

            for (auto device : remoteDescriptionVector) {
                
                DeviceTranslationDetails translationDetails = {                
                    .originalID = device.deviceId,
                    .isLocal = false
                };

                deviceDescriptionsTotal.push_back(device);                
                uniqueDeviceIdToNormalDeviceIdMap.insert({device.deviceId, translationDetails});

            }
            
        }catch (const std::bad_any_cast& e){ }   
    }
    
    DataContainer::setSignalValue(SIG_DEVICE_COLLECTION, deviceDescriptionsTotal);   
    std::any_cast<std::function<uint16_t(void)>>(DataContainer::getSignalValue(CBK_CALCULATE_RUNTIME_NODE_HASH))();  
}

void DeviceProvider::printIdMap() {

    Serial.println("=Device Provider - zawartosc mapy ");
    for (const auto& pair : uniqueDeviceIdToNormalDeviceIdMap) {
            uint8_t key = pair.first;
            DeviceTranslationDetails value = pair.second;            
            Serial.println("Unique ID: " + String(key) + ", Original ID: " + String(value.originalID) + ", Is Local: " + String (value.isLocal ? "true" : "false"));            
        }
}

void DeviceProvider::deviceReset() {

    Serial.println(" -> DeviceProvider.deviceReset");
}


bool DeviceProvider::receiveRequest(RcRequest& request) {
    // request.print();

    RcResponse response(request.getRequestId(), request.getRequestNodeMAC(),request.getRequestType(), INVALID_REQ_RESP);
    ServiceParameters_set1 params;

    uint16_t payloadSize = request.getData().size();
    const uint16_t MINIMAL_PAYLOAD_SIZE = 2;

    if(payloadSize < MINIMAL_PAYLOAD_SIZE) {
        Serial.println("Payload size too small");        
        
        sendResponse(response);
        return false;
    }

    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(request.getRequestDeviceId());
    
    if(devicedetails.originalID != 255) {
        if(devicedetails.isLocal) {
          
            ServiceRequestErrorCode result = SERV_GENERAL_FAILURE;
            /* Which function service overloading is received? */
            switch (request.getData().at(SERVICE_OVERLOADING_FUNCTION_INDEX))
            {
            case serviceCall_NoParams:
                result = (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_NoParams(
                    devicedetails.originalID,
                    (DeviceServicesType)request.getData().at(SERVICE_NAME_INDEX) /* TODO negative response*/
                );
                if(result == SERV_SUCCESS) {
                    // Serial.println("Success, adding response metadata");
                    response.setResponseType((uint8_t) POSITIVE_RESP);
                    addDeviceDescriptionToResponsePayload(response, devicedetails.originalID);
                }

                break;

            case serviceCall_1:
                /* Copy function parameter values from the request */
                
                if(payloadSize < sizeof(ServiceParameters_set1) + MINIMAL_PAYLOAD_SIZE) {
                    Serial.println("Payload size too small for set1");
                    response.setResponseType((uint8_t) INVALID_REQ_RESP);
                    sendResponse(response);
                    return false;
                }

                memcpy(&params, &request.getData().at(2), sizeof(ServiceParameters_set1));

                
                /* call the service */
                result = (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_set1(
                    devicedetails.originalID,
                    (DeviceServicesType)request.getData().at(SERVICE_NAME_INDEX), /* TODO negative response*/
                    params
                );
                if(result == SERV_SUCCESS) {
                    response.setResponseType((uint8_t) POSITIVE_RESP);
                    addDeviceDescriptionToResponsePayload(response, devicedetails.originalID);
                }
                
                break;
            
            default:
                break;
            }
        }else 
        {
            Serial.println("Device id corruption within received request " + String((int) request.getRequestId()));
        }
    }else 
    {
       Serial.println("No mapping found for received DeviceID (" + String((int)request.getRequestDeviceId())+ ") in request " + String((int) request.getRequestId()));
    }

    Serial.println("sending a response ...");
    sendResponse(response);    
    return true;
    //
}





ServiceRequestErrorCode DeviceProvider::service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
){
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if(devicedetails.originalID != 255) {
        if(devicedetails.isLocal) {
            //zawołaj deviceEnable() w device manager
            //deviceManager_DeviceEnable(devicedetails.originalID, state);
            /* TODO */
            return (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_NoParams(
                devicedetails.originalID,
                serviceType
            );

        }
        else {
            //zawołaj deviceEnable() w RC Server
            if(isRCServer) {
                //rcServer_DeviceEnable(devicedetails.originalID, state);
                /* TODO */
                return (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_REMOTE_DEVICE_SERVICES))).serviceCall_NoParams(
                    devicedetails.originalID,
                    serviceType
                );
            }
        }
    }
  
    return SERV_GENERAL_FAILURE;  
}

ServiceRequestErrorCode DeviceProvider::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set1 param
){
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if(devicedetails.originalID != 255) {
        if(devicedetails.isLocal) {
            //zawołaj deviceEnable() w device manager
            //deviceManager_DeviceEnable(devicedetails.originalID, state);
            /* TODO */
            return (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_set1(
                devicedetails.originalID,
                serviceType,
                param
            );
        }
        else {
            //zawołaj deviceEnable() w RC Server
            if(isRCServer) {
                //rcServer_DeviceEnable(devicedetails.originalID, state);
                /* TODO */
                return (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_REMOTE_DEVICE_SERVICES))).serviceCall_set1(
                    devicedetails.originalID,
                    serviceType,
                    param
                );
            }
        }
    }
  
    return SERV_GENERAL_FAILURE;
}

ServiceRequestErrorCode DeviceProvider::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set2 param
){
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if(devicedetails.originalID != 255) {
        if(devicedetails.isLocal) {
            //zawołaj deviceEnable() w device manager
            //deviceManager_DeviceEnable(devicedetails.originalID, state);
            /* TODO */
            return std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES)).serviceCall_set2(
                devicedetails.originalID,
                serviceType,
                param
            );
        }
        else {
            //zawołaj deviceEnable() w RC Server
            if(isRCServer) {
                //rcServer_DeviceEnable(devicedetails.originalID, state);
                /* TODO */
                return std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_REMOTE_DEVICE_SERVICES)).serviceCall_set2(
                    devicedetails.originalID,
                    serviceType,
                    param
                );
            }
        }
    }
  
    return SERV_GENERAL_FAILURE;
}

ServiceRequestErrorCode DeviceProvider::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set3 param
){
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if(devicedetails.originalID != 255) {
        if(devicedetails.isLocal) {
            //zawołaj deviceEnable() w device manager
            //deviceManager_DeviceEnable(devicedetails.originalID, state);
            /* TODO */
            return std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES)).serviceCall_set3(
                devicedetails.originalID,
                serviceType,
                param
            );
        }
        else {
            //zawołaj deviceEnable() w RC Server
            if(isRCServer) {
                //rcServer_DeviceEnable(devicedetails.originalID, state);
                /* TODO */
                return std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_REMOTE_DEVICE_SERVICES)).serviceCall_set3(
                    devicedetails.originalID,
                    serviceType,
                    param
                );
            }
        }
    }
    return SERV_GENERAL_FAILURE;
}

void DeviceProvider::addDeviceDescriptionToResponsePayload(RcResponse& response, uint8_t deviceId) {
    std::vector<DeviceDescription> deviceDescriptions = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_LOCAL_COLLECTION));
    for(auto& device: deviceDescriptions) {
        if(device.deviceId == deviceId) {
            uint8_t* serializedDescription = (uint8_t*)malloc(device.getSize());
            if(serializedDescription != nullptr){
                /*Serialize DeviceDescription to memory buffer*/
                device.toByteArray(serializedDescription, device.getSize());
                /*Add memory buffer to response payload*/
                response.pushData(serializedDescription, device.getSize());

                free(serializedDescription);
            }else {
                Serial.println("DeviceProvider:// Error during response memory allocation");
            }
            uint16_t nodeHash = std::any_cast<uint16_t>(DataContainer::getSignalValue(SIG_RUNTIME_NODE_HASH));
            response.pushData((uint8_t*)&nodeHash, sizeof(nodeHash));
            break;
        }
    }
}

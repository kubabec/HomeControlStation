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
    DataContainer::setSignalValue(SIG_CURRENT_REQUEST_PROCESSING_STATE, RequestProcessingState::eNO_REQUEST);

    
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
        
        // std::any_cast<std::function<bool(RequestType, std::function<bool(RcRequest&)>)>>
        // (DataContainer::getSignalValue(CBK_REGISTER_REQUEST_RECEIVER)) (EXTENDED_DATA_DOWNLOAD_REQ, DeviceProvider::receiveExtededDataRequest);
    
                
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

    static uint8_t uniqueId = 7; //unikalne ID dla wszystkich urządzeń na lokalnym ESP i zdalnych ESP

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

                //device.deviceId = uniqueId;
                deviceDescriptionsTotal.push_back(device);                
                //Serial.println("-------------------- Dodaje remote---------------");            
                uniqueDeviceIdToNormalDeviceIdMap.insert({device.deviceId, translationDetails});
                uniqueId++;
                if(uniqueId == 255) {
                    uniqueId = 7;
                }
            }
            
        }catch (const std::bad_any_cast& e){ }   
    }
    
    // Serial.println("DeviceProvider//: Content updated :");
    // printIdMap();
    Serial.println("///");    
    DataContainer::setSignalValue(SIG_DEVICE_COLLECTION, deviceDescriptionsTotal);     
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

    RcResponse response;
    response.getResponseId();
    response.getResponseType(); //200 is positive
    response.getRequestType();
    ServiceParameters_set1 params;



    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(request.getRequestDeviceId());

    if(devicedetails.originalID != 255) {
        if(devicedetails.isLocal) {
          
            /* Which function service overloading is received? */
            switch (request.getData().at(SERVICE_OVERLOADING_FUNCTION_INDEX))
            {
            case serviceCall_NoParams:
                (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_NoParams(
                    devicedetails.originalID,
                    (DeviceServicesType)request.getData().at(SERVICE_NAME_INDEX) /* TODO negative response*/
                );
                response.getResponseType();
                break;

            case serviceCall_1:
                /* Copy function parameter values from the request */
                

                memcpy(&params, &request.getData().at(2), sizeof(ServiceParameters_set1));

                /* call the service */
                (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES))).serviceCall_set1(
                    devicedetails.originalID,
                    (DeviceServicesType)request.getData().at(SERVICE_NAME_INDEX), /* TODO negative response*/
                    params
                );
                Serial.println("Setting state ");
                response.getResponseType();
                break;
            
            default:
                break;
            }

            Serial.println("Response sent.");
        }else 
        {
            Serial.println("Device id corruption within received request " + String((int) request.getRequestId()));
        }
    }else 
    {
       Serial.println("No mapping found for received DeviceID (" + String((int)request.getRequestDeviceId())+ ") in request " + String((int) request.getRequestId()));
    }

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

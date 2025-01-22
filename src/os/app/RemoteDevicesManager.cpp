#include <os/app/RemoteDevicesManager.hpp>

std::vector<DeviceDescription> RemoteDevicesManager::remoteDevicesCollection;
std::map<uint8_t, RCTranslation> RemoteDevicesManager::currentIdMapping;
std::array<ExternalNodeMapping, MAX_EXTERNAL_NODES> RemoteDevicesManager::mappingSlotsForExternalNodes;

void RemoteDevicesManager::init()
{
    Serial.println("RemoteDevicesManager init ...");

    DataContainer::subscribe(SIG_RC_DEVICES_INTERNAL_TUNNEL, RemoteDevicesManager::tunnelDataUpdate);

    
    std::any_cast<std::function<bool(RequestType, std::function<bool(RcResponse&)>)>> 
    (DataContainer::getSignalValue(CBK_REGISTER_RESPONSE_RECEIVER)) (SERVICE_CALL_REQ, RemoteDevicesManager::receiveResponse);

    // std::any_cast<std::function<bool(RequestType, std::function<bool(RcResponse&)>)>>
    // (DataContainer::getSignalValue(CBK_REGISTER_RESPONSE_RECEIVER)) (EXTENDED_DATA_DOWNLOAD_REQ, RemoteDevicesManager::receiveResponse);

   


    /*TESTCODE*/
    /* Link service API functions to RemoteDevicesManager function calls */
    DeviceServicesAPI servicesFunctionSet = {
        /* Funkcja lambda przekazuje wszystkie parametry do funkcji RemoteDevicesManager::service, która zajmuje się konkretną realizacją żądania.*/
        .serviceCall_NoParams = 
            [](uint8_t deviceId, DeviceServicesType request){ 
                return RemoteDevicesManager::service(deviceId, request);
            },
        .serviceCall_set1 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set1 params){
                return RemoteDevicesManager::service(deviceId, request, params);
            },
        .serviceCall_set2 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set2 params){
                return RemoteDevicesManager::service(deviceId, request, params);
            },
        .serviceCall_set3 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set3 params){
                return RemoteDevicesManager::service(deviceId, request, params);
            }
    };

    /* Push prepared service API to DataContainer */
    DataContainer::setSignalValue(
        SIG_REMOTE_DEVICE_SERVICES,
        static_cast<DeviceServicesAPI>(servicesFunctionSet));
    /*TESTCODE*/


    Serial.println("... done");
}

uint8_t RemoteDevicesManager::getMappingOffsetForNode(uint64_t& nodeMAC){
    uint8_t offset = 255;

    /* We must do:
    1. Check if this nodeMAC already exists in current mapping
    2. If not, assign first empty mapping to it */

    for(uint8_t i = 0; i < mappingSlotsForExternalNodes.size(); i++){
        if(mappingSlotsForExternalNodes.at(i).isUsed &&
           mappingSlotsForExternalNodes.at(i).mac == nodeMAC){
                /* mapping found at offset 'i' */
                return i;
           }
    }


    /* nodeMAC does not exist yet, new mapping must be found for it */
    for(uint8_t i = 0; i < mappingSlotsForExternalNodes.size(); i++){
        /* if unused mapping found */
        if(!mappingSlotsForExternalNodes.at(i).isUsed){
            /* set this mapping index as used */
            mappingSlotsForExternalNodes.at(i).isUsed = true;
            /* save node */
            mappingSlotsForExternalNodes.at(i).mac = nodeMAC;
            
            /* new slot found at offset 'i' */
            return i;
        }

    }



    return offset;
}

void RemoteDevicesManager::tunnelDataUpdate(std::any remoteDevices)
{
    try{
        remoteDevicesCollection = std::any_cast<std::vector<DeviceDescription>>(remoteDevices);
    
        currentIdMapping.clear();

        std::vector<DeviceDescription> vecRemoteDevicesDescription;

        const uint8_t numberOfLocalDevicesReserved = 10;
        const uint8_t rangeForEachNode = numberOfLocalDevicesReserved;
        
        for(auto device : remoteDevicesCollection) {
            /* find offset for the device */
            /* UniqueID presented to upper layers is calculated based on formula :
            (local devices count) + ((node identifiers range) * (offset for particular unique MAC)) + (local Device ID on it's node)
            e.g. :
            10 + (10 * 1) + 4 => 20 + 4 => 24 */
            uint8_t uniqueRcId = numberOfLocalDevicesReserved + (rangeForEachNode * getMappingOffsetForNode(device.macAddress)) + device.deviceId;

            // Create new translation {node, device}
            RCTranslation translation = {
                .mac = device.macAddress,
                .onSourceNodeLocalId = device.deviceId
            };
            Serial.println("New device with local ID" + String((int)device.deviceId) + ", MAC: "+String((int)translation.mac)+",  saved with ID" + String((int)uniqueRcId));
            
            /* Replace original ID with our new Unique */
            device.deviceId = uniqueRcId;
            /* Add modified record to public signal SIG_REMOTE_COLLECTION */
            vecRemoteDevicesDescription.push_back(device);
            
            /* Save translation {uniqueID, {node, device}} to internal map*/
            currentIdMapping.insert({uniqueRcId,translation});
            uniqueRcId ++;
        }


        DataContainer::setSignalValue(SIG_REMOTE_COLLECTION, vecRemoteDevicesDescription);

        Serial.println("UIBLOCKED:FALSE / tunnelUpdate");
        DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, static_cast<bool>(false));
        //Serial.println("->RCS - Ustawienie sygnalu w Data Container");   
        //printTranslationMap();
    }catch (std::bad_any_cast ex){}
}

RCTranslation RemoteDevicesManager::getTranslationFromUnique(uint8_t uniqueId) {
    RCTranslation retVal; //oryginalId ma default-owo 255
    
     if (currentIdMapping.find(uniqueId) == currentIdMapping.end()) {
            // not found 
            Serial.println("RCDevManager-Translation for invalid ID received: " + String(uniqueId));
            Serial.println("---------------------------------------");
        } else {
            // found            
            retVal = currentIdMapping.find(uniqueId)->second;
        }
        
    return retVal;
}

void RemoteDevicesManager::deinit()
{

}

void RemoteDevicesManager::cyclic()
{

}




void RemoteDevicesManager::printTranslationMap() {
    for(auto& mapping: currentIdMapping){
        Serial.println(String((int)mapping.first) + ": " );
        mapping.second.print();
    }
}


bool RemoteDevicesManager::receiveResponse(RcResponse& response)
{
    Serial.println("->Device Provider received response Id: " + String((int)response.responseId));
    return true;
}


/* TESTCODE */
ServiceRequestErrorCode RemoteDevicesManager::service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
){
     RCTranslation val = getTranslationFromUnique(deviceId);
    if(val.isValid()){
        RcRequest request;


        request.targetNodeMAC = val.mac;
        request.targetDeviceId = val.onSourceNodeLocalId;
        request.type = SERVICE_CALL_REQ;

        request.data[SERVICE_NAME_INDEX] = serviceType;
        /* TODO */
        request.data[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_NoParams; //0 - no params, 1 - set1 ...
        
        /* TODO */

        try{
            /* Pass request for processing to RCServer */
            std::any_cast<std::function<void(RcRequest&)>>(
                DataContainer::getSignalValue(CBK_CREATE_RC_REQUEST))(request);
            return SERV_SUCCESS;
        }catch(std::bad_any_cast ex){}
    }
    
    return SERV_GENERAL_FAILURE;  
}

ServiceRequestErrorCode RemoteDevicesManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set1 param
){
  RCTranslation val = getTranslationFromUnique(deviceId);
    if(val.isValid()){
        RcRequest request;


        request.targetNodeMAC = val.mac;
        request.targetDeviceId = val.onSourceNodeLocalId;
        request.type = SERVICE_CALL_REQ;

        request.data[SERVICE_NAME_INDEX] = serviceType;
        /* TODO */
        request.data[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1; //0 - no params, 1 - set1 ...
        memcpy(&(request.data[3]), &param, sizeof(param));
        /* TODO */

        try{
            /* Pass request for processing to RCServer */
            std::any_cast<std::function<void(RcRequest&)>>(
                DataContainer::getSignalValue(CBK_CREATE_RC_REQUEST))(request);
            return SERV_SUCCESS;
        }catch(std::bad_any_cast ex){}
    }
    
    return SERV_GENERAL_FAILURE;  
}

ServiceRequestErrorCode RemoteDevicesManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set2 param
){
  RCTranslation val = getTranslationFromUnique(deviceId);
    if(val.isValid()){
        RcRequest request;


        request.targetNodeMAC = val.mac;
        request.targetDeviceId = val.onSourceNodeLocalId;
        request.type = SERVICE_CALL_REQ;

        request.data[0] = serviceType;
        /* TODO */
        request.data[1] = 2; //0 - no params, 1 - set1 ...
        memcpy(&(request.data[3]), &param, sizeof(param));
        /* TODO */

        try{
            /* Pass request for processing to RCServer */
            std::any_cast<std::function<void(RcRequest&)>>(
                DataContainer::getSignalValue(CBK_CREATE_RC_REQUEST))(request);
            return SERV_SUCCESS;
        }catch(std::bad_any_cast ex){}
    }
    
    return SERV_GENERAL_FAILURE;  
}

ServiceRequestErrorCode RemoteDevicesManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set3 param
){
    
    return SERV_GENERAL_FAILURE;
}


void RemoteDevicesManager::downloadExtendedData(uint8_t deviceId) {
    
    RcRequest request;
    request.targetDeviceId = deviceId;
    request.type = EXTENDED_DATA_DOWNLOAD_REQ;

    DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, static_cast<bool>(true));

    try{
        
        std::any_cast<std::function<void(RcRequest&)>>(
                DataContainer::getSignalValue(CBK_CREATE_RC_REQUEST))(request);
            
    }catch(std::bad_any_cast ex){}    

}

/* TESTCODE */
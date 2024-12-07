#include <os/app/RemoteDevicesManager.hpp>

std::vector<OnOffDeviceDescription> RemoteDevicesManager::remoteDevicesCollection;
std::map<uint8_t, RCTranslation> RemoteDevicesManager::currentIdMapping;

void RemoteDevicesManager::init()
{
    Serial.println("RemoteDevicesManager init ...");

    DataContainer::subscribe(SIG_RC_DEVICES_INTERNAL_TUNNEL, "RcDeviceManager", RemoteDevicesManager::tunnelDataUpdate);

    
    std::any_cast<std::function<bool(RequestType, std::function<bool(RcResponse&)>)>> 
    (DataContainer::getSignalValue(CBK_REGISTER_RESPONSE_RECEIVER)) (SERVICE_CALL_REQ, RemoteDevicesManager::receiveResponse);

   


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
        "RemoteDeviceManager", 
        static_cast<DeviceServicesAPI>(servicesFunctionSet));
    /*TESTCODE*/


    Serial.println("... done");
}

void RemoteDevicesManager::tunnelDataUpdate(std::any remoteDevices)
{
    try{
        remoteDevicesCollection = std::any_cast<std::vector<OnOffDeviceDescription>>(remoteDevices);
    
        currentIdMapping.clear();
        /* This ID will be present in the public Remote_devices signal to have all devices 
        from all the nodes merged together to one vector with unique identifiers */
        static uint8_t uniqueRcId = 0;

        std::vector<OnOffDeviceDescription> vecRemoteOnOffDescription;
        
        for(auto device : remoteDevicesCollection) {
            // Create new translation {node, device}
            RCTranslation translation = {
                .nodeId = device.nodeId,
                .onSourceNodeLocalId = device.deviceId
            };

            /* Replace original ID with our new Unique */
            device.deviceId = uniqueRcId;
            device.nodeId = 0xFF; /* Obfuscate nodeID that it can't be used in upper layers */
            /* Add modified record to public signal SIG_REMOTE_COLLECTION_ONOFF */
            vecRemoteOnOffDescription.push_back(device);
            
            /* Save translation {uniqueID, {node, device}} to internal map*/
            currentIdMapping.insert({uniqueRcId,translation});
            uniqueRcId ++;
        }


        DataContainer::setSignalValue(SIG_REMOTE_COLLECTION_ONOFF,"RCManager", vecRemoteOnOffDescription);

        DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(false));
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
    Serial.println("->Device Provider received response Node Id: " + String((int)response.responceNodeId));
    DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(false));
    
    
    return true;
}


/* TESTCODE */
ServiceRequestErrorCode RemoteDevicesManager::service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
){
     RCTranslation val = getTranslationFromUnique(deviceId);
    // Serial.println("->RCS Translation-Device Enable - NodeId: " + String(val.nodeId) + " Local Id: "+ String(val.onSourceNodeLocalId));
    if(val.nodeId != 255){
        RcRequest request;


        request.targetNodeId = val.nodeId;
        request.targetDeviceId = val.onSourceNodeLocalId;
        request.type = SERVICE_CALL_REQ;

        request.data[0] = serviceType;
        /* TODO */
        request.data[1] = 0; //0 - no params, 1 - set1 ...
        
        /* TODO */
        DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(true));

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
    // Serial.println("->RCS Translation-Device Enable - NodeId: " + String(val.nodeId) + " Local Id: "+ String(val.onSourceNodeLocalId));
    if(val.nodeId != 255){
        RcRequest request;


        request.targetNodeId = val.nodeId;
        request.targetDeviceId = val.onSourceNodeLocalId;
        request.type = SERVICE_CALL_REQ;

        request.data[0] = serviceType;
        /* TODO */
        request.data[1] = 1; //0 - no params, 1 - set1 ...
        memcpy(&(request.data[2]), &param, sizeof(param));
        /* TODO */
        DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(true));

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
    // Serial.println("->RCS Translation-Device Enable - NodeId: " + String(val.nodeId) + " Local Id: "+ String(val.onSourceNodeLocalId));
    if(val.nodeId != 255){
        RcRequest request;


        request.targetNodeId = val.nodeId;
        request.targetDeviceId = val.onSourceNodeLocalId;
        request.type = SERVICE_CALL_REQ;

        request.data[0] = serviceType;
        /* TODO */
        request.data[1] = 2; //0 - no params, 1 - set1 ...
        memcpy(&(request.data[2]), &param, sizeof(param));
        /* TODO */
        DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(true));

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

/* TESTCODE */
#include <os/app/RemoteDevicesManager.hpp>

std::vector<DeviceDescription> RemoteDevicesManager::remoteDevicesCollection;
std::map<uint8_t, RCTranslation> RemoteDevicesManager::currentIdMapping;
std::array<ExternalNodeMapping, MAX_EXTERNAL_NODES> RemoteDevicesManager::mappingSlotsForExternalNodes;
RemoteDevicesManager::RDM_RequestProcessingState RemoteDevicesManager::requestProcessingState = RDM_NO_REQUEST;
RemoteDevicesManager::ServiceCallFingerprint RemoteDevicesManager::currentRequestFingerprint;
ServiceRequestErrorCode RemoteDevicesManager::currentRequestRespErrorCode;
uint8_t RemoteDevicesManager::awaitingResponseId = 255;


ServiceParameters_set3 localCopyOfLastActiveRequestParamSet3;
ServiceOverloadingFunction localCopyOfLastOverloading = serviceCall_INVALID;

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
            // Serial.println("New device with local ID" + String((int)device.deviceId) + ", MAC: "+String((int)translation.mac)+",  saved with ID" + String((int)uniqueRcId));
            
            /* Replace original ID with our new Unique */
            device.deviceId = uniqueRcId;
            /* Add modified record to public signal SIG_REMOTE_COLLECTION */
            vecRemoteDevicesDescription.push_back(device);
            
            /* Save translation {uniqueID, {node, device}} to internal map*/
            currentIdMapping.insert({uniqueRcId,translation});
            uniqueRcId ++;
        }


        DataContainer::setSignalValue(SIG_REMOTE_COLLECTION, vecRemoteDevicesDescription);

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


void RemoteDevicesManager::handleService3Response(RcResponse& response, DeviceDescription& responseDeviceDescription)
{
    /* we are expecting some data from the device */
    if(localCopyOfLastActiveRequestParamSet3.direction == e_OUT_from_DEVICE){
        /* Ensure that we received enough information from the slave in response , 
        which means there are bytes expected in request.size */
        if(response.getData().size() >= 
            (responseDeviceDescription.getSize() + 
            sizeof(uint16_t) + 
            localCopyOfLastActiveRequestParamSet3.size)){ 


                auto pointerToDynamicMemory = &(response.getData().at(responseDeviceDescription.getSize() + sizeof(uint16_t)));

                memcpy(
                    localCopyOfLastActiveRequestParamSet3.buff,
                    pointerToDynamicMemory,
                    localCopyOfLastActiveRequestParamSet3.size
                );


                localCopyOfLastActiveRequestParamSet3.size = 0;
                localCopyOfLastActiveRequestParamSet3.direction = e_UNKNOWN_PARAM_DIRECTION;
                localCopyOfLastOverloading = serviceCall_INVALID;
        }else {
            Serial.println("RDM : //// Received serviceCall_3 response, OUT from device, not enough data");
            Serial.println("received data size: " + String(response.getData().size()));
            Serial.println("expected data size: " + String(responseDeviceDescription.getSize() + sizeof(uint16_t) + localCopyOfLastActiveRequestParamSet3.size));
        }
    }

}


bool RemoteDevicesManager::receiveResponse(RcResponse& response)
{
    // Serial.println("->Device Provider received response Id: " + String((int)response.getResponseId()));
    // response.print();

    /* TODO : 
    - unpack the response success code ,
    - Update Devices description based on received data 
    - Update node hash
    */
   /* did we receive response for which we are waiting? */
    if(awaitingResponseId == response.getResponseId() && (response.getResponseType() != INVALID_REQ_RESP)){
        
        DeviceDescription responseDeviceDescription;
        if(response.getData().size() >= (responseDeviceDescription.getSize() + sizeof(uint16_t))){
            
            uint16_t responseNodeHash;

            /* extract device description and node hash from the response */
            //memcpy(&responseDeviceDescription, response.getData().data(), sizeof(DeviceDescription));
            responseDeviceDescription.fromByteArray(response.getData().data(), responseDeviceDescription.getSize());
            memcpy(&responseNodeHash, (response.getData().data() + responseDeviceDescription.getSize() ), sizeof(uint16_t));
            responseDeviceDescription.macAddress = response.getResponseNodeMAC();
 
            /* We need explicit handling for serviceCall_3 where memory is exchanged */
            if(localCopyOfLastOverloading == serviceCall_3 && localCopyOfLastActiveRequestParamSet3.direction == e_OUT_from_DEVICE){
                handleService3Response(response, responseDeviceDescription);
            }

            std::any_cast<std::function<void(DeviceDescription&, uint16_t)>>(
                DataContainer::getSignalValue(CBK_UPDATE_RC_SLAVE_INFORMATION)) (
                    responseDeviceDescription,
                    responseNodeHash
                );

            currentRequestRespErrorCode = (response.getResponseType() == POSITIVE_RESP) ? SERV_SUCCESS : SERV_GENERAL_FAILURE;
            Serial.println("RDM : //// Received response with error code: " + String((int)currentRequestRespErrorCode));

        }else {
            Serial.println("Invalid data lenght received from remote slave, returning GENERAL_FAILURE");
            /* invalid response payload length received */
            currentRequestRespErrorCode = SERV_GENERAL_FAILURE;
        }

        requestProcessingState = RDM_REQUEST_COMPLETED;
    }
    else if(response.getResponseType() == INVALID_REQ_RESP){
        Serial.println("Invalid request received, returning GENERAL_FAILURE");
        requestProcessingState = RDM_REQUEST_FAILED;
    }

    return true;
}


ServiceRequestErrorCode RemoteDevicesManager::service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
){

    /* Check if RDM is capable to receive new request */
    if(requestProcessingState == RDM_NO_REQUEST){

     RCTranslation val = getTranslationFromUnique(deviceId);
    if(val.isValid()){
        RcRequest request (val.onSourceNodeLocalId, val.mac, SERVICE_CALL_REQ);


        request.pushData(serviceType);
        
        /* TODO */
        request.pushData(serviceCall_NoParams); //0 - no params, 1 - set1 ...

            try{
                /* Pass request for processing to RCServer */
                awaitingResponseId = std::any_cast<std::function<uint8_t(RcRequest&)>>(
                    DataContainer::getSignalValue(CBK_CREATE_RC_REQUEST))(request);


                /* Update current new request fingerprint */
                currentRequestFingerprint = {
                    .deviceId = deviceId,
                    .serviceName = serviceType,
                    .overloading = serviceCall_NoParams
                };

                /* Change request processing state to IN PROGRESS, this state will be 
                set back to RDM_REQUEST_COMPLETED in response reception callback, when it will arrive */
                requestProcessingState = RDM_REQUEST_IN_PROGRESS;

                /* RDM starts to wait for the response */
                return SERV_PENDING;
            }catch(std::bad_any_cast ex){}
        }

    }else {

        /* check if we really received the same request for which response we are waiting for */
        ServiceCallFingerprint receivedRequestFingerprint = {
            .deviceId = deviceId,
            .serviceName = serviceType,
            .overloading = serviceCall_NoParams
        };
        if(!(receivedRequestFingerprint == currentRequestFingerprint)){
            /* When we received another request, when currently something else 
            is waiting for the response, we must reject the new caller by returning BUSY */
            return SERV_BUSY;
        }

        /* Here we are sure that we are working on this request ... */
        if(requestProcessingState > RDM_REQUEST_IN_PROGRESS){
            requestProcessingState = RDM_NO_REQUEST;
            /* We already took care for the buffer copying inside of receiveResponse function */
            ServiceRequestErrorCode retVal = 
                requestProcessingState == RDM_REQUEST_COMPLETED ? SERV_SUCCESS : SERV_GENERAL_FAILURE;
            return retVal;
        }

        /* RDM is waiting for the response to this request */
        return SERV_PENDING;
    }
    
    return SERV_GENERAL_FAILURE;  
}

ServiceRequestErrorCode RemoteDevicesManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set1 param
){
    /* Check if RDM is capable to receive new request */
    if(requestProcessingState == RDM_NO_REQUEST){

        RCTranslation val = getTranslationFromUnique(deviceId);
        if(val.isValid()){
            RcRequest request (val.onSourceNodeLocalId, val.mac, SERVICE_CALL_REQ);

            request.pushData(serviceType);
            /* TODO */
            
            request.pushData(serviceCall_1); //0 - no params, 1 - set1 ...
            request.pushData((uint8_t*)&param, sizeof(param));
            /* TODO */


            try{
                /* Pass request for processing to RCServer */
                awaitingResponseId = std::any_cast<std::function<uint8_t(RcRequest&)>>(
                    DataContainer::getSignalValue(CBK_CREATE_RC_REQUEST))(request);

                /* Update current new request fingerprint */
                currentRequestFingerprint = {
                    .deviceId = deviceId,
                    .serviceName = serviceType,
                    .overloading = serviceCall_1
                };

                /* Change request processing state to IN PROGRESS, this state will be 
                set back to RDM_REQUEST_COMPLETED in response reception callback, when it will arrive */
                requestProcessingState = RDM_REQUEST_IN_PROGRESS;

                /* RDM starts to wait for the response */
                return SERV_PENDING;

            }catch(std::bad_any_cast ex){}
        }

    }else {
        /* check if we really received the same request for which response we are waiting for */
        ServiceCallFingerprint receivedRequestFingerprint = {
            .deviceId = deviceId,
            .serviceName = serviceType,
            .overloading = serviceCall_1
        };
        if(!(receivedRequestFingerprint == currentRequestFingerprint)){
            /* When we received another request, when currently something else 
            is waiting for the response, we must reject the new caller by returning BUSY */
            return SERV_BUSY;
        }

        /* Here we are sure that we are working on this request ... */
        if(requestProcessingState > RDM_REQUEST_IN_PROGRESS){
            requestProcessingState = RDM_NO_REQUEST;
            /* We already took care for the buffer copying inside of receiveResponse function */
            ServiceRequestErrorCode retVal = 
                requestProcessingState == RDM_REQUEST_COMPLETED ? SERV_SUCCESS : SERV_GENERAL_FAILURE;
            return retVal;
        }

        /* RDM is waiting for the response to this request */
        return SERV_PENDING;
    }
    
    return SERV_GENERAL_FAILURE;  
}

ServiceRequestErrorCode RemoteDevicesManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set2 param
){

    /* Check if RDM is capable to receive new request */
    if(requestProcessingState == RDM_NO_REQUEST){
        RCTranslation val = getTranslationFromUnique(deviceId);
        if(val.isValid()){
            RcRequest request (val.onSourceNodeLocalId, val.mac, SERVICE_CALL_REQ);


            request.pushData(serviceType);
        /* TODO */
        
            request.pushData(2); //0 - no params, 1 - set1 ...
            request.pushData((uint8_t*)&param, sizeof(param));
                /* TODO */


            try{
                /* Pass request for processing to RCServer */
                awaitingResponseId = std::any_cast<std::function<uint8_t(RcRequest&)>>(
                    DataContainer::getSignalValue(CBK_CREATE_RC_REQUEST))(request);

                /* Update current new request fingerprint */
                currentRequestFingerprint = {
                    .deviceId = deviceId,
                    .serviceName = serviceType,
                    .overloading = serviceCall_2
                };

                /* Change request processing state to IN PROGRESS, this state will be 
                set back to RDM_REQUEST_COMPLETED in response reception callback, when it will arrive */
                requestProcessingState = RDM_REQUEST_IN_PROGRESS;

                /* RDM starts to wait for the response */
                return SERV_PENDING;
            }catch(std::bad_any_cast ex){}
        }
    }else {
        /* check if we really received the same request for which response we are waiting for */
        ServiceCallFingerprint receivedRequestFingerprint = {
            .deviceId = deviceId,
            .serviceName = serviceType,
            .overloading = serviceCall_2
        };
        if(!(receivedRequestFingerprint == currentRequestFingerprint)){
            /* When we received another request, when currently something else 
            is waiting for the response, we must reject the new caller by returning BUSY */
            return SERV_BUSY;
        }

        /* Here we are sure that we are working on this request ... */
        if(requestProcessingState > RDM_REQUEST_IN_PROGRESS){
            requestProcessingState = RDM_NO_REQUEST;
            /* We already took care for the buffer copying inside of receiveResponse function */
            ServiceRequestErrorCode retVal = 
                requestProcessingState == RDM_REQUEST_COMPLETED ? SERV_SUCCESS : SERV_GENERAL_FAILURE;
            return retVal;
        }

        /* RDM is waiting for the response to this request */
        return SERV_PENDING;
    }
    
    return SERV_GENERAL_FAILURE;  
}

ServiceRequestErrorCode RemoteDevicesManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set3 param
){

    /* Check if RDM is capable to receive new request */
    if(requestProcessingState == RDM_NO_REQUEST){
        Serial.println("Starting new request with param3...");
        RCTranslation val = getTranslationFromUnique(deviceId);
        if(val.isValid()){
            RcRequest request (val.onSourceNodeLocalId, val.mac, SERVICE_CALL_REQ);


            request.pushData(serviceType);        
            request.pushData(3); //0 - no params, 1 - set1 ...
            request.pushData((uint8_t*)&param, sizeof(param));

            if(param.direction == e_IN_to_DEVICE)
            {
                /* Data provided under service memory must be sent to the device*/
                request.pushData((byte*) param.buff, param.size);
            }

            try{
                /* Pass request for processing to RCServer */
                awaitingResponseId = std::any_cast<std::function<uint8_t(RcRequest&)>>(
                    DataContainer::getSignalValue(CBK_CREATE_RC_REQUEST))(request);

                /* Update current new request fingerprint */
                currentRequestFingerprint = {
                    .deviceId = deviceId,
                    .serviceName = serviceType,
                    .overloading = serviceCall_3,
                    .memoryRequestDirection = param.direction
                };

                localCopyOfLastActiveRequestParamSet3 = param;
                localCopyOfLastOverloading = serviceCall_3;

                /* Change request processing state to IN PROGRESS, this state will be 
                set back to RDM_REQUEST_COMPLETED in response reception callback, when it will arrive */
                requestProcessingState = RDM_REQUEST_IN_PROGRESS;

                Serial.println("RDM : //// Request with serviceCall_3 sent, waiting for the response");

                /* RDM starts to wait for the response */
                return SERV_PENDING;
            }catch(std::bad_any_cast ex){}
        }
    }else {
        /* check if we really received the same request for which response we are waiting for */
        ServiceCallFingerprint receivedRequestFingerprint = {
            .deviceId = deviceId,
            .serviceName = serviceType,
            .overloading = serviceCall_3
        };
        if(!(receivedRequestFingerprint == currentRequestFingerprint)){
            /* When we received another request, when currently something else 
            is waiting for the response, we must reject the new caller by returning BUSY */
            return SERV_BUSY;
        }

        /* Here we are sure that we are working on this request ... */
        if(requestProcessingState > RDM_REQUEST_IN_PROGRESS){
            /* We already took care for the buffer copying inside of receiveResponse function */
            ServiceRequestErrorCode retVal = 
                requestProcessingState == RDM_REQUEST_COMPLETED ? SERV_SUCCESS : SERV_GENERAL_FAILURE;


            requestProcessingState = RDM_NO_REQUEST;
            return retVal;
        }

        /* RDM is waiting for the response to this request */
        return SERV_PENDING;
    }
    
    return SERV_GENERAL_FAILURE;
}

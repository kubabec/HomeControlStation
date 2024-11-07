#include <os/app/DeviceProvider.hpp>
// mapa do przechowywania unikalnych ID i powiazania lokalnych ID + info ot tym czy jest to urzadzenie lokalne czy zadalne (tzn na slave ESP)
std::map<uint8_t,DeviceTranslationDetails> DeviceProvider::uniqueDeviceIdToNormalDeviceIdMap;

std::function<bool(uint8_t, bool)> DeviceProvider::deviceManager_DeviceEnable;
std::function<bool(uint8_t, uint8_t)> DeviceProvider::deviceManager_BrightnessChange;
std::function<bool(uint8_t, bool)> DeviceProvider::rcServer_DeviceEnable;
std::function<bool(uint8_t, uint8_t)> DeviceProvider::rcServer_BrightnessChange;
std::function<bool(SystemResponse&)> DeviceProvider::requestResponse;


bool DeviceProvider::isRCServer = false;

void DeviceProvider::deinit() {
    
}

void DeviceProvider::init()
{
    Serial.println("DeviceProvider init ...");
    DataContainer::setSignalValue(SIG_CURRENT_REQUEST_PROCESSING_STATE, "DeviceProvider", RequestProcessingState::eNO_REQUEST);

    DataContainer::setSignalValue(CBK_DEVICE_ENABLE,"DeviceProvider", static_cast<std::function<bool(uint8_t, bool)> > (DeviceProvider::deviceEnable));
    DataContainer::setSignalValue(CBK_DEVICE_BRIGHTNESS_CHANGE,"DeviceProvider", static_cast<std::function<bool(uint8_t, uint8_t)> > (DeviceProvider::deviceBrightnessChange));

    /*NEW*/
    DeviceControlFunctionSet controlSet = {
        .setDeviceState = DeviceProvider::deviceEnable,
        .changeBrightness = DeviceProvider::deviceBrightnessChange
    };
    DataContainer::setSignalValue(
        SIG_CONTROL_FUNCTIONS,
        "DeviceManager",
        static_cast<DeviceControlFunctionSet>(controlSet)
    );
    /*NEW*/


    std::any rcServerCoding = DataContainer::getSignalValue(SIG_IS_RC_SERVER);
    isRCServer = std::any_cast<bool> (rcServerCoding);

    

    initLocalDevicesSetup();

    if(isRCServer)  {
        initRemoteDevicesSetup();

        std::any_cast<std::function<bool(SystemRequestType, std::function<bool(SystemResponse&)>)>> 
        (DataContainer::getSignalValue(CBK_REGISTER_RESPONSE_RECEIVER)) (ENABLE_SYSREQ, DeviceProvider::receiveSystemResponse);

        std::any_cast<std::function<bool(SystemRequestType, std::function<bool(SystemResponse&)>)>> 
        (DataContainer::getSignalValue(CBK_REGISTER_RESPONSE_RECEIVER)) (DISABLE_SYSREQ, DeviceProvider::receiveSystemResponse);

        std::any_cast<std::function<bool(SystemRequestType, std::function<bool(SystemResponse&)>)>> 
        (DataContainer::getSignalValue(CBK_REGISTER_RESPONSE_RECEIVER)) (BRIGHTNESS_CHANGE_SYSREQ, DeviceProvider::receiveSystemResponse);

    }else {
        std::any_cast<std::function<bool(SystemRequestType, std::function<bool(SystemRequest&)>)>> 
        (DataContainer::getSignalValue(CBK_REGISTER_REQUEST_RECEIVER)) (ENABLE_SYSREQ, DeviceProvider::receiveSystemRequest);

        std::any_cast<std::function<bool(SystemRequestType, std::function<bool(SystemRequest&)>)>> 
        (DataContainer::getSignalValue(CBK_REGISTER_REQUEST_RECEIVER)) (DISABLE_SYSREQ, DeviceProvider::receiveSystemRequest);

        std::any_cast<std::function<bool(SystemRequestType, std::function<bool(SystemRequest&)>)>> 
        (DataContainer::getSignalValue(CBK_REGISTER_REQUEST_RECEIVER)) (BRIGHTNESS_CHANGE_SYSREQ, DeviceProvider::receiveSystemRequest);
        
        std::any responseCBK = DataContainer::getSignalValue(CBK_RESPONSE);
        requestResponse = (std::any_cast<std::function<bool(SystemResponse&)>>(responseCBK));
    }
    
    updateDeviceDescriptionSignal();

    Serial.println("... done");
}

void DeviceProvider::initLocalDevicesSetup() {
    DataContainer::subscribe(SIG_LOCAL_COLLECTION_ONOFF, "DeviceProvider", DeviceProvider::updateDeviceDescriptionSignal_onChange);

    std::any localDeviceEnableCBK = DataContainer::getSignalValue(CBK_LOCAL_DEVICE_ENABLE);
    deviceManager_DeviceEnable = (std::any_cast<std::function<bool(uint8_t, bool)>>(localDeviceEnableCBK));
    std::any localDeviceBrightnessCBK = DataContainer::getSignalValue(CBK_LOCAL_DEVICE_BRIGHTNESS_CHANGE);
    deviceManager_BrightnessChange = (std::any_cast<std::function<bool(uint8_t, uint8_t)>>(localDeviceBrightnessCBK));

    
}

void DeviceProvider::initRemoteDevicesSetup() {
    DataContainer::subscribe(SIG_REMOTE_COLLECTION_ONOFF, "DeviceProvider", DeviceProvider::updateDeviceDescriptionSignal_onChange);

    std::any RCDeviceEnableCBK = DataContainer::getSignalValue(CBK_REMOTE_DEVICE_ENABLE);
    rcServer_DeviceEnable = (std::any_cast<std::function<bool(uint8_t, bool)>>(RCDeviceEnableCBK));
    std::any RCDeviceBrightnessCBK = DataContainer::getSignalValue(CBK_REMOTE_DEVICE_BRIGHTNESS_CHANGE);
    rcServer_BrightnessChange = (std::any_cast<std::function<bool(uint8_t, uint8_t)>>(RCDeviceBrightnessCBK));

    
    
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




bool DeviceProvider::deviceEnable(uint8_t deviceId, bool state){

    Serial.println("");
    Serial.println("->Device Provider-Device enable request: DeviceId " + String(deviceId) + " State: " + String(state));
    
    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if(devicedetails.originalID != 255) {
        if(devicedetails.isLocal) {
            //zawołaj deviceEnable() w device manager
            deviceManager_DeviceEnable(devicedetails.originalID, state);
            
        }
        else {
            //zawołaj deviceEnable() w RC Server
            if(isRCServer) {
                rcServer_DeviceEnable(devicedetails.originalID, state);
            }
        }
    }
    
    printIdMap();
    return true;
}

bool DeviceProvider::deviceBrightnessChange(uint8_t deviceId, uint8_t brightnessLevel){

    DeviceTranslationDetails devicedetails = getOriginalIdFromUnique(deviceId);
    if(devicedetails.originalID != 255) {
        if(devicedetails.isLocal) {
            //zawołaj deviceBrightnesChange() w device manager
            deviceManager_BrightnessChange(devicedetails.originalID, brightnessLevel);
        }
        else {
            //zawołaj deviceBrightnesChange() w RC Server
            if(isRCServer) {
                rcServer_BrightnessChange(devicedetails.originalID, brightnessLevel);
            }
        }

    }
    return true;
}

void DeviceProvider::updateDeviceDescriptionSignal_onChange(std::any signal) {
    updateDeviceDescriptionSignal();
}

void DeviceProvider::updateDeviceDescriptionSignal() {

    uniqueDeviceIdToNormalDeviceIdMap.clear();
    std::vector<OnOffDeviceDescription> vecOnOffDescription;

    static uint8_t uniqueId = 7; //unikalne ID dla wszystkich urządzeń na lokalnym ESP i zdalnych ESP

    try {
        // Pobieranie i przetwarzanie lokalnych urządzeń
        std::any newLocalDescriptionVector = DataContainer::getSignalValue(SIG_LOCAL_COLLECTION_ONOFF);
        std::vector<OnOffDeviceDescription> localOnOffDescriptionVector = (std::any_cast<std::vector<OnOffDeviceDescription>>(newLocalDescriptionVector));

        for(auto device: localOnOffDescriptionVector) {
            
            DeviceTranslationDetails translationDetails = {                
                .originalID = device.deviceId,
                .isLocal = true            
            };

            //device.deviceId = uniqueId;
            vecOnOffDescription.push_back(device);
            
            //Serial.println("-------------------- Dodaje local---------------");
            uniqueDeviceIdToNormalDeviceIdMap.insert(std::pair<uint8_t,DeviceTranslationDetails>{device.deviceId,translationDetails});
            //uniqueId ++;
        }        
           
    }catch (const std::bad_any_cast& e){ }

    if(isRCServer) {
        try {            
            // Pobieranie i przetwarzanie zdalnych urządzeń
            std::any newRemoteDescriptionVector = DataContainer::getSignalValue(SIG_REMOTE_COLLECTION_ONOFF);
            std::vector<OnOffDeviceDescription> remoteOnOffDescriptionVector = std::any_cast<std::vector<OnOffDeviceDescription>>(newRemoteDescriptionVector);

            for (auto device : remoteOnOffDescriptionVector) {
                
                DeviceTranslationDetails translationDetails = {                
                    .originalID = device.deviceId,
                    .isLocal = false
                };

                device.deviceId = uniqueId;
                vecOnOffDescription.push_back(device);                
                //Serial.println("-------------------- Dodaje remote---------------");            
                uniqueDeviceIdToNormalDeviceIdMap.insert({uniqueId, translationDetails});
                uniqueId++;
                if(uniqueId == 0) {
                    uniqueId = 7;
                }
            }
            
        }catch (const std::bad_any_cast& e){ }   
    }
    
    DataContainer::setSignalValue(SIG_COLLECTION_ONOFF,"DeviceProvider", vecOnOffDescription);     
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


bool DeviceProvider::receiveSystemRequest(SystemRequest& request) {
    request.print();
    SystemResponse response;
    response.responseId = request.requestId;
    response.isPositive = 200; //200 is positive
    response.type = request.type;

    switch (request.type)
    {
    case ENABLE_SYSREQ:
        deviceEnable(request.data[0],true);
        requestResponse(response);
        break;
    case DISABLE_SYSREQ:
        deviceEnable(request.data[0],false);
        requestResponse(response);
        break;   
    case BRIGHTNESS_CHANGE_SYSREQ:
        deviceBrightnessChange(request.data[0],request.data[2]);
        requestResponse(response);
        break;  
    default:
        break;
    }
    return true;
    //
}

bool DeviceProvider::receiveSystemResponse(SystemResponse& response) {
    Serial.println(" Received respond Id: " + String((int)response.responseId));
    return true;

}

// uint8_t DeviceProvider::findUniqueIdByOriginalId(uint8_t originalId) {
//     for (const auto& pair : uniqueDeviceIdToNormalDeviceIdMap) {
//         if (pair.second.originalID == originalId) {
//             Serial.println("Znaleziono unikalny ID: " + String(pair.first));
//             return pair.first; // Zwracamy znaleziony unikalny ID
//         }
//     }
//     // Jeśli nie znaleziono, zwróć wartość błędu (np. 0)
//     Serial.println("Nie znaleziono unikalnego ID dla podanego oryginalnego ID");
//     return 0;
// }
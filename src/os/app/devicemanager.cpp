#include <os/app/DeviceManager.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <ArduinoJson.h>




std::vector<OnOffDevice> DeviceManager::vecOnOffDevices = { };
std::vector<LedWS1228bDeviceType> DeviceManager::ledws2812bDevices = {};
std::vector<TempSensorDHT11DeviceType> DeviceManager::tempSensorsDevices = {};


ConfigSlotsDataType DeviceManager::pinConfigSlotsRamMirror = {};
ExtendedDataAllocator DeviceManager::extDataAllocator;

/*TESTCODE*/
std::vector<Device*> DeviceManager::devices;
// TestDeviceType DeviceManager::testDev;
/*TESTCODE*/

void DeviceManager::deinit() {
    for(uint8_t i = e_BLOCK_DEVICE_1; i <= e_BLOCK_DEVICE_6; i++)
    {
        /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
        bool success = std::any_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(
            DataContainer::getSignalValue(CBK_SET_NVM_DATABLOCK)
        )(
            (PersistentDatablockID)i, // Datablock ID
            (uint8_t*)&pinConfigSlotsRamMirror.slots.at(i) // local memory buffer for datablock data
        );

        if(!success){
            Serial.println("Error during saving "+ String((int)i) + " datablock");
        }
    }
}

void DeviceManager::init()
{
    Serial.println("DeviceManager init ...");

    /* Protection against PersistentDataBlock size modification without DeviceConfigSlotType update */
    if(PersistentDataBlock::getSize() == DeviceConfigSlotType::getSize() )
    {
        /* Reserve buffer for a single config block */
        DeviceConfigSlotType* configBlock = (DeviceConfigSlotType*) malloc (DeviceConfigSlotType::getSize());

        if(configBlock != nullptr)
        {

            uint8_t numberOfSuccessfullyRetrievedDevices = 0;

            /* For each DEVICE relevant datablock */
            for(uint8_t datablock = e_BLOCK_DEVICE_1; datablock <= e_BLOCK_DEVICE_6; datablock ++){
                /* Fill memory area with default 0xFF values */
                memset(configBlock, 0x00, DeviceConfigSlotType::getSize());

                /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
                std::any_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(
                    DataContainer::getSignalValue(CBK_GET_NVM_DATABLOCK)
                )(
                    (PersistentDatablockID)datablock, // Datablock ID
                    (uint8_t*)configBlock // local memory buffer for datablock data
                 );

                /* Now NVM data of current datablock shall be inside of 'configBlock' variable */


                /* Try to extract (add to devices vector) device based on NVM data */
                if(extractDeviceInstanceBasedOnNvmData(*configBlock, datablock))
                {
                    numberOfSuccessfullyRetrievedDevices++;
                }

            }

            Serial.println(String((int)numberOfSuccessfullyRetrievedDevices) +
            " devices has been successfully retrieved from NVM memory");

            free(configBlock);

            /* push this notification only when network is available and there are no devices configured */
            if(std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION)).networkCredentialsAvailable && numberOfSuccessfullyRetrievedDevices == 0){
                UserInterfaceNotification notif;
                notif.title = "Missing GPIO setup";
                notif.body = "Looks like there are no GPIO devices configured yet. Navigate to 'Settings -> Devices Management' to add the device.";
                notif.type = UserInterfaceNotification::INFO;
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
            }

            /* Publish retrieved DeviceConfigSlots signal to the system */
            Serial.println("DeviceManager//: Publishin config slots.");
            DataContainer::setSignalValue(SIG_CONFIG_SLOTS, static_cast<ConfigSlotsDataType>(pinConfigSlotsRamMirror));
        }
    }else
    {
        Serial.println("DeviceConfigSlotType size does not match length of PersistentDataBlock");
    }

    Serial.println("DeviceManager//: Config slots setup:");
    for(auto& slot : pinConfigSlotsRamMirror.slots){
        slot.print();
    }


    /* devices merging after NVM restoration */
    {
        /* Do vektora devices wrzucam devices z vektora OnOffDevices*/
        for(OnOffDevice& device : vecOnOffDevices) 
        {
            devices.push_back(&device); //wrzucam pointer na device onOffDevice         
        }

        /* Add LED strips to common devices vector*/
        for(LedWS1228bDeviceType& device: ledws2812bDevices){
            devices.push_back(&device);
        }

        /* Add temperature sensors to common devices vector*/
        for(TempSensorDHT11DeviceType& device: tempSensorsDevices){
            devices.push_back(&device);
        }
    }

    if(devices.size() > 0){
        /* Extended memory allocation and assignment must happen here before init of each device*/
        ExtendedMemoryCtrlAPI extMemoryFunctions = std::any_cast<ExtendedMemoryCtrlAPI>(DataContainer::getSignalValue(SIG_EXT_MEM_CTRL_API));

        bool isOneMoreRebootNeeded = false;
        for(auto& device : devices){
            uint16_t extendedMemoryLengthForEachDevice = device->getExtendedMemoryLength();
            uint8_t deviceIdentifier = device->getDeviceIdentifier();

            /* check if device even need extra memory */
            if(extendedMemoryLengthForEachDevice > 0){
                uint16_t checkSize = 0;
                /* Try to get already existing extended memory for this particular device */
                uint8_t* memoryPtr = extMemoryFunctions.getExtMemoryPtrByDeviceId(
                    deviceIdentifier,
                    &checkSize
                );

                /* Did we receive valid data pointer, so it means extended memory for this device was already existing? */
                if(checkSize == extendedMemoryLengthForEachDevice && memoryPtr != nullptr){
                    ServiceParameters_set3 extMemParams = {
                        .buff = memoryPtr,
                        .size = checkSize
                    };
                    device->service(DEVSERVICE_SET_EXT_MEMORY_PTR, extMemParams);
                }else {
                    /* We could not receive valid data from ExtendedMemoryManager if the record for this device 
                        does not exist yet, it means device was just coded for the first time before reboot. */

                    /* we need to use other function to reserve extended data for future runtime cycle */
                    bool success = extMemoryFunctions.requestNewExtendedMemorySpace(
                        deviceIdentifier, 
                        extendedMemoryLengthForEachDevice
                    );

                    /* was memory space reservation successful ? */
                    if(success){
                        Serial.println("DeviceManager//:Additional reboot needed to prepare ExtMemory blocks");
                        /* it means that we need one more reboot to have extended memory space prepared */
                        isOneMoreRebootNeeded = true;
                    }

                }
            }

        }

        if(isOneMoreRebootNeeded){
            std::any_cast<std::function<void(uint16_t)>>
                (DataContainer::getSignalValue(CBK_RESET_DEVICE))(100);
        }
    }


    for(auto device : devices){
        device->init(); // to jest init() danego typu device np. onoffDevice
    }
    
    /*TESTCODE*/
    /* Link service API functions to DeviceManager function calls */
    DeviceServicesAPI servicesFunctionSet = {
        .serviceCall_NoParams = 
            [](uint8_t deviceId, DeviceServicesType request){ 
                return DeviceManager::service(deviceId, request);
            },
        .serviceCall_set1 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set1 params){
                return DeviceManager::service(deviceId, request, params);
            },
        .serviceCall_set2 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set2 params){
                return DeviceManager::service(deviceId, request, params);
            },
        .serviceCall_set3 = 
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set3 params){
                return DeviceManager::service(deviceId, request, params);
            }
    };

    /* Push prepared service API to DataContainer */
    DataContainer::setSignalValue(
        SIG_LOCAL_DEVICE_SERVICES,
        static_cast<DeviceServicesAPI>(servicesFunctionSet));
    /*TESTCODE*/

    DeviceConfigManipulationAPI cfgControls = {
        .setDeviceCfgViaJson = DeviceManager::setLocalSetupViaJson,
        .getDeviceCfgJson = DeviceManager::getLocalSetupJson,
        .loadConfigFromFile = DeviceManager::loadConfigFromFile
    };

    DataContainer::setSignalValue(
        SIG_SET_DEVICES_CONFIG_VIA_JSON,
        static_cast<DeviceConfigManipulationAPI>(cfgControls));
   

    updateDeviceDescriptionSignal();


    /*TESTCODE*/


    for(auto device : devices){
        DeviceDescription desc = device->getDeviceDescription();
        uint8_t* memory = (uint8_t*)malloc(desc.getSize());
        if(memory != nullptr){
            desc.toByteArray(memory, desc.getSize());
            desc.print();
            Serial.println("Serialized DeviceDescription:");
            for(uint16_t i = 0 ; i < desc.getSize(); i++){
                Serial.print(String((int)memory[i]));
            }
            Serial.println();


            DeviceDescription desc2;
            desc2.fromByteArray(memory, desc.getSize());
            desc2.print();
        }

        free(memory);
    }
    /*TESTCODE*/


    Serial.println("... done");
}

void DeviceManager::cyclic()
{
    static long lastInternalDescriptionUpdateTime = 0; 

    for(auto device : devices) 
    {
        device->cyclic();
        
    }

    if(millis() - lastInternalDescriptionUpdateTime > 5000){ /* 30sec */
        updateDeviceDescriptionSignal();
        lastInternalDescriptionUpdateTime = millis();
    }

}



void DeviceManager::updateDeviceDescriptionSignal() {
    std::vector<DeviceDescription> Descriptions;

    for(Device* device : devices) {
        
        Descriptions.push_back(device->getDeviceDescription());

    }

    DataContainer::setSignalValue(SIG_LOCAL_COLLECTION, Descriptions);
}


bool DeviceManager::extractDeviceInstanceBasedOnNvmData(DeviceConfigSlotType& nvmData, uint8_t configSlotID)
{
    bool isValidDeviceGiven = false;

    /* isn't nvm block active ? */
    if(nvmData.isActive)
    {
        /* Is known and valid device type inside?*/
        if(nvmData.isValid())
        {
            
            switch(nvmData.deviceType)
            {
                case e_ON_OFF_DEVICE :
                    vecOnOffDevices.push_back(OnOffDevice(
                        nvmData.pinNumber,          /* Pin number */
                        String(nvmData.deviceName), /* Device name */
                        nvmData.deviceId,           /* Device unique identifier */
                        nvmData.roomId              /* Room unique identifier */
                    ));

                    if(nvmData.customBytes[0] == 1)
                    {
                        vecOnOffDevices.back().setBrightnessLevelSupport(true);
                    }else 
                    {
                        vecOnOffDevices.back().setBrightnessLevelSupport(false); 
                    }

                    isValidDeviceGiven = true;
                break;

                case e_LED_STRIP :
                    /* create WS2812b instance by forwarding NVM data to it */
                    ledws2812bDevices.push_back(LedWS1228bDeviceType(nvmData));
                    isValidDeviceGiven = true;
                break;

                case e_TEMP_SENSOR:
                    tempSensorsDevices.push_back(TempSensorDHT11DeviceType(nvmData));
                    isValidDeviceGiven = true;
                break;

                default:break;
            }
                /* TODO more NVM Data to be extracted here ! */

        
            /* Save valid ConfigSlot configuration to relevant config slot */
            if(configSlotID >= 0 && configSlotID < pinConfigSlotsRamMirror.slots.size())
            {
                /* Save retrieved NVM config to ram mirror */
                pinConfigSlotsRamMirror.slots.at(configSlotID) = nvmData;
            }else
            { /* Invalid number of config slot passed, e.g. to many NVM data in comparison to number of slots */ 
                Serial.println("Invalid config slot ID given: " + String((int)configSlotID));
                std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, String )>>(
                DataContainer::getSignalValue(CBK_ERROR_REPORT)
                )(ERR_MON_INVALID_LOCAL_CONFIG, String("Invalid config slot ID: " + String((int)configSlotID)));
            }
        }
        else {
            NodeConfiguration currentConfig = 
                std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));

            /* Handle errors only when device is properly configured */
            if(currentConfig.networkSSID[0] != '\0'){
                Serial.println("Invalid Device type for config slot : " + String((int)configSlotID)); 
                std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, String)>>(
                DataContainer::getSignalValue(CBK_ERROR_REPORT)
                )(
                    ERR_MON_INVALID_LOCAL_CONFIG, 
                    String("Invalid Device type ("+ String((int)nvmData.deviceType)+") on slot " + String((int)configSlotID))
                );
            }
        }
    }
    // else 
    // { 

    //     Serial.println("Invalid NVM data for config slot : " + String((int)configSlotID));
    //     std::any_cast<std::function<void(ERR_MON_ERROR_TYPE errorCode, uint16_t extendedData)>>(
    //         DataContainer::getSignalValue(CBK_ERROR_REPORT)
    //         )(ERR_MON_INVALID_LOCAL_CONFIG, configSlotID);
    // }


    return isValidDeviceGiven;
}

String DeviceManager::getLocalSetupJson(){
    String nodeCfgJson = "\"PinConfig\":{";

    // bool isActive = false;            /* 1 byte */
    // char deviceName[25] = {'\0'};   /* 25 bytes */
    // uint8_t deviceType = 255;       /* 1 byte */
    // uint8_t pinNumber = 255;        /* 1 byte */
    // uint8_t deviceId = 255;         /* 1 byte */
    // uint8_t roomId = 255;           /* 1 byte */
    // uint8_t customBytes[20] = {0x00};          /* 20 bytes */

    int slotId = 1;
    for(auto& slot: pinConfigSlotsRamMirror.slots){
        String isActive = slot.isActive == true ? "true" : "false";
        String deviceName = String(slot.deviceName);
        String deviceType = String((int)slot.deviceType);
        String pinNumber = String((int)slot.pinNumber);
        String deviceId = String((int)slot.deviceId);
        String roomId = String((int)slot.roomId);


        
        nodeCfgJson += "\"slot"+String((int)slotId)+"\":{";

        nodeCfgJson += "\"isActive\":"+isActive+",";
        nodeCfgJson += "\"deviceName\":\""+deviceName+"\",";
        nodeCfgJson += "\"deviceType\":"+deviceType+",";
        nodeCfgJson += "\"pinNumber\":"+pinNumber+",";
        nodeCfgJson += "\"deviceId\":"+deviceId+",";
        nodeCfgJson += "\"roomId\":"+roomId+",";


        for(uint8_t i = 0 ; i < 20; i++){
            nodeCfgJson += "\"byte"+String((int)i)+"\":"+slot.customBytes[i]+",";
        }

        nodeCfgJson += "}},";
        nodeCfgJson.replace(",}", "");
        
        slotId++;
    }
    nodeCfgJson += "}}";
    nodeCfgJson.replace(",}", "");

    return nodeCfgJson;
}

bool isNull(const String& str){
    return (str == "null");
}

bool DeviceManager::loadConfigFromFile(JsonDocument& doc){
        
    ConfigSlotsDataType receivedConfigurationSet;

    for(int i = 1; i <= 6; i++){
        DeviceConfigSlotType& configSlot = receivedConfigurationSet.slots.at(i-1);

        String slotNumber = String((int)i);

        String isActive       = String(doc["PinConfig"]["slot"+slotNumber]["isActive"]);
        String deviceName       = String(doc["PinConfig"]["slot"+slotNumber]["deviceName"]);
        String deviceType       = String(doc["PinConfig"]["slot"+slotNumber]["deviceType"]);
        String pinNumber       = String(doc["PinConfig"]["slot"+slotNumber]["pinNumber"]);
        String deviceId       = String(doc["PinConfig"]["slot"+slotNumber]["deviceId"]);
        String roomId       = String(doc["PinConfig"]["slot"+slotNumber]["roomId"]);

        String byteValues[20];
        for(uint8_t j = 0 ; j < 20; j++){
            byteValues[j] = String(doc["PinConfig"]["slot"+slotNumber]["byte"+String((int)j)]);
        }


        /* none of the string values can be corrupted to assume whole config slot as correct */
        bool isCorrupted = false;
        isCorrupted |= isNull(isActive);
        isCorrupted |= isNull(deviceName);
        isCorrupted |= isNull(deviceType);
        isCorrupted |= isNull(pinNumber);
        isCorrupted |= isNull(deviceId);
        isCorrupted |= isNull(roomId);

        for(uint8_t j = 0 ; j < 20; j++){
            isCorrupted |= isNull(byteValues[j]);
        }

        if(isCorrupted){
            Serial.println("Missing configuration for slot ID: " + slotNumber);
            continue;
        }

        configSlot.deviceType = deviceType.toInt();
        configSlot.isActive = (isActive == "true" ? 1 : 0);
        configSlot.deviceId = deviceId.toInt();
        if(deviceName.length() < 25){
            memcpy(configSlot.deviceName, deviceName.c_str(), deviceName.length());
        }
        configSlot.pinNumber = pinNumber.toInt();
        configSlot.roomId = roomId.toInt();

        for(uint8_t j = 0 ; j < 20; j++){
            configSlot.customBytes[j] = byteValues[j].toInt();
        }

        // configSlot.print();

    }

    pinConfigSlotsRamMirror = receivedConfigurationSet;

    /* no failure */
    return false;
}

bool DeviceManager::setLocalSetupViaJson(String& json)
{
    json.replace("%7B", "{");
    json.replace("%22", "\"");
    json.replace("%7D", "}");
    json.replace("%20", " ");
    json.replace("/lclSetupJson&", "");

    Serial.println(json);

    JsonDocument doc;
    deserializeJson(doc, json.c_str());

    /* we will extract received JSON configuration to this instance, if is valid*/
    ConfigSlotsDataType receivedConfigurationSet;

    Serial.println("DeviceManager:// Following configuration slots found in JSON:");
    /* Process JSON to extrac each device slot*/
    for(uint16_t i = 0; i < 6; i++)
    {
        /*this exist for every slot*/
        String type =            String(doc["devices"][i]["type"]);
        int id =                        doc["devices"][i]["id"];
        bool isEnabled =                doc["devices"][i]["enabled"];

        /*Validate identifier*/
        if(id == (i+1)){
            DeviceConfigSlotType& configSlot = receivedConfigurationSet.slots.at(i);

            if(type == "OnOff"){
                /* extract remaining OnOff data */
                String name =              String(doc["devices"][i]["name"]);
                String pin =               String(doc["devices"][i]["pin"]);
                String room =              String(doc["devices"][i]["room"]);
                String brightnessSupport = String(doc["devices"][i]["briSup"]);
                String activationState =   String(doc["devices"][i]["activeState"]);
                
                /* Put data to config slot memory*/
                configSlot.deviceType = (uint8_t)type_ONOFFDEVICE;
                configSlot.isActive = isEnabled;
                configSlot.deviceId = id;
                if(name.length() < 25){
                    memcpy(configSlot.deviceName, name.c_str(), name.length());
                }
                configSlot.pinNumber = pin.toInt();
                configSlot.roomId = room.toInt();
                configSlot.customBytes[0] = brightnessSupport.toInt();
                configSlot.customBytes[1] = activationState.toInt();
                


            }else if(type == "LedStrip") {

                /* extract remaining OnOff data */
                String name =              String(doc["devices"][i]["name"]);
                String pin =               String(doc["devices"][i]["pin"]);
                String room =              String(doc["devices"][i]["room"]);
                String numberOfLeds =      String(doc["devices"][i]["ledCount"]);
                String sideFlp =      String(doc["devices"][i]["sideFlp"]);

                /* Put data to config slot memory*/
                configSlot.deviceType = (uint8_t)type_LED_STRIP;
                configSlot.isActive = isEnabled;
                configSlot.deviceId = id;
                if(name.length() < 25){
                    memcpy(configSlot.deviceName, name.c_str(), name.length());
                }
                configSlot.pinNumber = pin.toInt();
                configSlot.roomId = room.toInt();
                configSlot.customBytes[0] = numberOfLeds.toInt();
                configSlot.customBytes[1] = sideFlp.toInt();

            }else if(type == "TempSensor"){

                /* extract remaining OnOff data */
                String name =              String(doc["devices"][i]["name"]);
                String pin =               String(doc["devices"][i]["pin"]);
                String room =              String(doc["devices"][i]["room"]);

                /* Put data to config slot memory*/
                configSlot.deviceType = (uint8_t)type_TEMP_SENSOR;
                configSlot.isActive = isEnabled;
                configSlot.deviceId = id;
                if(name.length() < 25){
                    memcpy(configSlot.deviceName, name.c_str(), name.length());
                }
                configSlot.pinNumber = pin.toInt();
                configSlot.roomId = room.toInt();

            }

            configSlot.print();
        }

    }

    pinConfigSlotsRamMirror = receivedConfigurationSet;

    Serial.println("New config JSON received, reboot ...");

    std::any_cast<std::function<void(uint16_t)>>
        (DataContainer::getSignalValue(CBK_RESET_DEVICE))(2000);


    //{"devices":[{"type":"OnOff","id":1,"enabled":true,"name":"name","pin":"1","room":"2","briSup":"1"}]}

    return true;

}


/* TESTCODE */
ServiceRequestErrorCode DeviceManager::service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
){
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;
    /* Go through the devices list */
    for(auto& device : devices)
    {
        /* Device with requested identifier found */
        if(device->getDeviceIdentifier() == deviceId)
        {
            /* run the service and return execution code */
            
            retVal = device->service(serviceType); // na device uruchamiamy service type który nam przyszedł
            if(retVal == SERV_SUCCESS) {
                updateDeviceDescriptionSignal();
            };
        }
    }


    // std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, uint16_t)>>(
    //     DataContainer::getSignalValue(CBK_ERROR_REPORT))(
    //         ERR_MON_WRONG_DEVICE_ID_FOR_LOCAL_SERVICE_REQUEST,
    //         deviceId
    //     );
    /* Device with requested ID not found, return general failure */
    return retVal;  
}

ServiceRequestErrorCode DeviceManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set1 param
){
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;
    /* Go through the devices list */
    for(auto& device : devices)
    {
        /* Device with requested identifier found */
        if(device->getDeviceIdentifier() == deviceId)
        {
            /* run the service and return execution code */
            
            retVal = device->service(serviceType, param);
            if(retVal == SERV_SUCCESS) {
                updateDeviceDescriptionSignal();
            };
        }
    }


    // std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, uint16_t)>>(
    //     DataContainer::getSignalValue(CBK_ERROR_REPORT))(
    //         ERR_MON_WRONG_DEVICE_ID_FOR_LOCAL_SERVICE_REQUEST,
    //         deviceId
    //     );
    /* Device with requested ID not found, return general failure */
    return retVal;  
}

ServiceRequestErrorCode DeviceManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set2 param
){
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;
    /* Go through the devices list */
    for(auto& device : devices)
    {
        /* Device with requested identifier found */
        if(device->getDeviceIdentifier() == deviceId)
        {
            /* run the service and return execution code */
            
            retVal = device->service(serviceType, param);
            if(retVal == SERV_SUCCESS) {
                updateDeviceDescriptionSignal();
            };
        }
    }


    // std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, uint16_t)>>(
    //     DataContainer::getSignalValue(CBK_ERROR_REPORT))(
    //         ERR_MON_WRONG_DEVICE_ID_FOR_LOCAL_SERVICE_REQUEST,
    //         deviceId
    //     );
    /* Device with requested ID not found, return general failure */
    return retVal;  
}

ServiceRequestErrorCode DeviceManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set3 param
){
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;
    /* Go through the devices list */
    for(auto& device : devices)
    {
        /* Device with requested identifier found */
        if(device->getDeviceIdentifier() == deviceId)
        {
            /* run the service and return execution code */
            
            retVal = device->service(serviceType, param);
            if(retVal == SERV_SUCCESS) {
                updateDeviceDescriptionSignal();
            };
        }
    }


    // std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, uint16_t)>>(
    //     DataContainer::getSignalValue(CBK_ERROR_REPORT))(
    //         ERR_MON_WRONG_DEVICE_ID_FOR_LOCAL_SERVICE_REQUEST,
    //         deviceId
    //     );
    /* Device with requested ID not found, return general failure */
    return retVal;  
}

/* TESTCODE */

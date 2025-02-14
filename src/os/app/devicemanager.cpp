#include <os/app/DeviceManager.hpp>
#include <os/datacontainer/DataContainer.hpp>



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
            std::any_cast<std::function<void()>>
                (DataContainer::getSignalValue(CBK_RESET_DEVICE))();
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

    DataContainer::setSignalValue(
        CBK_SET_DEVICES_CONFIG_VIA_STRING,
        static_cast<std::function<void(String&)>>(DeviceManager::setLocalConfigViaString));
   

    updateDeviceDescriptionSignal();


    /*TESTCODE*/
    /* Add pointer to all existing devices to the device list */
    // devices.push_back(&testDev);
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
                break;

                case e_TEMP_SENSOR:
                    tempSensorsDevices.push_back(TempSensorDHT11DeviceType(nvmData));
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


void DeviceManager::setLocalConfigViaString(String& config)
{
    SecurityAccessLevelType currentAccessLevel = 
        std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));

    /* feature only available in SERVICE MODE */
    if(currentAccessLevel >= e_ACCESS_LEVEL_SERVICE_MODE){
        const String part1 = "localSetup";
        const String part5 = "";
        const uint8_t numberOfDevicesExpected = 6;
        bool isValidConfigReceived = false;
        String devicesConfigStrings[numberOfDevicesExpected];

        uint16_t charIndex = 0;
        String configExtracted =  config.substring(part1.length());
        uint8_t numberOfBytesForDataLength = String(configExtracted.charAt(0)).toInt();
        uint8_t dataLength = String(configExtracted.substring(1, numberOfBytesForDataLength+1)).toInt();

        //Serial.println(String((int)numberOfBytesForDataLength) + " , " + String((int)dataLength));
        String deviceConfigOnlyStr = "";

        deviceConfigOnlyStr = configExtracted.substring(1+numberOfBytesForDataLength, dataLength + (1+numberOfBytesForDataLength));

        //Serial.println("Device only:" + deviceConfigOnlyStr);
        //Serial.println("Extracted : " + configExtracted);

        charIndex = 0;
        for(int i = 0; i < numberOfDevicesExpected; i ++)
        {
            uint8_t numberOfBytesForSingleConfigLength = String(deviceConfigOnlyStr.charAt(charIndex)).toInt();
            uint8_t singleConfigLength = String(deviceConfigOnlyStr.substring(charIndex+1, (charIndex+1 + numberOfBytesForSingleConfigLength))).toInt();
            /* Copy single device config string to string array */
            devicesConfigStrings[i] = String(deviceConfigOnlyStr.substring(
                (charIndex + 1 + numberOfBytesForSingleConfigLength), // [CONFLENGTH] [LENGTH ...] [CONFIG ...]
                (charIndex + 1 + numberOfBytesForSingleConfigLength + singleConfigLength))
                );

            // Serial.println("Single Length : " + String((int)numberOfBytesForSingleConfigLength) + " , lenght: " + String((int)singleConfigLength));
        
            /*Move char index by this particular length + 1 (length count byte)*/
            charIndex += singleConfigLength + 1 + numberOfBytesForSingleConfigLength;
        }

        uint8_t crcLength = String(configExtracted.charAt(charIndex + 1 + numberOfBytesForDataLength)).toInt();

        // Serial.println("CRC length: " + String((int)crcLength));
        charIndex++;
        uint16_t crc = String(
            configExtracted.substring(
                charIndex + 1 + numberOfBytesForDataLength,
                (charIndex + 1 + numberOfBytesForDataLength + crcLength))
                ).toInt();
        // Serial.println("CRC : " + String((int)crc));

        uint16_t localCrc = configCrcCalculation(
            (uint8_t*)configExtracted.c_str(), 
            dataLength + 1 + numberOfBytesForDataLength);
        // Serial.println("Local Crc : " + String((int)localCrc));

        /* Proceed with data analysis STARTs here */
        /* Based on known algorithm, check if received CRC is equal to locally calculated */
        if(crc == localCrc) {
            /* further data extraction possible */

            /* This temp config set needs to be used in order to validate dependencies
            between separate slots and to apply or reject the whole configuration*/
            ConfigSlotsDataType temporaryConfigurationSet;

            Serial.println(deviceConfigOnlyStr);
            bool atLeastOneValid = false;
            for(int i = 0 ; i < 6 ; i++)
            {
                //Serial.println(devicesConfigStrings[i]);
                DeviceConfigSlotType slotData = extractDeviceConfigFromString(devicesConfigStrings[i]);
                if(slotData.isValid())
                {
                    //Serial.println("Config valid!");
                    atLeastOneValid = true;
                    //pinConfigSlotsRamMirror.slots[i] = slotData;
                    temporaryConfigurationSet.slots[i] = slotData;
                }
            }

            if(atLeastOneValid){
                if(validateConfigurationData(temporaryConfigurationSet))
                {
                    /* Temporary config validated correctly, settings can be applied */

                    pinConfigSlotsRamMirror = temporaryConfigurationSet;
                    isValidConfigReceived = true;

                    /* Publish retrieved DeviceConfigSlots signal to the system */
                    DataContainer::setSignalValue(SIG_CONFIG_SLOTS, pinConfigSlotsRamMirror);


                    Serial.println("New config found, reboot ...");


                    Serial.println("Applying new config !!!!");
                    for(auto& element : pinConfigSlotsRamMirror.slots)
                    {
                        element.print();
                    }

                    std::any_cast<std::function<void()>>
                        (DataContainer::getSignalValue(CBK_RESET_DEVICE))();
                }else 
                {
                    Serial.println("Invalid configuration set detected, ignoring.");
                }
            }

        }else 
        {
            /* invalid CRC, reject the request */
            Serial.println("Invalid CRC received for local config: " + String((int)crc) + " != " + String((int)localCrc));
        }


        if(!isValidConfigReceived)
        {
            std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, String)>>(
                DataContainer::getSignalValue(CBK_ERROR_REPORT)
                )(
                    ERR_MON_WRONG_LOCAL_DEVICES_CONFIG_RECEIVED,
                    "Invalid configuration data loaded"
                );
        }
    }else 
    {
        /* no access level to apply */
    }
}


uint16_t DeviceManager::configCrcCalculation(uint8_t* data, uint16_t size)
{
    uint16_t crc = 0;
    // Serial.println("Crc calculation");
    for(uint i = 0; i < size; i ++)
    {
        // Serial.print((char)data[i]);
        crc += (char)data[i];
    }
    // Serial.println();
    return crc;
}

DeviceConfigSlotType DeviceManager::extractDeviceConfigFromString(String& confStr)
{
    DeviceConfigSlotType newConfig;
    newConfig.isActive = false;

    if(String(confStr.charAt(0)).toInt() == 1){
        newConfig.isActive = true;
    }
    /* move config by 1 */
    confStr = confStr.substring(1);

    //if(newConfig.isActive){
        /* Parse device ID */
        uint8_t deviceId = confStr.substring(0, 2).toInt();
        newConfig.deviceId = deviceId;
        confStr = confStr.substring(2);

        /* Parse name length */
        uint8_t nameLength = confStr.substring(0, 2).toInt();
        confStr = confStr.substring(2);

        /* Parse name */
        String deviceName = confStr.substring(0, nameLength);
        memcpy(newConfig.deviceName, deviceName.c_str(), deviceName.length());
        confStr = confStr.substring(nameLength);

        /* Parse type */
        uint8_t deviceType = confStr.substring(0, 2).toInt();
        newConfig.deviceType = deviceType;
        // Serial.println();
        // Serial.println("DUPADUPA: " + String((int)deviceType));
        // Serial.println();
        confStr = confStr.substring(2);

        /* Parse PIN */
        uint8_t devicePin = confStr.substring(0, 2).toInt();
        newConfig.pinNumber = devicePin;
        confStr = confStr.substring(2);

        /* Parse Room */
        uint8_t deviceRoom = confStr.substring(0, 2).toInt();
        newConfig.roomId = deviceRoom;
        confStr = confStr.substring(2);


        /* Parse Extra Data */
        uint8_t extraData = confStr.substring(0, 2).toInt();
        newConfig.customBytes[0] = extraData;

        //Serial.println(String(deviceName));
        newConfig.print();
    //}

    return newConfig;
}

bool DeviceManager::validateConfigurationData(ConfigSlotsDataType& data)
{
    /* All dependencies in config need to be analyzed and verified before applying */
    const uint8_t numberOfPinsAllowed = 12;
    const uint8_t pinsAllowed[numberOfPinsAllowed] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,41,42}; 
    uint8_t pinsUsageCount[numberOfPinsAllowed] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0};

    bool validationSuccess = false;
    uint8_t errorFlag = 0;
    ExtendedMemoryCtrlAPI extMemoryFunctions = 
        std::any_cast<ExtendedMemoryCtrlAPI>(DataContainer::getSignalValue(SIG_EXT_MEM_CTRL_API));


    for(uint8_t slot = 0; slot < 6; slot++)
    {
        if(data.slots[slot].isActive){
            if(data.slots[slot].deviceName[24] != '\0'){
                /* EOS sign missing at the end of a name */
                Serial.println("Error:EOS sign missing at the end of a name");
                errorFlag |= 1;
            }

            if(data.slots[slot].deviceType != 255)
            {
                if(data.slots[slot].deviceType < e_DEVICE_TYPE_FIRST || 
                data.slots[slot].deviceType > e_DEVICE_TYPE_LAST){
                    /* In case of type different than unknown, type must be between FIRST and LAST types */
                    Serial.println("Error: Wrong device type");
                    errorFlag |= 1;
                }
            }

            bool pinValid = false;
            for(uint8_t i = 0; i < numberOfPinsAllowed; i ++)
            {
                if(data.slots[slot].pinNumber == pinsAllowed[i])
                {
                    pinsUsageCount[i] ++;
                    pinValid = true;
                    break;
                }
            }

            if(!pinValid){
                /* Pin value is out of allowed pins range */
                Serial.println("Error: Pin value is out of allowed pins range ");
                errorFlag |= 1;
            }
        }else {
            /* Release extended memory space for unused slot */
            extMemoryFunctions.releaseExtendedMemorySpace(slot+1);

        }
    }


    if(errorFlag == 0)
    {
        for(uint8_t i = 0 ; i < numberOfPinsAllowed; i++)
        {
            if(pinsUsageCount[i] > 1)
            {
                /* Pin used more than once */
                Serial.println("Error: Pin used more than once ");
                errorFlag |= 1;
                break;
            }
        }
    }
    
    if(errorFlag == 0)
    {
        validationSuccess = true;
    }

    return validationSuccess;
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

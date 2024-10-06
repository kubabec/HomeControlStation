#include <os/app/DeviceManager.hpp>
#include <os/datacontainer/DataContainer.hpp>



std::vector<OnOffDevice> DeviceManager::vecOnOffDevices = { };
ConfigSlotsDataType DeviceManager::pinConfigSlotsRamMirror = {};
    // OnOffDevice(13,"Garaz",0,8),
    // OnOffDevice(10,"WC",1,9),
    // OnOffDevice(11,"Dev",3,10)};

void DeviceManager::deinit() {
    for(uint8_t i = e_BLOCK_DEVICE_1; i < e_BLOCK_DEVICE_5; i++)
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
    /* TESTBLOCK TO BE REMOVED IN THE FUTURE */
    // DeviceConfigSlotType configData;
    // char* devicename = "TestDev";
    // configData.deviceId = 15;
    // memcpy(configData.deviceName, devicename, 7);
    // configData.brightnessSupport = true;
    // configData.deviceType = e_ON_OFF_DEVICE;
    // configData.lastBrightness = 30;
    // configData.pinNumber = 7;
    // configData.roomId = 5;

    // std::any_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(
    //                 DataContainer::getSignalValue(CBK_SET_NVM_DATABLOCK)
    // )(
    //     e_BLOCK_DEVICE_1, // Datablock ID
    //     (uint8_t*)&configData // local memory buffer for datablock data
    //  );
    /* TESTBLOCK TO BE REMOVED IN THE FUTURE */




    /* Protection against PersistentDataBlock size modification without DeviceConfigSlotType update */
    if(PersistentDataBlock::getSize() == DeviceConfigSlotType::getSize() )
    {
        /* Reserve buffer for a single config block */
        DeviceConfigSlotType* configBlock = (DeviceConfigSlotType*) malloc (DeviceConfigSlotType::getSize());

        if(configBlock != nullptr)
        {

            uint8_t numberOfSuccessfullyRetrievedDevices = 0;

            /* For each DEVICE relevant datablock */
            for(uint8_t datablock = e_BLOCK_DEVICE_1; datablock < e_PERSISTENT_BLOCK_LAST; datablock ++){
                /* Fill memory area with default 0xFF values */
                memset(configBlock, 0xFF, DeviceConfigSlotType::getSize());

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

            /* Publish retrieved DeviceConfigSlots signal to the system */
            DataContainer::setSignalValue(SIG_CONFIG_SLOTS, "DeviceManager", pinConfigSlotsRamMirror);
        }
    }else
    {
        Serial.println("DeviceConfigSlotType size does not match length of PersistentDataBlock");
    }




    // Serial.println("Local devices configuration start...");

    for(OnOffDevice& device : vecOnOffDevices) 
    {
        device.init();
        
    }

    // <std::function<bool(uint8_t, bool)> > (DeviceManager::deviceEnable) - adres funkcji deviceEnable
    DataContainer::setSignalValue(CBK_LOCAL_DEVICE_ENABLE,"DeviceManager", static_cast<std::function<bool(uint8_t, bool)> > (DeviceManager::deviceEnable));
    DataContainer::setSignalValue(CBK_LOCAL_DEVICE_BRIGHTNESS_CHANGE,"DeviceManager", static_cast<std::function<bool(uint8_t, uint8_t)> > (DeviceManager::deviceBrightnessChange));
    

    DataContainer::setSignalValue(
        CBK_SET_DEVICES_CONFIG_VIA_STRING,
        "DeviceManager", 
        static_cast<std::function<void(String&)>>(DeviceManager::setLocalConfigViaString));
   

    updateDeviceDescriptionSignal();
}

void DeviceManager::cyclic()
{
    for(OnOffDevice& device : vecOnOffDevices) 
    {
        device.cyclic();
        
    }
}

bool DeviceManager::deviceEnable(uint8_t deviceId, bool state) {
     Serial.println("->Device Manager-deviceEnable Device ID: " +  String(deviceId) + " State: " + String(state));
     Serial.println("");
    
    for(OnOffDevice& device : vecOnOffDevices) {
        if(deviceId == device.getDeviceId()) {
            if(device.getState() == false ) {
                device.on();
                updateDeviceDescriptionSignal();
                return true;
            } else {
                device.off();
                updateDeviceDescriptionSignal();
                return false;
            }

        }
    }
    
    return state;
}

void DeviceManager::updateDeviceDescriptionSignal() {
    std::vector<OnOffDeviceDescription> vecOnOffDescription;
    //Serial.println("=Stwozenie lokalnego wektora devDescription i wyslanie do vecOnOffDevices w Data Container=");

    for(OnOffDevice& device : vecOnOffDevices) {
        OnOffDeviceDescription devDescription;        
        devDescription.deviceId = device.getDeviceId();
        //Serial.print("Device ID: " + String(device.getDeviceId()));
        devDescription.deviceName = device.getName();
        //Serial.print(", Name: " + device.getName());
        
        devDescription.currentBrightness = device.getBrightnessLevelTarget();
        //Serial.print(", Brightnes Level: " + String(device.getBrightnessLevel()));
        devDescription.brightnessIsAdjustable = device.getBrightnessIsAdjustable();
        //Serial.print(", BrightnessIsAdjustable: " + String(device.getBrightnessIsAdjustable()));
        devDescription.isEnabled = device.getState();
        //Serial.println(", State: " + String(device.getState()));        
        
        vecOnOffDescription.push_back(devDescription);              
    }
        
    DataContainer::setSignalValue(SIG_LOCAL_COLLECTION_ONOFF,"DeviceManager", vecOnOffDescription);
    //Serial.println("===DeviceManager - Ustawienie sygnału w Data Container======");   
}

bool DeviceManager::deviceBrightnessChange(uint8_t deviceId, uint8_t brightnessLevel)
{
    //Serial.println("DeviceManager deviceId: " + String(deviceId) + "brightnessLevel: " + String(brightnessLevel));
    for(OnOffDevice& device : vecOnOffDevices) {
        if(deviceId == device.getDeviceId()) {
            device.changeBrightness(brightnessLevel);
            updateDeviceDescriptionSignal();
            return true;
        }
    }
    
    return false;
}


bool DeviceManager::extractDeviceInstanceBasedOnNvmData(DeviceConfigSlotType& nvmData, uint8_t configSlotID)
{
    bool isValidDeviceGiven = false;

    /* isn't nvm block empty ? */
    if(nvmData.isValid())
    {
        /* Is known and valid device type inside?*/
        if(nvmData.deviceType >= e_DEVICE_TYPE_FIRST && nvmData.deviceType <= e_DEVICE_TYPE_LAST)
        {
            if(nvmData.isActive){
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
                        /*TBD*/
                    break;

                    default:break;
                }

                /* TODO more NVM Data to be extracted here ! */

            }else { /* Ignore inactive slot whenever creating new device instance */}

            /* Save valid ConfigSlot configuration to relevant config slot */
            if(configSlotID >= 0 && configSlotID < pinConfigSlotsRamMirror.slots.size())
            {
                /* Save retrieved NVM config to ram mirror */
                pinConfigSlotsRamMirror.slots.at(configSlotID) = nvmData;



            }else
            { /* Invalid number of config slot passed, e.g. to many NVM data in comparison to number of slots */ 
                Serial.println("Invalid config slot ID given: " + String((int)configSlotID));
            }
        }else {Serial.println("Invalid Device type for config slot : " + String((int)configSlotID)); }
    }else { Serial.println("Invalid NVM data for config slot : " + String((int)configSlotID));}

    if(!isValidDeviceGiven){
        std::any_cast<std::function<void(ERR_MON_ERROR_TYPE errorCode, uint16_t extendedData)>>(
            DataContainer::getSignalValue(CBK_ERROR_REPORT)
            )(ERR_MON_INVALID_LOCAL_CONFIG, configSlotID);
    }

    return isValidDeviceGiven;
}


void DeviceManager::setLocalConfigViaString(String& config)
{
    const String part1 = "GET /localSetup";
    const String part5 = " HTTP/1.1";
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
                DataContainer::setSignalValue(SIG_CONFIG_SLOTS, "DeviceManager", pinConfigSlotsRamMirror);


                Serial.println("New config found, reboot ...");
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
        std::any_cast<std::function<void(ERR_MON_ERROR_TYPE errorCode, uint16_t extendedData)>>(
            DataContainer::getSignalValue(CBK_ERROR_REPORT)
            )(ERR_MON_WRONG_LOCAL_DEVICES_CONFIG_RECEIVED, localCrc);
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
    const uint8_t numberOfPinsAllowed = 10;
    const uint8_t pinsAllowed[numberOfPinsAllowed] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
    uint8_t pinsUsageCount[numberOfPinsAllowed] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool validationSuccess = false;
    uint8_t errorFlag = 0;

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

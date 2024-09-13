#include <os/app/DeviceManager.hpp>
#include <os/datacontainer/DataContainer.hpp>



std::vector<OnOffDevice> DeviceManager::vecOnOffDevices = { };
ConfigSlotsDataType DeviceManager::pinConfigSlotsRamMirror = {};
    // OnOffDevice(13,"Garaz",0,8),
    // OnOffDevice(10,"WC",1,9),
    // OnOffDevice(11,"Dev",3,10)};

void DeviceManager::init()
{
    /* TESTBLOCK TO BE REMOVED IN THE FUTURE */
    // OnOffConfigDatablock configData;
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




    /* Protection against PersistentDataBlock size modification without OnOffConfigDatablock update */
    if(PersistentDataBlock::getSize() == OnOffConfigDatablock::getSize() )
    {
        /* Reserve buffer for a single config block */
        OnOffConfigDatablock* configBlock = (OnOffConfigDatablock*) malloc (OnOffConfigDatablock::getSize());

        if(configBlock != nullptr)
        {

            uint8_t numberOfSuccessfullyRetrievedDevices = 0;

            /* For each DEVICE relevant datablock */
            for(uint8_t datablock = e_BLOCK_DEVICE_1; datablock < e_PERSISTENT_BLOCK_LAST; datablock ++){
                /* Fill memory area with default 0xFF values */
                memset(configBlock, 0xFF, OnOffConfigDatablock::getSize());

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
        Serial.println("OnOffConfigDatablock size does not match length of PersistentDataBlock");
    }




    // Serial.println("Local devices configuration start...");

    for(OnOffDevice& device : vecOnOffDevices) 
    {
        device.init();
        
    }

    // <std::function<bool(uint8_t, bool)> > (DeviceManager::deviceEnable) - adres funkcji deviceEnable
    DataContainer::setSignalValue(CBK_LOCAL_DEVICE_ENABLE,"DeviceManager", static_cast<std::function<bool(uint8_t, bool)> > (DeviceManager::deviceEnable));
    DataContainer::setSignalValue(CBK_LOCAL_DEVICE_BRIGHTNESS_CHANGE,"DeviceManager", static_cast<std::function<bool(uint8_t, uint8_t)> > (DeviceManager::deviceBrightnessChange));
    
   

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


bool DeviceManager::extractDeviceInstanceBasedOnNvmData(OnOffConfigDatablock& nvmData, uint8_t configSlotID)
{
    bool isValidDeviceGiven = false;

    /* isn't nvm block empty ? */
    if(nvmData.isValid())
    {
        /* Is known and valid device type inside?*/
        if(nvmData.deviceType >= e_DEVICE_TYPE_FIRST && nvmData.deviceType <= e_DEVICE_TYPE_LAST)
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

                    isValidDeviceGiven = true;
                break;

                case e_LED_STRIP :
                    /*TBD*/
                break;

                default:break;
            }

            /* Save valid ConfigSlot configuration to relevant config slot */
            if(configSlotID >= 0 && configSlotID < pinConfigSlotsRamMirror.slots.size())
            {
                DeviceConfigSlotType& configSlotRef = pinConfigSlotsRamMirror.slots.at(configSlotID);

                configSlotRef.isEmpty = false;
                configSlotRef.deviceId = nvmData.deviceId;
                configSlotRef.deviceType = nvmData.deviceType;
                configSlotRef.pinNumber = nvmData.pinNumber;
                configSlotRef.roomId = nvmData.roomId;
                memcpy(configSlotRef.deviceName, nvmData.deviceName, 25);

            }else
            { /* Invalid number of config slot passed, e.g. to many NVM data in comparison to number of slots */ }
        }
    }

    return isValidDeviceGiven;
}


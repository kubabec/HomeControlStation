#include <os/app/DeviceManager.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <os/Logger.hpp>
#include <ArduinoJson.h>
#if __has_include("generated/GeneratedDeviceRegistry.hpp")
#include "generated/GeneratedDeviceRegistry.hpp"
#else
#include "devices/fallback/DeviceRegistry.hpp"
#endif

/**
 * @file src/os/app/devicemanager.cpp
 * @brief Device manager: restores devices from NVM, manages lifecycle and service dispatch.
 */

ConfigSlotsDataType DeviceManager::pinConfigSlotsRamMirror = {};
ExtendedDataAllocator DeviceManager::extDataAllocator;
std::vector<std::unique_ptr<Device>> DeviceManager::devices;
std::vector<uint32_t> DeviceManager::lastDeviceCycleTimes;

namespace
{
constexpr size_t CONFIGURABLE_GPIO_COUNT = 49;

bool validateConfigurationSet(const ConfigSlotsDataType& configuration)
{
    bool claimedPins[CONFIGURABLE_GPIO_COUNT] = {};
    for (const auto& slot : configuration.slots)
    {
        if (slot.isActive && !GeneratedDeviceRegistry::validateConfiguration(slot, claimedPins, CONFIGURABLE_GPIO_COUNT))
        {
            return false;
        }
    }
    return true;
}
}

void DeviceManager::deinit()
{
    flushNvmData();
}

void DeviceManager::persistentDataChanged()
{
    /* Trigger NVM save to store devices persistent data safely */
    std::any_cast<std::function<void()>>(DataContainer::getSignalValue(CBK_START_NVM_SAVE_TIMER))();
}

void DeviceManager::flushNvmData()
{
    for (uint8_t i = e_BLOCK_DEVICE_1; i <= e_BLOCK_DEVICE_6; i++)
    {
        /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
        bool success = std::any_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(
            DataContainer::getSignalValue(CBK_SET_NVM_DATABLOCK))(
            (PersistentDatablockID)i,                       // Datablock ID
            (uint8_t *)&pinConfigSlotsRamMirror.slots.at(i) // local memory buffer for datablock data
        );

        if (!success)
        {
            Logger::log("Error during saving " + String((int)i) + " datablock");
        }
    }
}

void DeviceManager::init()
{
    Logger::log("DeviceManager init ...");
    devices.clear();
    lastDeviceCycleTimes.clear();

    /* Protection against PersistentDataBlock size modification without DeviceConfigSlotType update */
    if (PersistentDataBlock::getSize() == DeviceConfigSlotType::getSize())
    {
        /* Reserve buffer for a single config block */
        DeviceConfigSlotType *configBlock = (DeviceConfigSlotType *)malloc(DeviceConfigSlotType::getSize());

        if (configBlock != nullptr)
        {

            uint8_t numberOfSuccessfullyRetrievedDevices = 0;
            bool claimedPins[CONFIGURABLE_GPIO_COUNT] = {};

            /* For each DEVICE relevant datablock */
            for (uint8_t datablock = e_BLOCK_DEVICE_1; datablock <= e_BLOCK_DEVICE_6; datablock++)
            {
                /* Fill memory area with default 0xFF values */
                memset(configBlock, 0x00, DeviceConfigSlotType::getSize());

                /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
                std::any_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(
                    DataContainer::getSignalValue(CBK_GET_NVM_DATABLOCK))(
                    (PersistentDatablockID)datablock, // Datablock ID
                    (uint8_t *)configBlock            // local memory buffer for datablock data
                );

                /* Now NVM data of current datablock shall be inside of 'configBlock' variable */

                /* Try to extract (add to devices vector) device based on NVM data */
                if (extractDeviceInstanceBasedOnNvmData(*configBlock, datablock, claimedPins, CONFIGURABLE_GPIO_COUNT))
                {
                    numberOfSuccessfullyRetrievedDevices++;
                }
            }

            Logger::log(String((int)numberOfSuccessfullyRetrievedDevices) +
                           " devices has been successfully retrieved from NVM memory");

            free(configBlock);

            /* push this notification only when network is available and there are no devices configured */
            if (std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION)).networkCredentialsAvailable && numberOfSuccessfullyRetrievedDevices == 0)
            {
                UserInterfaceNotification notif;
                notif.title = "Missing GPIO setup";
                notif.body = "Looks like there are no GPIO devices configured yet. Navigate to 'Settings -> Devices Management' to add the device.";
                notif.type = UserInterfaceNotification::INFO;
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
            }

            /* Publish retrieved DeviceConfigSlots signal to the system */
            Logger::log("DeviceManager//: Publishin config slots.");
            DataContainer::setSignalValue(SIG_CONFIG_SLOTS, static_cast<ConfigSlotsDataType>(pinConfigSlotsRamMirror));
        }
    }
    else
    {
        Logger::log("DeviceConfigSlotType size does not match length of PersistentDataBlock");
    }

    Logger::log("DeviceManager//: Config slots setup:");
    for (auto &slot : pinConfigSlotsRamMirror.slots)
    {
        slot.print();
    }

    if (devices.size() > 0)
    {
        /* Extended memory allocation and assignment must happen here before init of each device*/
        ExtendedMemoryCtrlAPI extMemoryFunctions = std::any_cast<ExtendedMemoryCtrlAPI>(DataContainer::getSignalValue(SIG_EXT_MEM_CTRL_API));

        bool isOneMoreRebootNeeded = false;
        for (auto &device : devices)
        {
            uint16_t extendedMemoryLengthForEachDevice = device->getExtendedMemoryLength();
            uint8_t deviceIdentifier = device->getDeviceIdentifier();

            /* check if device even need extra memory */
            if (extendedMemoryLengthForEachDevice > 0)
            {
                uint16_t checkSize = 0;
                /* Try to get already existing extended memory for this particular device */
                uint8_t *memoryPtr = extMemoryFunctions.getExtMemoryPtrByDeviceId(
                    deviceIdentifier,
                    &checkSize);

                /* Did we receive valid data pointer, so it means extended memory for this device was already existing? */
                if (checkSize == extendedMemoryLengthForEachDevice && memoryPtr != nullptr)
                {
                    ServiceParameters_set3 extMemParams = {
                        .buff = memoryPtr,
                        .size = checkSize};
                    device->service(DEVSERVICE_SET_EXT_MEMORY_PTR, extMemParams);
                }
                else
                {
                    /* We could not receive valid data from ExtendedMemoryManager if the record for this device
                        does not exist yet, it means device was just coded for the first time before reboot. */

                    /* we need to use other function to reserve extended data for future runtime cycle */
                    bool success = extMemoryFunctions.requestNewExtendedMemorySpace(
                        deviceIdentifier,
                        extendedMemoryLengthForEachDevice);

                    /* was memory space reservation successful ? */
                    if (success)
                    {
                        Logger::log("DeviceManager//:Additional reboot needed to prepare ExtMemory blocks");
                        /* it means that we need one more reboot to have extended memory space prepared */
                        isOneMoreRebootNeeded = true;
                    }
                }
            }
        }

        if (isOneMoreRebootNeeded)
        {
            std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(100);
        }
    }

    for (auto &device : devices)
    {
        device->init(); // to jest init() danego typu device np. onoffDevice
    }

    /*TESTCODE*/
    /* Link service API functions to DeviceManager function calls */
    DeviceServicesAPI servicesFunctionSet = {
        .serviceCall_NoParams =
            [](uint8_t deviceId, DeviceServicesType request)
        {
            return DeviceManager::service(deviceId, request);
        },
        .serviceCall_set1 =
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set1 params)
        {
            return DeviceManager::service(deviceId, request, params);
        },
        .serviceCall_set2 =
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set2 params)
        {
            return DeviceManager::service(deviceId, request, params);
        },
        .serviceCall_set3 =
            [](uint8_t deviceId, DeviceServicesType request, ServiceParameters_set3 params)
        {
            return DeviceManager::service(deviceId, request, params);
        }};

    /* Push prepared service API to DataContainer */
    DataContainer::setSignalValue(
        SIG_LOCAL_DEVICE_SERVICES,
        static_cast<DeviceServicesAPI>(servicesFunctionSet));
    /*TESTCODE*/

    DeviceConfigManipulationAPI cfgControls = {
        .setDeviceCfgViaJson = DeviceManager::setLocalSetupViaJson,
        .getDeviceCfgJson = DeviceManager::getLocalSetupJson,
        .loadConfigFromFile = DeviceManager::loadConfigFromFile};

    DataContainer::setSignalValue(
        SIG_SET_DEVICES_CONFIG_VIA_JSON,
        static_cast<DeviceConfigManipulationAPI>(cfgControls));

    updateDeviceDescriptionSignal();

    /*TESTCODE*/

    // for (auto device : devices)
    // {
    //     DeviceDescription desc = device->getDeviceDescription();
    //     uint8_t *memory = (uint8_t *)malloc(desc.getSize());
    //     if (memory != nullptr)
    //     {
    //         desc.toByteArray(memory, desc.getSize());
    //         desc.print();
    //         Logger::log("Serialized DeviceDescription:");
    //         String out = "";
    //         for (uint16_t i = 0; i < desc.getSize(); i++)
    //         {
    //             out += String((int)memory[i]);
    //         }
    //         Logger::log(out);
    //         DeviceDescription desc2;
    //         desc2.fromByteArray(memory, desc.getSize());
    //         desc2.print();
    //     }

    //     free(memory);
    // }
    /*TESTCODE*/

    Logger::log("... done");
}

void DeviceManager::cyclic()
{
    static uint32_t lastInternalDescriptionUpdateTime = 0;
    const uint32_t now = millis();

    for (size_t index = 0; index < devices.size(); ++index)
    {
        auto &device = devices[index];
        const auto *registration = GeneratedDeviceRegistry::find(device->getDeviceType());
        const uint32_t intervalMs = registration != nullptr ? registration->cycleIntervalMs : 0;
        if (intervalMs == 0 || static_cast<uint32_t>(now - lastDeviceCycleTimes[index]) >= intervalMs)
        {
            lastDeviceCycleTimes[index] = now;
            device->cyclic();
        }
    }

    if (static_cast<uint32_t>(now - lastInternalDescriptionUpdateTime) >= 1000)
    {
        updateDeviceDescriptionSignal();
        lastInternalDescriptionUpdateTime = now;
    }
}

void DeviceManager::updateDeviceDescriptionSignal()
{
    std::vector<DeviceDescription> Descriptions;

    for (const auto &device : devices)
    {
        const auto* registration = GeneratedDeviceRegistry::find(device->getDeviceType());
        if (registration != nullptr && registration->includedInDeviceCollection)
        {
            Descriptions.push_back(device->getDeviceDescription());
        }
    }

    DataContainer::setSignalValue(SIG_LOCAL_COLLECTION, Descriptions);
}

bool DeviceManager::extractDeviceInstanceBasedOnNvmData(
    DeviceConfigSlotType &nvmData,
    uint8_t configSlotID,
    bool* claimedPins,
    size_t claimedPinCount)
{
    bool isValidDeviceGiven = false;

    /* isn't nvm block active ? */
    if (nvmData.isActive)
    {
        /* Is known and valid device type inside?*/
        if (nvmData.isValid() && GeneratedDeviceRegistry::validateConfiguration(nvmData, claimedPins, claimedPinCount))
        {

            GeneratedDeviceRegistry::RuntimeContext context = {
                .persistentDataChanged = DeviceManager::persistentDataChanged,
                .getRtcTime = DeviceManager::getRtcTimeWrapper,
                .toggleLocalDevice = [](uint16_t deviceToggleId)
                {
                    std::vector<DeviceDescription> devicesVector =
                        std::any_cast<std::vector<DeviceDescription>>(
                            DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
                    ServiceParameters_set1 parameters;
                    bool deviceExists = false;
                    for (const auto &device : devicesVector)
                    {
                        if (device.deviceId == deviceToggleId)
                        {
                            parameters.a = !device.isEnabled;
                            deviceExists = true;
                            break;
                        }
                    }
                    if (deviceExists)
                    {
                        ServiceRequestErrorCode errorCode =
                            std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))
                                .serviceCall_set1(deviceToggleId, DEVSERVICE_STATE_SWITCH, parameters);
                        if (errorCode != SERV_SUCCESS)
                        {
                            Logger::log("Error toggling " + String((int)deviceToggleId) + " device via HW Button");
                        }
                    }
                },
                .fireDigitalEvent = [](uint64_t eventId)
                {
                    std::any callback = DataContainer::getSignalValue(CBK_FIRE_DIGITAL_EVENT);
                    if (auto function = std::any_cast<std::function<void(uint64_t)>>(&callback))
                    {
                        (*function)(eventId);
                    }
                },
                .getNodeMacAddress = []()
                {
                    std::any value = DataContainer::getSignalValue(SIG_MAC_ADDRESS);
                    if (auto macAddress = std::any_cast<uint64_t>(&value)) return *macAddress;
                    return uint64_t{0};
                },
                .fireDigitalEventWithSource = [](uint64_t eventId, const String &source)
                {
                    std::any callback = DataContainer::getSignalValue(CBK_FIRE_DIGITAL_EVENT_WITH_SOURCE);
                    if (auto function = std::any_cast<std::function<void(uint64_t, const String &)>>(&callback))
                    {
                        (*function)(eventId, source);
                    }
                }};

            std::unique_ptr<Device> device = GeneratedDeviceRegistry::create(nvmData.deviceType, nvmData, context);
            if (device)
            {
                devices.push_back(std::move(device));
                lastDeviceCycleTimes.push_back(millis());
                isValidDeviceGiven = true;
            }
            /* TODO more NVM Data to be extracted here ! */

            /* Save valid ConfigSlot configuration to relevant config slot */
            if (configSlotID >= 0 && configSlotID < pinConfigSlotsRamMirror.slots.size())
            {
                /* Save retrieved NVM config to ram mirror */
                pinConfigSlotsRamMirror.slots.at(configSlotID) = nvmData;
            }
            else
            { /* Invalid number of config slot passed, e.g. to many NVM data in comparison to number of slots */
                Logger::log("Invalid config slot ID given: " + String((int)configSlotID));
            }
        }
        else
        {
            NodeConfiguration currentConfig =
                std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));

            /* Handle errors only when device is properly configured */
            if (currentConfig.networkSSID[0] != '\0')
            {
                Logger::log("Invalid device type or GPIO configuration for config slot: " + String((int)configSlotID));
            }
        }
    }

    return isValidDeviceGiven;
}

String DeviceManager::getLocalSetupJson()
{
    String nodeCfgJson = "\"PinConfig\":{";

    // bool isActive = false;            /* 1 byte */
    // char deviceName[25] = {'\0'};   /* 25 bytes */
    // uint8_t deviceType = 255;       /* 1 byte */
    // uint8_t pinNumber = 255;        /* 1 byte */
    // uint8_t deviceId = 255;         /* 1 byte */
    // uint8_t roomId = 255;           /* 1 byte */
    // uint8_t customBytes[20] = {0x00};          /* 20 bytes */

    int slotId = 1;
    for (auto &slot : pinConfigSlotsRamMirror.slots)
    {
        String isActive = slot.isActive == true ? "true" : "false";
        String deviceName = String(slot.deviceName);
        String deviceType = String((int)slot.deviceType);
        String pinNumber = String((int)slot.pinNumber);
        String deviceId = String((int)slot.deviceId);
        String roomId = String((int)slot.roomId);

        nodeCfgJson += "\"slot" + String((int)slotId) + "\":{";

        nodeCfgJson += "\"isActive\":" + isActive + ",";
        nodeCfgJson += "\"deviceName\":\"" + deviceName + "\",";
        nodeCfgJson += "\"deviceType\":" + deviceType + ",";
        nodeCfgJson += "\"pinNumber\":" + pinNumber + ",";
        nodeCfgJson += "\"deviceId\":" + deviceId + ",";
        nodeCfgJson += "\"roomId\":" + roomId + ",";

        for (uint8_t i = 0; i < 20; i++)
        {
            nodeCfgJson += "\"byte" + String((int)i) + "\":" + slot.customBytes[i] + ",";
        }

        nodeCfgJson += "}},";
        nodeCfgJson.replace(",}", "");

        slotId++;
    }
    nodeCfgJson += "}}";
    nodeCfgJson.replace(",}", "");

    return nodeCfgJson;
}

bool isNull(const String &str)
{
    return (str == "null");
}

bool DeviceManager::loadConfigFromFile(JsonDocument &doc)
{

    ConfigSlotsDataType receivedConfigurationSet;

    for (int i = 1; i <= 6; i++)
    {
        DeviceConfigSlotType &configSlot = receivedConfigurationSet.slots.at(i - 1);

        String slotNumber = String((int)i);

        String isActive = String(doc["PinConfig"]["slot" + slotNumber]["isActive"]);
        String deviceName = String(doc["PinConfig"]["slot" + slotNumber]["deviceName"]);
        String deviceType = String(doc["PinConfig"]["slot" + slotNumber]["deviceType"]);
        String pinNumber = String(doc["PinConfig"]["slot" + slotNumber]["pinNumber"]);
        String deviceId = String(doc["PinConfig"]["slot" + slotNumber]["deviceId"]);
        String roomId = String(doc["PinConfig"]["slot" + slotNumber]["roomId"]);

        String byteValues[20];
        for (uint8_t j = 0; j < 20; j++)
        {
            byteValues[j] = String(doc["PinConfig"]["slot" + slotNumber]["byte" + String((int)j)]);
        }

        /* none of the string values can be corrupted to assume whole config slot as correct */
        bool isCorrupted = false;
        isCorrupted |= isNull(isActive);
        isCorrupted |= isNull(deviceName);
        isCorrupted |= isNull(deviceType);
        isCorrupted |= isNull(pinNumber);
        isCorrupted |= isNull(deviceId);
        isCorrupted |= isNull(roomId);

        for (uint8_t j = 0; j < 20; j++)
        {
            isCorrupted |= isNull(byteValues[j]);
        }

        if (isCorrupted)
        {
            Logger::log("Missing configuration for slot ID: " + slotNumber);
            continue;
        }

        configSlot.deviceType = deviceType.toInt();
        configSlot.isActive = (isActive == "true" ? 1 : 0);
        configSlot.deviceId = deviceId.toInt();
        if (deviceName.length() < 25)
        {
            memcpy(configSlot.deviceName, deviceName.c_str(), deviceName.length());
        }
        configSlot.pinNumber = pinNumber.toInt();
        configSlot.roomId = roomId.toInt();

        for (uint8_t j = 0; j < 20; j++)
        {
            configSlot.customBytes[j] = byteValues[j].toInt();
        }

        // configSlot.print();
    }

    if (!validateConfigurationSet(receivedConfigurationSet))
    {
        Logger::log("DeviceManager: imported configuration contains invalid or conflicting GPIO assignments");
        return true;
    }

    pinConfigSlotsRamMirror = receivedConfigurationSet;

    /* no failure */
    return false;
}

bool DeviceManager::setLocalSetupViaJson(String &json)
{
    json.replace("%7B", "{");
    json.replace("%22", "\"");
    json.replace("%7D", "}");
    json.replace("%20", " ");
    json.replace("/lclSetupJson&", "");

    Logger::log(json);

    JsonDocument doc;
    DeserializationError jsonError = deserializeJson(doc, json.c_str());
    if (jsonError || !doc["devices"].is<JsonArray>())
    {
        Logger::log("DeviceManager: invalid device configuration JSON");
        return false;
    }

    ConfigSlotsDataType receivedConfigurationSet;
    JsonArray devicesJson = doc["devices"];
    if (devicesJson.size() != receivedConfigurationSet.slots.size())
    {
        Logger::log("DeviceManager: wrong number of configuration slots");
        return false;
    }

    for (uint16_t i = 0; i < devicesJson.size(); ++i)
    {
        JsonObject deviceJson = devicesJson[i];
        if (!deviceJson["id"].is<uint8_t>() || !deviceJson["typeId"].is<uint8_t>() ||
            !deviceJson["enabled"].is<bool>() || !deviceJson["name"].is<const char*>() ||
            !deviceJson["pin"].is<uint8_t>() || !deviceJson["room"].is<uint8_t>() ||
            !deviceJson["customBytes"].is<JsonArray>())
        {
            Logger::log("DeviceManager: invalid configuration value type");
            return false;
        }
        const uint8_t id = deviceJson["id"] | 255;
        const uint8_t typeId = deviceJson["typeId"] | 255;
        if (id != i + 1)
        {
            Logger::log("DeviceManager: invalid slot identifier");
            return false;
        }

        DeviceConfigSlotType &configSlot = receivedConfigurationSet.slots.at(i);
        const auto* registration = GeneratedDeviceRegistry::find(typeId);
        const bool enabled = deviceJson["enabled"].as<bool>();
        if (enabled && registration == nullptr)
        {
            Logger::log("DeviceManager: active slot uses an unknown device type");
            return false;
        }
        configSlot.deviceType = registration ? typeId : 255;
        configSlot.isActive = registration && enabled;
        configSlot.deviceId = id;
        configSlot.pinNumber = deviceJson["pin"] | 255;
        configSlot.roomId = deviceJson["room"] | 255;

        String name = deviceJson["name"] | "";
        if (name.length() >= sizeof(configSlot.deviceName))
        {
            Logger::log("DeviceManager: device name is too long");
            return false;
        }
        name.toCharArray(configSlot.deviceName, sizeof(configSlot.deviceName));

        JsonArray customBytes = deviceJson["customBytes"];
        if (registration && customBytes.size() != sizeof(configSlot.customBytes))
        {
            Logger::log("DeviceManager: invalid custom byte payload");
            return false;
        }
        for (uint8_t byte = 0; byte < customBytes.size() && byte < sizeof(configSlot.customBytes); ++byte)
        {
            if (!customBytes[byte].is<uint8_t>())
            {
                Logger::log("DeviceManager: invalid custom byte value");
                return false;
            }
            configSlot.customBytes[byte] = customBytes[byte].as<uint8_t>();
        }
    }

    if (!validateConfigurationSet(receivedConfigurationSet))
    {
        Logger::log("DeviceManager: invalid or conflicting GPIO configuration");
        return false;
    }

    pinConfigSlotsRamMirror = receivedConfigurationSet;

    Logger::log("New config JSON received, reboot ...");

    std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(1000);

    //{"devices":[{"type":"OnOff","id":1,"enabled":true,"name":"name","pin":"1","room":"2","briSup":"1"}]}

    return true;
}

ServiceRequestErrorCode DeviceManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType)
{
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;
    /* Go through the devices list */
    for (auto &device : devices)
    {
        const auto* registration = GeneratedDeviceRegistry::find(device->getDeviceType());
        if (registration == nullptr || !registration->includedInDeviceCollection) continue;
        /* Device with requested identifier found */
        if (device->getDeviceIdentifier() == deviceId)
        {
            /* run the service and return execution code */

            retVal = device->service(serviceType); // na device uruchamiamy service type który nam przyszedł
            if (retVal == SERV_SUCCESS)
            {
                updateDeviceDescriptionSignal();
            };
        }
    }

    /* Device with requested ID not found, return general failure */
    return retVal;
}

ServiceRequestErrorCode DeviceManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set1 param)
{
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;
    /* Go through the devices list */
    for (auto &device : devices)
    {
        const auto* registration = GeneratedDeviceRegistry::find(device->getDeviceType());
        if (registration == nullptr || !registration->includedInDeviceCollection) continue;
        /* Device with requested identifier found */
        if (device->getDeviceIdentifier() == deviceId)
        {
            /* run the service and return execution code */

            retVal = device->service(serviceType, param);
            if (retVal == SERV_SUCCESS)
            {
                updateDeviceDescriptionSignal();
            };
        }
    }
    /* Device with requested ID not found, return general failure */
    return retVal;
}

ServiceRequestErrorCode DeviceManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set2 param)
{
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;
    /* Go through the devices list */
    for (auto &device : devices)
    {
        const auto* registration = GeneratedDeviceRegistry::find(device->getDeviceType());
        if (registration == nullptr || !registration->includedInDeviceCollection) continue;
        /* Device with requested identifier found */
        if (device->getDeviceIdentifier() == deviceId)
        {
            /* run the service and return execution code */

            retVal = device->service(serviceType, param);
            if (retVal == SERV_SUCCESS)
            {
                updateDeviceDescriptionSignal();
            };
        }
    }

    /* Device with requested ID not found, return general failure */
    return retVal;
}

ServiceRequestErrorCode DeviceManager::service(
    uint8_t deviceId,
    DeviceServicesType serviceType,
    ServiceParameters_set3 param)
{
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;
    /* Go through the devices list */
    for (auto &device : devices)
    {
        const auto* registration = GeneratedDeviceRegistry::find(device->getDeviceType());
        if (registration == nullptr || !registration->includedInDeviceCollection) continue;
        /* Device with requested identifier found */
        if (device->getDeviceIdentifier() == deviceId)
        {
            /* run the service and return execution code */

            retVal = device->service(serviceType, param);
            if (retVal == SERV_SUCCESS)
            {
                updateDeviceDescriptionSignal();
            };
        }
    }

    /* Device with requested ID not found, return general failure */
    return retVal;
}

RtcTime DeviceManager::getRtcTimeWrapper()
{
    /* this function is a wrapper for lower level devices, which do not have access
    DataContainer directly, but they can read RTC value via this function when pointer to it
    will be passed to specific device type as a constructor parameter */
    RtcTime retTimeVal;

    std::any localAny = DataContainer::getSignalValue(CBK_GET_CURRENT_TIME);
    if (auto p = std::any_cast<std::function<RtcTime()>>(&localAny))
    {
        retTimeVal = (*p)();

        /* TODO: map currentTime to device-friendly type and change this function to return this value */
    }
    else
    {
    }

    return retTimeVal;
}

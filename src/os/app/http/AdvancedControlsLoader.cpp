#include <os/app/http/AdvancedControlsLoader.hpp>
#include "generated/GeneratedAdvancedControlTemplates.hpp"
#include "os/Logger.hpp"
#include <SystemDefinition.hpp>

/**
 * @file AdvancedControlsLoader.cpp
 * @brief Generic advanced-controls template and opaque payload transport.
 */

uint8_t* AdvancedControlsLoader::currentAdvancedControls = nullptr;
DeviceDescription AdvancedControlsLoader::currentlyRequestedDeviceDescription = {};
String AdvancedControlsLoader::currentResponse = "";

uint16_t AdvancedControlsLoader::getControlsSizeBasedOnDevType(uint8_t deviceType)
{
    switch (deviceType)
    {
#include "generated/GeneratedAdvancedControlsSize.inc"
    default:
        return 0;
    }
}

uint8_t* AdvancedControlsLoader::allocateMemoryForControlsBasedOnDeviceType(uint8_t deviceType)
{
    if (currentAdvancedControls != nullptr)
    {
        free(currentAdvancedControls);
        currentAdvancedControls = nullptr;
    }

    const uint16_t controlsSize = getControlsSizeBasedOnDevType(deviceType);
    if (controlsSize == 0)
    {
        Logger::log("AdvancedControlsLoader:// No payload registered for type " + String((int)deviceType));
        return nullptr;
    }

    currentAdvancedControls = static_cast<uint8_t*>(malloc(controlsSize));
    if (currentAdvancedControls == nullptr)
    {
        Logger::log("AdvancedControlsLoader:// Unable to allocate " + String((int)controlsSize) + " bytes");
    }
    return currentAdvancedControls;
}

void AdvancedControlsLoader::appendJsonString(String& destination, const char* value)
{
    destination += '"';
    if (value != nullptr)
    {
        while (*value != '\0')
        {
            const char character = *value++;
            switch (character)
            {
            case '"': destination += "\\\""; break;
            case '\\': destination += "\\\\"; break;
            case '\b': destination += "\\b"; break;
            case '\f': destination += "\\f"; break;
            case '\n': destination += "\\n"; break;
            case '\r': destination += "\\r"; break;
            case '\t': destination += "\\t"; break;
            default:
                if (static_cast<uint8_t>(character) < 0x20)
                {
                    destination += "\\u00";
                    const char hex[] = "0123456789ABCDEF";
                    destination += hex[(static_cast<uint8_t>(character) >> 4) & 0x0F];
                    destination += hex[static_cast<uint8_t>(character) & 0x0F];
                }
                else
                {
                    destination += character;
                }
                break;
            }
        }
    }
    destination += '"';
}

void AdvancedControlsLoader::prepareResponse()
{
    const char* pattern = GeneratedAdvancedControlTemplates::find(currentlyRequestedDeviceDescription.deviceType);
    if (pattern == nullptr || currentAdvancedControls == nullptr)
    {
        currentResponse = "{\"error\":\"Advanced controls are not registered for this device type\"}";
        return;
    }

    const uint16_t payloadSize = getControlsSizeBasedOnDevType(currentlyRequestedDeviceDescription.deviceType);
    currentResponse = "{\"deviceId\":" + String((int)currentlyRequestedDeviceDescription.deviceId);
    currentResponse += ",\"deviceType\":" + String((int)currentlyRequestedDeviceDescription.deviceType);
    currentResponse += ",\"deviceName\":";
    appendJsonString(currentResponse, currentlyRequestedDeviceDescription.deviceName.c_str());
    currentResponse += ",\"template\":";
    appendJsonString(currentResponse, pattern);
    currentResponse += ",\"payload\":[";
    for (uint16_t index = 0; index < payloadSize; ++index)
    {
        if (index != 0) currentResponse += ',';
        currentResponse += String((int)currentAdvancedControls[index]);
    }
    currentResponse += "],\"descriptionBytes\":[";
    for (uint16_t index = 0; index < NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION; ++index)
    {
        if (index != 0) currentResponse += ',';
        currentResponse += String((int)currentlyRequestedDeviceDescription.customBytes[index]);
    }
    currentResponse += "]}";
}

String AdvancedControlsLoader::getOutput()
{
    String output = currentResponse;
    currentResponse = "";
    currentlyRequestedDeviceDescription = {};
    if (currentAdvancedControls != nullptr)
    {
        free(currentAdvancedControls);
        currentAdvancedControls = nullptr;
    }
    return output;
}

ServiceRequestErrorCode AdvancedControlsLoader::loadAdvancedControls(uint16_t deviceIdentifier)
{
    if (deviceIdentifier == 255)
    {
        return SERV_GENERAL_FAILURE;
    }

    if (currentlyRequestedDeviceDescription.deviceId != deviceIdentifier)
    {
        currentlyRequestedDeviceDescription = {};
        const auto devicesCollection = std::any_cast<std::vector<DeviceDescription>>(
            DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
        for (const auto& description : devicesCollection)
        {
            if (description.deviceId == deviceIdentifier)
            {
                currentlyRequestedDeviceDescription = description;
                break;
            }
        }

        if (currentlyRequestedDeviceDescription.deviceId != deviceIdentifier)
        {
            Logger::log("AdvancedControlsLoader:// Unable to find device " + String((int)deviceIdentifier));
            return SERV_GENERAL_FAILURE;
        }

        if (GeneratedAdvancedControlTemplates::find(currentlyRequestedDeviceDescription.deviceType) == nullptr ||
            allocateMemoryForControlsBasedOnDeviceType(currentlyRequestedDeviceDescription.deviceType) == nullptr)
        {
            currentResponse = "{\"error\":\"Advanced controls are unavailable for this device\"}";
            return SERV_SUCCESS;
        }
    }

    ServiceParameters_set3 parameters;
    parameters.buff = currentAdvancedControls;
    parameters.size = getControlsSizeBasedOnDevType(currentlyRequestedDeviceDescription.deviceType);
    parameters.additionalParam = 0xFF;
    parameters.direction = static_cast<uint8_t>(e_OUT_from_DEVICE);

    const ServiceRequestErrorCode result =
        std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set3(
            currentlyRequestedDeviceDescription.deviceId,
            DEVSERVICE_GET_ADVANCED_CONTROLS,
            parameters);

    if (result == SERV_SUCCESS)
    {
        prepareResponse();
        return SERV_SUCCESS;
    }
    if (result == SERV_PENDING)
    {
        return SERV_PENDING;
    }

    currentResponse = "{\"error\":\"Device failed to provide advanced-controls data\"}";
    UserInterfaceNotification notification;
    notification.title = "Request processing failure";
    notification.body = "Could not load advanced controls from " + currentlyRequestedDeviceDescription.deviceName + ". Please try again.";
    notification.type = UserInterfaceNotification::WARNING;
    std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
    Logger::log("AdvancedControlsLoader:// Service failed with error " + String((int)result));
    return SERV_SUCCESS;
}

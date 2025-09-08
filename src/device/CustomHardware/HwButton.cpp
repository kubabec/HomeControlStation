#include "devices/CustomHardware/HwButton.hpp"

void ICACHE_RAM_ATTR HwButton::interruptRouter(void *arg)
{
    reinterpret_cast<HwButton *>(arg)->buttonAction();
}

void ICACHE_RAM_ATTR HwButton::buttonAction()
{
    buttonActionsQueue.push(std::pair(digitalRead(pinNumber), millis()));
}

HwButton::HwButton(DeviceConfigSlotType nvmData, std::function<void(uint16_t)> localToggleCbk, std::function<void(uint64_t)> eventTriggerCbk)
{
    localDeviceTriggerFunction = localToggleCbk;
    eventTriggerFunction = eventTriggerCbk;

    deviceId = nvmData.deviceId;
    pinNumber = nvmData.pinNumber;

    uint16_t nvmOffset = 0;
    activeHigh = nvmData.customBytes[nvmOffset];
    nvmOffset += sizeof(uint8_t);

    memcpy(&localDeviceIdToTrigger, &nvmData.customBytes[nvmOffset], sizeof(localDeviceIdToTrigger));
    nvmOffset += sizeof(localDeviceIdToTrigger);

    memcpy(&doubleClickEventId, &nvmData.customBytes[nvmOffset], sizeof(doubleClickEventId));
    nvmOffset += sizeof(doubleClickEventId);

    memcpy(&longPressEventId, &nvmData.customBytes[nvmOffset], sizeof(longPressEventId));
    nvmOffset += sizeof(longPressEventId);
}

HwButton::HwButton(DeviceConfigSlotType nvmData, std::function<void(uint64_t)> eventTriggerCbk)
{
    localDeviceTriggerFunction = nullptr;
    eventTriggerFunction = eventTriggerCbk;

    deviceId = nvmData.deviceId;
    pinNumber = nvmData.pinNumber;

    uint16_t nvmOffset = 0;
    activeHigh = nvmData.customBytes[nvmOffset];
    nvmOffset += sizeof(uint8_t);

    memcpy(&localDeviceIdToTrigger, &nvmData.customBytes[nvmOffset], sizeof(localDeviceIdToTrigger));
    nvmOffset += sizeof(localDeviceIdToTrigger);

    memcpy(&doubleClickEventId, &nvmData.customBytes[nvmOffset], sizeof(doubleClickEventId));
    nvmOffset += sizeof(doubleClickEventId);

    memcpy(&longPressEventId, &nvmData.customBytes[nvmOffset], sizeof(longPressEventId));
    nvmOffset += sizeof(longPressEventId);

    memcpy(&optionalSinglePressEventId, &nvmData.customBytes[nvmOffset], sizeof(optionalSinglePressEventId));
    nvmOffset += sizeof(optionalSinglePressEventId);
}

void HwButton::init()
{
    Serial.println("Initializing button");
    pinMode(pinNumber, INPUT_PULLUP);

    attachInterruptArg(digitalPinToInterrupt(pinNumber), HwButton::interruptRouter, (void *)this, CHANGE);
}

void HwButton::processButtonAction()
{
    enum EventType
    {
        SINGLE_CLICK,
        DOUBLE_CLICK,
        LONG_PRESS
    };

    if (!buttonActionsQueue.empty())
    {
        bool isPressed = activeHigh ? (buttonActionsQueue.front().first == 1) : (buttonActionsQueue.front().first == 0);
        // Button clicked - event started here!
        if (!eventStarted && isPressed)
        {
            eventStartTime = buttonActionsQueue.front().second;
            eventStarted = true;
        }
        else if (eventStarted && !isPressed)
        { // Button released - event either finished or midtime release happened

            eventEndTime = buttonActionsQueue.front().second;
        }
        else if (eventStarted && isPressed)
        {
            // Pressed more than one time
            doubleClick = true;
            eventForceFinished = true;
        }

        buttonActionsQueue.pop();
    }

    if (eventStarted)
    {
        // Shall event results be evaluated?
        if (eventForceFinished || millis() - eventStartTime >= eventDurationTimeMs)
        {
            // Event completed
            eventStarted = false;

            // Evaluate results
            EventType event = SINGLE_CLICK;

            if (doubleClick)
            {
                event = DOUBLE_CLICK;
                // Serial.println("DOUBLE CLICK, eventStartTime:" + String((int)eventStartTime) + " , eventEndTime: " + String((int)eventEndTime) + " ");
            }
            else
            {
                // Buton was not released at all
                if (eventEndTime <= eventStartTime)
                {
                    event = LONG_PRESS;
                    // Serial.println("LONG PRESS 1, eventStartTime:" + String((int)eventStartTime) + " , eventEndTime: " + String((int)eventEndTime) + " ");
                }
                else
                {
                    // Button was released during event, check the event type
                    if ((float)(eventEndTime - eventStartTime) > (3.f * (((float)eventDurationTimeMs) / 4.f)))
                    {
                        // Touch longer than half of cycle
                        event = LONG_PRESS;
                        // Serial.println("LONG PRESS 2, eventStartTime:" + String((int)eventStartTime) + " , eventEndTime: " + String((int)eventEndTime) + " ");
                    }
                    else
                    {
                        event = SINGLE_CLICK;
                        // Serial.println("SINGLE CLICK, eventStartTime:" + String((int)eventStartTime) + " , eventEndTime: " + String((int)eventEndTime) + " ");
                    }
                }
            }

            switch (event)
            {
            case SINGLE_CLICK:
                if(localDeviceTriggerFunction != nullptr){
                    Serial.println("Triggering local device");
                    localDeviceTriggerFunction(localDeviceIdToTrigger);
                }else {
                    if(eventTriggerFunction != nullptr){
                        Serial.println("Triggering remote device");
                        eventTriggerFunction(optionalSinglePressEventId);
                    }
                }
                break;
            case DOUBLE_CLICK:
                if(eventTriggerFunction != nullptr){
                        Serial.println("Triggering double click device");
                        eventTriggerFunction(doubleClickEventId);
                    }
                break;
            case LONG_PRESS:
                if(eventTriggerFunction != nullptr){
                        Serial.println("Triggering long press device");
                        eventTriggerFunction(longPressEventId);
                    }
                break;

            default:
                break;
            }

            // Clean up
            eventEndTime = 0;
            eventStartTime = 0;
            eventStarted = false;
            doubleClick = false;
            eventForceFinished = false;
        }
    }
}

void HwButton::cyclic()
{
    processButtonAction();
}

uint16_t HwButton::getExtendedMemoryLength()
{
    return 0;
}

uint8_t HwButton::getDeviceIdentifier()
{
    return deviceId;
}
uint8_t HwButton::getDeviceType()
{
    return (uint8_t)type_HARDWARE_BUTTON;
}

ServiceRequestErrorCode HwButton::service(DeviceServicesType serviceType)
{
    switch (serviceType)
    {
    default:
        return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode HwButton::service(DeviceServicesType serviceType, ServiceParameters_set1 param)
{
    switch (serviceType)
    {
    default:
        return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode HwButton::service(DeviceServicesType serviceType, ServiceParameters_set2 param)
{
    switch (serviceType)
    {
    default:
        return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode HwButton::service(DeviceServicesType serviceType, ServiceParameters_set3 param)
{
    switch (serviceType)
    {
    default:
        return SERV_NOT_SUPPORTED;
    };
}

DeviceDescription HwButton::getDeviceDescription()
{
    DeviceDescription desc;
    desc.deviceType = (uint8_t)type_HARDWARE_BUTTON;
    desc.deviceId = deviceId;
    desc.roomId = 0;
    desc.isEnabled = 3; // Always enabled
    desc.deviceName = "HwButton" + String((int)desc.deviceId);
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    return desc;
}
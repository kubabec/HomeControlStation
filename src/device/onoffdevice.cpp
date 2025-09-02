#include <devices/OnOffDevice.hpp>

OnOffDevice::OnOffDevice(int pin, String devName, uint8_t a_deviceId, uint8_t a_roomId)
{
    pinNumber = pin;
    deviceName = devName;
    deviceId = a_deviceId;
    roomId = a_roomId;
}

OnOffDevice::OnOffDevice(DeviceDescription &description, uint8_t pin)
{
    deviceName = description.deviceName;
    deviceId = description.deviceId;
    roomId = description.roomId;
    pinNumber = pin;
}

OnOffDevice::OnOffDevice(DeviceConfigSlotType nvmData)
{
    deviceName = String(nvmData.deviceName);
    deviceId = nvmData.deviceId;
    roomId = nvmData.roomId;
    pinNumber = nvmData.pinNumber;
    activeLow = (!nvmData.customBytes[1]);

    if (nvmData.customBytes[0] == 1)
    {
        brightnessLevelSupport = true;
    }
    else
    {
        brightnessLevelSupport = false;
    }

    minPwmValue = nvmData.customBytes[2];
    maxPwmValue = nvmData.customBytes[3];
}

void OnOffDevice::on()
{

    Serial.println("OnOffDevice::on() called");
    Serial.println("brigtnessLevelTarget: " + String(brightnessLevelTarget));
    Serial.println("brightnessLevel: " + String(brightnessLevel));

    isOn = true;
    if (brightnessLevelSupport)
    {
        brightnessLevelTarget = brightnessLevelBackupWhenOff; // Restore brightness level
        brightnessLevel = 0;                                  // = brightnessLevelTarget;
        timePrevious1 = millis();
        // analogWrite(pinNumber, mapBrightness(brightnessLevel));
    }
    else
    {
        digitalWrite(pinNumber, (activeLow ? LOW : HIGH));
    }
}

void OnOffDevice::off()
{

    Serial.println("OnOffDevice::off() called");
    Serial.println("brigtnessLevelTarget: " + String(brightnessLevelTarget));
    Serial.println("brightnessLevel: " + String(brightnessLevel));

    isOn = false;
    if (brightnessLevelSupport)
    {
        brightnessLevelBackupWhenOff = brightnessLevel; // Save current brightness level
        brightnessLevelTarget = 0;                      // Zapisujemy aktualną jasność
        Serial.println("brightnessLevelBackupWhenOff: " + String(brightnessLevelBackupWhenOff));
        Serial.println("Triggering brightness change to 0");
        // analogWrite(pinNumber, activeLow ? 255 : 0);
    }
    else
    {
        digitalWrite(pinNumber, activeLow ? HIGH : LOW);
    }
}

bool OnOffDevice::getState()
{
    return isOn;
}

void OnOffDevice::setBrightnessLevelSupport(bool p_brightnessLevelSupport)
{
    brightnessLevelSupport = p_brightnessLevelSupport;
}

int OnOffDevice::getBrightnessStep()
{
    return brightnessStep;
}
int OnOffDevice::getBrightnessStepDuration()
{
    return brightnessStepDurationMS;
}

void OnOffDevice::brightnessChangeHandler()
{

    if (brightnessLevel != brightnessLevelTarget)
    {
        if ((millis() - timePrevious1) >= 10 /*brightnessStepDurationMS*/)
        {
            // Serial.print("Zmiana jasności : ");
            if (brightnessLevel < brightnessLevelTarget)
            {
                brightnessLevel++;
            }
            if (brightnessLevel > brightnessLevelTarget)
            {
                brightnessLevel--;
            }

            if (brightnessLevelSupport)
            {
                analogWrite(pinNumber, mapBrightness(brightnessLevel));
            }

            timePrevious1 = millis();
        }
    }
}

void OnOffDevice::timerHandler()
{
    // if(lightDurationTimerMS > 0) {

    //     if ((millis() - timePrevious2) > lightDurationTimerMS) {
    //             timePrevious2 = millis();
    //             off();
    //             setLightDurationTimerMS(0);

    //         }

    //     }
}

void OnOffDevice::changeBrightness(int requestedBrightness)
{
    Serial.println("Requested: " + String(requestedBrightness));
    brightnessLevelTarget = requestedBrightness;
    brightnessLevelBackupWhenOff = requestedBrightness; // Save current brightness level
    float brightnessDelta = abs(brightnessLevelTarget - brightnessLevel);
    brightnessStepDurationMS = brightnessChangeTime / brightnessDelta;

    isOn = true; // Ensure device is ON when changing brightness
    timePrevious1 = millis();
}

void OnOffDevice::init()
{
    pinMode(pinNumber, OUTPUT);
    off();

    if (brightnessLevelSupport)
    {
        analogWrite(pinNumber, mapBrightness(brightnessLevel));
    }

    controls.switchAnimationTime = 200 + deviceId;
    // Serial.println("Device " + deviceName + " initialized on pin " + String(pinNumber));
}

void OnOffDevice::cyclic()
{
    brightnessChangeHandler();
    timerHandler();
}

uint16_t OnOffDevice::getExtendedMemoryLength()
{
    return 0;
}

int OnOffDevice::getBrightnessLevel()
{
    return brightnessLevel;
}

int OnOffDevice::getBrightnessLevelTarget()
{
    return brightnessLevelTarget;
}

void OnOffDevice::setLightDurationTimerMS(int requestedLightDuration)
{
    lightDurationTimerMS = requestedLightDuration * 1000;
    // Serial.println();
    // Serial.print("========Timer : ");
    // Serial.println(lightDurationTimerMS);
}

int OnOffDevice::getLightDurationTimerMS()
{
    return lightDurationTimerMS;
}

String OnOffDevice::getName()
{
    return deviceName;
}

uint8_t OnOffDevice::getDeviceId()
{
    return deviceId;
}

uint8_t OnOffDevice::getRoomId()
{
    return roomId;
}

bool OnOffDevice::getBrightnessIsAdjustable()
{
    return brightnessLevelSupport;
}

uint8_t OnOffDevice::getDeviceIdentifier()
{

    return deviceId;
}
uint8_t OnOffDevice::getDeviceType()
{
    return type_ONOFFDEVICE;
}

ServiceRequestErrorCode OnOffDevice::service(DeviceServicesType serviceType)
{
    switch (serviceType)
    {

    default:
        Serial.println("Device_" + String((int)deviceId) + ":Service {" + String((int)serviceType) + "} is not supported (noParam)");
        return SERV_NOT_SUPPORTED;
    };
}

ServiceRequestErrorCode OnOffDevice::service(DeviceServicesType serviceType, ServiceParameters_set1 param)
{
    switch (serviceType)
    {
    case DEVSERVICE_STATE_SWITCH:
        if (param.a == 1)
        {
            if (!isOn)
            {
                on();
            }
            Serial.println("Setting state: ON");
        }
        else
        {
            if (isOn)
            {
                off();
            }
            Serial.println("Setting state: OFF");
        }
        return SERV_SUCCESS;

    case DEVSERVICE_BRIGHTNESS_CHANGE:
        changeBrightness(param.a);
        Serial.println("Changing brightness to " + String((int)param.a));
        return SERV_SUCCESS;

    default:
        Serial.println("Device_" + String((int)deviceId) + ":Service {" + String((int)serviceType) + "} is not supported (param1)");
        return SERV_NOT_SUPPORTED;
    };
}

ServiceRequestErrorCode OnOffDevice::service(DeviceServicesType serviceType, ServiceParameters_set2 param)
{
    switch (serviceType)
    {
    default:
        Serial.println("Device_" + String((int)deviceId) + ":Service {" + String((int)serviceType) + "} is not supported (param2)");
        return SERV_NOT_SUPPORTED;
    };
}

ServiceRequestErrorCode OnOffDevice::service(DeviceServicesType serviceType, ServiceParameters_set3 param)
{
    switch (serviceType)
    {
    case DEVSERVICE_GET_ADVANCED_CONTROLS:
        Serial.println("DEVSERVICE_GET_ADVANCED_CONTROLS");
        if (param.size == sizeof(AdvancedControlsOnOff))
        {
            memcpy(param.buff, &controls, sizeof(AdvancedControlsOnOff));
        }

        return SERV_SUCCESS;
        break;

    default:
        Serial.println("Device_" + String((int)deviceId) + ":Service {" + String((int)serviceType) + "} is not supported (param3)");
        return SERV_NOT_SUPPORTED;
    };
}

DeviceDescription OnOffDevice::getDeviceDescription()
{
    DeviceDescription desc;
    desc.deviceType = type_ONOFFDEVICE;
    desc.deviceId = deviceId;
    desc.roomId = roomId;
    desc.isEnabled = isOn;
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    desc.customBytes[0] = brightnessLevelSupport;
    desc.customBytes[1] = activeLow ? 0 : 1;
    desc.customBytes[2] = brightnessLevelBackupWhenOff;

    return desc;
}

int OnOffDevice::mapBrightness(int brightness)
{
    // if(brightness < 95){
    //     // Need to map that 0-99 percent must be PWM in range between 0-60%
    //     brightness = (brightness * 70) / 100; // Map to 0-60%
    // }

    if (activeLow)
    {
        // For devices activated by LOW (0V = ON)
        return maxPwmValue - ((brightness * (maxPwmValue - minPwmValue)) / 100);
    }
    else
    {
        // For devices activated by HIGH (3.3V/5V = ON)
        return minPwmValue + ((brightness * (maxPwmValue - minPwmValue)) / 100);
    }
}

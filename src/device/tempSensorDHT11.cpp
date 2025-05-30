#include "devices/tempSensorDHT11.hpp"

#ifdef TEMP_SENSOR_SUPPORTED

TempSensorDHT11DeviceType::TempSensorDHT11DeviceType(DeviceConfigSlotType nvmData)
{
    isOn = false;
    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;

    dht = new DHT(pinNumber, DHT11);
    Serial.println("DHT11 device id: " + String((int)deviceId) + ", pin: " + String(pinNumber));

    
    dht->begin();

    float h = dht->readHumidity();
    float t = dht->readTemperature();
    if(!isnan(t))
    {
        currentTemp = t;
        Serial.println("Temp: " + String(currentTemp));
    }
    if(!isnan(h)){
        currentHumid = (int)h;
        Serial.println("Humidity: " + String(currentHumid));
    }
}

void TempSensorDHT11DeviceType::init()
{
}

void TempSensorDHT11DeviceType::cyclic()
{
    if (millis() - lastDataUpdateTime > 4000)
    {
        float h = dht->readHumidity();
        float t = dht->readTemperature();

        if(!isnan(t))
        {
            if (lastTemp == 0 || abs(t - lastTemp) <= 2.0)
            {
                temHumSensError = 0;
                currentTemp = t;
                lastTemp = t;
                Serial.println("Temp: " + String(currentTemp));
            }
            else
            {
                Serial.println("Rejected due to unpredicted temperature jump: " + String(t));
            }
            if(!isnan(h))
            {
                currentHumid = (int)h;
                Serial.println("Humidity: " + String(currentHumid));
            }
        }
        else 
        {
            temHumSensError = 1;
            Serial.println("Temperature and humidity sensor error");
        }

        lastDataUpdateTime = millis();

        // Serial.println("Temp: " + String(currentTemp));
    }
}

uint16_t TempSensorDHT11DeviceType::getExtendedMemoryLength()
{
    return 0;
}

uint8_t TempSensorDHT11DeviceType::getDeviceIdentifier()
{
    return deviceId;
}
uint8_t TempSensorDHT11DeviceType::getDeviceType()
{
    return type_TEMP_SENSOR;
}

ServiceRequestErrorCode TempSensorDHT11DeviceType::service(DeviceServicesType serviceType)
{
    switch (serviceType)
    {
    default:
        return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TempSensorDHT11DeviceType::service(DeviceServicesType serviceType, ServiceParameters_set1 param)
{
    switch (serviceType)
    {
    default:
        return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TempSensorDHT11DeviceType::service(DeviceServicesType serviceType, ServiceParameters_set2 param)
{
    switch (serviceType)
    {
    default:
        return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TempSensorDHT11DeviceType::service(DeviceServicesType serviceType, ServiceParameters_set3 param)
{
    switch (serviceType)
    {
    default:
        return SERV_NOT_SUPPORTED;
    };
}

DeviceDescription TempSensorDHT11DeviceType::getDeviceDescription()
{
    DeviceDescription desc;
    desc.deviceType = getDeviceType();
    desc.deviceId = getDeviceIdentifier();
    desc.roomId = roomId;
    desc.isEnabled = 3; // always enabled, does not count to room state
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);

    desc.customBytes[2] = currentHumid; // humidity
    desc.customBytes[0] = temHumSensError;
    memcpy(&desc.customBytes[3], &currentTemp, sizeof(currentTemp));
    // desc.customBytes[3] = 73; // average color G
    return desc;
}

#endif
#ifndef TEMP_SENSOR_DHT11_TYPE_H
#define TEMP_SENSOR_DHT11_TYPE_H
#include <SystemDefinition.hpp>
#ifdef TEMP_SENSOR_SUPPORTED


#include "devices/Device.hpp"
#include "Arduino.h"
#include <DHT.h>
#include <vector>

class TempSensorDHT11DeviceType : public Device
{
    struct SensorReading{
        float temperature;
        float humidity;
        RtcTime timestamp;
    };

    bool isOn = true; // stan urzadzenia
    int pinNumber;
    uint8_t deviceId;
    String deviceName;
    uint8_t roomId;
    float currentTemp = 255.f;
    uint8_t currentHumid = 255;
    uint8_t temHumSensError = 0; // blad czujnika temperatury i wilgotnosci 0 - czujnik dziala poprawnie, 1 - nie dziala
    float lastTemp = 0;

    const size_t MAX_ENTRIES = 288; // 3 dni x 4 pomiary/godz.
    // const unsigned long TIME_STORE_PERIOD = 15 * 60 * 1000; // Zapis do vectora co 15 minut w ms
    const unsigned long TIME_STORE_PERIOD = 30 * 1000; // Zapis do vectora co 30 s dla testowania
    unsigned long lastDataUpdateTime = 0;
    unsigned long lastStoredTime = 0;
    unsigned long lastCheckedTime = 0;
    uint8_t lastLoggedMinute = 255;

  
    DHT *dht = nullptr;
    std::function<RtcTime(void)> getTime;
    virtual void temHumReading(); // odczyt wilgotnosci i temperatury z sensora DHT
    virtual void printSensorData(float temp, float humid, SensorReading reading); // wypisuje zawarrosc vectora z danymi zebranymi przez sensor DH!
    virtual void dhtSensorRecords(); // zapisuje dane z sensora DHT do vectora
    
    std::vector<SensorReading> sensorData;
    

public:
    TempSensorDHT11DeviceType(DeviceConfigSlotType nvmData, std::function<RtcTime(void)> getTimeCallback);

    virtual void init();
    virtual void cyclic();
    virtual uint8_t getDeviceIdentifier();
    virtual uint8_t getDeviceType();
    virtual DeviceDescription getDeviceDescription();
    virtual uint16_t getExtendedMemoryLength();

    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);
};

#endif

#endif
#include "devices/tempSensorDHT11.hpp"
#include "os/Logger.hpp"

#ifdef TEMP_SENSOR_SUPPORTED

TempSensorDHT11DeviceType::TempSensorDHT11DeviceType(DeviceConfigSlotType nvmData, std::function<RtcTime(void)> getTimeCallback)
{
    isOn = false;
    /* save pointer to getRTCTime callback */
    getTime = getTimeCallback;

    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;

    dht = new DHT(pinNumber, DHT22);
    Logger::log("DHT11 device id: " + String((int)deviceId) + ", pin: " + String(pinNumber));

    dht->begin();

    float h = dht->readHumidity();
    float t = dht->readTemperature();
    if (!isnan(t))
    {
        currentTemp = t;
        Logger::log("Temp: " + String(currentTemp));
    }
    if (!isnan(h))
    {
        currentHumid = (int)h;
        Logger::log("Humidity: " + String(currentHumid));
    }
}

void TempSensorDHT11DeviceType::printSensorData(float temp, float humid, SensorReading reading)
{
    Logger::log(".....................................................");
    Logger::log(".....................................................");
    Logger::log(".....................................................");
    Logger::log("Dodano nowy pomiar do wektora:");
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print("°C, Hum: ");
    Serial.print(humid);
    Serial.print("%, Rok: ");
    Serial.print(reading.timestamp.year);
    Serial.print(", Godzina: ");
    Serial.print(reading.timestamp.hour);
    Serial.print(":");
    Serial.print(reading.timestamp.min);
    Serial.print(":");
    Logger::log(String((int)reading.timestamp.sec));
    Logger::log(".....................................................");
    Serial.print("Laczna liczba pomiarow: ");
    Logger::log(String((int)sensorData.size()));
    Logger::log("Zawartosc wektora sensorData:");
    for (const auto &r : sensorData)
    {
        // Format czasu z zerowaniem minut (np. 10:05)
        Serial.print("Czas: ");
        Serial.print(r.timestamp.hour);
        Serial.print(":");
        if (r.timestamp.min < 10)
            Serial.print("0");
        Serial.print(r.timestamp.min);
        // Dane pomiarowe
        Serial.print(" | Temperatura: ");
        Serial.print(r.temperature);
        Serial.print(" °C");
        Serial.print(" | Wilgotnosc: ");
        Serial.print(r.humidity);
        Logger::log(" %");
        Logger::log(".....................................................");
    }
}

void TempSensorDHT11DeviceType::temHumReading()
{
    if (millis() - lastDataUpdateTime > 4000)
    {
        float h = dht->readHumidity();
        float t = dht->readTemperature();

        if (!isnan(t))
        {
            if (lastTemp == 0 || abs(t - lastTemp) <= 2.0)
            {
                temHumSensError = 0;
                currentTemp = t;
                lastTemp = t;
                Logger::log("Temperature: " + String(currentTemp));
            }
            else
            {
                Logger::log("Rejected due to unpredicted temperature jump: " + String(t));
            }
            if (!isnan(h))
            {
                currentHumid = (int)h;
                Logger::log("Humidity: " + String(currentHumid));
            }
        }
        else
        {
            temHumSensError = 1;
            Logger::log("Temperature and humidity sensor error");
        }

        lastDataUpdateTime = millis();

        // Logger::log("Temp: " + String(currentTemp));
    }
}

void TempSensorDHT11DeviceType::dhtSensorRecords()
{
    RtcTime time;
    if (getTime)
    {
        time = getTime();
    }
    // Sprawdzaj co 30 sekund
    if (millis() - lastCheckedTime >= TIME_STORE_PERIOD && (time.year != 1970))
    {
        // Zapisz tylko jesli:
        // 1. Minuta jest podzielna przez 15 (czyli 00, 15, 30, 45)
        // 2. Jeszcze nie zapisywalismy w tej minucie
        // 3. Rok inny niż 1970 - brak internetu po resecie
        if ((time.min % 2 == 0) && (time.min != lastLoggedMinute))
        {
            float h = dht->readHumidity();
            float t = dht->readTemperature();
            if (!isnan(t))
            {
                if (sensorData.size() >= MAX_ENTRIES)
                {
                    sensorData.erase(sensorData.begin());
                }
                SensorReading reading;
                reading.temperature = t;
                reading.humidity = h;
                reading.timestamp = time;
                sensorData.push_back(reading);
                lastLoggedMinute = time.min;
                printSensorData(t, h, reading);
            }
            else
            {
                Logger::log("Blad czujnika DHT");
            }
        }
        lastCheckedTime = millis();
    }
}

void TempSensorDHT11DeviceType::init()
{
}

void TempSensorDHT11DeviceType::cyclic()
{
    temHumReading();
    // dhtSensorRecords();
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
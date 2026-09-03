#include "devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.hpp"
#include "generated/GeneratedEnablingConditions.hpp"
#include "os/Logger.hpp"

#ifdef TEMP_SENSOR_SUPPORTED

namespace
{
template <typename Value>
Value readValue(const uint8_t *data, uint16_t offset)
{
    Value value;
    memcpy(&value, data + offset, sizeof(value));
    return value;
}

template <typename Value>
void writeValue(uint8_t *data, uint16_t offset, Value value)
{
    memcpy(data + offset, &value, sizeof(value));
}
}

TempSensorDHT11DeviceType::TempSensorDHT11DeviceType(DeviceConfigSlotType nvmData,
                                                     std::function<RtcTime(void)> getTimeCallback,
                                                     std::function<void(void)> reportNvmDataChangedCallback)
{
    isOn = false;
    /* save pointer to getRTCTime callback */
    getTime = getTimeCallback;
    reportNvmDataChanged = reportNvmDataChangedCallback;

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
        Logger::log("[Sensor '"+ deviceName+ "']Temp: " + String(currentTemp));
    }
    if (!isnan(h))
    {
        currentHumid = (int)h;
        Logger::log("[Sensor '"+ deviceName+ "']Humidity: " + String(currentHumid));
    }
}

uint16_t TempSensorDHT11DeviceType::recordOffset(uint8_t index) const
{
    return HISTORY_HEADER_SIZE + static_cast<uint16_t>(index) * HISTORY_RECORD_SIZE;
}

void TempSensorDHT11DeviceType::initializeHistory()
{
    memset(historyData, 0, HISTORY_SIZE);
    writeValue<uint32_t>(historyData, 0, HISTORY_MAGIC);
    historyData[4] = HISTORY_VERSION;
}

void TempSensorDHT11DeviceType::recordStatistics()
{
    if (historyData == nullptr || !getTime || temHumSensError || currentHumid > 100) return;

    const RtcTime time = getTime();
    if (time.year < 2000 || time.year > 2200 || time.yday < 0 || time.yday > 366 || time.hour < 0 || time.hour > 23) return;

    uint8_t writeIndex = historyData[5];
    uint8_t recordCount = historyData[6];
    uint16_t offset = recordOffset(writeIndex);
    const uint8_t halfDay = time.hour >= 12 ? 1 : 0;
    const bool samePeriod = historyData[offset + 5] != 0 &&
                            readValue<uint16_t>(historyData, offset) == static_cast<uint16_t>(time.year) &&
                            readValue<uint16_t>(historyData, offset + 2) == static_cast<uint16_t>(time.yday) &&
                            historyData[offset + 4] == halfDay;

    if (!samePeriod)
    {
        const bool completedPeriod = historyData[offset + 5] != 0;
        if (completedPeriod)
        {
            writeIndex = static_cast<uint8_t>((writeIndex + 1) % HISTORY_RECORD_COUNT);
            historyData[5] = writeIndex;
            if (recordCount < HISTORY_RECORD_COUNT) historyData[6] = ++recordCount;
            offset = recordOffset(writeIndex);
        }
        else if (recordCount == 0)
        {
            historyData[6] = recordCount = 1;
        }

        memset(historyData + offset, 0, HISTORY_RECORD_SIZE);
        writeValue<uint16_t>(historyData, offset, static_cast<uint16_t>(time.year));
        writeValue<uint16_t>(historyData, offset + 2, static_cast<uint16_t>(time.yday));
        historyData[offset + 4] = halfDay;
        historyData[offset + 5] = 1;
        writeValue<int16_t>(historyData, offset + 12, static_cast<int16_t>(currentTemp * 100.0f));
        writeValue<int16_t>(historyData, offset + 14, static_cast<int16_t>(currentTemp * 100.0f));
        historyData[offset + 20] = currentHumid;
        historyData[offset + 21] = currentHumid;

        if (completedPeriod && reportNvmDataChanged) reportNvmDataChanged();
    }

    const uint16_t sampleCount = readValue<uint16_t>(historyData, offset + 6);
    writeValue<uint16_t>(historyData, offset + 6, sampleCount + 1);
    writeValue<float>(historyData, offset + 8, readValue<float>(historyData, offset + 8) + currentTemp);
    const int16_t temperature = static_cast<int16_t>(currentTemp * 100.0f);
    if (temperature < readValue<int16_t>(historyData, offset + 12)) writeValue<int16_t>(historyData, offset + 12, temperature);
    if (temperature > readValue<int16_t>(historyData, offset + 14)) writeValue<int16_t>(historyData, offset + 14, temperature);
    writeValue<uint32_t>(historyData, offset + 16, readValue<uint32_t>(historyData, offset + 16) + currentHumid);
    if (currentHumid < historyData[offset + 20]) historyData[offset + 20] = currentHumid;
    if (currentHumid > historyData[offset + 21]) historyData[offset + 21] = currentHumid;
}

void TempSensorDHT11DeviceType::temHumReading()
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
            Logger::log("[Sensor '"+ deviceName+ "']Temperature: " + String(currentTemp));
        }
        else
        {
            Logger::log("Rejected due to unpredicted temperature jump: " + String(t));
        }
        if (!isnan(h))
        {
            currentHumid = (int)h;
            Logger::log("[Sensor '"+ deviceName+ "']Humidity: " + String(currentHumid));
        }
    }
    else
    {
        temHumSensError = 1;
        Logger::log("[Sensor '"+ deviceName+ "']Temperature and humidity sensor error");
    }
}

void TempSensorDHT11DeviceType::init()
{
}

void TempSensorDHT11DeviceType::cyclic()
{
    temHumReading();
    recordStatistics();
}

uint16_t TempSensorDHT11DeviceType::getExtendedMemoryLength()
{
    return HISTORY_SIZE;
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
    case DEVSERVICE_SET_EXT_MEMORY_PTR:
        if (param.buff == nullptr || param.size != HISTORY_SIZE) return SERV_EXECUTION_FAILURE;
        historyData = param.buff;
        if (readValue<uint32_t>(historyData, 0) != HISTORY_MAGIC || historyData[4] != HISTORY_VERSION ||
            historyData[5] >= HISTORY_RECORD_COUNT || historyData[6] > HISTORY_RECORD_COUNT)
        {
            initializeHistory();
        }
        return SERV_SUCCESS;
    case DEVSERVICE_GET_ADVANCED_CONTROLS:
        if (param.buff == nullptr || param.size != HISTORY_SIZE || param.direction != e_OUT_from_DEVICE || historyData == nullptr)
        {
            return SERV_EXECUTION_FAILURE;
        }
        memcpy(param.buff, historyData, HISTORY_SIZE);
        return SERV_SUCCESS;
    case DEVSERVICE_CHECK_ENABLING_CONDITION:
        temHumReading();
        if (temHumSensError) return SERV_EXECUTION_FAILURE;
        return GeneratedEnablingConditions::evaluateService(getDeviceDescription(), param);
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

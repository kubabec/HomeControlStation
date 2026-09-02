#include "TestHarness.hpp"
#include "Arduino.h"
#include "DHT.h"

#include "devices/HwButton/HwButton.hpp"
#include "devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.hpp"

#include <cstring>
#include <limits>
#include <vector>

namespace
{
DeviceConfigSlotType predefinedConfig(uint8_t pin, uint8_t id)
{
    DeviceConfigSlotType value; value.pinNumber = pin; value.deviceId = id; value.roomId = 2;
    std::strncpy(value.deviceName, "Predefined", sizeof(value.deviceName) - 1); return value;
}
void writeU16(uint8_t* output, uint16_t value) { std::memcpy(output, &value, sizeof(value)); }
void writeU64(uint8_t* output, uint64_t value) { std::memcpy(output, &value, sizeof(value)); }
}

TEST_CASE("HwButton reports identity and unsupported services")
{
    ArduinoFake::reset(); auto cfg = predefinedConfig(22, 10); std::vector<uint64_t> events;
    HwButton button(cfg, [&](uint64_t event) { events.push_back(event); }); button.init();
    EXPECT_EQ(ArduinoFake::pinModeOf(22), INPUT_PULLUP); EXPECT_EQ(button.getDeviceIdentifier(), uint8_t(10));
    EXPECT_EQ(button.getDeviceType(), uint8_t(type_HARDWARE_BUTTON));
    EXPECT_EQ(button.service(DEVSERVICE_ENABLE), SERV_NOT_SUPPORTED);
}

TEST_CASE("HwButton recognizes single, double, and long press")
{
    ArduinoFake::reset(); auto cfg = predefinedConfig(23, 11); cfg.customBytes[0] = 0;
    writeU16(&cfg.customBytes[1], 77); writeU64(&cfg.customBytes[3], 1001); writeU64(&cfg.customBytes[11], 1002);
    std::vector<uint16_t> local; std::vector<uint64_t> events;
    HwButton button(cfg, [&](uint16_t id) { local.push_back(id); }, [&](uint64_t id) { events.push_back(id); }); button.init();

    ArduinoFake::setDigitalInput(23, LOW); ArduinoFake::triggerInterrupt(23); button.cyclic();
    ArduinoFake::advanceMillis(100); ArduinoFake::setDigitalInput(23, HIGH); ArduinoFake::triggerInterrupt(23); button.cyclic();
    ArduinoFake::advanceMillis(700); button.cyclic(); EXPECT_EQ(local.size(), size_t(1)); EXPECT_EQ(local[0], uint16_t(77));

    ArduinoFake::setDigitalInput(23, LOW); ArduinoFake::triggerInterrupt(23); button.cyclic();
    ArduinoFake::advanceMillis(100); ArduinoFake::setDigitalInput(23, HIGH); ArduinoFake::triggerInterrupt(23); button.cyclic();
    ArduinoFake::advanceMillis(100); ArduinoFake::setDigitalInput(23, LOW); ArduinoFake::triggerInterrupt(23); button.cyclic();
    EXPECT_EQ(events.size(), size_t(1)); EXPECT_EQ(events[0], uint64_t(1001));

    ArduinoFake::advanceMillis(1000); ArduinoFake::setDigitalInput(23, LOW); ArduinoFake::triggerInterrupt(23); button.cyclic();
    ArduinoFake::advanceMillis(800); button.cyclic(); EXPECT_EQ(events.size(), size_t(2)); EXPECT_EQ(events[1], uint64_t(1002));
}

TEST_CASE("DHT predefined device publishes valid readings")
{
    ArduinoFake::reset(); DHT::temperature = 21.5f; DHT::humidity = 63.8f;
    auto cfg = predefinedConfig(24, 12);
    TempSensorDHT11DeviceType sensor(cfg, [] { RtcTime time; time.year = 2026; return time; });
    sensor.cyclic(); auto description = sensor.getDeviceDescription(); float temperature = 0;
    std::memcpy(&temperature, &description.customBytes[3], sizeof(temperature));
    EXPECT_NEAR(temperature, 21.5f, 0.01f); EXPECT_EQ(description.customBytes[2], uint8_t(63));
    EXPECT_EQ(description.customBytes[0], uint8_t(0)); EXPECT_EQ(sensor.service(DEVSERVICE_ENABLE), SERV_NOT_SUPPORTED);
}

TEST_CASE("DHT predefined device flags failed and implausible samples")
{
    ArduinoFake::reset(); DHT::temperature = 20.0f; DHT::humidity = 50.0f;
    auto cfg = predefinedConfig(25, 13); TempSensorDHT11DeviceType sensor(cfg, {});
    sensor.cyclic();
    DHT::temperature = std::numeric_limits<float>::quiet_NaN(); sensor.cyclic();
    EXPECT_EQ(sensor.getDeviceDescription().customBytes[0], uint8_t(1));
    DHT::temperature = 40.0f; DHT::humidity = 70.0f; sensor.cyclic();
    auto description = sensor.getDeviceDescription(); float temperature = 0;
    std::memcpy(&temperature, &description.customBytes[3], sizeof(temperature));
    EXPECT_NEAR(temperature, 20.0f, 0.01f);
}

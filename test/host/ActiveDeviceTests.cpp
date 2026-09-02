#include "TestHarness.hpp"
#include "Arduino.h"

#include "devices/AquariumController/AquariumController.hpp"
#include "devices/Gate/Gate.hpp"
#include "devices/OnOffDevice/OnOffDevice.hpp"
#include "devices/WindowBlinder/WindowBlinder.hpp"
#include "devices/WindowDoorSensor/WindowDoorSensor.hpp"

#include <cstring>

namespace
{
DeviceConfigSlotType config(const char* name, uint8_t id, uint8_t room, uint8_t pin)
{
    DeviceConfigSlotType value;
    std::strncpy(value.deviceName, name, sizeof(value.deviceName) - 1);
    value.deviceId = id; value.roomId = room; value.pinNumber = pin; value.isActive = true;
    return value;
}
ServiceParameters_set3 bufferParams(uint8_t* data, uint16_t size, ServiceDirectionType direction)
{
    ServiceParameters_set3 value; value.buff = data; value.size = size; value.direction = direction; return value;
}
}

TEST_CASE("OnOffDevice initializes active-low output and switches state")
{
    ArduinoFake::reset();
    auto cfg = config("Lamp", 1, 2, 5); cfg.customBytes[0] = 0; cfg.customBytes[1] = 0;
    OnOffDevice device(cfg); device.init();
    EXPECT_EQ(ArduinoFake::pinModeOf(5), OUTPUT); EXPECT_EQ(ArduinoFake::digitalOutput(5), HIGH);
    EXPECT_EQ(device.service(DEVSERVICE_STATE_SWITCH, ServiceParameters_set1{.a = 1}), SERV_SUCCESS);
    EXPECT_TRUE(device.getState()); EXPECT_EQ(ArduinoFake::digitalOutput(5), LOW);
    EXPECT_EQ(device.service(DEVSERVICE_STATE_SWITCH, ServiceParameters_set1{.a = 0}), SERV_SUCCESS);
    EXPECT_FALSE(device.getState()); EXPECT_EQ(ArduinoFake::digitalOutput(5), HIGH);
}

TEST_CASE("OnOffDevice maps PWM polarity and advances brightness")
{
    ArduinoFake::reset();
    auto cfg = config("Dimmer", 2, 1, 6); cfg.customBytes[0] = 1; cfg.customBytes[1] = 1;
    cfg.customBytes[2] = 10; cfg.customBytes[3] = 210;
    OnOffDevice device(cfg); device.init();
    EXPECT_EQ(device.mapBrightness(0), 10); EXPECT_EQ(device.mapBrightness(100), 210);
    EXPECT_EQ(device.service(DEVSERVICE_BRIGHTNESS_CHANGE, ServiceParameters_set1{.a = 3}), SERV_SUCCESS);
    for (int i = 0; i < 3; ++i) { ArduinoFake::advanceMillis(10); device.cyclic(); }
    EXPECT_EQ(device.getBrightnessLevel(), 3); EXPECT_EQ(ArduinoFake::analogOutput(6), 16);
}

TEST_CASE("OnOffDevice advanced controls validate size and round-trip")
{
    ArduinoFake::reset(); auto cfg = config("Lamp", 3, 1, 7); OnOffDevice device(cfg); device.init();
    uint8_t input[4] = {1, 2, 0x34, 0x12};
    EXPECT_EQ(device.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(input, 4, e_IN_to_DEVICE)), SERV_SUCCESS);
    uint8_t output[4] = {};
    EXPECT_EQ(device.service(DEVSERVICE_GET_ADVANCED_CONTROLS, bufferParams(output, 4, e_OUT_from_DEVICE)), SERV_SUCCESS);
    EXPECT_TRUE(std::memcmp(input, output, 4) == 0);
    EXPECT_EQ(device.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(input, 3, e_IN_to_DEVICE)), SERV_EXECUTION_FAILURE);
}

TEST_CASE("OnOffDevice rejects out-of-range service values")
{
    ArduinoFake::reset(); auto cfg = config("Lamp", 3, 1, 7); cfg.customBytes[0] = 1; OnOffDevice device(cfg);
    EXPECT_EQ(device.service(DEVSERVICE_STATE_SWITCH, ServiceParameters_set1{.a = 2}), SERV_EXECUTION_FAILURE);
    EXPECT_EQ(device.service(DEVSERVICE_BRIGHTNESS_CHANGE, ServiceParameters_set1{.a = 101}), SERV_EXECUTION_FAILURE);
    EXPECT_EQ(device.service(DEVSERVICE_BRIGHTNESS_CHANGE, ServiceParameters_set1{.a = 0}), SERV_SUCCESS);
    EXPECT_EQ(device.getBrightnessStepDuration(), 0);
}

TEST_CASE("WindowDoorSensor debounces both transitions")
{
    ArduinoFake::reset(); auto cfg = config("Door", 4, 2, 8); cfg.customBytes[0] = 1; cfg.customBytes[2] = 20;
    ArduinoFake::setDigitalInput(8, LOW); WindowDoorSensor sensor(cfg); sensor.init();
    EXPECT_EQ(sensor.getDeviceDescription().customBytes[0], uint8_t(0));
    ArduinoFake::setDigitalInput(8, HIGH); sensor.cyclic(); ArduinoFake::advanceMillis(19); sensor.cyclic();
    EXPECT_EQ(sensor.getDeviceDescription().customBytes[0], uint8_t(0));
    ArduinoFake::advanceMillis(1); sensor.cyclic(); EXPECT_EQ(sensor.getDeviceDescription().customBytes[0], uint8_t(1));
    ArduinoFake::setDigitalInput(8, LOW); sensor.cyclic(); ArduinoFake::advanceMillis(20); sensor.cyclic();
    EXPECT_EQ(sensor.getDeviceDescription().customBytes[0], uint8_t(0));
}

TEST_CASE("WindowBlinder interlocks outputs, honors limits, and times out")
{
    ArduinoFake::reset(); auto cfg = config("Blind", 5, 3, 9); cfg.customBytes[0] = 10;
    cfg.customBytes[1] = 11; cfg.customBytes[2] = 12; cfg.customBytes[3] = 1; cfg.customBytes[4] = 1;
    cfg.customBytes[5] = 1; cfg.customBytes[7] = 1;
    ArduinoFake::setDigitalInput(11, LOW); ArduinoFake::setDigitalInput(12, LOW);
    WindowBlinder blind(cfg); blind.init();
    uint8_t command[6] = {1};
    EXPECT_EQ(blind.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(command, 6, e_IN_to_DEVICE)), SERV_SUCCESS);
    EXPECT_EQ(ArduinoFake::digitalOutput(9), HIGH); EXPECT_EQ(ArduinoFake::digitalOutput(10), LOW);
    ArduinoFake::setDigitalInput(11, HIGH); blind.cyclic();
    auto description = blind.getDeviceDescription(); EXPECT_EQ(description.customBytes[1], uint8_t(100)); EXPECT_EQ(description.isEnabled, uint8_t(0));
    ArduinoFake::setDigitalInput(11, LOW); command[0] = 2;
    blind.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(command, 6, e_IN_to_DEVICE));
    ArduinoFake::advanceMillis(1000); blind.cyclic();
    EXPECT_EQ(blind.getDeviceDescription().customBytes[4], uint8_t(1)); EXPECT_EQ(ArduinoFake::digitalOutput(10), LOW);
}

TEST_CASE("Gate enforces reversal delay, endpoint stop, and timeout fault")
{
    ArduinoFake::reset(); auto cfg = config("Gate", 6, 1, 13);
    cfg.customBytes[0] = 14; cfg.customBytes[1] = 15; cfg.customBytes[2] = 16;
    cfg.customBytes[3] = 17; cfg.customBytes[4] = 18; cfg.customBytes[5] = 2; cfg.customBytes[6] = 1; cfg.customBytes[7] = 1;
    ArduinoFake::setDigitalInput(17, HIGH); ArduinoFake::setDigitalInput(18, HIGH);
    Gate gate(cfg); gate.init(); uint8_t command[5] = {1};
    gate.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(command, 5, e_IN_to_DEVICE));
    EXPECT_EQ(ArduinoFake::digitalOutput(13), LOW); ArduinoFake::advanceMillis(249); gate.cyclic();
    EXPECT_EQ(ArduinoFake::digitalOutput(13), LOW); ArduinoFake::advanceMillis(1); gate.cyclic();
    EXPECT_EQ(ArduinoFake::digitalOutput(13), HIGH); EXPECT_EQ(ArduinoFake::digitalOutput(15), HIGH);
    ArduinoFake::setDigitalInput(17, LOW); gate.cyclic(); EXPECT_EQ(gate.getDeviceDescription().isEnabled, uint8_t(0));
    ArduinoFake::setDigitalInput(17, HIGH); command[0] = 2;
    gate.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(command, 5, e_IN_to_DEVICE));
    ArduinoFake::advanceMillis(250); gate.cyclic(); ArduinoFake::advanceMillis(1000); gate.cyclic();
    EXPECT_EQ(gate.getDeviceDescription().customBytes[3], uint8_t(1));
}

TEST_CASE("Gate and WindowBlinder reject malformed control payloads")
{
    ArduinoFake::reset(); auto cfg = config("Motor", 7, 1, 20); cfg.customBytes[0] = 21;
    cfg.customBytes[1] = cfg.customBytes[2] = 255; cfg.customBytes[3] = cfg.customBytes[4] = 1;
    WindowBlinder blind(cfg); Gate gate(cfg); uint8_t bytes[6] = {3};
    EXPECT_EQ(blind.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(bytes, 6, e_IN_to_DEVICE)), SERV_EXECUTION_FAILURE);
    EXPECT_EQ(gate.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(bytes, 5, e_IN_to_DEVICE)), SERV_EXECUTION_FAILURE);
    EXPECT_EQ(blind.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(nullptr, 6, e_IN_to_DEVICE)), SERV_EXECUTION_FAILURE);
}

TEST_CASE("AquariumController controls heater with hysteresis and safety faults")
{
    ArduinoFake::reset(); auto cfg = config("Tank", 8, 4, 1); cfg.customBytes[0] = 2; cfg.customBytes[1] = 3;
    cfg.customBytes[2] = 4; cfg.customBytes[3] = 5; cfg.customBytes[4] = 1;
    cfg.customBytes[5] = 250 & 0xff; cfg.customBytes[6] = 250 >> 8;
    cfg.customBytes[7] = 5; cfg.customBytes[9] = 64; cfg.customBytes[10] = 1;
    ArduinoFake::setAnalogInput(1, 2500); ArduinoFake::setDigitalInput(5, HIGH);
    AquariumController tank(cfg); tank.init();
    auto description = tank.getDeviceDescription(); EXPECT_EQ(description.customBytes[4], uint8_t(1));
    EXPECT_EQ(description.customBytes[6], uint8_t(1)); EXPECT_EQ(ArduinoFake::digitalOutput(2), HIGH);
    ArduinoFake::setDigitalInput(5, LOW); ArduinoFake::advanceMillis(1000); tank.cyclic();
    description = tank.getDeviceDescription(); EXPECT_EQ(description.customBytes[5], uint8_t(1));
    EXPECT_EQ(description.customBytes[6], uint8_t(0)); EXPECT_TRUE((description.customBytes[10] & 2) != 0);
    ArduinoFake::setDigitalInput(5, HIGH); ArduinoFake::setAnalogInput(1, 0); ArduinoFake::advanceMillis(1000); tank.cyclic();
    EXPECT_TRUE((tank.getDeviceDescription().customBytes[10] & 1) != 0);
}

TEST_CASE("AquariumController validates commands and updates auxiliaries")
{
    ArduinoFake::reset(); auto cfg = config("Tank", 9, 4, 1); cfg.customBytes[0] = 2; cfg.customBytes[1] = 3;
    cfg.customBytes[2] = 4; cfg.customBytes[3] = 255; cfg.customBytes[4] = 1;
    AquariumController tank(cfg); tank.init(); uint8_t command[10] = {0, 1, 0};
    EXPECT_EQ(tank.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(command, 10, e_IN_to_DEVICE)), SERV_SUCCESS);
    EXPECT_EQ(ArduinoFake::digitalOutput(3), HIGH); EXPECT_EQ(ArduinoFake::digitalOutput(4), LOW);
    command[0] = 3;
    EXPECT_EQ(tank.service(DEVSERVICE_SET_ADVANCED_CONTROLS, bufferParams(command, 10, e_IN_to_DEVICE)), SERV_EXECUTION_FAILURE);
    EXPECT_EQ(tank.service(DEVSERVICE_GET_ADVANCED_CONTROLS, bufferParams(command, 9, e_OUT_from_DEVICE)), SERV_EXECUTION_FAILURE);
}

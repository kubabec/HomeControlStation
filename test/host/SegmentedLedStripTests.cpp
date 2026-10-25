#include "TestHarness.hpp"
#include "Arduino.h"

#include "devices/SegmentedLedStripWS2812/SegmentedLedStripWS2812.hpp"

#include <array>
#include <cstring>
#include <vector>

namespace
{
DeviceConfigSlotType segmentedConfig(std::vector<uint8_t> counts, uint16_t total = 0)
{
    DeviceConfigSlotType config; config.pinNumber = 26; config.deviceId = 15; config.roomId = 3;
    std::strncpy(config.deviceName, "Segmented", sizeof(config.deviceName) - 1);
    if (total == 0) { for (auto count : counts) total = static_cast<uint16_t>(total + count); }
    std::memcpy(config.customBytes, &total, sizeof(total));
    config.customBytes[2] = config.customBytes[19] = 128;
    config.customBytes[4] = static_cast<uint8_t>(counts.size());
    for (size_t i = 0; i < counts.size(); ++i) config.customBytes[5 + i] = counts[i];
    return config;
}

ServiceParameters_set3 segParams(uint8_t* bytes, uint16_t size, ServiceDirectionType direction)
{
    ServiceParameters_set3 parameters; parameters.buff = bytes; parameters.size = size;
    parameters.direction = direction; return parameters;
}

/* Provides extended memory to the strip and returns the buffer which must outlive it */
std::vector<uint8_t> attachMemory(SegmentedLedStripWS2812& strip)
{
    std::vector<uint8_t> memory(strip.getExtendedMemoryLength(), 0x00);
    EXPECT_EQ(strip.service(DEVSERVICE_SET_EXT_MEMORY_PTR, segParams(memory.data(), static_cast<uint16_t>(memory.size()), e_IN_to_DEVICE)), SERV_SUCCESS);
    return memory;
}
}

TEST_CASE("Segmented LED strip rejects configurations that do not add up")
{
    EXPECT_TRUE(SegmentedLedStripWS2812::isConfigurationValid(segmentedConfig({10, 20, 30})));
    EXPECT_FALSE(SegmentedLedStripWS2812::isConfigurationValid(segmentedConfig({10, 20, 30}, 61)));
    EXPECT_FALSE(SegmentedLedStripWS2812::isConfigurationValid(segmentedConfig({10, 0, 30})));
    EXPECT_FALSE(SegmentedLedStripWS2812::isConfigurationValid(segmentedConfig({})));
    EXPECT_FALSE(SegmentedLedStripWS2812::isConfigurationValid(segmentedConfig({1, 1, 1, 1, 1, 1, 1, 1, 1})));
    auto mismatch = segmentedConfig({10, 20}); mismatch.customBytes[19] = 7;
    EXPECT_FALSE(SegmentedLedStripWS2812::isConfigurationValid(mismatch));
}

TEST_CASE("Segmented LED strip merges diodes to stay below the virtual limit")
{
    ArduinoFake::reset();
    SegmentedLedStripWS2812 strip(segmentedConfig({100, 100, 100}), [] {});
    auto memory = attachMemory(strip);
    auto description = strip.getDeviceDescription();
    EXPECT_EQ(description.customBytes[1], uint8_t(3));
    EXPECT_EQ(description.customBytes[18], uint8_t(25));
    EXPECT_EQ(description.customBytes[0], uint8_t(75));
    EXPECT_EQ(strip.getExtendedMemoryLength(), uint16_t(75 * 3 + 40));
}

TEST_CASE("Segmented LED strip toggles all segments like a room")
{
    ArduinoFake::reset();
    SegmentedLedStripWS2812 strip(segmentedConfig({4, 4}), [] {});
    auto memory = attachMemory(strip);

    EXPECT_EQ(strip.service(DEVSERVICE_SEGMENT_STATE_SWITCH, ServiceParameters_set1{.a = 1, .b = 1}), SERV_SUCCESS);
    auto description = strip.getDeviceDescription();
    EXPECT_EQ(description.customBytes[2], uint8_t(0)); EXPECT_EQ(description.customBytes[3], uint8_t(1));
    EXPECT_TRUE(description.isEnabled);
    EXPECT_EQ(strip.service(DEVSERVICE_STATE_SWITCH, ServiceParameters_set1{.a = 2}), SERV_BUSY);

    for (int tick = 0; tick < 200; ++tick) strip.cyclic();
    EXPECT_EQ(strip.service(DEVSERVICE_STATE_SWITCH, ServiceParameters_set1{.a = 2}), SERV_SUCCESS);
    EXPECT_FALSE(strip.getDeviceDescription().isEnabled);
    for (int tick = 0; tick < 200; ++tick) strip.cyclic();
    EXPECT_EQ(strip.service(DEVSERVICE_STATE_SWITCH, ServiceParameters_set1{.a = 2}), SERV_SUCCESS);
    description = strip.getDeviceDescription();
    EXPECT_EQ(description.customBytes[2], uint8_t(1)); EXPECT_EQ(description.customBytes[3], uint8_t(1));
}

TEST_CASE("Segmented LED strip stores colors and properties per segment")
{
    ArduinoFake::reset(); bool changed = false;
    SegmentedLedStripWS2812 strip(segmentedConfig({2, 2}), [&] { changed = true; });
    auto memory = attachMemory(strip);

    std::vector<uint8_t> payload(40 + 4 * 3, 0x00);
    payload[0] = 2; payload[1] = 1; payload[2] = 1;           /* segment 0 properties */
    payload[5] = 3;                                           /* segment 1 properties */
    for (size_t i = 0; i < 4; ++i) { payload[40 + i * 3] = static_cast<uint8_t>(10 * (i + 1)); }
    EXPECT_EQ(strip.service(DEVSERVICE_SET_ADVANCED_CONTROLS, segParams(payload.data(), static_cast<uint16_t>(payload.size()), e_IN_to_DEVICE)), SERV_SUCCESS);
    EXPECT_TRUE(changed);

    for (int tick = 0; tick < 200; ++tick) strip.cyclic();
    std::vector<uint8_t> output(payload.size(), 0xAA);
    EXPECT_EQ(strip.service(DEVSERVICE_GET_ADVANCED_CONTROLS, segParams(output.data(), static_cast<uint16_t>(output.size()), e_OUT_from_DEVICE)), SERV_SUCCESS);
    EXPECT_TRUE(output == payload);
    EXPECT_EQ(strip.service(DEVSERVICE_GET_ADVANCED_CONTROLS, segParams(output.data(), 10, e_OUT_from_DEVICE)), SERV_EXECUTION_FAILURE);
    EXPECT_EQ(strip.getDeviceDescription().customBytes[26], uint8_t(15));
}

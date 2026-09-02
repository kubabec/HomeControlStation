#include "TestHarness.hpp"
#include "Arduino.h"

#include "devices/AdvancedControls.hpp"
#include "devices/LedStrip/Animations/FadeIn.hpp"
#include "devices/LedStrip/Animations/FadeOut.hpp"
#include "devices/LedStrip/Animations/RollInAnimation.hpp"
#include "devices/LedStrip/Animations/RollOutAnimation.hpp"
#include "devices/LedStrip/Animations/SparkleInAnimation.hpp"
#include "devices/LedStrip/Animations/TwinkleInAnimation.hpp"
#include "devices/LedStrip/Animations/WaveInAnimation.hpp"
#include "devices/LedStrip/Animations/BounceInAnimation.hpp"
#include "devices/LedStrip/Animations/live/SingleWaveAnimation.hpp"
#include "devices/LedWS1228bDeviceType/LedWS1228bDeviceType.hpp"

#include <array>
#include <cstring>

namespace
{
DeviceConfigSlotType ledConfig(uint16_t count)
{
    DeviceConfigSlotType config; config.pinNumber = 26; config.deviceId = 14; config.roomId = 3;
    std::strncpy(config.deviceName, "LED strip", sizeof(config.deviceName) - 1);
    std::memcpy(config.customBytes, &count, sizeof(count));
    config.customBytes[3] = config.customBytes[19] = 128;
    return config;
}

ServiceParameters_set3 ledParams(uint8_t* bytes, uint16_t size, ServiceDirectionType direction)
{
    ServiceParameters_set3 parameters; parameters.buff = bytes; parameters.size = size;
    parameters.direction = direction; return parameters;
}
}

TEST_CASE("Fade animations reach targets and restore colors")
{
    LedColor colors[] = {{25, 5, 0}, {0, 13, 30}};
    FadeInAnimation fadeIn(colors, 2); fadeIn.start(true);
    EXPECT_EQ(colors[0], LedColor(0, 0, 0));
    for (int step = 0; step < 4; ++step) fadeIn.process();
    EXPECT_FALSE(fadeIn.isInProgress()); EXPECT_EQ(colors[0], LedColor(25, 5, 0)); EXPECT_EQ(colors[1], LedColor(0, 13, 30));

    FadeOutAnimation fadeOut(colors, 2); fadeOut.start(false);
    for (int step = 0; step < 4; ++step) fadeOut.process();
    EXPECT_FALSE(fadeOut.isInProgress()); EXPECT_EQ(colors[0], LedColor(0, 0, 0));
    fadeOut.restoreColors(); EXPECT_EQ(colors[0], LedColor(25, 5, 0)); EXPECT_EQ(colors[1], LedColor(0, 13, 30));
}

TEST_CASE("Roll animations process both directions and skip black targets")
{
    LedColor leftColors[] = {{10, 0, 0}, {0, 0, 0}, {0, 20, 0}};
    RollInAnimation left(leftColors, 3, Direction::LeftToRight); left.start(true);
    left.process(); EXPECT_EQ(leftColors[0], LedColor(10, 0, 0)); EXPECT_EQ(leftColors[2], LedColor(0, 0, 0));
    left.process(); EXPECT_EQ(leftColors[2], LedColor(0, 20, 0)); EXPECT_FALSE(left.isInProgress());

    LedColor rightColors[] = {{10, 0, 0}, {0, 20, 0}, {0, 0, 30}};
    RollInAnimation right(rightColors, 3, Direction::RightToLeft); right.start(true); right.process();
    EXPECT_EQ(rightColors[2], LedColor(0, 0, 30));

    LedColor outColors[] = {{10, 0, 0}, {0, 20, 0}, {0, 0, 30}};
    RollOutAnimation rollOut(outColors, 3, Direction::LeftToRight); rollOut.start(false);
    while (rollOut.isInProgress()) rollOut.process();
    EXPECT_EQ(outColors[0], LedColor(0, 0, 0)); rollOut.restoreColors(); EXPECT_EQ(outColors[0], LedColor(10, 0, 0));
}

TEST_CASE("Additional finite LED animations complete without corrupting target bounds")
{
    LedColor colors[] = {{20, 30, 40}, {40, 30, 20}, {10, 10, 10}, {5, 15, 25}, {25, 15, 5}};
    BounceInAnimation bounce(colors, 5, 2); bounce.start(true);
    for (int step = 0; step < 20 && bounce.isInProgress(); ++step) bounce.process();
    EXPECT_FALSE(bounce.isInProgress()); bounce.restoreColors(); EXPECT_EQ(colors[0], LedColor(20, 30, 40));

    SparkleInAnimation sparkle(colors, 5); sparkle.start(true);
    for (int step = 0; step < 1000 && sparkle.isInProgress(); ++step) sparkle.process();
    EXPECT_FALSE(sparkle.isInProgress()); sparkle.restoreColors();

    TwinkleInAnimation twinkle(colors, 5); twinkle.start(true);
    for (int step = 0; step < 1000 && twinkle.isInProgress(); ++step) twinkle.process();
    EXPECT_FALSE(twinkle.isInProgress()); twinkle.restoreColors();

}

TEST_CASE("WaveInAnimation eventually completes")
{
    LedColor colors[] = {{20, 30, 40}, {40, 30, 20}, {10, 10, 10}, {5, 15, 25}, {25, 15, 5}};
    WaveInAnimation wave(colors, 5); wave.start(true);
    for (int step = 0; step < 1000 && wave.isInProgress(); ++step) wave.process();
    EXPECT_FALSE(wave.isInProgress());
}

TEST_CASE("Live wave animation restores the original strip on stop")
{
    LedColor colors[] = {{100, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 100}};
    const std::array<LedColor, 4> original = {colors[0], colors[1], colors[2], colors[3]};
    SingleWaveAnimation animation(2, SingleWaveAnimation::Direction::LeftToRight, 0.25f);
    animation.initialize(colors, 4); animation.start(); animation.process();
    EXPECT_TRUE(animation.isRunning()); EXPECT_EQ(colors[2], LedColor(0, 0, 0));
    animation.stop(); EXPECT_FALSE(animation.isRunning());
    for (size_t index = 0; index < original.size(); ++index) EXPECT_EQ(colors[index], original[index]);
}

TEST_CASE("LED strip validates extended memory and publishes averaged colors")
{
    ArduinoFake::reset(); bool changed = false; LedWS1228bDeviceType strip(ledConfig(3), [&] { changed = true; });
    EXPECT_TRUE(strip.isStripInitialized()); EXPECT_EQ(strip.getExtendedMemoryLength(), uint16_t(36));
    std::array<LedColor, 12> memory;
    for (auto& color : memory) color = LedColor(30, 60, 90);
    auto invalid = ledParams(reinterpret_cast<uint8_t*>(memory.data()), 35, e_IN_to_DEVICE);
    EXPECT_EQ(strip.service(DEVSERVICE_SET_EXT_MEMORY_PTR, invalid), SERV_EXECUTION_FAILURE);
    auto valid = ledParams(reinterpret_cast<uint8_t*>(memory.data()), 36, e_IN_to_DEVICE);
    EXPECT_EQ(strip.service(DEVSERVICE_SET_EXT_MEMORY_PTR, valid), SERV_SUCCESS);
    auto description = strip.getDeviceDescription(); EXPECT_EQ(description.customBytes[0], uint8_t(3));
    EXPECT_EQ(description.customBytes[2], uint8_t(30)); EXPECT_EQ(description.customBytes[3], uint8_t(60));
    EXPECT_EQ(description.customBytes[4], uint8_t(90));

    EXPECT_EQ(strip.service(DEVSERVICE_LED_STRIP_SAVE_CONTENT, ServiceParameters_set1{.a = 1}), SERV_SUCCESS);
    EXPECT_TRUE(changed);
}

TEST_CASE("LED strip reports busy during animation and then returns its payload")
{
    ArduinoFake::reset(); LedWS1228bDeviceType strip(ledConfig(2), [] {});
    std::array<LedColor, 8> memory; for (auto& color : memory) color = LedColor(1, 2, 3);
    EXPECT_EQ(strip.service(DEVSERVICE_SET_EXT_MEMORY_PTR,
                           ledParams(reinterpret_cast<uint8_t*>(memory.data()), 24, e_IN_to_DEVICE)), SERV_SUCCESS);
    std::array<uint8_t, 11> payload = {0, 0, 1, 0, 0, 10, 20, 30, 40, 50, 60};
    EXPECT_EQ(strip.service(DEVSERVICE_SET_ADVANCED_CONTROLS, ledParams(payload.data(), payload.size(), e_IN_to_DEVICE)), SERV_SUCCESS);
    std::array<uint8_t, 11> output = {};
    EXPECT_EQ(strip.service(DEVSERVICE_GET_ADVANCED_CONTROLS, ledParams(output.data(), output.size(), e_OUT_from_DEVICE)), SERV_BUSY);
    for (int tick = 0; tick < 30; ++tick) strip.cyclic();
    EXPECT_EQ(strip.service(DEVSERVICE_GET_ADVANCED_CONTROLS, ledParams(output.data(), output.size(), e_OUT_from_DEVICE)), SERV_SUCCESS);
    EXPECT_TRUE(std::memcmp(payload.data(), output.data(), payload.size()) == 0);
    EXPECT_EQ(strip.service(DEVSERVICE_GET_ADVANCED_CONTROLS, ledParams(output.data(), 10, e_OUT_from_DEVICE)), SERV_EXECUTION_FAILURE);
}

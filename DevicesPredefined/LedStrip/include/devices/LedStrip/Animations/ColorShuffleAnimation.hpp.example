#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include "ILedAnimation.hpp"
/**
 * @class ColorShuffleAnimation
 * @brief Enable animation: the strip erupts in flowing random colors, then LEDs lock one by one into the target colors.
 */

class ColorShuffleAnimation : public ILedAnimation
{
public:
    ColorShuffleAnimation(LedColor *mainStripPtr, uint16_t ledsCount);
    ~ColorShuffleAnimation() override;

    void initialize(LedColor *mainStripPtr, uint16_t ledsCount) override;
    void start(bool startFromZero) override;
    void process() override;
    bool isInProgress() override;
    void restoreColors() override;

private:
    static const uint8_t chaosSteps = 14;

    uint16_t *order = nullptr;
    uint16_t *hue = nullptr;
    bool *locked = nullptr;
    uint8_t *flash = nullptr;
    uint16_t step = 0;
    uint16_t nextLock = 0;
    uint8_t locksPerStep = 1;
    bool inProgress = false;
};

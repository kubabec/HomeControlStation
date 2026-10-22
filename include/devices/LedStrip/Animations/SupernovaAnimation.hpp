#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include "ILedAnimation.hpp"
/**
 * @class SupernovaAnimation
 * @brief Disable animation: two white-hot comets devour the strip from both ends and collide in a fading supernova flash.
 */

class SupernovaAnimation : public ILedAnimation
{
public:
    SupernovaAnimation(LedColor *mainStripPtr, uint16_t ledsCount);
    ~SupernovaAnimation() override = default;

    void initialize(LedColor *mainStripPtr, uint16_t ledsCount) override;
    void start(bool startFromZero) override;
    void process() override;
    bool isInProgress() override;
    void restoreColors() override;

private:
    static const uint8_t tailLength = 5;
    static const uint8_t burstSteps = 9;

    int16_t leftHead = 0;
    int16_t rightHead = 0;
    uint8_t speed = 2;
    uint8_t burstStep = 0;
    uint16_t center = 0;
    bool bursting = false;
    bool inProgress = false;
};

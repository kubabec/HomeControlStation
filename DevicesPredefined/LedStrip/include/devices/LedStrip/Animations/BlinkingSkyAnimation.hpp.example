#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include "ILedAnimation.hpp"
/**
 * @class BlinkingSkyAnimation
 * @brief Disable animation: the strip turns into a twinkling night sky whose stars burn out one by one.
 */

class BlinkingSkyAnimation : public ILedAnimation
{
public:
    BlinkingSkyAnimation(LedColor *mainStripPtr, uint16_t ledsCount);
    ~BlinkingSkyAnimation() override;

    void initialize(LedColor *mainStripPtr, uint16_t ledsCount) override;
    void start(bool startFromZero) override;
    void process() override;
    bool isInProgress() override;
    void restoreColors() override;

private:
    static const uint8_t nightFallSteps = 12;
    static const uint8_t burnOutSpan = 28;

    uint16_t *skyAt = nullptr;   /* step when the LED turns into the night sky */
    uint16_t *deathAt = nullptr; /* step when the star blinks one last time */
    uint8_t *nightRamp = nullptr;
    uint8_t *spark = nullptr;
    uint8_t *tint = nullptr;
    uint16_t step = 0;
    bool inProgress = false;
};

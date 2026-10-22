#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include "ILedAnimation.hpp"
/**
 * @class FallingStarsAnimation
 * @brief Enable animation: comets streak in from the strip ends and flash into the target colors where they land.
 */

class FallingStarsAnimation : public ILedAnimation
{
public:
    FallingStarsAnimation(LedColor *mainStripPtr, uint16_t ledsCount);
    ~FallingStarsAnimation() override;

    void initialize(LedColor *mainStripPtr, uint16_t ledsCount) override;
    void start(bool startFromZero) override;
    void process() override;
    bool isInProgress() override;
    void restoreColors() override;

private:
    struct Star
    {
        int16_t position;
        int8_t direction;
        uint16_t target;
        bool active;
    };

    static const uint8_t maxStars = 40;
    static const uint8_t tailLength = 4;

    uint16_t *order = nullptr;
    bool *landed = nullptr;
    uint8_t *flash = nullptr;
    Star stars[maxStars];
    uint16_t nextLaunch = 0;
    uint8_t launchesPerStep = 1;
    uint8_t speed = 3;
    bool inProgress = false;
};

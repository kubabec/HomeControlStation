#pragma once
#include <cstdint>
#include <cstring>
#include "ILedAnimation.hpp"

// RollOutAnimation: gradually lights LEDs from off to target colors, skipping fully off targets
class RollOutAnimation : public ILedAnimation {
public:
    RollOutAnimation(LedColor* mainStripPtr, uint16_t ledsCount, Direction dir = Direction::LeftToRight);
    ~RollOutAnimation() override;

    void initialize(LedColor* mainStripPtr, uint16_t ledsCount) override;

    void start(bool /*startFromZero*/) override ;

    void process() override ;

    bool isInProgress() override ;

    void restoreColors() override ;

private:
    Direction direction;
    bool inProgress;
    int currentPos;
    LedColor* mainStrip;
};
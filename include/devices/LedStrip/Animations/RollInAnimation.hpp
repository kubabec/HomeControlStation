#pragma once
#include <cstdint>
#include <cstring>
#include "ILedAnimation.hpp"

// RollInAnimation: gradually lights LEDs from off to target colors, skipping fully off targets
class RollInAnimation : public ILedAnimation {
public:
    RollInAnimation(LedColor* mainStripPtr, uint16_t ledsCount, Direction dir = Direction::LeftToRight);
    ~RollInAnimation() override;

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
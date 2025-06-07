#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "ILedAnimation.hpp"

class BounceInAnimation : public ILedAnimation {
public:
    BounceInAnimation(LedColor* mainStripPtr, uint16_t ledsCount, uint16_t center = 0);
    ~BounceInAnimation() = default;

    void initialize(LedColor* mainStripPtr, uint16_t ledsCount) override;
    void start(bool) override;
    void process() override ;
    bool isInProgress() override;
    void restoreColors() override ;

private:
    uint16_t center;
    int radius;
    bool inProgress = false;
    uint16_t ledsCount;
    LedColor* mainStrip = nullptr;
    LedColor* animationBuffer = nullptr;
};
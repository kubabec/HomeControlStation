#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "ILedAnimation.hpp"

class TwinkleInAnimation : public ILedAnimation {
public:
    TwinkleInAnimation(LedColor* mainStripPtr, uint16_t ledsCount);
    ~TwinkleInAnimation() = default;

    void initialize(LedColor* mainStripPtr, uint16_t ledsCount) override;
    void start(bool) override;
    void process() override ;
    bool isInProgress() override;
    void restoreColors() override ;

private:
    bool inProgress = false;
    uint16_t ledsCount;
    LedColor* mainStrip = nullptr;
    LedColor* animationBuffer = nullptr;
    std::vector<bool> remaining;
};
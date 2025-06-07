#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "ILedAnimation.hpp"

class WaveInAnimation : public ILedAnimation {
public:
    WaveInAnimation(LedColor* mainStripPtr, uint16_t ledsCount, float wavelength = 10.0f, float speed = 0.5f);
    ~WaveInAnimation() = default;

    void initialize(LedColor* mainStripPtr, uint16_t ledsCount) override;
    void start(bool) override;
    void process() override ;
    bool isInProgress() override;
    void restoreColors() override ;

private:
    float wavelength, speed, time;
    bool inProgress = false;
    uint16_t ledsCount;
    LedColor* mainStrip = nullptr;
    LedColor* animationBuffer = nullptr;
};
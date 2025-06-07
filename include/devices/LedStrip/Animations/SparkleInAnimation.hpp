#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "ILedAnimation.hpp"

class SparkleInAnimation : public ILedAnimation {
public:
    SparkleInAnimation(LedColor* mainStripPtr, uint16_t ledsCount, Direction dir = Direction::LeftToRight, uint8_t sparkleCount = 5);
    ~SparkleInAnimation() = default;

    void initialize(LedColor* mainStripPtr, uint16_t ledsCount) override;
    void start(bool) override;
    void process() override ;
    bool isInProgress() override;
    void restoreColors() override ;

private:
    Direction direction;
    bool inProgress = false;
    int position;
    LedColor* mainStrip;
    LedColor* animationBuffer;
    uint32_t rngState;
    uint8_t sparkleCount;
    static uint32_t randR(uint32_t* state) {
        *state = (*state * 1103515245 + 12345) & 0x7fffffff;
        return *state;
    }
};
#include "devices/LedStrip/Animations/RollOutAnimation.hpp"

RollOutAnimation::RollOutAnimation(LedColor* mainStripPtr, uint16_t ledsCount, Direction dir)
    : direction(dir), inProgress(false), currentPos(0) 
    {
        initialize(mainStripPtr, ledsCount);
    }
RollOutAnimation::~RollOutAnimation()  {
    }

void RollOutAnimation::initialize(LedColor* mainStripPtr, uint16_t ledsCount)  {
    this->ledsCount = ledsCount;
    this->mainStrip = mainStripPtr;
    // copy current (target) colors
    animationBuffer = new LedColor[ledsCount];
    std::memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
    // ensure strip starts fully lit
    std::memcpy(mainStripPtr, animationBuffer, ledsCount * sizeof(LedColor));
}

void RollOutAnimation::start(bool /*unused*/)  {
    inProgress = true;
    currentPos = (direction == Direction::LeftToRight) ? 0 : (ledsCount - 1);
}

void RollOutAnimation::process()  {
    if (!inProgress) return;
    // advance to next lit LED
    bool found = false;
    while (!found && inProgress) {
        const LedColor &orig = animationBuffer[currentPos];
        // if originally lit, turn off
        if (orig.r != 0 || orig.g != 0 || orig.b != 0) {
            mainStrip[currentPos] = LedColor(0, 0, 0);
            found = true;
        }
        // move index
        if (direction == Direction::LeftToRight) {
            ++currentPos;
            if (currentPos >= ledsCount) inProgress = false;
        } else {
            if (currentPos == 0) {
                inProgress = false;
            } else {
                --currentPos;
            }
        }
    }
}

bool RollOutAnimation::isInProgress()  {
    return inProgress;
}

void RollOutAnimation::restoreColors()  {
    // restore full target colors
    std::memcpy(mainStrip, animationBuffer, ledsCount * sizeof(LedColor));
}
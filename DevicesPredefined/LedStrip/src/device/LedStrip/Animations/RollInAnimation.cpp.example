#include "devices/LedStrip/Animations/RollInAnimation.hpp"


RollInAnimation::RollInAnimation(LedColor* mainStripPtr, uint16_t ledsCount, Direction dir)
    : direction(dir), inProgress(false), currentPos(0) {
        initialize(mainStripPtr, ledsCount);
    }

RollInAnimation::~RollInAnimation()  {
}

void RollInAnimation::initialize(LedColor* mainStripPtr, uint16_t ledsCount)  {
    this->ledsCount = ledsCount;
    this->mainStrip = mainStripPtr;
    // copy target colors
    animationBuffer = new LedColor[ledsCount];
    std::memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
    // clear strip for animation start
    for (uint16_t i = 0; i < ledsCount; ++i) {
        mainStripPtr[i] = LedColor(0, 0, 0);
    }
}

void RollInAnimation::start(bool /*startFromZero*/)  {
    inProgress = true;
    // set starting index based on direction
    currentPos = (direction == Direction::LeftToRight) ? 0 : (ledsCount - 1);
}

void RollInAnimation::process()  {
    if (!inProgress) return;
    // advance to next non-zero target
    bool found = false;
    while (!found && inProgress) {
        const LedColor &target = animationBuffer[currentPos];
        if (target.r != 0 || target.g != 0 || target.b != 0) {
            // set LED to its target color
            mainStrip[currentPos] = target;
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

bool RollInAnimation::isInProgress() {
    return inProgress;
}

void RollInAnimation::restoreColors() {
    // restore full target colors
    std::memcpy(mainStrip, animationBuffer, ledsCount * sizeof(LedColor));
}

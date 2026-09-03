#include "devices/LedStrip/Animations/BounceInAnimation.hpp"

BounceInAnimation::BounceInAnimation(LedColor* mainStripPtr, uint16_t ledsCount, uint16_t center )
        : center(center) {
            initialize(mainStripPtr, ledsCount);
        }

void BounceInAnimation::initialize(LedColor* mainStripPtr, uint16_t ledsCount)  {
this->ledsCount = ledsCount;
        this->mainStrip = mainStripPtr;
        animationBuffer = new LedColor[ledsCount];
        std::memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
        for (uint16_t i = 0; i < ledsCount; ++i)
            mainStrip[i] = LedColor(0,0,0);
        radius = 0;
        inProgress = true;
}

void BounceInAnimation::start(bool)  { }

void BounceInAnimation::process()  {
if (!inProgress) return;
        inProgress = false;
        for (uint16_t i = 0; i < ledsCount; ++i) {
            if (std::abs(int(i) - int(center)) <= radius)
                mainStrip[i] = animationBuffer[i];
            if (mainStrip[i] != animationBuffer[i]) inProgress = true;
        }
        radius++;
        if (radius > ledsCount) {
            inProgress = false;
            restoreColors();
        }
}

bool BounceInAnimation::isInProgress()  { return inProgress; }

void BounceInAnimation::restoreColors()  {
std::memcpy(mainStrip, animationBuffer, ledsCount * sizeof(LedColor));
}
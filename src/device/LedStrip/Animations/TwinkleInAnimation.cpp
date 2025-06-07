#include "devices/LedStrip/Animations/TwinkleInAnimation.hpp"

TwinkleInAnimation::TwinkleInAnimation(LedColor* mainStripPtr, uint16_t ledsCount){
    initialize(mainStripPtr, ledsCount);
}

void TwinkleInAnimation::initialize(LedColor* mainStripPtr, uint16_t ledsCount)  {
this->ledsCount = ledsCount;
        this->mainStrip = mainStripPtr;
        animationBuffer = new LedColor[ledsCount];
        std::memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
        remaining.assign(ledsCount, true);
        for (uint16_t i = 0; i < ledsCount; ++i)
            mainStrip[i] = LedColor(rand()%256, rand()%256, rand()%256);
        inProgress = true;
}

void TwinkleInAnimation::start(bool)  { }

void TwinkleInAnimation::process()  {
if (!inProgress) return;
if (!inProgress) return;
        inProgress = false;
        for (uint16_t i = 0; i < ledsCount; ++i) {
            if (!remaining[i]) continue;
            LedColor &cur = mainStrip[i];
            LedColor &tgt = animationBuffer[i];
            cur.r = cur.r > tgt.r ? cur.r - 1 : tgt.r;
            cur.g = cur.g > tgt.g ? cur.g - 1 : tgt.g;
            cur.b = cur.b > tgt.b ? cur.b - 1 : tgt.b;
            if (cur.r != tgt.r || cur.g != tgt.g || cur.b != tgt.b) inProgress = true;
            else remaining[i] = false;
        }
        if (!inProgress) {
            restoreColors();
        }
}

bool TwinkleInAnimation::isInProgress()  { return inProgress; }

void TwinkleInAnimation::restoreColors()  {
std::memcpy(mainStrip, animationBuffer, ledsCount * sizeof(LedColor));
}
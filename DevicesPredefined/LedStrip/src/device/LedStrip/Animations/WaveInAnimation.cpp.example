#include "devices/LedStrip/Animations/WaveInAnimation.hpp"

WaveInAnimation::WaveInAnimation(LedColor* mainStripPtr, uint16_t ledsCount, float wavelength, float speed )
: wavelength(wavelength), speed(speed) {
    initialize(mainStripPtr, ledsCount);
}

void WaveInAnimation::initialize(LedColor* mainStripPtr, uint16_t ledsCount)  {
this->ledsCount = ledsCount;
        this->mainStrip = mainStripPtr;
        animationBuffer = new LedColor[ledsCount];
        std::memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
        time = 0.0f;
        inProgress = true;
}

void WaveInAnimation::start(bool)  { }

void WaveInAnimation::process()  {
if (!inProgress) return;
        time += speed;
        bool anyBelowFull = false;
        for (uint16_t i = 0; i < ledsCount; ++i) {
            float phase = (i / wavelength) + time;
            float factor = (std::sin(phase) + 1.0f) * 0.5f;
            if (factor < 1.0f) anyBelowFull = true;
            float clamped = (factor > 1.0f) ? 1.0f : factor;
            mainStrip[i].r = uint8_t(animationBuffer[i].r * clamped);
            mainStrip[i].g = uint8_t(animationBuffer[i].g * clamped);
            mainStrip[i].b = uint8_t(animationBuffer[i].b * clamped);
        }
        if (!anyBelowFull) {
            inProgress = false;
            restoreColors();
        }
}

bool WaveInAnimation::isInProgress()  { return inProgress; }

void WaveInAnimation::restoreColors()  {
std::memcpy(mainStrip, animationBuffer, ledsCount * sizeof(LedColor));
}
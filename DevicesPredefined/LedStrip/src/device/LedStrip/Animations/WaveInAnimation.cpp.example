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
        const float duration = 2.0f * static_cast<float>(M_PI);
        const float progress = std::min(time / duration, 1.0f);
        for (uint16_t i = 0; i < ledsCount; ++i) {
            float phase = (i / wavelength) + time;
            const float wave = (std::sin(phase) + 1.0f) * 0.5f;
            const float clamped = std::min(progress + ((1.0f - progress) * wave), 1.0f);
            mainStrip[i].r = uint8_t(animationBuffer[i].r * clamped);
            mainStrip[i].g = uint8_t(animationBuffer[i].g * clamped);
            mainStrip[i].b = uint8_t(animationBuffer[i].b * clamped);
        }
        if (progress >= 1.0f) {
            inProgress = false;
            restoreColors();
        }
}

bool WaveInAnimation::isInProgress()  { return inProgress; }

void WaveInAnimation::restoreColors()  {
std::memcpy(mainStrip, animationBuffer, ledsCount * sizeof(LedColor));
}
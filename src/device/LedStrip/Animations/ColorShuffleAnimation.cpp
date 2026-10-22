#include "devices/LedStrip/Animations/ColorShuffleAnimation.hpp"
#include "devices/LedStrip/Animations/LedAnimationUtils.hpp"

using namespace LedAnimationUtils;

static const LedColor flashWhite(255, 255, 255);

ColorShuffleAnimation::ColorShuffleAnimation(LedColor *mainStripPtr, uint16_t ledsCount)
{
    initialize(mainStripPtr, ledsCount);
}

ColorShuffleAnimation::~ColorShuffleAnimation()
{
    free(order);
    free(hue);
    free(locked);
    free(flash);
}

void ColorShuffleAnimation::initialize(LedColor *mainStripPtr, uint16_t ledsCount)
{
    this->ledsCount = ledsCount;
    this->colorValues = mainStripPtr;

    animationBuffer = (LedColor *)malloc(sizeof(LedColor) * ledsCount);
    order = (uint16_t *)malloc(sizeof(uint16_t) * ledsCount);
    hue = (uint16_t *)malloc(sizeof(uint16_t) * ledsCount);
    locked = (bool *)calloc(ledsCount, sizeof(bool));
    flash = (uint8_t *)calloc(ledsCount, sizeof(uint8_t));
    if (animationBuffer == nullptr || order == nullptr || hue == nullptr || locked == nullptr || flash == nullptr)
    {
        return; /* isInitialized() stays false, animation is skipped */
    }

    memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
    memset(mainStripPtr, 0, ledsCount * sizeof(LedColor));
    shuffledIndices(order, ledsCount);
    for (uint16_t i = 0; i < ledsCount; ++i)
        hue[i] = rand() % 768;

    locksPerStep = ledsCount / 20 + 1;
    inProgress = ledsCount != 0;
}

void ColorShuffleAnimation::start(bool) {}

void ColorShuffleAnimation::process()
{
    if (!inProgress || !isInitialized())
        return;

    /* after the chaos phase start locking random LEDs into their target colors */
    if (step >= chaosSteps)
    {
        for (uint8_t n = 0; n < locksPerStep && nextLock < ledsCount; ++n)
        {
            uint16_t idx = order[nextLock++];
            locked[idx] = true;
            flash[idx] = 255;
        }
    }

    /* chaos colors ramp up from black so the effect "ignites" */
    uint8_t brightness = step >= 4 ? 255 : (step + 1) * 64 - 1;
    bool settled = nextLock >= ledsCount;
    for (uint16_t i = 0; i < ledsCount; ++i)
    {
        if (locked[i])
        {
            colorValues[i] = mix(animationBuffer[i], flashWhite, flash[i]);
            if (flash[i] != 0)
                settled = false;
            flash[i] = decay(flash[i], 64);
        }
        else
        {
            hue[i] = (hue[i] + 30 + rand() % 60) % 768;
            colorValues[i] = scale(wheel(hue[i]), brightness);
        }
    }

    step++;
    if (settled)
        inProgress = false;
}

bool ColorShuffleAnimation::isInProgress() { return inProgress; }

void ColorShuffleAnimation::restoreColors()
{
    if (isInitialized())
        memcpy(colorValues, animationBuffer, ledsCount * sizeof(LedColor));
}

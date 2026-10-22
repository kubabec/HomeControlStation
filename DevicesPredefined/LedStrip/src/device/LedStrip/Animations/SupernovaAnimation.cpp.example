#include "devices/LedStrip/Animations/SupernovaAnimation.hpp"
#include "devices/LedStrip/Animations/LedAnimationUtils.hpp"

using namespace LedAnimationUtils;

static const LedColor hotWhite(255, 255, 255);

SupernovaAnimation::SupernovaAnimation(LedColor *mainStripPtr, uint16_t ledsCount)
{
    initialize(mainStripPtr, ledsCount);
}

void SupernovaAnimation::initialize(LedColor *mainStripPtr, uint16_t ledsCount)
{
    this->ledsCount = ledsCount;
    this->colorValues = mainStripPtr;

    animationBuffer = (LedColor *)malloc(sizeof(LedColor) * ledsCount);
    if (animationBuffer == nullptr)
    {
        return; /* isInitialized() stays false, animation is skipped */
    }

    memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
    leftHead = 0;
    rightHead = (int16_t)ledsCount - 1;
    speed = ledsCount / 40 + 1;
    inProgress = ledsCount != 0;
}

void SupernovaAnimation::start(bool) {}

void SupernovaAnimation::process()
{
    if (!inProgress || !isInitialized())
        return;

    if (!bursting)
    {
        for (uint16_t i = 0; i < ledsCount; ++i)
        {
            colorValues[i] = (i > leftHead && i < rightHead) ? animationBuffer[i] : LedColor(0, 0, 0);
        }

        /* heads and glowing tails; the tails fade behind the heads over the already devoured area */
        for (uint8_t k = 0; k <= tailLength; ++k)
        {
            LedColor glow = scale(mix(LedColor(0, 0, 0), hotWhite, 255), 255 - k * 45);
            int16_t l = leftHead - k;
            int16_t r = rightHead + k;
            if (l >= 0 && l < (int16_t)ledsCount)
                colorValues[l] = addSaturated(colorValues[l], mix(scale(animationBuffer[l], 255 - k * 45), glow, k == 0 ? 220 : 90));
            if (r >= 0 && r < (int16_t)ledsCount && r != l)
                colorValues[r] = addSaturated(colorValues[r], mix(scale(animationBuffer[r], 255 - k * 45), glow, k == 0 ? 220 : 90));
        }

        leftHead += speed;
        rightHead -= speed;
        if (leftHead >= rightHead)
        {
            bursting = true;
            center = ((leftHead + rightHead) / 2 < 0) ? 0 : (uint16_t)((leftHead + rightHead) / 2);
            if (center >= ledsCount)
                center = ledsCount - 1;
        }
        return;
    }

    /* supernova: an expanding white flash that cools into the original colors and dies out */
    for (uint16_t i = 0; i < ledsCount; ++i)
    {
        int distance = i > center ? i - center : center - i;
        int intensity = 255 - burstStep * 30 - distance * 5;
        if (burstStep >= burstSteps - 1 || distance > burstStep * 3 + 2 || intensity <= 0)
        {
            colorValues[i] = LedColor(0, 0, 0);
            continue;
        }
        colorValues[i] = scale(mix(animationBuffer[i], hotWhite, 170), (uint8_t)intensity);
    }

    burstStep++;
    if (burstStep >= burstSteps)
        inProgress = false; /* the last composed frame is fully black */
}

bool SupernovaAnimation::isInProgress() { return inProgress; }

void SupernovaAnimation::restoreColors()
{
    if (isInitialized())
        memcpy(colorValues, animationBuffer, ledsCount * sizeof(LedColor));
}

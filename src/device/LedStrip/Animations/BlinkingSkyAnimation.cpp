#include "devices/LedStrip/Animations/BlinkingSkyAnimation.hpp"
#include "devices/LedStrip/Animations/LedAnimationUtils.hpp"

using namespace LedAnimationUtils;

static const LedColor nightColor(0, 0, 20);
static const LedColor starPalette[3] = {LedColor(255, 255, 255), LedColor(150, 190, 255), LedColor(255, 225, 150)};

BlinkingSkyAnimation::BlinkingSkyAnimation(LedColor *mainStripPtr, uint16_t ledsCount)
{
    initialize(mainStripPtr, ledsCount);
}

BlinkingSkyAnimation::~BlinkingSkyAnimation()
{
    free(skyAt);
    free(deathAt);
    free(nightRamp);
    free(spark);
    free(tint);
}

void BlinkingSkyAnimation::initialize(LedColor *mainStripPtr, uint16_t ledsCount)
{
    this->ledsCount = ledsCount;
    this->colorValues = mainStripPtr;

    animationBuffer = (LedColor *)malloc(sizeof(LedColor) * ledsCount);
    skyAt = (uint16_t *)malloc(sizeof(uint16_t) * ledsCount);
    deathAt = (uint16_t *)malloc(sizeof(uint16_t) * ledsCount);
    nightRamp = (uint8_t *)calloc(ledsCount, sizeof(uint8_t));
    spark = (uint8_t *)calloc(ledsCount, sizeof(uint8_t));
    tint = (uint8_t *)calloc(ledsCount, sizeof(uint8_t));
    if (animationBuffer == nullptr || skyAt == nullptr || deathAt == nullptr ||
        nightRamp == nullptr || spark == nullptr || tint == nullptr)
    {
        return; /* isInitialized() stays false, animation is skipped */
    }

    /* keep the strip lit, the animation takes over on the first process() call */
    memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
    for (uint16_t i = 0; i < ledsCount; ++i)
    {
        skyAt[i] = rand() % nightFallSteps;
        deathAt[i] = nightFallSteps + 6 + rand() % burnOutSpan;
        tint[i] = rand() % 3;
    }
    inProgress = ledsCount != 0;
}

void BlinkingSkyAnimation::start(bool) {}

void BlinkingSkyAnimation::process()
{
    if (!inProgress || !isInitialized())
        return;

    bool allDark = true;
    for (uint16_t i = 0; i < ledsCount; ++i)
    {
        bool dying = step >= deathAt[i];
        if (dying)
        {
            if (step == deathAt[i])
                spark[i] = 255; /* final blink */
            colorValues[i] = scale(starPalette[tint[i]], spark[i]);
            spark[i] = decay(spark[i], 64);
            if (colorValues[i].r || colorValues[i].g || colorValues[i].b)
                allDark = false;
            continue;
        }

        allDark = false;
        if (step < skyAt[i])
        {
            colorValues[i] = animationBuffer[i];
            continue;
        }

        nightRamp[i] = nightRamp[i] > 255 - 64 ? 255 : nightRamp[i] + 64;
        if (spark[i] == 0 && rand() % 100 < 18)
        {
            spark[i] = 255;
            tint[i] = rand() % 3;
        }
        LedColor sky = mix(animationBuffer[i], mix(scale(animationBuffer[i], 40), nightColor, 200), nightRamp[i]);
        colorValues[i] = mix(sky, starPalette[tint[i]], spark[i]);
        spark[i] = decay(spark[i], 85);
    }

    step++;
    if (allDark)
        inProgress = false; /* the last composed frame is fully black */
}

bool BlinkingSkyAnimation::isInProgress() { return inProgress; }

void BlinkingSkyAnimation::restoreColors()
{
    if (isInitialized())
        memcpy(colorValues, animationBuffer, ledsCount * sizeof(LedColor));
}

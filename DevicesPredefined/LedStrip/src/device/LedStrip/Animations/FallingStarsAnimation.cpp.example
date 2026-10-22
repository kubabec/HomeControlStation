#include "devices/LedStrip/Animations/FallingStarsAnimation.hpp"
#include "devices/LedStrip/Animations/LedAnimationUtils.hpp"

using namespace LedAnimationUtils;

static const LedColor starWhite(255, 255, 255);

FallingStarsAnimation::FallingStarsAnimation(LedColor *mainStripPtr, uint16_t ledsCount)
{
    initialize(mainStripPtr, ledsCount);
}

FallingStarsAnimation::~FallingStarsAnimation()
{
    free(order);
    free(landed);
    free(flash);
}

void FallingStarsAnimation::initialize(LedColor *mainStripPtr, uint16_t ledsCount)
{
    this->ledsCount = ledsCount;
    this->colorValues = mainStripPtr;

    animationBuffer = (LedColor *)malloc(sizeof(LedColor) * ledsCount);
    order = (uint16_t *)malloc(sizeof(uint16_t) * ledsCount);
    landed = (bool *)calloc(ledsCount, sizeof(bool));
    flash = (uint8_t *)calloc(ledsCount, sizeof(uint8_t));
    if (animationBuffer == nullptr || order == nullptr || landed == nullptr || flash == nullptr)
    {
        return; /* isInitialized() stays false, animation is skipped */
    }

    memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
    memset(mainStripPtr, 0, ledsCount * sizeof(LedColor));
    shuffledIndices(order, ledsCount);
    for (uint8_t i = 0; i < maxStars; ++i)
        stars[i].active = false;

    launchesPerStep = ledsCount / 14 + 1;
    speed = ledsCount / 40 + 2;
    inProgress = ledsCount != 0;
}

void FallingStarsAnimation::start(bool) {}

void FallingStarsAnimation::process()
{
    if (!inProgress || !isInitialized())
        return;

    /* launch new stars, each one travels from the end that is farther from its landing spot */
    uint8_t launched = 0;
    for (uint8_t s = 0; s < maxStars && nextLaunch < ledsCount && launched < launchesPerStep; ++s)
    {
        if (stars[s].active)
            continue;
        uint16_t target = order[nextLaunch++];
        bool fromLeft = target >= ledsCount / 2;
        stars[s].target = target;
        stars[s].direction = fromLeft ? 1 : -1;
        stars[s].position = fromLeft ? -(int16_t)(rand() % 3) : (int16_t)(ledsCount - 1 + rand() % 3);
        stars[s].active = true;
        launched++;
    }

    /* move stars and land them */
    uint8_t activeStars = 0;
    for (uint8_t s = 0; s < maxStars; ++s)
    {
        if (!stars[s].active)
            continue;
        stars[s].position += stars[s].direction * speed;
        bool arrived = stars[s].direction > 0 ? stars[s].position >= (int16_t)stars[s].target
                                              : stars[s].position <= (int16_t)stars[s].target;
        if (arrived)
        {
            landed[stars[s].target] = true;
            flash[stars[s].target] = 255;
            stars[s].active = false;
        }
        else
        {
            activeStars++;
        }
    }

    /* compose the frame: landed LEDs fade from white to the target, flying stars glow on top of dark LEDs */
    bool settled = (nextLaunch >= ledsCount) && activeStars == 0;
    for (uint16_t i = 0; i < ledsCount; ++i)
    {
        if (landed[i])
        {
            colorValues[i] = mix(animationBuffer[i], starWhite, flash[i]);
            if (flash[i] != 0)
                settled = false;
            flash[i] = decay(flash[i], 55);
        }
        else
        {
            colorValues[i] = LedColor(0, 0, 0);
        }
    }

    for (uint8_t s = 0; s < maxStars; ++s)
    {
        if (!stars[s].active)
            continue;
        LedColor head = mix(animationBuffer[stars[s].target], starWhite, 200);
        for (uint8_t k = 0; k <= tailLength; ++k)
        {
            int16_t pos = stars[s].position - stars[s].direction * k;
            if (pos < 0 || pos >= (int16_t)ledsCount || landed[pos])
                continue;
            LedColor glow = scale(head, 255 >> k);
            colorValues[pos] = addSaturated(colorValues[pos], glow);
        }
    }

    if (settled)
        inProgress = false;
}

bool FallingStarsAnimation::isInProgress() { return inProgress; }

void FallingStarsAnimation::restoreColors()
{
    if (isInitialized())
        memcpy(colorValues, animationBuffer, ledsCount * sizeof(LedColor));
}

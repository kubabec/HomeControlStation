#pragma once
#include <cstdint>
#include <cstdlib>
#include "devices/AdvancedControls.hpp"

/** Small color helpers shared by the effect-style LED animations. */
namespace LedAnimationUtils
{
/** Linear blend: t = 0 returns a, t = 255 returns b. */
inline LedColor mix(const LedColor &a, const LedColor &b, uint8_t t)
{
    return LedColor(
        (uint8_t)(a.r + ((int)b.r - (int)a.r) * t / 255),
        (uint8_t)(a.g + ((int)b.g - (int)a.g) * t / 255),
        (uint8_t)(a.b + ((int)b.b - (int)a.b) * t / 255));
}

/** Scales brightness: s = 255 keeps the color, s = 0 returns black. */
inline LedColor scale(const LedColor &c, uint8_t s)
{
    return LedColor((uint8_t)(c.r * s / 255), (uint8_t)(c.g * s / 255), (uint8_t)(c.b * s / 255));
}

/** Per-channel saturating addition. */
inline LedColor addSaturated(const LedColor &a, const LedColor &b)
{
    int r = a.r + b.r, g = a.g + b.g, bl = a.b + b.b;
    return LedColor(r > 255 ? 255 : r, g > 255 ? 255 : g, bl > 255 ? 255 : bl);
}

/** Fully saturated rainbow color for a position on a 768-step color wheel. */
inline LedColor wheel(uint16_t position)
{
    position %= 768;
    uint8_t segment = position / 256;
    uint8_t x = position % 256;
    switch (segment)
    {
    case 0:
        return LedColor(255 - x, x, 0);
    case 1:
        return LedColor(0, 255 - x, x);
    default:
        return LedColor(x, 0, 255 - x);
    }
}

/** Fisher-Yates shuffle of 0..count-1 into order. */
inline void shuffledIndices(uint16_t *order, uint16_t count)
{
    for (uint16_t i = 0; i < count; ++i)
        order[i] = i;
    for (uint16_t i = count; i > 1; --i)
    {
        uint16_t j = rand() % i;
        uint16_t tmp = order[i - 1];
        order[i - 1] = order[j];
        order[j] = tmp;
    }
}

inline uint8_t decay(uint8_t value, uint8_t step)
{
    return value > step ? value - step : 0;
}
} // namespace LedAnimationUtils

#ifndef ADVANCED_CONTROLS_H
#define ADVANCED_CONTROLS_H
#include <Arduino.h>
enum class Direction {
    LeftToRight,
    RightToLeft
};

struct AdvancedControlsOnOff{
    enum OnOffAnimations{
        ONOFF_SWITCH = 0,
        ONOFF_FADE,
        ONOFF_BLINK
    };

    uint8_t switchOnAnimation = OnOffAnimations::ONOFF_SWITCH;
    uint8_t switchOffAnimation = OnOffAnimations::ONOFF_SWITCH;
    uint16_t switchAnimationTime = 0;

};

struct LedStripAnimationProperties{
    uint8_t enableAnimation = 0; // 0 - switch, 1 - fade, 2 - blink 
    uint8_t disableAnimation = 0; // 0 - switch, 1 - fade, 2 - blink
    uint8_t animationSpeed = 0; // 0 - slow, 1 - medium, 2 - fast
    uint8_t liveAnimation = 0; 
    uint8_t liveAnimationSpeed = 0; // 0 - slow, 1 - medium, 2 - fast
};

struct LedColor{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    LedColor(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) 
        : r(red), g(green), b(blue) {}

            // operator== przyda się do implementacji operator!=
    bool operator==(const LedColor& o) const {
        return r == o.r && g == o.g && b == o.b;
    }

    // operator!=
    bool operator!=(const LedColor& o) const {
        return !(*this == o);
    }
};

#endif
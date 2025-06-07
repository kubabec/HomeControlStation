#ifndef ADVANCED_CONTROLS_H
#define ADVANCED_CONTROLS_H
#include <Arduino.h>


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
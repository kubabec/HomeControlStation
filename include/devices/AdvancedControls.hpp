/**
 * @file AdvancedControls.hpp
 * @brief Shared control and data structures used by smart devices and LED animations.
 */
#ifndef ADVANCED_CONTROLS_H
#define ADVANCED_CONTROLS_H
#include <Arduino.h>

/** Defines the direction used by motion and animation helpers. */
enum class Direction {
    LeftToRight,
    RightToLeft
};

/**
 * @brief Configuration describing how an on/off device transitions between states.
 */
struct AdvancedControlsOnOff{
    /** Animation variants available for on/off transitions. */
    enum OnOffAnimations{
        ONOFF_SWITCH = 0,
        ONOFF_FADE,
        ONOFF_BLINK
    };

    /** Animation used when switching the device on. */
    uint8_t switchOnAnimation = OnOffAnimations::ONOFF_SWITCH;
    /** Animation used when switching the device off. */
    uint8_t switchOffAnimation = OnOffAnimations::ONOFF_SWITCH;
    /** Duration of the selected animation in milliseconds. */
    uint16_t switchAnimationTime = 0;

};

/**
 * @brief Runtime configuration of LED strip animation behavior.
 */
struct LedStripAnimationProperties{
    /** Animation type used when enabling the strip. */
    uint8_t enableAnimation = 0; // 0 - switch, 1 - fade, 2 - blink 
    /** Animation type used when disabling the strip. */
    uint8_t disableAnimation = 0; // 0 - switch, 1 - fade, 2 - blink
    /** Overall speed of the animation cycle. */
    uint8_t animationSpeed = 0; // 0 - slow, 1 - medium, 2 - fast
    /** Enum value selecting the currently active live animation pattern. */
    uint8_t liveAnimation = 0; 
    /** Speed for the live animation effect. */
    uint8_t liveAnimationSpeed = 0; // 0 - slow, 1 - medium, 2 - fast
};

/**
 * @brief Represents a single RGB LED color value.
 */
struct LedColor{
    /** Red channel intensity. */
    uint8_t r = 0;
    /** Green channel intensity. */
    uint8_t g = 0;
    /** Blue channel intensity. */
    uint8_t b = 0;

    /**
     * @brief Creates a color using explicit byte channels.
     * @param red Red component.
     * @param green Green component.
     * @param blue Blue component.
     */
    LedColor(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) 
        : r(red), g(green), b(blue) {}

    /**
     * @brief Compares two color values for equality.
     * @param o The color to compare against.
     * @return True when all RGB channels match.
     */
    bool operator==(const LedColor& o) const {
        return r == o.r && g == o.g && b == o.b;
    }

    /**
     * @brief Compares two color values for inequality.
     * @param o The color to compare against.
     * @return True when at least one RGB channel differs.
     */
    bool operator!=(const LedColor& o) const {
        return !(*this == o);
    }
};

#endif
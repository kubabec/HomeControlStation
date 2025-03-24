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


#endif
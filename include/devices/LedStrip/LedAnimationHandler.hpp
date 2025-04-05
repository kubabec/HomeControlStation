#ifndef LED_ANIMATION_HANDLER_H
#define LED_ANIMATION_HANDLER_H
#include <Arduino.h>
#include "devices/AdvancedControls.hpp"
#include <map>

enum LedAnimations{
    eLED_ANIM_FADE_OUT,
    eLED_ANIM_FADE_IN,
    eLED_ANIM_ROLL_IN
};


class LedAnimationHandler{
    enum LedAnimationState{
        eLED_ANIM_IDLE,
        eLED_ANIM_IN_PROGRESS,
        eLED_ANIM_FINISHED
    };

    /* number of led items to animate */
    uint16_t m_ledsCount = 0;
    /* real color values */
    const LedColor* m_activeColorsArray = nullptr;
    /* Buffer with temporary animated values of the colors*/
    LedColor* m_animatedColorsBuffer = nullptr;


    public:
    LedAnimationHandler(
        uint16_t ledsCount,
        const LedColor* activeColors
    );

    ~LedAnimationHandler();

    void cyclicTask();
    void startAnimation(LedAnimations animation);


    bool hasPendingColorsToApply();
    std::map<uint16_t, LedColor>& getColorsToApply();
};



#endif
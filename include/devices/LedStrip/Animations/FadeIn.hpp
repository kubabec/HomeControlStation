#ifndef FADE_IN_H
#define FADE_IN_H
#include "ILedAnimation.hpp"

class FadeInAnimation : public ILedAnimation
{
    void processDiode(uint16_t diodeIdx);

public:
    FadeInAnimation() = default;
    FadeInAnimation(
        LedColor* colorsPtr,
        uint16_t ledsCount)
    ;

    virtual void initialize(
        LedColor* colorsPtr,
        uint16_t ledsCount
    );
    virtual void start();
    virtual void process();

};

#endif
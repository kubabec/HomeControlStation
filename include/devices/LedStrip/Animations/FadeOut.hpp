#ifndef FADE_OUT_H
#define FADE_OUT_H
#include "ILedAnimation.hpp"

class FadeOutAnimation : public ILedAnimation
{
    enum FadeOutColor{
        Red,
        Green,
        Blue
    };

    bool inProgress = false;

    bool processDiode(uint16_t diodeIdx);
    bool processColor(FadeOutColor color, uint16_t diodeIndex);

public:
    FadeOutAnimation() = default;
    FadeOutAnimation(
        LedColor* colorsPtr,
        uint16_t ledsCount)
    ;

    virtual void initialize(
        LedColor* colorsPtr,
        uint16_t ledsCount
    );
    virtual void start();
    virtual void process();
    virtual bool isInProgress();
    virtual void restoreColors();

};

#endif
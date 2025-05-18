#ifndef FADE_IN_H
#define FADE_IN_H
#include "ILedAnimation.hpp"

class FadeInAnimation : public ILedAnimation
{
    enum FadeInColor{
        Red,
        Green,
        Blue
    };

    bool inProgress = false;

    bool processDiode(uint16_t diodeIdx);
    bool processColor(FadeInColor color, uint16_t diodeIndex);

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
    virtual void start(bool startFromZero);
    virtual void process();
    virtual bool isInProgress();
    virtual void restoreColors();

};

#endif

#ifndef ILED_ANIMATION_H
#define ILED_ANIMATION_H
#include "devices/AdvancedControls.hpp"

class ILedAnimation {
protected:
    uint16_t ledsCount;
    LedColor* colorValues = nullptr;
    LedColor* animationBuffer = nullptr;

    bool isInitialized();

public:
    virtual void initialize(
        LedColor* mainStripPtr,
        uint16_t ledsCount
    ) = 0;
    virtual void start(bool startFromZero) = 0;
    virtual void process() = 0;
    virtual bool isInProgress() = 0;
    virtual void restoreColors() = 0;

    virtual ~ILedAnimation();
};

#endif
#include "devices/LedStrip/Animations/ILedAnimation.hpp"

bool ILedAnimation::isInitialized()
{
    return (this->ledsCount != 0 && this->colorValues != nullptr && animationBuffer != nullptr);
}

ILedAnimation::~ILedAnimation(){
    if(animationBuffer != nullptr){
        free(animationBuffer);
    }
}
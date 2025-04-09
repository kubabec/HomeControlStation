#include <devices/LedStrip/Animations/FadeIn.hpp>

#define COLOR_INCR_STEP 10

FadeInAnimation::FadeInAnimation(
    LedColor* colorsPtr,
    uint16_t ledsCount)
{
    this->ledsCount = ledsCount;
    this->colorValues = colorsPtr;

    animationBuffer = (LedColor*)malloc(sizeof(LedColor) * ledsCount);
}

void FadeInAnimation::initialize(
    LedColor* colorsPtr,
    uint16_t ledsCount
){
    this->ledsCount = ledsCount;
    this->colorValues = colorsPtr;
    
    if(animationBuffer != nullptr){
        animationBuffer = (LedColor*)malloc(sizeof(LedColor) * ledsCount);
    }
}

void FadeInAnimation::start(){
    if(isInitialized()){
        memset(animationBuffer, 0, sizeof(LedColor)*ledsCount);
    }
}

void FadeInAnimation::processDiode(uint16_t diodeIdx){
    uint8_t redIncrement = 0;
    uint8_t greenIncrement = 0;
    uint8_t blueIncrement = 0;

    if((colorValues->r - animationBuffer->r) < COLOR_INCR_STEP){
        colorValues->r = animationBuffer->r;
    }else {
        colorValues->r = (animationBuffer->r + COLOR_INCR_STEP);
    }
}

void FadeInAnimation::process(){
    if(isInitialized()){
        for(uint16_t i = 0 ; i < ledsCount; i++){
            processDiode(i);
        }
    }
}
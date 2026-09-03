#include <devices/LedStrip/Animations/FadeOut.hpp>

#define COLOR_INCR_STEP 10

FadeOutAnimation::FadeOutAnimation(
    LedColor* colorsPtr,
    uint16_t ledsCount)
{
    this->ledsCount = ledsCount;
    this->colorValues = colorsPtr;

    animationBuffer = (LedColor*)malloc(sizeof(LedColor) * ledsCount);
}

void FadeOutAnimation::initialize(
    LedColor* colorsPtr,
    uint16_t ledsCount
){
    this->ledsCount = ledsCount;
    this->colorValues = colorsPtr;

    if(animationBuffer != nullptr){
        animationBuffer = (LedColor*)malloc(sizeof(LedColor) * ledsCount);
    }
}

void FadeOutAnimation::start(bool startFromZero){
    if(isInitialized()){
        //memset(animationBuffer, 0, sizeof(LedColor)*ledsCount);
        /* save target values in the buffer */
        memcpy(animationBuffer, colorValues, ledsCount*sizeof(LedColor)); 
        inProgress = true;
    }
}

bool FadeOutAnimation::isInProgress()
{
    return inProgress;
}

bool FadeOutAnimation::processColor(FadeOutColor color, uint16_t diodeIndex){
    uint8_t* targetLedColor = nullptr;

    bool retVal = false;


    switch(color){
        case FadeOutColor::Red:
            targetLedColor = &colorValues[diodeIndex].r;
        break;

        case FadeOutColor::Green:
            targetLedColor = &colorValues[diodeIndex].g;
        break;

        case FadeOutColor::Blue:
            targetLedColor = &colorValues[diodeIndex].b;
        break;

        default: break;
    }
    int targetValue = (*targetLedColor - COLOR_INCR_STEP) > 0 ? ((*targetLedColor - COLOR_INCR_STEP)) : 0;
    if(targetValue == 0){
        retVal = true; /* color change completed */
    }

    if(targetLedColor != nullptr){

        *targetLedColor = targetValue;

    }else {
        inProgress = false;

        retVal = true; /* failure */
    }

    return retVal;
}

bool FadeOutAnimation::processDiode(uint16_t diodeIdx){
    bool redFinished = processColor(Red, diodeIdx);
    bool greenFinished = processColor(Green, diodeIdx);
    bool blueFinished = processColor(Blue, diodeIdx);

    return (redFinished && greenFinished && blueFinished);
}

void FadeOutAnimation::process(){
    if(isInitialized()){
        uint16_t finishedCount = 0;
        for(uint16_t i = 0 ; i < ledsCount; i++){
            if(processDiode(i))
            {
                finishedCount ++;
            }
        }

        if(finishedCount == ledsCount){
            inProgress = false;
        }
    }
}


void FadeOutAnimation::restoreColors()
{
    if(isInitialized()){
        memcpy(colorValues, animationBuffer, ledsCount*sizeof(LedColor));
    }
}
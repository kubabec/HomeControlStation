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

void FadeInAnimation::start(bool startFromZero = false){
    if(isInitialized()){
        //memset(animationBuffer, 0, sizeof(LedColor)*ledsCount);
        /* save target values in the buffer */
        memcpy(animationBuffer, colorValues, ledsCount*sizeof(LedColor)); 
        if(startFromZero){
            memset(colorValues, 0, sizeof(LedColor) * ledsCount);
        }
        inProgress = true;
    }
}

bool FadeInAnimation::isInProgress()
{
    return inProgress;
}

bool FadeInAnimation::processColor(FadeInColor color, uint16_t diodeIndex){
    uint8_t* bufferColor = nullptr;
    uint8_t* targetLedColor = nullptr;

    bool retVal = false;


    switch(color){
        case FadeInColor::Red:
            bufferColor = &animationBuffer[diodeIndex].r;
            targetLedColor = &colorValues[diodeIndex].r;
        break;

        case FadeInColor::Green:
            bufferColor = &animationBuffer[diodeIndex].g;
            targetLedColor = &colorValues[diodeIndex].g;
        break;

        case FadeInColor::Blue:
            bufferColor = &animationBuffer[diodeIndex].b;
            targetLedColor = &colorValues[diodeIndex].b;
        break;

        default: break;
    }

    /* increment or decrement color ? */
    const int increment = (*targetLedColor < *bufferColor) ? COLOR_INCR_STEP : -COLOR_INCR_STEP;
    

    if(bufferColor != nullptr && targetLedColor != nullptr){
        int target = abs(*targetLedColor - *bufferColor) > abs(increment) ? (*targetLedColor + increment) : *bufferColor;
        if(target == *bufferColor){
            retVal = true; /* color change completed */
        }

        *targetLedColor = target;
    }else {
        inProgress = false;

        retVal = true; /* failure */
    }

    return retVal;
}

bool FadeInAnimation::processDiode(uint16_t diodeIdx){
    bool redFinished = processColor(Red, diodeIdx);
    bool greenFinished = processColor(Green, diodeIdx);
    bool blueFinished = processColor(Blue, diodeIdx);

    return (redFinished && greenFinished && blueFinished);
}

void FadeInAnimation::process(){
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


void FadeInAnimation::restoreColors()
{

}
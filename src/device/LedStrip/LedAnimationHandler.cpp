#include "devices/LedStrip/LedAnimationHandler.hpp"

LedAnimationHandler::LedAnimationHandler(
    uint16_t ledsCount,
    const LedColor* activeColors
): m_ledsCount(ledsCount), m_activeColorsArray(activeColors)
{
    m_animatedColorsBuffer = (LedColor*)malloc(ledsCount * sizeof(LedColor));

}

LedAnimationHandler::~LedAnimationHandler(){
    if(m_animatedColorsBuffer != nullptr){
        free(m_animatedColorsBuffer);
    }
}
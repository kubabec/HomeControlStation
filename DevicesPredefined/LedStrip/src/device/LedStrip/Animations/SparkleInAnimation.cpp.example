#include "devices/LedStrip/Animations/SparkleInAnimation.hpp"

SparkleInAnimation::SparkleInAnimation(LedColor* mainStripPtr, uint16_t ledsCount, Direction dir, uint8_t sparkleCount)
        : direction(dir), sparkleCount(sparkleCount) {
            initialize(mainStripPtr, ledsCount);
        }

    void SparkleInAnimation::initialize(LedColor* mainStripPtr, uint16_t ledsCount)  {
        this->ledsCount = ledsCount;
        this->mainStrip = mainStripPtr;
        animationBuffer = new LedColor[ledsCount];
        std::memcpy(animationBuffer, mainStripPtr, ledsCount * sizeof(LedColor));
        for (uint16_t i = 0; i < ledsCount; ++i)
            mainStrip[i] = LedColor(0,0,0);
        position = (direction == Direction::LeftToRight) ? 0 : ledsCount - 1;
        rngState = 20;
        inProgress = true;
    }

    void SparkleInAnimation::start(bool)  { /* no-op */ }

    void SparkleInAnimation::process()  {
        if (!inProgress) return;
        // ustaw docelową diodę
        if (animationBuffer[position].r || animationBuffer[position].g || animationBuffer[position].b)
            mainStrip[position] = animationBuffer[position];
        // losowe iskierki
        for (uint8_t i = 0; i < sparkleCount; ++i) {
            int idx = randR(&rngState) % ledsCount;
            mainStrip[idx] = LedColor(randR(&rngState)%256, randR(&rngState)%256, randR(&rngState)%256);
        }
        // gaszenie iskier
        for (uint16_t i = 0; i < ledsCount; ++i) {
            if (mainStrip[i] != animationBuffer[i])
                mainStrip[i] = LedColor(0,0,0);
        }
        // przesuwanie
        if (direction == Direction::LeftToRight) {
            position++;
            if (position >= ledsCount) inProgress = false;
        } else {
            if (position == 0) inProgress = false;
            else position--;
        }
        if (!inProgress) {
            restoreColors();
        }
    }

    bool SparkleInAnimation::isInProgress()  { return inProgress; }

    void SparkleInAnimation::restoreColors()  {
        std::memcpy(mainStrip, animationBuffer, ledsCount * sizeof(LedColor));
    }

// #include "devices/LedStrip/Animations/live/SingleWaveAnimation.hpp"

// SingleWaveAnimation::SingleWaveAnimation(uint16_t width, Direction dir, uint16_t speedMs)
//     : width(width), dir(dir), speedMs(speedMs) {}

// void SingleWaveAnimation::process()
// {
//     if (!running)
//         return;
//     // czasowa pętla uproszczona: zakładamy, że caller wywołuje w stałych odstępach
//     advance();
//     // czyścimy cały pasek do oryginalnych kolorów
//     std::memcpy(mainStrip, savedState.data(), ledsCount * sizeof(LedColor));
//     // nakładamy blok
//     for (uint16_t i = 0; i < width; ++i)
//     {
//         int idx = pos + (dir == Direction::LeftToRight ? i : -int(i));
//         // odbicie
//         if (idx < 0)
//             idx = -idx;
//         if (idx >= int(ledsCount))
//             idx = 2 * ledsCount - idx - 2;
//         mainStrip[idx] = highlightColor;
//     }
// }

// void SingleWaveAnimation::onStart()
// {
//     pos = (dir == Direction::LeftToRight ? 0 : ledsCount - 1);
// }

// void SingleWaveAnimation::advance()
// {
//     if (dir == Direction::LeftToRight)
//     {
//         if (++pos > int(ledsCount - width))
//         {
//             dir = Direction::RightToLeft;
//             pos = ledsCount - width;
//         }
//     }
//     else
//     {
//         if (--pos < 0)
//         {
//             dir = Direction::LeftToRight;
//             pos = 0;
//         }
//     }
// }

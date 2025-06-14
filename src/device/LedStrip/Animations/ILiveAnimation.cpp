// #include "devices/LedStrip/Animations/ILiveAnimation.hpp"

// ILiveAnimation::~ILiveAnimation() {}

// void ILiveAnimation::initialize(LedColor *stripPtr, uint16_t count)
// {
//     mainStrip = stripPtr;
//     ledsCount = count;
//     savedState.assign(stripPtr, stripPtr + count);
//     initialized = true;
// }
// /// uruchamia (i resetuje wewnętrzne liczniki)
// void ILiveAnimation::start()
// {
//     if (!initialized)
//         return;
//     running = true;
//     onStart();
// }
// /// zatrzymuje i przywraca zapisany stan
// void ILiveAnimation::stop()
// {
//     if (!initialized)
//         return;
//     running = false;
//     std::memcpy(mainStrip, savedState.data(), ledsCount * sizeof(LedColor));
//     onStop();
// }
// /// czy animacja dalej pracuje?
// bool ILiveAnimation::isRunning() const { return running; }

// /// hook do resetu liczników w start()
// void ILiveAnimation::onStart() {}
// /// hook do sprzątania w stop()
// void ILiveAnimation::onStop() {}

// #ifndef ILIVEANIMATION_HPP
// #define ILIVEANIMATION_HPP
// #include <cstdint>
// #include <vector>
// #include <cstring>

// #include "devices/AdvancedControls.hpp"
// // 1. ILiveAnimation
// class ILiveAnimation {
// protected:
//     uint16_t ledsCount = 0;
//     LedColor* mainStrip = nullptr;
//     std::vector<LedColor> savedState;
//     bool initialized = false;
//     bool running = false;

// public:
//     virtual ~ILiveAnimation() {}

//     virtual void initialize(LedColor* stripPtr, uint16_t count) {
//         mainStrip = stripPtr;
//         ledsCount = count;
//         savedState.assign(stripPtr, stripPtr + count);
//         initialized = true;
//     }

//     virtual void start() {
//         if (!initialized) return;
//         running = true;
//         onStart();
//     }

//     virtual void process() = 0;

//     virtual void stop() {
//         if (!initialized) return;
//         running = false;
//         std::memcpy(mainStrip, savedState.data(), ledsCount * sizeof(LedColor));
//         onStop();
//     }

//     bool isRunning() const { return running; }

// protected:
//     virtual void onStart() {}
//     virtual void onStop() {}
// };

// #endif
#pragma once

#include <functional>

class DisplayHardware
{
public:
    static bool init();
    static void cyclic();
    static void deinit();
    static bool isReady();
    static void setTouchCallback(std::function<void()> callback);
};

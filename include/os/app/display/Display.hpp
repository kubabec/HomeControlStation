
#pragma once
#include "os/tools/messageUDP.hpp"
#include "os/app/display/DisplayDatatypes.hpp"

class Display
{

public:
    Display(DisplayInfo info);

    // Comparison operator, taking into account display's unique identifier (MAC address)
    bool operator==(const Display &other) const;

    unsigned long getLastDataUpdateTime();
    uint16_t getDisplayDataHash();

private:
    unsigned long lastDataUpdateTime = 0;
    uint16_t displayDataHash = 0;
    DisplayInfo info;
};
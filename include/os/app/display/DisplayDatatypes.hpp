#pragma once


#include "os/drivers/networkdriver.hpp"


enum class DisplayMessageIds : uint8_t {
    DISCOVER_DISPLAYS = PacketRangeDefinition::DISPLAY_CONTROLS_RANGE + 1,
    DISPLAY_PRESENT,

};


struct DisplayInfo{
    uint64_t macAddress = 0;
    uint16_t resWidth = 480;
    uint16_t resHeight = 480;
    MessageUDP::IPAddr ip{0, 0, 0, 0};
};

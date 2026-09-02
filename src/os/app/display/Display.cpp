#include "os/app/display/Display.hpp"

/**
 * @file src/os/app/display/Display.cpp
 * @brief Display and packing logic for the Home Control Station UI.
 */


Display::Display(DisplayInfo info) : info(info)
{
}

bool Display::operator==(const Display &other) const
{
    // Porównanie na podstawie unikalnego identyfikatora (MAC address)
    return this->info.macAddress == other.info.macAddress;
}

unsigned long Display::getLastDataUpdateTime()
{
    return lastDataUpdateTime;
}

uint16_t Display::getDisplayDataHash()
{
    return displayDataHash;
}
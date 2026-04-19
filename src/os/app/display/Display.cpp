#include "os/app/display/Display.hpp"

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
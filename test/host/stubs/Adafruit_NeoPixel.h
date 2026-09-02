#pragma once

#include <cstdint>
#include <vector>

#define NEO_GRB 1
#define NEO_KHZ800 2

class Adafruit_NeoPixel
{
public:
    Adafruit_NeoPixel(uint16_t count, uint8_t, uint8_t) : pixels_(count, 0) {}
    void begin() { begun_ = true; }
    void setBrightness(uint8_t brightness) { brightness_ = brightness; }
    uint32_t Color(uint8_t red, uint8_t green, uint8_t blue) const
    {
        return (static_cast<uint32_t>(red) << 16) | (static_cast<uint32_t>(green) << 8) | blue;
    }
    void setPixelColor(uint16_t index, uint32_t color)
    {
        if (index < pixels_.size()) pixels_[index] = color;
    }
    void show() { ++showCount_; }

private:
    bool begun_ = false;
    uint8_t brightness_ = 0;
    unsigned showCount_ = 0;
    std::vector<uint32_t> pixels_;
};

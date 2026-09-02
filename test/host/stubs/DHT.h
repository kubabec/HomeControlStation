#pragma once

#include <cstdint>

#define DHT22 22

class DHT
{
public:
    DHT(uint8_t, uint8_t) {}
    void begin() {}
    float readHumidity() { return humidity; }
    float readTemperature() { return temperature; }

    static inline float humidity = 50.0f;
    static inline float temperature = 20.0f;
};

#ifndef EXTENDED_DATA_TYPES_H
#define EXTENDED_DATA_TYPES_H

#include <Arduino.h>

typedef struct {
    uint8_t identifier = 255;
    uint16_t length = 0;
    uint8_t* memoryPtr = nullptr;
}ExtendedData;

#endif
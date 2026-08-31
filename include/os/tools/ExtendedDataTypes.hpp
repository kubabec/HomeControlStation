#ifndef EXTENDED_DATA_TYPES_H
#define EXTENDED_DATA_TYPES_H

#include <Arduino.h>

/**
 * @file include/os/tools/ExtendedDataTypes.hpp
 * @brief Runtime utility types and helpers used by the Home Control Station OS.
 */


typedef struct {
    uint8_t identifier = 255;
    uint16_t length = 0;
    uint8_t* memoryPtr = nullptr;
}ExtendedData;

#endif
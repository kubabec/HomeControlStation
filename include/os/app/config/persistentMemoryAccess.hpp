#ifndef PERSISTENT_MEMORY_ACCESS_H
#define PERSISTENT_MEMORY_ACCESS_H
#include <Arduino.h>


class PersistentMemoryAccess 
{
    static bool eepromInitializedSuccessfully;
    static uint16_t standardDataEepromSize;

public:
    static void init(uint16_t eepromSize);

    static bool saveData(uint8_t* data, uint16_t size);
    static bool readData(uint8_t* buffer, uint16_t size);
    static void massErase(uint16_t eepromSize);

    static void readExtendedMemoryWithOffset(uint16_t offsetToStandardData, uint8_t* buffer, uint16_t size);
    static void writeExtendedMemory(uint16_t offset, uint8_t* buffer, uint16_t size);
    static void flushExtendedMemory();
};


#endif
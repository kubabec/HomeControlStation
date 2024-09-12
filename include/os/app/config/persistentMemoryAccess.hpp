#ifndef PERSISTENT_MEMORY_ACCESS_H
#define PERSISTENT_MEMORY_ACCESS_H
#include <Arduino.h>


class PersistentMemoryAccess 
{
    static bool eepromInitializedSuccessfully;

public:
    static void init(uint16_t eepromSize);

    static bool saveData(uint8_t* data, uint16_t size);
    static bool readData(uint8_t* buffer, uint16_t size);
        
};


#endif
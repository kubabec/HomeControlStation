#ifndef EXTENDED_MEMORY_MANAGER_H
#define EXTENDED_MEMORY_MANAGER_H

#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <vector>

#define MAX_EXT_MEMORY_SIZE_TOTAL 2500 /* bytes */
#define SLOTS_FOR_EXT_MEMORY_BUFFERS 10 /* number of slots for independent data pointers */

typedef struct{
    /* keep 10 for potential slot number increase in the future */
    uint16_t memoryPerDeviceSlotNeeded[SLOTS_FOR_EXT_MEMORY_BUFFERS];
    /* reserved */
    uint8_t reserved[PERSISTENT_DATABLOCK_SIZE - (SLOTS_FOR_EXT_MEMORY_BUFFERS * sizeof(uint16_t))];

}ExtMemoryMetadataType;


typedef struct{
    uint8_t ownerDeviceId = 0xff; /* expected range 1 - 6*/
    uint16_t length = 0;
    uint8_t* dataPtr = nullptr;

    void printExtMem(){
        Serial.println("OwnerID: " + String((int)ownerDeviceId));
        Serial.println("length: " + String((int)length));
        Serial.println("dataPtr: " + String((int)dataPtr));
    }
}ExtMemoryData;


class ExtendedMemoryManager {
    static ExtMemoryMetadataType extMemoryMetadata;
    static uint16_t extMemoryInUse;
    static std::vector<ExtMemoryData> extMemoryContainer;


    static void updateCurrentExtMemoryUsage();
    static void restoreExtMemoryFromNvm();

public: 
    static void init();
    static void deinit();
    static void cyclic();
    static void flushNvmData();

    static bool requestNewExtendedMemorySpace(uint8_t deviceId, uint16_t spaceSize);
    static void releaseExtendedMemorySpace(uint8_t deviceId);
    static uint8_t* getExtMemoryPtrByDeviceId(uint8_t deviceId, uint16_t* extMemoryLengthPtr);
    static uint16_t getCurrentMemoryUsage();

};


#endif
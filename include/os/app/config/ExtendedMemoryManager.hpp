#ifndef EXTENDED_MEMORY_MANAGER_H
#define EXTENDED_MEMORY_MANAGER_H

#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <vector>
#include "os/Logger.hpp"

#define MAX_EXT_MEMORY_SIZE_TOTAL 1900 /* bytes */
#define SLOTS_FOR_EXT_MEMORY_BUFFERS 10 /* number of slots for independent data pointers */

/**
 * @file include/os/app/config/ExtendedMemoryManager.hpp
 * @brief Configuration persistence and runtime configuration management for the Home Control Station.
 */


/**
 * @struct ExtMemoryMetadataType
 * @brief Metadata describing how much extended memory is reserved for each device slot.
 */
typedef struct{
    /**
     * Per-device allocation target reserved for the current slot configuration.
     */
    uint16_t memoryPerDeviceSlotNeeded[SLOTS_FOR_EXT_MEMORY_BUFFERS];

    /**
     * Reserved buffer to allow future metadata growth without changing the NVM layout.
     */
    uint8_t reserved[PERSISTENT_DATABLOCK_SIZE - (SLOTS_FOR_EXT_MEMORY_BUFFERS * sizeof(uint16_t))];

}ExtMemoryMetadataType;

/**
 * @struct ExtMemoryData
 * @brief Runtime information describing a single extended-memory allocation owned by a device.
 */
typedef struct{
    /**
     * Device identifier that owns the allocation.
     */
    uint8_t ownerDeviceId = 0xff;

    /**
     * Number of bytes currently assigned to this allocation.
     */
    uint16_t length = 0;

    /**
     * Pointer to the memory region associated with the device payload.
     */
    uint8_t* dataPtr = nullptr;

    /**
     * Logs the current allocation metadata for debugging.
     */
    void printExtMem(){
        Logger::log("OwnerID: " + String((int)ownerDeviceId));
        Logger::log("length: " + String((int)length));
        Logger::log("dataPtr: " + String((int)dataPtr));
    }
}ExtMemoryData;
/**
 * @class ExtendedMemoryManager
 * @brief Allocates and tracks extra memory used for large device payloads or extended runtime data.
 */

class ExtendedMemoryManager {
    /**
     * Metadata describing how much extra memory each device has reserved.
     */
    static ExtMemoryMetadataType extMemoryMetadata;

    /**
     * Total amount of extended memory currently in use across all device slots.
     */
    static uint16_t extMemoryInUse;

    /**
     * Runtime container holding the active extended-memory allocations.
     */
    static std::vector<ExtMemoryData> extMemoryContainer;

    /**
     * Recalculates the current memory usage based on the active allocation container.
     */
    static void updateCurrentExtMemoryUsage();

    /**
     * Restores the extended-memory allocation state from NVM at startup.
     */
    static void restoreExtMemoryFromNvm();

public: 
    /**
     * Initializes the allocation tracker and restores the persisted metadata.
     */
    static void init();

    /**
     * Releases runtime resources attached to the extended-memory manager.
     */
    static void deinit();

    /**
     * Runs periodic maintenance for the allocator and its persisted state.
     */
    static void cyclic();

    /**
     * Flushes the current memory metadata back to the persistent store.
     */
    static void flushNvmData();

    /**
     * Allocates a memory block for the given device and required size.
     * @param deviceId Device identifier requesting the allocation.
     * @param spaceSize Number of bytes required by the device payload.
     * @return true when memory was reserved successfully; false when the request exceeds available space.
     */
    static bool requestNewExtendedMemorySpace(uint8_t deviceId, uint16_t spaceSize);

    /**
     * Frees the extended-memory block associated with the given device.
     * @param deviceId Device identifier whose memory should be released.
     */
    static void releaseExtendedMemorySpace(uint8_t deviceId);

    /**
     * Returns the pointer to the extended-memory region associated with a device.
     * @param deviceId Device identifier to query.
     * @param extMemoryLengthPtr Output pointer receiving the size of the returned memory block.
     * @return Pointer to the raw buffer or nullptr when no allocation exists.
     */
    static uint8_t* getExtMemoryPtrByDeviceId(uint8_t deviceId, uint16_t* extMemoryLengthPtr);

    /**
     * Returns the total number of bytes currently used by all allocations.
     * @return Present extended-memory usage in bytes.
     */
    static uint16_t getCurrentMemoryUsage();

};


#endif
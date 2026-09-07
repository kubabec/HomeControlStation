#include <os/app/config/ExtendedMemoryManager.hpp>
#include <os/app/config/PersistentMemoryAccess.hpp>
#include "os/Logger.hpp"

/**
 * @file src/os/app/config/ExtendedMemoryManager.cpp
 * @brief Configuration persistence and runtime configuration logic for the Home Control Station.
 */


ExtMemoryMetadataType ExtendedMemoryManager::extMemoryMetadata;
uint16_t ExtendedMemoryManager::extMemoryInUse;
std::vector<ExtMemoryData> ExtendedMemoryManager::extMemoryContainer;



void ExtendedMemoryManager::init(){
    Logger::log("ExtendedMemoryManager init ...");
    /* clear the ext memory sizes */
    for(uint8_t i = 0 ; i < SLOTS_FOR_EXT_MEMORY_BUFFERS; i ++)
    {
        extMemoryMetadata.memoryPerDeviceSlotNeeded[i] = 0x00;
        extMemoryMetadata.ownerDeviceIds[i] = 0x00;
    }


    ExtendedMemoryCtrlAPI api;
    api.requestNewExtendedMemorySpace = ExtendedMemoryManager::requestNewExtendedMemorySpace;
    api.releaseExtendedMemorySpace = ExtendedMemoryManager::releaseExtendedMemorySpace;
    api.getExtMemoryPtrByDeviceId = ExtendedMemoryManager::getExtMemoryPtrByDeviceId;
    api.getCurrentMemoryUsage = ExtendedMemoryManager::getCurrentMemoryUsage;

    DataContainer::setSignalValue(SIG_EXT_MEM_CTRL_API, static_cast<ExtendedMemoryCtrlAPI>(api));

    /* Try to read metadata block from NVM */
    bool success = std::any_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(
        DataContainer::getSignalValue(CBK_GET_NVM_DATABLOCK)
    )(
        e_BLOCK_EXT_MEMORY_METADATA, // Datablock ID
        (uint8_t*)&extMemoryMetadata // local memory buffer for datablock data
    );

    if(!success){
        Logger::log("Error during reading ExtMemory metadata nvm block");
        return;
    }


    restoreExtMemoryFromNvm();
    Logger::log("ExtendedMemoryManager// Extended memory for " + String((int)extMemoryContainer.size()) + " devices restored with total size of "+String((int)extMemoryInUse)+ " bytes ");



    for(auto& entry : extMemoryContainer){
        entry.printExtMem();
    }

    Logger::log("... done");
}

void ExtendedMemoryManager::cyclic(){

}

void ExtendedMemoryManager::flushNvmData(){
    /* Try to read metadata block from NVM */
    bool success = std::any_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(
        DataContainer::getSignalValue(CBK_SET_NVM_DATABLOCK)
    )(
        e_BLOCK_EXT_MEMORY_METADATA, // Datablock ID
        (uint8_t*)&extMemoryMetadata // local memory buffer for datablock data
    );
    if(!success){
        Logger::log("Error during saving ExtMemory metadata nvm block");
    }


    /* sort extended memory records by devices id */
    std::sort(extMemoryContainer.begin(), extMemoryContainer.end(), [&](ExtMemoryData& a, ExtMemoryData& b){
        return a.ownerDeviceId < b.ownerDeviceId;
    });


    /* Serialize all the extended memory to NVM */
    uint16_t offset = 0;
    for(auto& extMemoryEntry : extMemoryContainer){
        if(extMemoryEntry.length > 0 && extMemoryEntry.dataPtr != nullptr){
            PersistentMemoryAccess::writeExtendedMemory(
                offset,
                extMemoryEntry.dataPtr,
                extMemoryEntry.length
            );
            offset += extMemoryEntry.length;
        }
    }

    PersistentMemoryAccess::flushExtendedMemory();
}

void ExtendedMemoryManager::deinit(){
    flushNvmData();
}


void ExtendedMemoryManager::restoreExtMemoryFromNvm()
{
    uint16_t offsetInNvm = 0;
    for(uint8_t slot = 0 ; slot < SLOTS_FOR_EXT_MEMORY_BUFFERS; slot++){
        /* Are there any data for this device? */
        if(extMemoryMetadata.memoryPerDeviceSlotNeeded[slot] > 0){
            const uint16_t requestedLength = extMemoryMetadata.memoryPerDeviceSlotNeeded[slot];
            if (requestedLength > MAX_EXT_MEMORY_SIZE_TOTAL - offsetInNvm)
            {
                Logger::log("ExtendedMemoryManager//ERROR: Persisted extended memory exceeds current 1200-byte capacity");
                extMemoryMetadata.memoryPerDeviceSlotNeeded[slot] = 0;
                continue;
            }
            /* try to allocate RAM buffer for data */
            uint8_t* data = (uint8_t*)malloc(requestedLength);
            if(data != nullptr){
                /* Read data from NVM to buffer under 'data' pointer */
                PersistentMemoryAccess::readExtendedMemoryWithOffset(
                    offsetInNvm,
                    data,
                    requestedLength
                );

                /* Create new ExtMemory entry */
                ExtMemoryData entry {
                    .ownerDeviceId = extMemoryMetadata.ownerDeviceIds[slot] != 0 ? extMemoryMetadata.ownerDeviceIds[slot] : slot + 1,
                    .length = requestedLength,
                    .dataPtr = data
                };

                extMemoryInUse += entry.length;

                /* Add ExtMemory entry to the container */
                extMemoryContainer.push_back(entry);

                /* Move offset to next ExtMemory section in NVM */
                offsetInNvm += extMemoryMetadata.memoryPerDeviceSlotNeeded[slot];
            }else 
            {
                Logger::log("ExtendedMemoryManager//ERROR : Cannot allocate RAM mirror with length :" + String((int)extMemoryMetadata.memoryPerDeviceSlotNeeded[slot]));
                return;
            }
        }
    }
}


bool ExtendedMemoryManager::requestNewExtendedMemorySpace(uint8_t deviceId, uint16_t spaceSize)
{
    if(deviceId == 0 || spaceSize == 0){
        Logger::log("ExtendedMemoryManager//ERROR: Invalid extended memory request");
        return false;
    }

    uint8_t slot = SLOTS_FOR_EXT_MEMORY_BUFFERS;
    for(uint8_t index = 0; index < SLOTS_FOR_EXT_MEMORY_BUFFERS; ++index){
        if(extMemoryMetadata.ownerDeviceIds[index] == deviceId ||
           (extMemoryMetadata.ownerDeviceIds[index] == 0 && index + 1 == deviceId)){
            slot = index;
            break;
        }
        if(slot == SLOTS_FOR_EXT_MEMORY_BUFFERS && extMemoryMetadata.memoryPerDeviceSlotNeeded[index] == 0){
            slot = index;
        }
    }

    if(slot == SLOTS_FOR_EXT_MEMORY_BUFFERS){
        Logger::log("ExtendedMemoryManager//ERROR: No metadata slot for device ID: "+String((int)deviceId));
        return false;
    }

    const uint16_t previousSpaceSize = extMemoryMetadata.memoryPerDeviceSlotNeeded[slot];
    const uint16_t spaceWithRequestedNeeds = extMemoryInUse - previousSpaceSize + spaceSize;
    if(spaceWithRequestedNeeds > MAX_EXT_MEMORY_SIZE_TOTAL){
        Logger::log("ExtendedMemoryManager//ERROR: Cannot allocate space length: "+String((int)spaceSize)+", already in use: "+String((int)extMemoryInUse));
        return false;
    }

    extMemoryInUse -= previousSpaceSize;
    extMemoryMetadata.memoryPerDeviceSlotNeeded[slot] = spaceSize;
    extMemoryMetadata.ownerDeviceIds[slot] = deviceId;
    extMemoryInUse += spaceSize;
    return true;
}

void ExtendedMemoryManager::releaseExtendedMemorySpace(uint8_t deviceId){
    for(uint8_t slot = 0; slot < SLOTS_FOR_EXT_MEMORY_BUFFERS; ++slot){
        if(extMemoryMetadata.ownerDeviceIds[slot] == deviceId ||
           (extMemoryMetadata.ownerDeviceIds[slot] == 0 && slot + 1 == deviceId)){
            extMemoryInUse -= extMemoryMetadata.memoryPerDeviceSlotNeeded[slot];
            extMemoryMetadata.memoryPerDeviceSlotNeeded[slot] = 0;
            extMemoryMetadata.ownerDeviceIds[slot] = 0;
            return;
        }
    }
    Logger::log("ExtendedMemoryManager//ERROR: Invalid device ID to release ext memory: "+String((int)deviceId));
}

uint8_t* ExtendedMemoryManager::getExtMemoryPtrByDeviceId(uint8_t deviceId, uint16_t* extMemoryLengthPtr){
    uint8_t* dataPtr = nullptr;
    *extMemoryLengthPtr = 0;

    for(auto& entry : extMemoryContainer){
        if(entry.ownerDeviceId == deviceId){
            if(entry.length > 0 && entry.dataPtr != nullptr){
                dataPtr = entry.dataPtr;
                *extMemoryLengthPtr = entry.length;

                return dataPtr;
            }
        }
    }

    return dataPtr;
}


uint16_t ExtendedMemoryManager::getCurrentMemoryUsage()
{
    return extMemoryInUse;
}
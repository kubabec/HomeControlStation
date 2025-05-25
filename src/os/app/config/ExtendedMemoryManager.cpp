#include <os/app/config/ExtendedMemoryManager.hpp>
#include <os/app/config/PersistentMemoryAccess.hpp>

ExtMemoryMetadataType ExtendedMemoryManager::extMemoryMetadata;
uint16_t ExtendedMemoryManager::extMemoryInUse;
std::vector<ExtMemoryData> ExtendedMemoryManager::extMemoryContainer;



void ExtendedMemoryManager::init(){
    Serial.println("ExtendedMemoryManager init ...");
    /* clear the ext memory sizes */
    for(uint8_t i = 0 ; i < SLOTS_FOR_EXT_MEMORY_BUFFERS; i ++)
    {
        extMemoryMetadata.memoryPerDeviceSlotNeeded[i] = 0x00;
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
        Serial.println("Error during reading ExtMemory metadata nvm block");
        return;
    }


    restoreExtMemoryFromNvm();
    Serial.println("ExtendedMemoryManager// Extended memory for " + String((int)extMemoryContainer.size()) + " devices restored with total size of "+String((int)extMemoryInUse)+ " bytes ");



    for(auto& entry : extMemoryContainer){
        entry.printExtMem();
    }

    Serial.println("... done");
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
        Serial.println("Error during saving ExtMemory metadata nvm block");
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
    for(uint8_t deviceId = 0 ; deviceId < SLOTS_FOR_EXT_MEMORY_BUFFERS; deviceId++){
        /* Are there any data for this device? */
        if(extMemoryMetadata.memoryPerDeviceSlotNeeded[deviceId] > 0){
            /* try to allocate RAM buffer for data */
            uint8_t* data = (uint8_t*)malloc(extMemoryMetadata.memoryPerDeviceSlotNeeded[deviceId]);
            if(data != nullptr){
                /* Read data from NVM to buffer under 'data' pointer */
                PersistentMemoryAccess::readExtendedMemoryWithOffset(
                    offsetInNvm,
                    data,
                    extMemoryMetadata.memoryPerDeviceSlotNeeded[deviceId]
                );

                /* Create new ExtMemory entry */
                ExtMemoryData entry {
                    .ownerDeviceId = deviceId + 1,
                    .length = extMemoryMetadata.memoryPerDeviceSlotNeeded[deviceId],
                    .dataPtr = data
                };

                extMemoryInUse += entry.length;

                /* Add ExtMemory entry to the container */
                extMemoryContainer.push_back(entry);

                /* Move offset to next ExtMemory section in NVM */
                offsetInNvm += extMemoryMetadata.memoryPerDeviceSlotNeeded[deviceId];
            }else 
            {
                Serial.println("ExtendedMemoryManager//ERROR : Cannot allocate RAM mirror with length :" + String((int)extMemoryMetadata.memoryPerDeviceSlotNeeded[deviceId]));
                return;
            }
        }
    }
}


bool ExtendedMemoryManager::requestNewExtendedMemorySpace(uint8_t deviceId, uint16_t spaceSize)
{
    bool success = false;
    if(deviceId > 0 && deviceId < 10){
        uint16_t spaceWithRequestedNeeds = extMemoryInUse + spaceSize;
        if(spaceWithRequestedNeeds <= MAX_EXT_MEMORY_SIZE_TOTAL){
            extMemoryMetadata.memoryPerDeviceSlotNeeded[deviceId-1] = spaceSize;
            success = true;
        }else {
            Serial.println("ExtendedMemoryManager//ERROR: Cannot allocate space length: "+String((int)spaceSize)+", already in use: "+String((int)extMemoryInUse));
        }

    }else {
        Serial.println("ExtendedMemoryManager//ERROR: Invalid device ID to allocate ext memory: "+String((int)deviceId));
    }

    return success;
}

void ExtendedMemoryManager::releaseExtendedMemorySpace(uint8_t deviceId){
    if(deviceId > 0 && deviceId < 10){
        extMemoryMetadata.memoryPerDeviceSlotNeeded[deviceId-1] = 0;
    }else {
        Serial.println("ExtendedMemoryManager//ERROR: Invalid device ID to release ext memory: "+String((int)deviceId));
    }
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
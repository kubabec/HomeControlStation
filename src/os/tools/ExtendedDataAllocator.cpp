#include <os/tools/ExtendedDataAllocator.hpp>


ExtendedData ExtendedDataAllocator::createNewExtendedData(uint16_t length){
    /* Try to allocate memory for new data */
    uint8_t* memory = (uint8_t*)malloc(length);

    /* create empty record */
    ExtendedData newRecord {
        .identifier = 255,
        .length = 0,
        .memoryPtr = nullptr
    };

    /* if data allocated successfully */
    if(memory != nullptr){
        newRecord.identifier = extendedDataId;
        newRecord.length = length;
        newRecord.memoryPtr = memory;

        /* Save new Extended Data in class container */
        extendedDataContainer.push_back(newRecord);
        extendedDataId++;

        printExtendedDataContainer();
    }

    return newRecord;
}

bool ExtendedDataAllocator::deleteExtendedData(uint8_t identifier){
    bool retVal = false;

    for(int i = 0 ; i < extendedDataContainer.size(); i++){
        if(extendedDataContainer.at(i).identifier == identifier){
            free(extendedDataContainer.at(i).memoryPtr);
            auto new_end = std::remove_if(extendedDataContainer.begin(), extendedDataContainer.end(),
                              [&](const ExtendedData& record)
                              { return record.identifier == identifier; });

            extendedDataContainer.erase(new_end, extendedDataContainer.end());

            retVal = true;
        }
    }

    return retVal;
}

ExtendedData ExtendedDataAllocator::getExtendedDataById(uint8_t id)
{
    for(int i = 0 ; i < extendedDataContainer.size(); i++){
        if(extendedDataContainer.at(i).identifier == id){
            return extendedDataContainer.at(i);
        }
    }

    return ExtendedData{};
}

void ExtendedDataAllocator::printExtendedDataContainer()
{
    Serial.println("-- Extended data list: --" );
    for(int i = 0 ; i < extendedDataContainer.size(); i++){
        Serial.println("Identifier: " + String((int)extendedDataContainer.at(i).identifier));
        Serial.println("Length: " + String((int)extendedDataContainer.at(i).length));
        Serial.println(String("Address: %x", (int)extendedDataContainer.at(i).memoryPtr));    
    }
    Serial.println("-- --");
}
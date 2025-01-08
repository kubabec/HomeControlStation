#ifndef EXTENDED_DATA_ALLOCATOR_H
#define EXTENDED_DATA_ALLOCATOR_H

#include <os/tools/ExtendedDataTypes.hpp>
#include <vector>

class ExtendedDataAllocator {
    private:
    uint8_t extendedDataId = 1;
    std::vector<uint8_t*> memoryBlocks;
    std::vector<ExtendedData> extendedDataContainer;



    public:
    ExtendedDataAllocator() = default;

    ExtendedData createNewExtendedData(uint16_t length);
    bool deleteExtendedData(uint8_t identifier);
    ExtendedData getExtendedDataById(uint8_t id);
    void printExtendedDataContainer();


};



#endif
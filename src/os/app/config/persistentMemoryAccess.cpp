#include <os/app/config/PersistentMemoryAccess.hpp>
#include "os/Logger.hpp"

#include <EEPROM.h>

/**
 * @file src/os/app/config/persistentMemoryAccess.cpp
 * @brief Configuration persistence and runtime configuration logic for the Home Control Station.
 */


const uint8_t START_OF_DATA = 0b10101010;
const uint8_t END_OF_DATA = 0b01010101;

uint16_t PersistentMemoryAccess::standardDataEepromSize = 0;
uint16_t PersistentMemoryAccess::extendedDataEepromSize = 0;
bool PersistentMemoryAccess::eepromInitializedSuccessfully = false;

void PersistentMemoryAccess::init(uint16_t eepromSize, uint16_t extendedMemorySize)
{
    const uint16_t finalSize = 
        sizeof(START_OF_DATA) + 
        sizeof(END_OF_DATA) +
        eepromSize +
        sizeof(int);

    eepromInitializedSuccessfully = EEPROM.begin(finalSize + extendedMemorySize);
    if(eepromInitializedSuccessfully){
        standardDataEepromSize = finalSize;
        extendedDataEepromSize = extendedMemorySize;
        Logger::log("EEPROM initialized: standard=" + String((int)standardDataEepromSize) +
                    " extended=" + String((int)extendedDataEepromSize) +
                    " total=" + String((int)(standardDataEepromSize + extendedDataEepromSize)));
    }else{
        Logger::log("EEPROM initialization failed for " + String((int)(finalSize + extendedMemorySize)) + " bytes");
    }
}


bool PersistentMemoryAccess::saveData(uint8_t* data, uint16_t size)
{
    if(eepromInitializedSuccessfully && data != nullptr)
    {
        // Data address variable
        uint16_t currentAddress = 0;
        // Checksum value
        int checkSum = 0;

        // Start memory block with START_OF_DATA
        EEPROM.write(currentAddress, START_OF_DATA);
        checkSum += START_OF_DATA;
        // Go to next byte address
        currentAddress++;

        for(uint16_t i = 0; i < size; i++)
        {
            EEPROM.write(currentAddress, data[i]);
            // Go to next byte address
            currentAddress++;

            checkSum += data[i];
        }
        // End data block with END_OF_DATA
        EEPROM.write(currentAddress, END_OF_DATA);
        // Go to next byte address
        currentAddress++;

        checkSum += END_OF_DATA;

        // Store the complete checksum in the sizeof(int) bytes reserved by init().
        EEPROM.put(currentAddress, checkSum);

        if(EEPROM.commit())
        {
            Logger::log("Data saved with checkSum :" + String(checkSum));
            return true;
        }else
        {
            Logger::log("Failed to save EEPROM data!");
            return false;
        }

        
    }else 
    {
        Logger::log("EEPROM not initialized successfully!");
        return false;
    }
}

bool PersistentMemoryAccess::saveDataMigratingExtendedMemory(uint8_t* data, uint16_t size,
                                                              uint16_t legacyDataSize, uint16_t extendedMemorySize)
{
    if (!eepromInitializedSuccessfully || data == nullptr || extendedMemorySize > extendedDataEepromSize)
        return false;
    const uint16_t legacyExtendedBase = sizeof(START_OF_DATA) + legacyDataSize + sizeof(END_OF_DATA) + sizeof(int);
    uint8_t *extendedData = static_cast<uint8_t *>(malloc(extendedMemorySize));
    if (extendedData == nullptr) return false;
    for (uint16_t index = 0; index < extendedMemorySize; ++index)
    {
        extendedData[index] = EEPROM.read(legacyExtendedBase + index);
    }

    uint16_t currentAddress = 0;
    int checkSum = START_OF_DATA + END_OF_DATA;
    EEPROM.write(currentAddress++, START_OF_DATA);
    for (uint16_t index = 0; index < size; ++index)
    {
        EEPROM.write(currentAddress++, data[index]);
        checkSum += data[index];
    }
    EEPROM.write(currentAddress++, END_OF_DATA);
    EEPROM.put(currentAddress, checkSum);
    for (uint16_t index = 0; index < extendedMemorySize; ++index)
    {
        EEPROM.write(standardDataEepromSize + index, extendedData[index]);
    }
    const bool saved = EEPROM.commit();
    Logger::log(saved ? "NVM layout migration committed atomically" : "NVM layout migration commit failed");
    free(extendedData);
    return saved;
}

bool PersistentMemoryAccess::readData(uint8_t* buffer, uint16_t size)
{
    if(eepromInitializedSuccessfully && buffer != nullptr)
    {
        // Data address variable
        uint16_t currentAddress = 1;
        // Checksum value
        int checkSum = 0;
        if(START_OF_DATA == EEPROM.read(0))
        {
            for(uint16_t i = 0; i < size; i++)
            {
                buffer[i] = EEPROM.read(currentAddress);
                currentAddress++;

                checkSum += buffer[i];
            }

            if(END_OF_DATA == EEPROM.read(currentAddress))
            {
                //validate checksum
                checkSum += START_OF_DATA;
                checkSum += END_OF_DATA;

                currentAddress++;
                int storedCheckSum = 0;
                EEPROM.get(currentAddress, storedCheckSum);
                if(checkSum == storedCheckSum)
                {
                    Logger::log("Data read successfully!");
                    return true;
                }
                Logger::log("Cannot read persistent data. Checksum mismatch");
                return false;
            }else
            {
                Logger::log("Cannot read persistant data. END_OF_DATA missing");
                return false;
            }

        }else
        {
            Logger::log("Cannot read persistant data. START_OF_DATA missing");
            return false;
        }

        return false;
        

    }
    else 
    {
        Logger::log("EEPROM not initialized successfully!");
        return false;
    }
}

void PersistentMemoryAccess::massErase(uint16_t eepromSize)
{
    uint16_t eepromSize2 = standardDataEepromSize + extendedDataEepromSize;
    Logger::log("NVM :: ERASING " + String((int)eepromSize2) + " BYTES FROM FLASH MEMORY ... ");
    for(uint16_t i = 0; i < eepromSize2; i ++){
        EEPROM.write(i, 0);
    }

    EEPROM.commit();

    EEPROM.end();
}


void PersistentMemoryAccess::readExtendedMemoryWithOffset(uint16_t offsetToStandardData, uint8_t* buffer, uint16_t size){
    for(uint16_t i = 0; i < size; i++){
        /*
        STANDARD_NVM_START_OF_DATA
        STANDARD_NVM_DATA . . . 
        . . . 
        . . . 
        STANDARD_NVM_END_OF_DATA
        FIRST_BYTE_OF_EXTENDED_MEMORY_DATA (offset: standardDataEepromSize)
        . . . 
        . . . 
        . . . 
        . . . 
        . . . 
        . . . 
        . . . 
        . . . 2,5kB
        */
        buffer[i] = EEPROM.read(standardDataEepromSize + offsetToStandardData + i);
    }

}

void PersistentMemoryAccess::writeExtendedMemory(uint16_t offset, uint8_t* buffer, uint16_t size)
{
    for(uint16_t i = 0; i < size; i++)
    {
        EEPROM.write(standardDataEepromSize + offset + i, buffer[i]);
    }
}

void PersistentMemoryAccess::flushExtendedMemory()
{
    EEPROM.commit();
}
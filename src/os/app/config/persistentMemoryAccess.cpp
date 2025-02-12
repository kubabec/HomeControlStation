#include <os/app/config/PersistentMemoryAccess.hpp>

#include <EEPROM.h>

const uint8_t START_OF_DATA = 0b10101010;
const uint8_t END_OF_DATA = 0b01010101;

uint16_t PersistentMemoryAccess::standardDataEepromSize = 0;
bool PersistentMemoryAccess::eepromInitializedSuccessfully = false;

void PersistentMemoryAccess::init(uint16_t eepromSize)
{
    const uint16_t finalSize = 
        sizeof(START_OF_DATA) + 
        sizeof(END_OF_DATA) +
        eepromSize +
        sizeof(int);

    eepromInitializedSuccessfully = EEPROM.begin(finalSize + 2500);
    if(eepromInitializedSuccessfully){
        standardDataEepromSize = finalSize;
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

        // Save checksum on the last position;
        EEPROM.write(currentAddress, checkSum);

        if(EEPROM.commit())
        {
            Serial.println("Data saved with checkSum :" + String(checkSum));
            return true;
        }else
        {
            Serial.println("Failed to save EEPROM data!");
            return false;
        }

        
    }else 
    {
        Serial.println("EEPROM not initialized successfully!");
        return false;
    }
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
                // Compare calculated checkSum to memory read value
                if(checkSum == EEPROM.read(currentAddress))
                {
                    Serial.println("Data read successfully!");
                    return true;
                }
            }else
            {
                Serial.println("Cannot read persistant data. END_OF_DATA missing");
                return false;
            }

        }else
        {
            Serial.println("Cannot read persistant data. START_OF_DATA missing");
            return false;
        }
        

    }
    else 
    {
        Serial.println("EEPROM not initialized successfully!");
        return false;
    }
}

void PersistentMemoryAccess::massErase(uint16_t eepromSize)
{
    uint16_t eepromSize2 = standardDataEepromSize + 2500;
    Serial.println("NVM :: ERASING " + String((int)eepromSize2) + " BYTES FROM FLASH MEMORY ... ");
    for(uint16_t i = 0; i < eepromSize2; i ++){
        EEPROM.write(i, 0);
    }

    EEPROM.commit();
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
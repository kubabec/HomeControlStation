#ifndef CONFIG_SLOT_DEFINITION_H
#define CONFIG_SLOT_DEFINITION_H
#include <Arduino.h>

/* Description of single configuration NVM slot */
struct DeviceConfigSlotType
{
    bool isActive = false;            /* 1 byte */
    char deviceName[25] = {'\0'};   /* 25 bytes */
    uint8_t deviceType = 255;       /* 1 byte */
    uint8_t pinNumber = 255;        /* 1 byte */
    uint8_t deviceId = 255;         /* 1 byte */
    uint8_t roomId = 255;           /* 1 byte */
    uint8_t customBytes[20] = {0x00};          /* 20 bytes */

    DeviceConfigSlotType(){
        memset(deviceName, (char)'\0', 25);
        memset(customBytes, 0x00, 20);
    }

    void print()
    {
        Serial.println("--> Config SLOT <--");
        if(isActive){
            Serial.println("isActive: YES ");
        }else
        {
            Serial.println("isActive: NO ");
        }
        
        Serial.print("Device Type: "); 
        switch(deviceType)
        {
            case 43:
            Serial.println("ON/OFF");
            break;

            case 44:
            Serial.println("LED STRIP");
            break;

            case 255:
            Serial.println("Not configured");
            break;

            default: 
            Serial.println("UNKNOWN");
            break;
        }
        Serial.println("Device ID " + String(deviceId)); 
        Serial.println("Name: " + String(deviceName));
        Serial.println("PIN : " + String((int)pinNumber));
        Serial.println("Room : " + String((int)roomId));
        Serial.println("Extra data:");
        for(int i = 0 ; i < 20; i ++)
        {
            Serial.print(String((int)customBytes[i]) + " ");
        }
        Serial.println("");
    }

    bool isValid()
    {
        return (deviceType >= 43 || deviceType <= 45);
    }

    static uint8_t getSize()
    {
        return sizeof(isActive) + 25 + sizeof(deviceType) + sizeof(pinNumber) + sizeof(deviceId) + sizeof(roomId) + 20;
    }

};

#endif
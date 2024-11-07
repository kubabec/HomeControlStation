#ifndef DATA_CONTAINER_TYPES_H
#define DATA_CONTAINER_TYPES_H

#include "DeviceControlApi.hpp"

#define PERSISTENT_DATABLOCK_SIZE 50
#define NUMBER_OF_CONFIG_SLOTS 6

/* Description of ON/OFF device */
typedef struct
{
    uint8_t nodeId = 255;
    uint8_t deviceId = -1;
    bool isEnabled;
    bool brightnessIsAdjustable = false;
    uint8_t currentBrightness;
    String deviceName;
    void print() {
        //Serial.println(" ======DEVICE DESCRIPTION=============");
        Serial.print("nodeId: " + String(nodeId) + " - ");
        Serial.print("deviceId: " + String(deviceId) + " - ");
        Serial.print("isEnabled: " + String(isEnabled) + " - ");
        Serial.print("brightnessIsAdjustable: " + String(brightnessIsAdjustable) + " - ");
        Serial.print("currentBrightness: " + String(currentBrightness) + " - ");
        Serial.print("deviceName: " + deviceName);
        Serial.println();
        }
}OnOffDeviceDescription;

// aktualny stan przerabianego requesta
/* State of request processing */
enum RequestProcessingState {
    eNO_REQUEST,
    eREQUEST_PENDING,
    eREQUEST_COMPLETED
};

/* Basic node (ESP32) configuration data */
typedef struct {
    bool isHttpServer = 0;
    bool isRcServer = 0;
    bool networkCredentialsAvailable = false;
    uint8_t nodeId = 255;
    uint8_t nodeType = 0;
    String networkSSID = "";
    String networkPassword = "";
}NodeConfiguration;

/* NVM datablocks identifiers */
typedef enum
{
    e_PERSISTENT_BLOCK_FIRST = 0,
    e_BLOCK_DEVICE_1 = e_PERSISTENT_BLOCK_FIRST,
    e_BLOCK_DEVICE_2,
    e_BLOCK_DEVICE_3,
    e_BLOCK_DEVICE_4,
    e_BLOCK_DEVICE_5,
    e_BLOCK_DEVICE_6,
    e_PERSISTENT_BLOCK_LAST = e_BLOCK_DEVICE_6
}PersistentDatablockID;

/* Description of single configuration NVM slot */
typedef struct 
{
    bool isActive = false;            /* 1 byte */
    char deviceName[25] = {'\0'};   /* 25 bytes */
    uint8_t deviceType = 255;       /* 1 byte */
    uint8_t pinNumber = 255;        /* 1 byte */
    uint8_t deviceId = 255;         /* 1 byte */
    uint8_t roomId = 255;           /* 1 byte */
    uint8_t customBytes[20] = {0xFA};          /* 20 bytes */

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
        for(int i = 0 ; i < 10; i ++)
        {
            Serial.print(String((int)customBytes[i]));
        }
    }

    bool isValid()
    {
        return (deviceType == 43 || deviceType == 44);
    }

    static uint8_t getSize()
    {
        return sizeof(isActive) + 25 + sizeof(deviceType) + sizeof(pinNumber) + sizeof(deviceId) + sizeof(roomId) + 20;
    }

}DeviceConfigSlotType;

/* Array of available configuration slots wrapper */
typedef struct {
    std::array<DeviceConfigSlotType, NUMBER_OF_CONFIG_SLOTS> slots;
}ConfigSlotsDataType;

/* Generic datatype for NVM PERSISTENT_DATABLOCK_SIZE-bytes long block */
typedef struct
{
    uint8_t data[PERSISTENT_DATABLOCK_SIZE];

    static size_t getSize() {
        return PERSISTENT_DATABLOCK_SIZE;
    }
}PersistentDataBlock;

typedef enum{
    ERR_MON_UNEXPECTED_RESET = 1,
    ERR_MON_INVALID_NVM_DATA,
    ERR_MON_INVALID_LOCAL_CONFIG,
    ERR_MON_WRONG_CONFIG_STRING_RECEIVED,
    ERR_MON_WRONG_LOCAL_DEVICES_CONFIG_RECEIVED,
    ERR_MON_INVALID_ERROR_REPORTED,
    ERR_MON_LAST_ERROR = ERR_MON_INVALID_ERROR_REPORTED,
    ERR_MONT_ERROR_COUNT = ERR_MON_LAST_ERROR
}ERR_MON_ERROR_TYPE;

typedef struct
{
    uint8_t occurrenceCount = 0;
    uint16_t extendedData = 0;
    uint64_t lastOccurrenceTime = 0;
}SystemErrorType;

typedef enum {
    SYSREQ_FIRST,
    ENABLE_SYSREQ = SYSREQ_FIRST,
    DISABLE_SYSREQ,
    BRIGHTNESS_CHANGE_SYSREQ,
    COLOR_CHANGE_SYSREQ,
    RESET_SYSREQ,
    UNKNOWN_SYSREQ,
    SYSREQ_COUNT = UNKNOWN_SYSREQ
}SystemRequestType;


typedef struct {
    uint8_t requestId = 255;
    uint8_t type = UNKNOWN_SYSREQ;
    uint8_t data[30] = {0xFF};

    void print() {
        Serial.println("$$$ System Request $$$");
        Serial.println("requestId :" + String((int)requestId));
        Serial.println("type :" + String((int)type));
        for(uint8_t i=0; i<30; i++) {
            Serial.print((int)data[i]);
        }
        Serial.println("");
        Serial.println("$$$$$$$$$$$$$$$$$$$$");
    }

}SystemRequest;

typedef struct {
    uint8_t isPositive = 0;
    uint8_t responseId = 255;
    uint8_t type = UNKNOWN_SYSREQ;
    uint8_t data[30] = {0xFF};

    void print() {
        Serial.println("$$$ System Response $$$");
        Serial.println("responseId :" + String((int)responseId));
        Serial.println("type :" + String((int)type));
        for(uint8_t i=0; i<30; i++) {
            Serial.print((int)data[i]);
        }
        Serial.println("");
        Serial.println("$$$$$$$$$$$$$$$$$$$$");
    }

}SystemResponse;

#endif 
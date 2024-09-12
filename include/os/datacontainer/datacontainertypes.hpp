#ifndef DATA_CONTAINER_TYPES_H
#define DATA_CONTAINER_TYPES_H

#define PERSISTENT_DATABLOCK_SIZE 50
#define NUMBER_OF_CONFIG_SLOTS 6

enum LightStatus
{
    eUNKNOWN,
    eON,
    eOFF
};

enum ConnectionStatus
{
    NEVER_CONNECTED,
    CONNECTED,
    CONNECTION_LOST
};

typedef enum
{
    eENABLE_REQUEST,
    eDISABLE_REQUEST,
    eNOT_REQESTED,
    eREQEST_RECEIVED
}StateChangeRequest;

typedef enum
{
    eWIFI_UNKNOWN = 7,
    eWIFI_SAVED_NOT_CONNECTED,
    eWIFI_SAVED_CONNECTED
}SystemNetworkModeEnum;

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
enum RequestProcessingState {
    eNO_REQUEST,
    eREQUEST_PENDING,
    eREQUEST_COMPLETED
};

typedef struct {
    bool isHttpServer = 0;
    bool isRcServer = 0;
    bool networkCredentialsAvailable = false;
    String networkSSID = "";
    String networkPassword = "";
}NodeConfiguration;


typedef enum
{
    e_PERSISTENT_BLOCK_FIRST = 0,
    e_BLOCK_DEVICE_1 = e_PERSISTENT_BLOCK_FIRST,
    e_BLOCK_DEVICE_2,
    e_BLOCK_DEVICE_3,
    e_BLOCK_DEVICE_4,
    e_BLOCK_DEVICE_5,
    e_PERSISTENT_BLOCK_LAST = e_BLOCK_DEVICE_5
}PersistentDatablockID;

typedef struct 
{
    char deviceName[25] = {'\0'};   /* 25 bytes */
    uint8_t deviceType = 255;       /* 1 byte */
    uint8_t pinNumber = 255;        /* 1 byte */
    uint8_t deviceId = 255;         /* 1 byte */
    uint8_t roomId = 255;           /* 1 byte */
    struct customData{
        uint8_t bytes[10];          /* 10 bytes */
    };
}DeviceConfigSlotType;

typedef struct {
    std::array<DeviceConfigSlotType, NUMBER_OF_CONFIG_SLOTS> slots;
}ConfigSlotsDataType;

typedef struct
{
    uint8_t data[PERSISTENT_DATABLOCK_SIZE];

    static size_t getSize() {
        return PERSISTENT_DATABLOCK_SIZE;
    }
}PersistentDataBlock;

#endif 
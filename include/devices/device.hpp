#ifndef DEVICE_H
#define DEVICE_H
#include "Arduino.h"

#define NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION 50

#define SERVICE_OVERLOADING_FUNCTION_INDEX 1
#define SERVICE_NAME_INDEX 0

enum {
    serviceCall_NoParams,
    serviceCall_1,
    serviceCall_2,
    serviceCall_3
} ServiceOverloadingFunction;

typedef enum
{
    SERV_SUCCESS,
    SERV_EXECUTION_FAILURE,
    SERV_GENERAL_FAILURE,
    SERV_NOT_SUPPORTED
}ServiceRequestErrorCode;

typedef enum
{
    DEVSERVICE_ENABLE,
    DEVSERVICE_DISABLE,
    DEVSERVICE_SET_DATA,
    DEVSERVICE_GET_DATA,
    DEVSERVICE_STATE_SWITCH,
    DEVSERVICE_BRIGHTNESS_CHANGE
}DeviceServicesType;

typedef enum {
    type_ONOFFDEVICE = 100,
    type_TESTDEVICETYPE,
    type_LEDSTRIP
}DevType;

typedef struct
{
    uint8_t a = 0xff;
    uint8_t b = 0xff;
    uint8_t c = 0xff;
    uint8_t d = 0xff;
    uint8_t e = 0xff;
}ServiceParameters_set1;

typedef struct
{
    float a = 0xff;
    float b = 0xff;
    float c = 0xff;
    float d = 0xff;
    float e = 0xff;
}ServiceParameters_set2;

typedef struct
{
    uint8_t* buff = 0;
    uint8_t size = 0xff;
}ServiceParameters_set3;

// struct MACAddress
// {
//     MACAddress(){
//         memset(bytes, 0xff, 6);
//     }

//     MACAddress(uint64_t mac){
//         memcpy(bytes, &mac, 6);
//     }

//     MACAddress(const MACAddress& other){
//         this->bytes[0] = other.bytes[0];
//         this->bytes[1] = other.bytes[1];
//         this->bytes[2] = other.bytes[2];
//         this->bytes[3] = other.bytes[3];
//         this->bytes[4] = other.bytes[4];
//         this->bytes[5] = other.bytes[5];
//     }

//     uint8_t bytes[6];

//     void print(){
//         Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n", bytes[0], bytes[1],bytes[2],bytes[3],bytes[4],bytes[5]);
//     }

//     bool isValid(){
//         return (bytes[0] != 255 && 
//                 bytes[1] != 255 && 
//                 bytes[2] != 255 && 
//                 bytes[3] != 255 && 
//                 bytes[4] != 255 && 
//                 bytes[5] != 255);
//     }

//     void clear(){
//         bytes[0] = 255;
//         bytes[1] = 255;
//         bytes[2] = 255;
//         bytes[3] = 255;
//         bytes[4] = 255;
//         bytes[5] = 255;
//     }

//     uint64_t toUint64(){
//         uint64_t value = 0;
//         memcpy(&value, bytes, 6);

//         return value;
//     }
// };

// inline bool operator==(const MACAddress& lhv, const MACAddress& rhv){
//         return (lhv.bytes[0] == rhv.bytes[0] && 
//                 lhv.bytes[1] == rhv.bytes[1] && 
//                 lhv.bytes[2] == rhv.bytes[2] && 
//                 lhv.bytes[3] == rhv.bytes[3] && 
//                 lhv.bytes[4] == rhv.bytes[4] && 
//                 lhv.bytes[5] == rhv.bytes[5]);
//     }


typedef struct
{
    uint8_t deviceType = 255;
    uint64_t macAddress;
    uint8_t deviceId = -1;
    uint8_t roomId = 255;
    bool isEnabled;
    uint8_t customBytes [NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION];
    
    String deviceName;
    void print() {
        //Serial.println(" ======DEVICE DESCRIPTION=============");
        Serial.print("deviceType: " + String(deviceType) + " - ");
        Serial.print("macAddress: " + String(macAddress) + " - ");
        Serial.print("deviceId: " + String(deviceId) + " - ");
        Serial.print("isEnabled: " + String(isEnabled) + " - ");
        Serial.println("deviceName: " + deviceName);
        Serial.println();
        Serial.print(" customBytes: ");
        Serial.println(" dla OnOff [0] = brightnessLevelSupport, [1] = brightnessLevel");
        Serial.println();        
        for(int i = 0; i < NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION; i++) {
            Serial.print(String(customBytes[i]) + " ");
        }        
        Serial.println();
        }
}DeviceDescription;


class Device
{
private:
    uint8_t DeviceIdentifier = 0xFF;
    
public:

    virtual void init() = 0; //funkcje ktore nazucaja potomka koniecznosc ich implementacji
    virtual void cyclic() = 0;
    virtual DeviceDescription getDeviceDescription() = 0;
    virtual uint8_t getDeviceIdentifier() = 0;
    virtual uint8_t getDeviceType() = 0;

    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param) = 0;
   
};



#endif
#ifndef DEVICE_H
#define DEVICE_H
#include "Arduino.h"
#include "os/datacontainer/NvmConfigSlotDefinition.hpp"
#include "AdvancedControls.hpp"

#define NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION 50
#define DEVICE_NAME_MAX_LENGHT 25

#define SERVICE_OVERLOADING_FUNCTION_INDEX 1
#define SERVICE_NAME_INDEX 0

enum ServiceOverloadingFunction{
    serviceCall_NoParams,
    serviceCall_1,
    serviceCall_2,
    serviceCall_3,
    serviceCall_INVALID
};

typedef enum
{
    SERV_SUCCESS,
    SERV_EXECUTION_FAILURE,
    SERV_GENERAL_FAILURE,
    SERV_BUSY,
    SERV_PENDING,
    SERV_NOT_SUPPORTED
}ServiceRequestErrorCode;

typedef enum
{
    DEVSERVICE_ENABLE,
    DEVSERVICE_DISABLE,
    DEVSERVICE_SET_DATA,
    DEVSERVICE_GET_DATA,
    DEVSERVICE_STATE_SWITCH,
    DEVSERVICE_LED_STRIP_SAVE_CONTENT,
    DEVSERVICE_LED_STRIP_SWITCH_CONTENT,
    DEVSERVICE_BRIGHTNESS_CHANGE,
    DEVSERVICE_SET_EXT_MEMORY_PTR,
    DEVSERVICE_GET_ADVANCED_CONTROLS,
    DEVSERVICE_GET_DETAILED_COLORS,
    DEVSERVICE_SET_DETAILED_COLORS,
    DEVSERVICE_ROOM_STATE_CHANGE,
    DEVSERVICE_INVALID
}DeviceServicesType;

typedef enum {
    type_ONOFFDEVICE = 43,
    type_LED_STRIP,
    type_TEMP_SENSOR,
    type_LED_STRIP_SEGMENTED,
    type_DEVICE_TYPE_LAST = type_LED_STRIP_SEGMENTED
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

typedef enum{
    e_OUT_from_DEVICE = 15,
    e_IN_to_DEVICE,
    e_UNKNOWN_PARAM_DIRECTION
}ServiceDirectionType;

typedef struct
{
    uint8_t* buff = 0;
    uint16_t size = 0xff;
    uint16_t additionalParam = 0xFF;
    uint8_t direction = (uint8_t)e_UNKNOWN_PARAM_DIRECTION;

    void print(){
        Serial.println("ServiceParameters_set3");
        Serial.println("buff: " + String((int)buff));
        Serial.println("size: " + String((int)size));
        Serial.println("additionalParam: " + String((int)additionalParam));
        switch(direction){
            case e_UNKNOWN_PARAM_DIRECTION:
            Serial.println("direction : e_UNKNOWN_PARAM_DIRECTION" );
            break;
            case e_OUT_from_DEVICE:
            Serial.println("direction : e_OUT_from_DEVICE" );
            break;
            case e_IN_to_DEVICE:
            Serial.println("direction : e_IN_to_DEVICE" );
            break;
        }
        
    }
}ServiceParameters_set3;

struct DeviceDescription
{
    uint8_t deviceType = 255;
    uint64_t macAddress = 0LL;
    uint8_t deviceId = -1;
    uint8_t roomId = 255;
    uint8_t isEnabled;
    uint8_t customBytes [NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION] = { 0x00};
    
    String deviceName;

    DeviceDescription(){
        memset(customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    }

    void print() {
        //Serial.println(" ======DEVICE DESCRIPTION=============");
        Serial.print("deviceType: " + String(deviceType) + " - ");
        Serial.print("macAddress: " + String((int)macAddress) + " - ");
        Serial.print("deviceId: " + String(deviceId) + " - ");
        Serial.print("isEnabled: " + String(isEnabled) + " - ");
        Serial.println("deviceName: " + deviceName);
        Serial.println();
        for(int i = 0; i < NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION; i++) {
            Serial.print(String(customBytes[i]) + " ");
        }        
        Serial.println();
        }

    uint16_t getSize(){
        return (sizeof(deviceType) + sizeof(macAddress) + sizeof(deviceId) + sizeof(roomId) + sizeof(isEnabled) + DEVICE_NAME_MAX_LENGHT + NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    };

    bool toByteArray(uint8_t* buffer, uint16_t size){
        uint16_t offset = 0;
        if(buffer != nullptr && size == getSize()){
            memcpy(&buffer[offset], &deviceType, sizeof(deviceType)); 
            offset += sizeof(deviceType);

            memcpy(&buffer[offset], &macAddress, sizeof(macAddress)); 
            offset += sizeof(macAddress);

            memcpy(&buffer[offset], &deviceId, sizeof(deviceId)); 
            offset += sizeof(deviceId);

            memcpy(&buffer[offset], &roomId, sizeof(roomId)); 
            offset += sizeof(roomId);

            memcpy(&buffer[offset], &isEnabled, sizeof(isEnabled)); 
            offset += sizeof(isEnabled);

            /* prepare empty space string value */
            memset(&buffer[offset], '\0', DEVICE_NAME_MAX_LENGHT);
            if(deviceName.length() < DEVICE_NAME_MAX_LENGHT){
                deviceName.getBytes(&buffer[offset], DEVICE_NAME_MAX_LENGHT);
            }else {
                return false;
            }
            offset += DEVICE_NAME_MAX_LENGHT;

            memcpy(&buffer[offset], customBytes, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION); 

            return true;
        }
        return false;
    }


    bool fromByteArray(uint8_t* buffer, uint16_t size){
        uint16_t offset = 0;
        if(buffer != nullptr && size == getSize()){
            memcpy(&deviceType, &buffer[offset], sizeof(deviceType)); 
            offset += sizeof(deviceType);

            memcpy(&macAddress, &buffer[offset], sizeof(macAddress)); 
            offset += sizeof(macAddress);

            memcpy(&deviceId, &buffer[offset], sizeof(deviceId)); 
            offset += sizeof(deviceId);

            memcpy(&roomId, &buffer[offset], sizeof(roomId)); 
            offset += sizeof(roomId);

            memcpy(&isEnabled, &buffer[offset], sizeof(isEnabled)); 
            offset += sizeof(isEnabled);

            /* prepare empty space string value */
            deviceName = String((char*)&buffer[offset]);
            if(deviceName.length() > DEVICE_NAME_MAX_LENGHT){
                return false;
            }
            offset += DEVICE_NAME_MAX_LENGHT;

            memcpy(customBytes, &buffer[offset], NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION); 

            return true;
        }
        else {
            Serial.println("Wrong buffer length");
        }
        return false;
    }
};


class Device
{
private:
    uint8_t DeviceIdentifier = 0xFF;
    
public:
    virtual void init() = 0;
    virtual void cyclic() = 0;
    virtual DeviceDescription getDeviceDescription() = 0;
    virtual uint8_t getDeviceIdentifier() = 0;
    virtual uint8_t getDeviceType() = 0;
    virtual uint16_t getExtendedMemoryLength() = 0;

    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param) = 0;
   
};



#endif
#ifndef RC_PUBLIC_DATA_TYPES
#define RC_PUBLIC_DATA_TYPES
#include <Arduino.h>
#include <devices/device.hpp>

#define REQUEST_DATA_SIZE 30
#define RESPONSE_DATA_SIZE 10
#define REQEST_SIZE (REQUEST_DATA_SIZE + 13)

typedef enum {
    REQ_FIRST,
    SERVICE_CALL_REQ = REQ_FIRST,
    UNKNOWN_REQ,
    REQ_COUNT = UNKNOWN_REQ
}RequestType;

typedef struct {
    uint8_t requestId = 255;
    uint64_t targetNodeMAC;
    uint8_t targetDeviceId = 255;
    uint8_t type = UNKNOWN_REQ;
    uint8_t data[REQUEST_DATA_SIZE] = {0xFF};
    uint8_t requestSendCount = 0;
    uint16_t crc = 5;

    bool toByteArray(uint8_t* buffer, uint8_t sizeCheck){
        if((buffer != 0) && (sizeCheck == this->getSize())) {
            buffer[0] = requestId;
            memcpy(&buffer[1], &targetNodeMAC, sizeof(uint64_t));
            buffer[8] = targetDeviceId;
            buffer[9] = type;
            memcpy(&buffer[10], data, REQUEST_DATA_SIZE);
            buffer[41] = requestSendCount;
            
            calculateCrc();
            memcpy(&buffer[42], &crc, 2);

            return true;
        }

        return false;
    }

    void calculateCrc(){
        crc = requestId + targetNodeMAC + targetDeviceId + type + requestSendCount ;
        for(uint8_t i=0; i< REQUEST_DATA_SIZE; i++){
            crc += data[i];
        }
    }

    uint8_t getSize(){
             
        return REQEST_SIZE;
    }
    
    void print() {
        Serial.println("### Request ###");
        Serial.println("requestId :" + String((int)requestId));
        Serial.println("macAddress: " + String(targetNodeMAC));
        Serial.println("targetDeviceId :" + String((int)targetDeviceId));
        Serial.println("type :" + String((int)type));
        for(uint8_t i=0; i<REQUEST_DATA_SIZE; i++) {
            Serial.print((int)data[i]);
        }
        Serial.println("");
        Serial.println("requestSendCount :" + String((int)requestSendCount));      
        Serial.println("crc :" + String((int)crc));
        Serial.println("###############");
    }

}RcRequest;

typedef enum {
    POSITIVE_RESP,
    NEGATIVE_RESP,
    INVALID_REQ_RESP,
    UNKNOWN_RESP
}ResponseType;

typedef struct {
    uint8_t responseId;
    uint64_t responseNodeMAC = 0LL;
    uint8_t requestType = UNKNOWN_REQ;
    uint8_t responseType = UNKNOWN_RESP;   
    uint8_t data[REQUEST_DATA_SIZE] = {0xFF};
    uint16_t crc ;

    uint8_t getSize(){
             
        return 34;
    }

    void print() {
        Serial.println("   Response ");
        Serial.println("responseId :" + String((int)responseId));
        Serial.println("macAddress: " + String(responseNodeMAC));
        Serial.println("requestType :" + String((int)requestType));
        Serial.println("responseType :" + String((int)responseType));
        Serial.print(" Payload : ");
        for(uint8_t i=0; i<REQUEST_DATA_SIZE; i++) {
            Serial.print((int)data[i]);
        }
        Serial.println("");
             
        Serial.println("crc :" + String((int)crc));
        Serial.println(" ");
    }

}RcResponse;

#endif
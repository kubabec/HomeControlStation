#ifndef RC_PUBLIC_DATA_TYPES
#define RC_PUBLIC_DATA_TYPES
#include <Arduino.h>
#include <devices/device.hpp>
#include <os/app/remoteControl/rcRequest.hpp>
#include "os/Logger.hpp"
#include <os/app/remoteControl/rcResponse.hpp>

#define REQUEST_DATA_SIZE 30
#define RESPONSE_DATA_SIZE 10
#define REQEST_SIZE (REQUEST_DATA_SIZE + 13)
#define RESPONSE_DATA_SIZE_LONG 100



// typedef struct {
//     uint8_t requestId = 255;
//     uint64_t targetNodeMAC;
//     uint8_t targetDeviceId = 255;
//     uint8_t type = UNKNOWN_REQ;
//     uint8_t data[REQUEST_DATA_SIZE] = {0xFF};
//     uint8_t requestSendCount = 0;
//     uint16_t crc = 5;

//     bool toByteArray(uint8_t* buffer, uint8_t sizeCheck){
//         if((buffer != 0) && (sizeCheck == this->getSize())) {
//             buffer[0] = requestId;
//             memcpy(&buffer[1], &targetNodeMAC, sizeof(uint64_t));
//             buffer[8] = targetDeviceId;
//             buffer[9] = type;
//             memcpy(&buffer[10], data, REQUEST_DATA_SIZE);
//             buffer[41] = requestSendCount;
            
//             calculateCrc();
//             memcpy(&buffer[42], &crc, 2);

//             return true;
//         }

//         return false;
//     }

//     void calculateCrc(){
//         crc = requestId + targetNodeMAC + targetDeviceId + type + requestSendCount ;
//         for(uint8_t i=0; i< REQUEST_DATA_SIZE; i++){
//             crc += data[i];
//         }
//     }

//     uint8_t getSize(){
             
//         return REQEST_SIZE;
//     }
    
//     void print() {
//         Logger::log("### Request ###");
//         Logger::log("requestId :" + String((int)requestId));
//         Logger::log("macAddress: " + String(targetNodeMAC));
//         Logger::log("targetDeviceId :" + String((int)targetDeviceId));
//         Logger::log("type :" + String((int)type));
//         for(uint8_t i=0; i<REQUEST_DATA_SIZE; i++) {
//             Logger::log((int)data[i]);
//         }
//         Logger::log("");
//         Logger::log("requestSendCount :" + String((int)requestSendCount));      
//         Logger::log("crc :" + String((int)crc));
//         Logger::log("###############");
//     }

// }RcRequest;



// typedef struct {
//     uint8_t responseId;
//     uint64_t responseNodeMAC = 0LL;
//     uint8_t requestType = UNKNOWN_REQ;
//     uint8_t responseType = UNKNOWN_RESP;   
//     uint8_t data[REQUEST_DATA_SIZE] = {0xFF};
//     uint16_t crc ;

//     uint8_t getSize(){
             
//         return 34;
//     }

//     void print() {
//         Logger::log("   Response ");
//         Logger::log("responseId :" + String((int)responseId));
//         Logger::log("macAddress: " + String(responseNodeMAC));
//         Logger::log("requestType :" + String((int)requestType));
//         Logger::log("responseType :" + String((int)responseType));
//         Logger::log(" Payload : ");
//         for(uint8_t i=0; i<REQUEST_DATA_SIZE; i++) {
//             Logger::log((int)data[i]);
//         }
//         Logger::log("");
             
//         Logger::log("crc :" + String((int)crc));
//         Logger::log(" ");
//     }

// }RcResponse;

typedef struct {
    uint8_t responseId;
    uint64_t responseNodeMAC = 0LL;
    uint8_t requestType = UNKNOWN_REQ;
    uint8_t responseType = UNKNOWN_RESP;   
    uint8_t data[RESPONSE_DATA_SIZE_LONG] = {0xFF};
    uint16_t crc ;

    uint8_t getSize(){
             
        return 104;
    }

    void print() {
        Logger::log("   Response ");
        Logger::log("responseId :" + String((int)responseId));
        Logger::log("responceNodeMAC :" + String((int)responseNodeMAC));
        Logger::log("requestType :" + String((int)requestType));
        Logger::log("responseType :" + String((int)responseType));
        String Payload = "";
        for(uint8_t i=0; i<REQUEST_DATA_SIZE; i++) {
            Payload += String((int)data[i]);
        }
        Logger::log(" Payload :" + Payload);
             
        Logger::log("crc :" + String((int)crc));
        Logger::log(" ");
    }

}RcResponseLong;

#endif
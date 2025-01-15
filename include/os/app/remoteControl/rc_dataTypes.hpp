#ifndef RC_DATA_TYPES_H
#define RC_DATA_TYPES_H

#include <Arduino.h>
#include <devices/device.hpp>

typedef enum {
    REQUEST_NODE_INITIAL_DATA = 50,
    REQUEST_NODE_DETAILED_DATA,
    REQUEST_KEEP_ALIVE,
    RESPONSE_NODE_INITIAL_DATA,
    RESPONSE_NODE_DETAILED_DATA,
    RESPONSE_KEEP_ALIVE,
    RC_REQUEST,
    RC_RESPONSE,
    REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE,
    RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE
}UdpFrames_RCS; //Remote Control Server

typedef struct {
    uint64_t macAddress = 0LL;
    uint16_t nodeHash = 0;
    uint8_t numberOfDevices = 255;
    bool isValid() {
        return (macAddress != 0LL && numberOfDevices != 255);
        
    }
    
}NodeInitialData;

typedef struct {
    uint64_t mac = 0LL;
    uint16_t nodeHash = 0;

} KeepAliveData;


#endif
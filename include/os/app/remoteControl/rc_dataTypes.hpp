#ifndef RC_DATA_TYPES_H
#define RC_DATA_TYPES_H

#include <Arduino.h>

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
    uint16_t nodeId = 255;
    uint8_t roomId = 255;
    uint16_t nodeHash = 0;
    uint8_t numberOfOnOffDevices = 255;
    uint8_t numberOfLedStrips = 255;
    bool isValid() {
        return (nodeId != 255 && /*roomId != 255 && */ numberOfOnOffDevices != 255 && numberOfLedStrips != 255);
    }
    
}NodeInitialData;

typedef struct {
    uint16_t nodeId = 255;
    uint16_t nodeHash = 0;

} KeepAliveData;


#endif
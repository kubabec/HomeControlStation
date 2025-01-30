#ifndef RC_REQUEST_H
#define RC_REQUEST_H

#include <Arduino.h>
#include <vector>


typedef enum {
    REQ_FIRST,
    SERVICE_CALL_REQ = REQ_FIRST,
    EXTENDED_DATA_DOWNLOAD_REQ,
    UNKNOWN_REQ,
    REQ_COUNT = UNKNOWN_REQ
}RequestType;

class RcRequest {

    uint8_t requestId = 255;
    uint64_t targetNodeMAC;
    uint8_t targetDeviceId = 255;
    uint8_t requestType = UNKNOWN_REQ;
    std::vector<uint8_t> data ;
    uint8_t requestSendCount = 0;
    uint16_t crc; 

    
    

public:
    RcRequest();
    RcRequest(uint8_t id, uint64_t mac, uint8_t requestType);

    uint8_t getSize();
    uint8_t getRequestId();
    uint64_t getRequestNodeMAC();
    uint8_t getRequestType();
    uint8_t getRequestDeviceId();
    std::vector<uint8_t>& getData();
    
    void pushData(uint8_t byte);
    void pushData(uint8_t* data, uint16_t size);

    bool fromByteArray(uint8_t* buffer, uint16_t size);
    void setID(uint8_t id);

    bool toByteArray(uint8_t* buffer, uint16_t size);
    void calculateCrc();

    void clear();
};

#endif
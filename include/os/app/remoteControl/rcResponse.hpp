#ifndef RC_RESPONSE_H
#define RC_RESPONSE_H

#include <Arduino.h>
#include <vector>
#include <os/app/remoteControl/rcRequest.hpp>

#define RC_RESPONSE_MIN_SIZE 11

typedef enum {
    POSITIVE_RESP,
    NEGATIVE_RESP,
    INVALID_REQ_RESP,
    EXTENDED_DATA_DOWNLOAD_RESP,
    UNKNOWN_RESP
}ResponseType;


class RcResponse {

    uint8_t responseId;
    uint64_t responseNodeMAC;
    uint8_t requestType = UNKNOWN_REQ;;
    uint8_t responseType = UNKNOWN_RESP;;
    std::vector<uint8_t> data;
    uint16_t crc;    

public:
    RcResponse();
    RcResponse(uint8_t respId, uint64_t respNodeMAC, uint8_t reqType, uint8_t respType);

    uint8_t getResponseId() ;
    uint64_t getResponseNodeMAC() ;
    uint8_t getRequestType() ;
    uint8_t getResponseType() ;
    std::vector<uint8_t>& getData() ;
    uint16_t getSize();
    void print() ;

    void pushData(uint8_t byte);
    void pushData(uint8_t* data, uint16_t size);
    
    void setResponseId(uint8_t id);
    void setResponseType(uint8_t respType);
    void setRequestType(uint8_t reqType);

    bool fromByteArray(uint8_t* buffer, uint16_t size);
    bool toByteArray(uint8_t* buffer, uint16_t size);

    uint16_t calculateCrc();

    bool isValid();
 
};

#endif // RC_RESPONSE_H
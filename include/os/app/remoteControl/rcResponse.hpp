#ifndef RC_RESPONSE_H
#define RC_RESPONSE_H

#include <Arduino.h>
#include <vector>

class RcResponse {

    uint8_t responseId;
    uint64_t responseNodeMAC;
    uint8_t requestType;
    uint8_t responseType;
    std::vector<uint8_t> data;
    uint16_t crc;

public:
    RcResponse(uint8_t id, uint64_t mac, uint8_t reqType, uint8_t respType);

    uint8_t getResponseId() ;
    uint64_t getResponseNodeMAC() ;
    uint8_t getRequestType() ;
    uint8_t getResponseType() ;
    std::vector<uint8_t>& getData() ;
    uint16_t getCrc() ;

    void setData( std::vector<uint8_t>& data);
    void setCrc(uint16_t crc);

    void print() ;

private:
    
};

#endif // RC_RESPONSE_H
#include "os/app/remoteControl/rcResponse.hpp"
#include <iostream>

RcResponse::RcResponse(uint8_t id, uint64_t mac, uint8_t reqType, uint8_t respType)
    : responseId(id), responseNodeMAC(mac), requestType(reqType), responseType(respType) {}

// Gettery
uint8_t RcResponse::getResponseId()  { return responseId; }
uint64_t RcResponse::getResponseNodeMAC()  { return responseNodeMAC; }
uint8_t RcResponse::getRequestType()  { return requestType; }
uint8_t RcResponse::getResponseType()  { return responseType; }
std::vector<uint8_t>& RcResponse::getData()  { return data; }
uint16_t RcResponse::getCrc()  { return crc; }

// Settery
void RcResponse::setData(std::vector<uint8_t>& data) {
    this->data = data;
}
void RcResponse::setCrc(uint16_t crc) {
    this->crc = crc;
}

// Drukowanie informacji o obiekcie
void RcResponse::print()  {
    std::cout << "Response:" << std::endl;
    std::cout << "  responseId: " << (int)responseId << std::endl;
    std::cout << "  responseNodeMAC: " << responseNodeMAC << std::endl;
    
}
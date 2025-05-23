#include "os/app/remoteControl/rcResponse.hpp"
#include <iostream>



RcResponse::RcResponse(){};
RcResponse::RcResponse(uint8_t respId, uint64_t respNodeMAC, uint8_t reqType, uint8_t respType)
    : responseId(respId), responseNodeMAC(respNodeMAC), requestType(reqType), responseType(respType) {}

// Gettery
uint8_t RcResponse::getResponseId()  { return responseId; }
uint64_t RcResponse::getResponseNodeMAC()  { return responseNodeMAC; }
uint8_t RcResponse::getRequestType()  { 
    return requestType; 
}
uint8_t RcResponse::getResponseType()  { return responseType; }
std::vector<uint8_t>& RcResponse::getData()  { return data; }


uint16_t RcResponse::getSize() { 
    size_t fixedSize = sizeof(responseId) + sizeof(responseNodeMAC) + sizeof(requestType) + sizeof(responseType) + sizeof(crc);
    // Rozmiar pola data (dynamiczny)
    size_t dataSize = data.size() ; // Zakładamy, że data jest wektorem uint8_t
    // Całkowity rozmiar obiektu
    return fixedSize + dataSize; 
}

void RcResponse::print() {
    Serial.println("### Response ###");
    Serial.println("responseId: " + String((int)responseId));
    Serial.println("macAddress: " + String((int)responseNodeMAC));
    Serial.println("requestType: " + String((int)requestType));
    Serial.println("responseType: " + String((int)responseType));
    int i = 0;  
    for(auto& byte : data) {
        Serial.print((int)byte);
        i++;
    }
    Serial.println("");

    Serial.println("crc: " + String((int)crc));
    Serial.println("###############");
}

void RcResponse::setResponseId(uint8_t id)
{
    responseId=id;
}

void RcResponse::setRequestType(uint8_t reqType)
{
    requestType=reqType;
}

void RcResponse::pushData(uint8_t byte) {
    data.push_back(byte);
}

//wstawiamy wektor do wektora data
void RcResponse::pushData(uint8_t* data, uint16_t size) {
    if(data != nullptr && size > 0 ){
        this->data.insert(this->data.end(), data, data + size);
    }
}

bool RcResponse::fromByteArray(uint8_t* buffer, uint16_t size) {
    if (buffer == nullptr || size < RC_REQUEST_MIN_SIZE) {
        return false; // Błąd: nieprawidłowy bufor lub za mały rozmiar
    }

    const uint8_t dataLengthBeforeDynamicPart = 
        sizeof(responseId) + sizeof(responseNodeMAC) + sizeof(requestType) + sizeof(responseType);
    const uint8_t dataLengthAfterDynamicPart = sizeof(crc);

    /* copy first part */
    uint16_t offset = 0;
    memcpy(&responseId, &buffer[offset], sizeof(responseId)); 
    offset += sizeof(responseId);
    memcpy(&responseNodeMAC, &buffer[offset], sizeof(responseNodeMAC)); 
    offset += sizeof(responseNodeMAC);
    memcpy(&requestType, &buffer[offset], sizeof(requestType)); 
    offset += sizeof(requestType);
    memcpy(&responseType, &buffer[offset], sizeof(responseType)); 
    offset += sizeof(responseType);


    /* copy payload data if any */
    uint16_t payloadLength = size - (dataLengthBeforeDynamicPart + dataLengthAfterDynamicPart); 
    data.clear(); 
    // Serial.println("Adding payload to Response ");
    for (uint16_t i = 0; i < payloadLength; i++) {
        data.push_back(buffer[offset + i]); 
    }
    offset += data.size();
    // Serial.println("Added payload to Response ");

    /* copy last part */
    memcpy(&crc, &buffer[offset], sizeof(crc));   
    
    return true;
}

bool RcResponse::toByteArray(uint8_t* buffer, uint16_t size) {
    if (buffer == nullptr ) {
        return false; 
    }  
    crc = calculateCrc();

    /* copy first part */
    uint16_t offset = 0;
    memcpy(&buffer[offset], &responseId, sizeof(responseId)); 
    offset += sizeof(responseId);
    memcpy(&buffer[offset], &responseNodeMAC, sizeof(responseNodeMAC)); 
    offset += sizeof(responseNodeMAC);
    memcpy(&buffer[offset], &requestType, sizeof(requestType)); 
    offset += sizeof(requestType);
    memcpy(&buffer[offset], &responseType, sizeof(responseType)); 
    offset += sizeof(responseType);

    memcpy(&buffer[offset], data.data(), data.size());
    offset += data.size();
    memcpy(&buffer[offset], &crc, sizeof(crc));    

    // Serial.println("### Response toByteArray ###");
    return true;
}


uint16_t RcResponse::calculateCrc()
{
    uint16_t tmpCrc = 0;
    tmpCrc += responseId;
    tmpCrc += (uint16_t)responseNodeMAC;
    tmpCrc += requestType;
    tmpCrc += responseType;
    for(auto& byte : data){
        tmpCrc += byte;
    }
    
    return tmpCrc;
}


bool RcResponse::isValid()
{
    // Serial.println("CRC:| "+ String((int)crc));
    // Serial.println("CRC:| "+ String((int)calculateCrc()));
    return (crc == calculateCrc());
}

void RcResponse::setResponseType(uint8_t respType) {
    responseType = respType;
}
#include "os/app/remoteControl/rcRequest.hpp"




RcRequest::RcRequest(){}

RcRequest::RcRequest(uint8_t targetDeviceId, uint64_t targetNodeMAC, uint8_t requestType)
    : targetDeviceId(targetDeviceId), targetNodeMAC(targetNodeMAC), requestType(requestType) {

}

RcRequest& RcRequest::operator=(RcRequest& other) {
    requestId = other.requestId;
    targetNodeMAC = other.targetNodeMAC;
    targetDeviceId = other.targetDeviceId;
    requestType = other.requestType;
    data = other.data;
    requestSendCount = other.requestSendCount;
    crc = other.crc;
    return *this;
}

uint8_t RcRequest::getRequestId()  { 
    return requestId;
}
uint64_t RcRequest::getRequestNodeMAC()  { 
    return targetNodeMAC; }
uint8_t RcRequest::getRequestType()  { return requestType; 
}
uint8_t RcRequest::getRequestDeviceId()  { 
    return targetDeviceId; 
}


void RcRequest::setID(uint8_t id) { 
    requestId = id;
    this->print();
}

uint8_t RcRequest::getSize() {
    size_t fixedSize = sizeof(requestId) + sizeof(targetNodeMAC) + sizeof(targetDeviceId) + sizeof(requestType) + sizeof(requestSendCount) + sizeof(crc);
    // Rozmiar pola data (dynamiczny)
    size_t dataSize = data.size() ; // Zakładamy, że data jest wektorem uint8_t
    // Całkowity rozmiar obiektu
    return fixedSize + dataSize; 
}

std::vector<uint8_t>& RcRequest::getData() { return data; }

//wstawiamy pojedynczy bajt do wektora data
void RcRequest::pushData(uint8_t byte) {
    data.push_back(byte);
}

//wstawiamy wektor do wektora data
void RcRequest::pushData(uint8_t* data, uint16_t size) {
    if(data != nullptr && size > 0 ){
        this->data.resize(size);
        memcpy(this->data.data(), data, size);
    }
}


bool RcRequest::fromByteArray(uint8_t* buffer, uint16_t size) {
    if (buffer == nullptr || size < RC_REQUEST_MIN_SIZE) {
        return false; // Błąd: nieprawidłowy bufor lub za mały rozmiar
    }
    
    const uint8_t dataLengthBeforeDynamicPart = 
        sizeof(requestId) + sizeof(targetNodeMAC) + sizeof(targetDeviceId) + sizeof(requestType);
    const uint8_t dataLengthAfterDynamicPart = 
        sizeof(requestSendCount) + sizeof(crc);

    /* copy first part */
    uint16_t offset = 0;
    memcpy(&requestId, &buffer[offset], sizeof(requestId)); 
    offset += sizeof(requestId);
    memcpy(&targetNodeMAC, &buffer[offset], sizeof(targetNodeMAC)); 
    offset += sizeof(targetNodeMAC);
    memcpy(&targetDeviceId, &buffer[offset], sizeof(targetDeviceId)); 
    offset += sizeof(targetDeviceId);
    memcpy(&requestType, &buffer[offset], sizeof(requestType)); 
    offset += sizeof(requestType);


    /* copy payload data if any */
    uint16_t payloadLength = size - (dataLengthBeforeDynamicPart + dataLengthAfterDynamicPart); 
    data.clear(); 
    for (uint16_t i = 0; i < payloadLength; i++) {
        data.push_back(buffer[offset + i]); 
    }
    offset += data.size();

    /* copy last part */
    memcpy(&requestSendCount, &buffer[offset], sizeof(requestSendCount));
    offset += sizeof(requestSendCount);
    memcpy(&crc, &buffer[offset], sizeof(crc));    
    
    return true;
}

bool RcRequest::toByteArray(uint8_t* buffer, uint16_t size) {
    if (buffer == nullptr ) {
        return false; 
    }

    /* copy first part */
    uint16_t offset = 0;
    memcpy(&buffer[offset], &requestId, sizeof(requestId)); 
    offset += sizeof(requestId);
    memcpy(&buffer[offset], &targetNodeMAC, sizeof(targetNodeMAC)); 
    offset += sizeof(targetNodeMAC);
    memcpy(&buffer[offset], &targetDeviceId, sizeof(targetDeviceId)); 
    offset += sizeof(targetDeviceId);
    memcpy(&buffer[offset], &requestType, sizeof(requestType)); 
    offset += sizeof(requestType);

    memcpy(&buffer[offset], data.data(), data.size());
    offset += data.size();

    /* copy last part */
    memcpy(&buffer[offset], &requestSendCount, sizeof(requestSendCount));
    offset += sizeof(requestSendCount);
    memcpy(&buffer[offset], &crc, sizeof(crc));    

    return true;
}

void RcRequest::calculateCrc() {
    crc = requestId + targetNodeMAC + targetDeviceId + requestType + requestSendCount;
    for (uint8_t i = 0; i < data.size(); i++) {
        crc += data[i];
    }
    
}

void RcRequest::print() {
    Serial.println("Request ID: " + String((int)requestId));
    Serial.println("Target Node MAC: " + String(targetNodeMAC));
    Serial.println("Target Device ID: " + String((int)targetDeviceId));
    Serial.println("Request Type: " + String((int)requestType));
    Serial.println("Data: ");
    for (uint8_t i = 0; i < data.size(); i++) {
        Serial.print((int)data[i]);
        Serial.print(" ");
    }
    Serial.println();
    Serial.println("Request Send Count: " + String((int)requestSendCount));
    Serial.println("CRC: " + String(crc));
}

void RcRequest::clear() {
    requestId = 255;
    targetNodeMAC = 0LL;
    targetDeviceId = 255;
    requestType = UNKNOWN_REQ;
    data.clear();
    requestSendCount = 0;
    crc = 5;
}

void RcRequest::setRequestSendCount(uint8_t count)
{
    requestSendCount = count;
}

uint8_t RcRequest::getRequestSendCount()
{
    return requestSendCount;
}

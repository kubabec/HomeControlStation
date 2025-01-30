#include "os/app/remoteControl/rcRequest.hpp"


RcRequest::RcRequest(){}

RcRequest::RcRequest(uint8_t id, uint64_t mac, uint8_t requestType)
    : requestId(id), targetNodeMAC(mac), requestType(requestType) {

}

uint8_t RcRequest::getRequestId()  { 
    return requestId; 
}

uint64_t RcRequest::getRequestNodeMAC()  { 
    return targetNodeMAC; 
}

uint8_t RcRequest::getRequestType()  { 
    return requestType; 
}

uint8_t RcRequest::getRequestDeviceId()  { 
    return targetDeviceId; 
}


void RcRequest::setID(uint8_t id) {
    requestId = id;
}

uint8_t RcRequest::getSize() { 
    size_t fixedSize = sizeof(requestId) + sizeof(targetNodeMAC) + sizeof(targetDeviceId) + sizeof(requestType) + sizeof(requestSendCount) + sizeof(crc);

    // Rozmiar pola data (dynamiczny)
    size_t dataSize = data.size() ; // Zakładamy, że data jest wektorem uint8_t

    // Całkowity rozmiar obiektu
    return fixedSize + dataSize; 
}

std::vector<uint8_t>& RcRequest::getData() {
     
    return data; 

}

//wstawiamy pojedynczy bajt do wektora data
void RcRequest::pushData(uint8_t byte) {
    data.push_back(byte);
}

//wstawiamy wektor do wektora data
void RcRequest::pushData(uint8_t* data, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        this->data.push_back(data[i]);
    }
}


bool RcRequest::fromByteArray(uint8_t* buffer, uint16_t size) {
    if (buffer == nullptr || size < 11) {
        return false; // Błąd: nieprawidłowy bufor lub za mały rozmiar
    }

    // Kopiowanie requestId, targetNodeMAC, targetDeviceId, type
    memcpy(&requestId, &buffer[0], 1); // requestId to 1 bajt
    memcpy(&targetNodeMAC, &buffer[1], 8); // targetNodeMAC to 8 bajtów
    memcpy(&targetDeviceId, &buffer[9], 1); // targetDeviceId to 1 bajt
    memcpy(&requestType, &buffer[10], 1); // type to 1 bajt

    // Obliczenie rozmiaru danych w wektorze data
    uint16_t dataSize = size - 14; // 11 bajtów (pola przed data) + 3 bajty (pola po data) = 14 bajtów

    // Kopiowanie danych do wektora data
    data.clear(); // Czyścimy wektor przed wstawieniem nowych danych
    for (uint16_t i = 0; i < dataSize; i++) {
        data.push_back(buffer[11 + i]); // Kopiujemy bajty do wektora
    }

    // Kopiowanie requestSendCount i crc
    memcpy(&requestSendCount, &buffer[size - 3], 1); // requestSendCount to 1 bajt
    memcpy(&crc, &buffer[size - 2], 2); // crc to 2 bajty

    return true;
}


bool RcRequest::toByteArray(uint8_t* buffer, uint16_t size) {
    if (buffer == nullptr ) {
        return false; // Błąd: nieprawidłowy bufor lub rozmiar
    }
    //kopiujemy requestId, targetNodeMAC, targetDeviceId, requestType
    memcpy(&(buffer[0]), &requestId, 11);
    //kopiuje dane z data do buffer
    uint16_t index = 11;
    for(auto& byte : data) {
        buffer[index] = byte;
        index++;
    }
    // kopiujemy requestSendCount i crc
    memcpy(&(buffer[size-3]), &requestSendCount, 3);

    return true;
}

void RcRequest::calculateCrc() {
    crc = requestId + targetNodeMAC + targetDeviceId + requestType + requestSendCount;
    for (uint8_t i = 0; i < data.size(); i++) {
        crc += data[i];
    }

    
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

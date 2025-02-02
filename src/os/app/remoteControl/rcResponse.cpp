#include "os/app/remoteControl/rcResponse.hpp"
#include <iostream>



RcResponse::RcResponse(){};
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

bool RcResponse::fromByteArray(uint8_t* buffer, uint16_t size) {
    memcpy(&responseId, &buffer[0], RC_RESPONSE_MIN_SIZE); // Kopiowanie requestId, targetNodeMAC, targetDeviceId, requestType
    // Obliczenie rozmiaru danych w wektorze data
    uint16_t dataSize = size - 14; // 11 bajtów (pola przed data) + 3 bajty (pola po data) = 14 bajtów

    // Kopiowanie danych do wektora data
    data.clear(); // Czyścimy wektor przed wstawieniem nowych danych
    for (uint16_t i = 0; i < dataSize; i++) {
        data.push_back(buffer[11 + i]); // Kopiujemy bajty do wektora
    }

    // Kopiowanie requestSendCount i crc
    memcpy(&responseSendCount, &buffer[size - 3], 1); // requestSendCount to 1 bajt
    memcpy(&crc, &buffer[size - 2], 2); // crc to 2 bajty

    return true;
}

bool RcResponse::toByteArray(uint8_t* buffer, uint16_t size) {
    if (buffer == nullptr ) {
        return false; // Błąd: nieprawidłowy bufor lub rozmiar
    }
    //kopiujemy requestId, targetNodeMAC, targetDeviceId, requestType
    memcpy(&(buffer[0]), &responseId, 11);
    //kopiuje dane z data do buffer
    uint16_t index = 11;
    for(auto& byte : data) {
        buffer[index] = byte;
        index++;
    }
    // kopiujemy responseSendCount i crc
    memcpy(&(buffer[size-3]), &responseSendCount, 3);

    return true;
}


uint8_t RcResponse::getSize() { 
    size_t fixedSize = sizeof(responseId) + sizeof(responseNodeMAC) + sizeof(requestType) + sizeof(responseType) + sizeof(responseSendCount) + sizeof(crc);
    // Rozmiar pola data (dynamiczny)
    size_t dataSize = data.size() ; // Zakładamy, że data jest wektorem uint8_t
    // Całkowity rozmiar obiektu
    return fixedSize + dataSize; 
}

void RcResponse::responsePrint() {
    std::cout << "### Response ###" << std::endl;
    std::cout << "responseId: " << (int)responseId << std::endl;
    std::cout << "macAddress: " << responseNodeMAC << std::endl;
    std::cout << "requestType: " << (int)requestType << std::endl;
    std::cout << "responseType: " << (int)responseType << std::endl;
    for(auto& byte : data) {
        std::cout << (int)byte;
    }
    std::cout << std::endl;
    std::cout << "responseSendCount: " << (int)responseSendCount << std::endl;
    std::cout << "crc: " << (int)crc << std::endl;
    std::cout << "###############" << std::endl;
}

void RcResponse::setResponseId(uint8_t id)
{
    responseId=id;
}

void RcResponse::setRequestType(uint8_t reqType)
{
    requestType=reqType;
}

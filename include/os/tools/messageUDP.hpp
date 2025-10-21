#ifndef MESSAGE_UDP_H
#define MESSAGE_UDP_H
#include <Arduino.h>
#include <vector>
// #include <WiFi.h>
// #include <WiFiUdp.h>

class MessageUDP
{
public:
    struct IPAddr
    {
        uint8_t octet1;
        uint8_t octet2;
        uint8_t octet3;
        uint8_t octet4;

        bool operator==(const IPAddr &rhv)
        {
            if (this->octet1 == rhv.octet1 &&
                this->octet2 == rhv.octet2 &&
                this->octet3 == rhv.octet3 &&
                this->octet4 == rhv.octet4)
            {
                return true;
            }

            return false;
        }

        String toString()
        {
            static char buffer[16];
            sprintf(buffer, "%d.%d.%d.%d", octet1, octet2, octet3, octet4);
            return String(buffer);
        }
    };

private:
    int id = -1;            /* uint8_t */
    uint16_t totalSize = 0; /* Total size of message in bytes ^^^*/
    MessageUDP::IPAddr ipAddress{0, 0, 0, 0};
    int udpPort = 3333;

    std::vector<byte> dataBuffer; /* Data buffer for message content */

    int byteIterationIndex = -1; /* current byte while iterating through all the message to send */

    static const uint8_t lastByteCheckValue = 0b01010101;
    void updateTotalSize();

    static bool validateMessageId(int messageId);
    static bool validateTotalSize(uint16_t bufferSize, uint16_t tmpSize);
    static bool validateLastByteValue(uint8_t lastByteValue);
    static bool validateIpAddress(IPAddr &addr);

    static uint8_t getMinimumSize();

public:
    MessageUDP();
    MessageUDP(int myId, MessageUDP::IPAddr addr, int port);
    MessageUDP(int myId, MessageUDP::IPAddr addr, int port, byte *buffer, uint8_t bufferSize);

    int getId();

    void setIpAddress(MessageUDP::IPAddr addr);
    void setPort(int port);

    bool pushData(byte data);
    bool pushData(byte *arr, size_t size);

    size_t getSize();
    bool isValid();
    bool toByteArray(byte *bufferPtr, size_t sizeCheck);

    // static MessageUART fromByteArray(byte* dataPtr, uint8_t dataSize);
    static MessageUDP fromUint8Vector(std::vector<uint8_t> &vec);
    static void serialPrintMessageUDP(MessageUDP &msg);

    /* Interfaces to reach byte by byte to transmitt via I2C*/
    /* Reset current intex counter */
    void resetByteIterationCount();
    /* Read current byte */
    uint8_t getCurrentByte();
    /* Moves to the next byte; if next byte exist returns true, otherwise false */
    bool switchToNextByte();

    // payload accessor
    std::vector<uint8_t> &getPayload();

    int getPort();
    MessageUDP::IPAddr &getIPAddress();
};

#endif
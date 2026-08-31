#ifndef MESSAGE_UDP_H
#define MESSAGE_UDP_H
#include <Arduino.h>
#include <vector>
// #include <WiFi.h>
// #include <WiFiUdp.h>
/**
 * @class MessageUDP
 * @brief Wraps a UDP payload together with routing metadata needed by the Home Control Station.
 *
 * The class validates packet IDs and sizes, stores the payload bytes, and provides helpers to encode
 * and decode the message for transmission or inspection.
 */

class MessageUDP
{
public:
/**
 * @struct IPAddr
 * @brief IPv4 address representation used by the UDP transport layer.
 */
    struct IPAddr
    {
        /**
         * First octet of the IPv4 address.
         */
        uint8_t octet1;

        /**
         * Second octet of the IPv4 address.
         */
        uint8_t octet2;

        /**
         * Third octet of the IPv4 address.
         */
        uint8_t octet3;

        /**
         * Fourth octet of the IPv4 address.
         */
        uint8_t octet4;

        /**
         * Compares two IPv4 addresses for equality.
         * @param rhv Right-hand value to compare.
         * @return true when all octets match.
         */
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

        /**
         * Formats the IP address as a dotted-decimal string.
         * @return IPv4 string suitable for logs or UI output.
         */
        String toString()
        {
            static char buffer[16];
            sprintf(buffer, "%d.%d.%d.%d", octet1, octet2, octet3, octet4);
            return String(buffer);
        }
    };

private:
    /**
     * Identifier of the UDP message type or family.
     */
    int id = -1;

    /**
     * Total size of the payload in bytes.
     */
    uint16_t totalSize = 0;

    /**
     * Destination or source IP address associated with the packet.
     */
    MessageUDP::IPAddr ipAddress{0, 0, 0, 0};

    /**
     * UDP port used by the message transport.
     */
    int udpPort = 3333;

    /**
     * Raw payload bytes held in the message.
     */
    std::vector<byte> dataBuffer;

    /**
     * Current iteration index used while reading bytes sequentially.
     */
    int byteIterationIndex = -1;

    /**
     * Byte pattern used to validate packet integrity.
     */
    static const uint8_t lastByteCheckValue = 0b01010101;

    /**
     * Recomputes total payload size from the current content.
     */
    void updateTotalSize();

    /**
     * Validates the message identifier.
     * @param messageId ID to validate.
     * @return true when valid.
     */
    static bool validateMessageId(int messageId);

    /**
     * Validates whether the stored buffer size fits the encoded total size.
     * @param bufferSize Available buffer size.
     * @param tmpSize Parsed message size.
     * @return true when the values are consistent.
     */
    static bool validateTotalSize(uint16_t bufferSize, uint16_t tmpSize);

    /**
     * Validates the trailing integrity byte of the packet.
     * @param lastByteValue Last byte to validate.
     * @return true when the value matches the expected pattern.
     */
    static bool validateLastByteValue(uint8_t lastByteValue);

    /**
     * Validates the provided IP address value.
     * @param addr Address to validate.
     * @return true when the address is usable.
     */
    static bool validateIpAddress(IPAddr &addr);

    /**
     * Returns the minimum valid packet size for a MessageUDP object.
     * @return Minimum byte count.
     */
    static uint8_t getMinimumSize();

public:
    /**
     * Creates an empty invalid message instance.
     */
    MessageUDP();

    /**
     * Creates a message for a specific target address and port.
     * @param myId Message identifier.
     * @param addr Destination IP.
     * @param port Destination UDP port.
     */
    MessageUDP(int myId, MessageUDP::IPAddr addr, int port);

    /**
     * Creates a message with a pre-populated byte buffer.
     * @param myId Message identifier.
     * @param addr Destination IP.
     * @param port Destination UDP port.
     * @param buffer Source payload bytes.
     * @param bufferSize Number of bytes in the payload.
     */
    MessageUDP(int myId, MessageUDP::IPAddr addr, int port, byte *buffer, uint8_t bufferSize);

    /**
     * Returns the message ID.
     * @return Message identifier.
     */
    int getId();

    /**
     * Sets the target IP address for the message.
     * @param addr New destination IP address.
     */
    void setIpAddress(MessageUDP::IPAddr addr);

    /**
     * Sets the destination UDP port.
     * @param port New destination port.
     */
    void setPort(int port);

    /**
     * Appends a single payload byte to the message.
     * @param data Byte to append.
     * @return true when the byte was accepted.
     */
    bool pushData(byte data);

    /**
     * Appends a byte array to the message payload.
     * @param arr Buffer to append.
     * @param size Number of bytes in the buffer.
     * @return true when the payload was appended successfully.
     */
    bool pushData(byte *arr, size_t size);

    /**
     * Returns the current payload size in bytes.
     * @return Packed size of the message.
     */
    size_t getSize();

    /**
     * Validates the message structure and buffer contents.
     * @return true when the message is structurally valid.
     */
    bool isValid();

    /**
     * Serializes the message to a raw byte buffer.
     * @param bufferPtr Destination memory.
     * @param sizeCheck Size available in the destination buffer.
     * @return true when serialization succeeds.
     */
    bool toByteArray(byte *bufferPtr, size_t sizeCheck);

    /**
     * Deserializes a MessageUDP object from a vector of bytes.
     * @param vec Source data vector.
     * @return Reconstructed MessageUDP object.
     */
    static MessageUDP fromUint8Vector(std::vector<uint8_t> &vec);

    /**
     * Logs a MessageUDP packet for diagnostic inspection.
     * @param msg Message to print.
     */
    static void serialPrintMessageUDP(MessageUDP &msg);

    /**
     * Resets the byte iteration pointer before sequential reading.
     */
    void resetByteIterationCount();

    /**
     * Returns the next byte in the message during sequential iteration.
     * @return Current byte value.
     */
    uint8_t getCurrentByte();

    /**
     * Advances the byte iterator to the next payload byte.
     * @return true when another byte is available; false when the end of the payload is reached.
     */
    bool switchToNextByte();

    /**
     * Returns the payload buffer used by the message.
     * @return Reference to the internal payload vector.
     */
    std::vector<uint8_t> &getPayload();

    /**
     * Returns the destination UDP port associated with the message.
     * @return UDP port number.
     */
    int getPort();

    /**
     * Returns the target or source IP info used by the message.
     * @return Reference to the stored IP address.
     */
    MessageUDP::IPAddr &getIPAddress();
};

#endif
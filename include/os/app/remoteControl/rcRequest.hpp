#ifndef RC_REQUEST_H
#define RC_REQUEST_H

#include <Arduino.h>
#include <vector>

#define RC_REQUEST_MIN_SIZE 11

typedef enum {
    REQ_FIRST,
    SERVICE_CALL_REQ = REQ_FIRST,
    EXTENDED_DATA_DOWNLOAD_REQ,
    UNKNOWN_REQ,
    REQ_COUNT = UNKNOWN_REQ
}RequestType;
/**
 * @class RcRequest
 * @brief Encodes a request packet sent from the local node to a remote node.
 *
 * The request carries the target node, the target device, the operation type, and a byte payload so
 * the remote side can execute service calls or data transfers in a consistent format.
 */

class RcRequest {
    /**
     * Unique identifier assigned to the request so a response can be matched to the correct sender call.
     */
    uint8_t requestId = 255;

    /**
     * MAC address of the destination node that should process this request.
     */
    uint64_t targetNodeMAC;

    /**
     * Device identifier inside the target node that should receive the call.
     */
    uint8_t targetDeviceId = 255;

    /**
     * Type of the request being sent, such as a service call or data-download request.
     */
    uint8_t requestType = UNKNOWN_REQ;

    /**
     * Raw payload bytes appended to the request body.
     */
    std::vector<uint8_t> data;

    /**
     * Number of times the request has already been transmitted.
     */
    uint8_t requestSendCount = 0;

    /**
     * Cyclic redundancy check value associated with the serialized request.
     */
    uint16_t crc;

public:
    /**
     * Default constructor; creates an empty, invalid request ready for later population.
     */
    RcRequest();

    /**
     * Creates a request targeted at a specific remote node and device.
     * @param targetDeviceId Device ID inside the target node.
     * @param targetNodeMAC MAC address of the destination node.
     * @param requestType Classification of the request.
     */
    RcRequest(uint8_t targetDeviceId, uint64_t targetNodeMAC, uint8_t requestType);

    /**
     * Copies the content of another request object.
     * @param other Existing request to copy from.
     * @return Reference to the updated object.
     */
    RcRequest& operator=(RcRequest& other);

    /**
     * Returns the serialized size of the request packet.
     * @return Number of bytes required to encode the request.
     */
    uint16_t getSize();

    /**
     * Returns the request identifier assigned to this packet.
     * @return Request ID.
     */
    uint8_t getRequestId();

    /**
     * Returns the destination MAC address encoded in the request.
     * @return Target node MAC address.
     */
    uint64_t getRequestNodeMAC();

    /**
     * Returns the type of request encoded in the object.
     * @return Request category, such as a service call or data download.
     */
    uint8_t getRequestType();

    /**
     * Returns the target device ID encoded in the request.
     * @return Device ID of the destination device.
     */
    uint8_t getRequestDeviceId();

    /**
     * Returns the mutable payload buffer for appending or reading raw bytes.
     * @return Reference to the internal payload vector.
     */
    std::vector<uint8_t>& getData();

    /**
     * Appends a single byte to the request payload.
     * @param byte Byte to append.
     */
    void pushData(uint8_t byte);

    /**
     * Appends a raw byte array to the request payload.
     * @param data Buffer containing the payload bytes.
     * @param size Number of bytes in the buffer.
     */
    void pushData(uint8_t* data, uint16_t size);

    /**
     * Deserializes the object from a byte array.
     * @param buffer Source memory containing the serialized request.
     * @param size Number of bytes in the buffer.
     * @return true when parsing succeeded; false when the packet is malformed.
     */
    bool fromByteArray(uint8_t* buffer, uint16_t size);

    /**
     * Serializes the request contents into a byte array.
     * @param buffer Destination memory.
     * @param size Available size in the destination buffer.
     * @return true when serialization succeeded; false when the buffer is too small.
     */
    bool toByteArray(uint8_t* buffer, uint16_t size);

    /**
     * Sets the packet request identifier.
     * @param id New request ID.
     */
    void setID(uint8_t id);

    /**
     * Recomputes the CRC based on the current serialized contents.
     */
    void calculateCrc();

    /**
     * Prints the request structure for debugging and protocol tracing.
     */
    void print();

    /**
     * Clears the request content and resets it to the default empty state.
     */
    void clear();

    /**
     * Sets the number of times the request has been retried.
     * @param count Retry count to store.
     */
    void setRequestSendCount(uint8_t count);

    /**
     * Returns the number of times the request was already retransmitted.
     * @return Retry count for the request.
     */
    uint8_t getRequestSendCount();
};

#endif
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
/**
 * @class RcResponse
 * @brief Encodes the response packet returned by a remote node to a pending request.
 *
 * The response contains the originating node, the original request type, the response class, and raw
 * payload bytes that allow the caller to determine whether the remote service succeeded and which data
 * was produced.
 */

class RcResponse {
    /**
     * Identifier assigned to this response so it can be matched to the originating request.
     */
    uint8_t responseId;

    /**
     * MAC address of the node that generated the response.
     */
    uint64_t responseNodeMAC;

    /**
     * Type of request that triggered this response.
     */
    uint8_t requestType = UNKNOWN_REQ;

    /**
     * Category of the response, such as positive, negative, or invalid request.
     */
    uint8_t responseType = UNKNOWN_RESP;

    /**
     * Raw payload bytes returned by the remote device or protocol layer.
     */
    std::vector<uint8_t> data;

    /**
     * CRC value computed for the response body.
     */
    uint16_t crc;

public:
    /**
     * Default constructor creating an empty response object.
     */
    RcResponse();

    /**
     * Creates a response for a specific request and destination node.
     * @param respId Response identifier.
     * @param respNodeMAC MAC address of the responding node.
     * @param reqType Original request type.
     * @param respType Response category.
     */
    RcResponse(uint8_t respId, uint64_t respNodeMAC, uint8_t reqType, uint8_t respType);

    /**
     * Returns the response identifier.
     * @return Response ID.
     */
    uint8_t getResponseId();

    /**
     * Returns the MAC address of the responding node.
     * @return Responding node MAC address.
     */
    uint64_t getResponseNodeMAC();

    /**
     * Returns the original request type encoded in the response.
     * @return Type of request that produced this response.
     */
    uint8_t getRequestType();

    /**
     * Returns the response classification.
     * @return Response category such as positive or invalid request.
     */
    uint8_t getResponseType();

    /**
     * Returns the payload data stored in the response.
     * @return Mutable payload vector.
     */
    std::vector<uint8_t>& getData();

    /**
     * Returns the serialized size of the response in bytes.
     * @return Byte count required for the response packet.
     */
    uint16_t getSize();

    /**
     * Prints the response structure for debugging and traffic inspection.
     */
    void print();

    /**
     * Appends a single payload byte to the response.
     * @param byte Byte to append.
     */
    void pushData(uint8_t byte);

    /**
     * Appends a raw byte buffer to the response payload.
     * @param data Buffer to append.
     * @param size Number of bytes in the buffer.
     */
    void pushData(uint8_t* data, uint16_t size);

    /**
     * Sets the response ID used for matching the request/response pair.
     * @param id New response identifier.
     */
    void setResponseId(uint8_t id);

    /**
     * Sets the response-type classification.
     * @param respType Response category.
     */
    void setResponseType(uint8_t respType);

    /**
     * Sets the original request type associated with the response.
     * @param reqType Original request category.
     */
    void setRequestType(uint8_t reqType);

    /**
     * Deserializes a response packet from a raw byte array.
     * @param buffer Source bytes containing a serialized response.
     * @param size Number of bytes in the source buffer.
     * @return true when parsing succeeded; false on malformed input.
     */
    bool fromByteArray(uint8_t* buffer, uint16_t size);

    /**
     * Serializes the response into a raw byte array.
     * @param buffer Destination buffer for the serialized response.
     * @param size Available size in the destination buffer.
     * @return true when the response could be encoded successfully.
     */
    bool toByteArray(uint8_t* buffer, uint16_t size);

    /**
     * Recomputes the CRC value for the response object.
     * @return Newly calculated CRC value.
     */
    uint16_t calculateCrc();

    /**
     * Validates the current response object.
     * @return true when the response fields and CRC are coherent; false otherwise.
     */
    bool isValid();
 
};

#endif // RC_RESPONSE_H
#ifndef REMOTE_CONTROL_CLIENT_H
#define REMOTE_CONTROL_CLIENT_H
#include <Arduino.h>
#include <os/drivers/NetworkDriver.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <map>
#include <os/datacontainer/SigMessages.hpp>
#include <os/app/config/ConfigProvider.hpp>
#include <os/app/remoteControl/rc_DataTypes.hpp>
#include <optional>

#define TIME_TO_ASK_FOR_DISCOVERY 45000 // 45s

/**
 * @file include/os/app/remoteControl/remoteControlClient.hpp
 * @brief Remote-control request and response definitions for the Home Control Station.
 */

typedef struct
{
    RcRequest request;
    std::optional<RcResponse> response;
} Transaction;
/**
 * @class RemoteControlClient
 * @brief Implements the node-side RC client that handles discovery, requests, and responses from the master.
 */
class RemoteControlClient
{
    /**
     * UDP packets received from the network and queued for protocol processing.
     */
    static std::queue<MessageUDP> receivedBuffer;

    /**
     * Mapping of request types to callbacks that should consume the corresponding incoming RC requests.
     */
    static std::array<std::function<bool(RcRequest &)>, REQ_COUNT> requestReceivers;

    /**
     * MAC address of the local node used in discovery and response packets.
     */
    static uint64_t localNodeMACAddress;

    /**
     * Queue of pending RC responses waiting to be transmitted back to the master.
     */
    static std::queue<RcResponse> vecResponseMessage;

    /**
     * Outbound UDP messages queued for transmission.
     */
    static std::queue<MessageUDP> pendingTxQueue;

    /**
     * ID of the last request received from the master.
     */
    static uint8_t lastReceivedRequestId;

    /**
     * Timestamp of the last observed master activity.
     */
    static unsigned long lastMasterActivityTime;

    /**
     * Last IP address from which a valid master packet was received.
     */
    static MessageUDP::IPAddr lastKnownMasterIp;

    /**
     * Current request/response transaction being processed by the client.
     */
    static Transaction currentTransaction;

    /**
     * Handles the state where the node is sending its initial device snapshot.
     */
    static void handleNodeInitialDataState();

    /**
     * Handles the state where the node sends detailed device data to the master.
     */
    static void handleNodeDetailedDataState();

    /**
     * Handles the keep-alive exchange with the master node.
     */
    static void handleKeepAliveState();

    /**
     * Parses and routes a UDP request received by the client.
     * @param msg Packet arrived over the network.
     */
    static void processUDPRequest(MessageUDP &msg);

    /**
     * Sends the initial device description payload in response to discovery.
     */
    static void sendInitialDataResponse();

    /**
     * Sends the detailed node description response for the selected UDP frame type.
     * @param udpHeaderValue Type of the detailed data packet being generated.
     */
    static void sendDetailedDataResponse(UdpFrames_RCS udpHeaderValue);

    /**
     * Sends a keep-alive packet to the master node.
     */
    static void sendKeepAlive();

    /**
     * Processes a generic request that does not match one of the specialized states.
     * @param msg UDP request message to process.
     */
    static void processGenericRequest(MessageUDP &msg);

    /**
     * Validates and consumes a pending RC response.
     * @return true when a response was successfully handled.
     */
    static bool processResponse();

    /**
     * Sends all queued outgoing UDP packets waiting in the transmit queue.
     */
    static void processPendingTxData();

    /**
     * Stores the IP address of the most recently seen master node.
     * @param address Network address of the master.
     */
    static void updateLastKnownMasterIp(MessageUDP::IPAddr address);

public:
    /**
     * Initializes the RC client and registers the default handlers.
     */
    static void init();

    /**
     * Clears the client state and releases resources used by the protocol flow.
     */
    static void deinit();

    /**
     * Runs the periodic processing loop for queued messages and keep-alive tracking.
     */
    static void cyclic();

    /**
     * Queues a received UDP payload for protocol processing.
     * @param msg Incoming network packet.
     */
    static void receiveUDP(MessageUDP &msg);

    /**
     * Registers a callback that handles a specific request type.
     * @param request Request type to bind.
     * @param receiverCallback Callback invoked when a request of that type is received.
     * @return true when registration succeeded.
     */
    static bool registerRequestReceiver(RequestType request, std::function<bool(RcRequest &)> receiverCallback);

    /**
     * Sends a prepared response back to the master node.
     * @param response RC response to transmit.
     * @return true when the response is queued or sent successfully.
     */
    static bool sendResponse(RcResponse &response);
};

#endif
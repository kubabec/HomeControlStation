#ifndef REMOTECONTROLSERVER_H
#define REMOTECONTROLSERVER_H
#include <Arduino.h>
#include <os/drivers/NetworkDriver.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <map>
#include <os/datacontainer/SigMessages.hpp>
#include <os/app/remoteControl/rc_DataTypes.hpp>
#include <os/app/remoteControl/ReqestProcessor.hpp>
#include "os/Logger.hpp"

#define TIME_TO_REPEAT_INITIAL_DATA_REQEST 1200
#define TIME_TO_REPEAT_DETAILED_DATA_REQEST 500
#define TIME_TO_REPEAT_KEEP_ALIVE_REQEST 20000
#define TIME_TO_SWITCH_FROM_INITIAL_TO_DETAILED 12000

/**
 * @file include/os/app/remoteControl/remotecontrolserver.hpp
 * @brief Remote-control request and response definitions for the Home Control Station.
 */

/**
 * @struct RemoteNodeInformation
 * @brief Stores the runtime status of a remote node discovered by the RC server.
 */
typedef struct
{
    /**
     * Number of devices present on the remote node at the last synchronized snapshot.
     */
    uint8_t numberOfDevices = 255;

    /**
     * IP address of the remote node used to reach it over UDP.
     */
    MessageUDP::IPAddr nodeIpAddress{0, 0, 0, 0};

    /**
     * Device descriptions collected from the remote node.
     */
    std::vector<DeviceDescription> devicesCollection;

    /**
     * true when the device list for this node is considered complete and no more chunks are expected.
     */
    bool isDeviceCollectionCompleted = false;

    /**
     * Last timestamp when a keep-alive request was sent to the node.
     */
    uint64_t lastKeepAliveRequestedTime = 0;

    /**
     * Last timestamp when a valid keep-alive response was received from the node.
     */
    uint64_t lastKeepAliveReceivedTime = 0;

    /**
     * Hash of the most recent node description used to detect stale data.
     */
    uint16_t lastKnownNodeHash = 0;

    /**
     * Logs the current remote-node snapshot in a readable format.
     */
    void printLn()
    {
        Logger::log("Remote Node Info | Devices: " + String(numberOfDevices) +
                    " | IP: " + String(nodeIpAddress.octet1) + "." + String(nodeIpAddress.octet2) + "." +
                    String(nodeIpAddress.octet3) + "." + String(nodeIpAddress.octet4) +
                    " | Hash: " + String((int)lastKnownNodeHash) +
                    " | Completed: " + String(isDeviceCollectionCompleted) + "] | ---");
        for (auto &device : devicesCollection)
        {
            device.print();
        }
    }
} RemoteNodeInformation;
/**
 * @class RemoteControlServer
 * @brief Coordinates remote-node discovery, request dispatch, and response handling for the OS.
 *
 * The server maintains the list of remote nodes, sends handshake and keep-alive requests, and routes
 * incoming UDP packets to the correct request processor so remote devices can be controlled as if they
 * were local members of the station.
 */

class RemoteControlServer
{
    /**
     * Counter used to generate unique UDP request identifiers.
     */
    static uint8_t requestIdCounter;

    /**
     * true when the remote-node monitor is temporarily blocked because a request is being processed.
     */
    static bool slaveMonitoringBlockedDueToRequestProcessing;

    /**
     * Current state machine state of the RC server.
     */
    static ServerState currentState;

    /**
     * Buffer of incoming UDP packets waiting to be processed.
     */
    static std::queue<MessageUDP> receivedBuffer;

    /**
     * Pending RC requests yet to be dispatched to remote nodes.
     */
    static std::queue<RcRequest> pendingRequestsQueue;

    /**
     * Nodes whose detailed device descriptions need to be refreshed.
     */
    static std::queue<uint64_t> pendingDDRefreshNodeMACs;

    /**
     * Mapping from request type to the callback that handles the associated response.
     */
    static std::array<std::function<bool(RcResponse &)>, REQ_COUNT> responseReceivers;

    /**
     * Request processor responsible for transforming requests into network payloads and working states.
     */
    static RequestProcessor requestProcessor;

    /**
     * MAC address of the remote node currently waiting for detailed device data.
     */
    static uint64_t detailedDataPendingNodeMAC;

    /**
     * Time of the last keep-alive request sent to the remote nodes.
     */
    static uint64_t lastKeepAliveRequestTime;

    /**
     * Container holding the current state and description snapshots for each discovered remote node.
     */
    static std::map<uint64_t, RemoteNodeInformation> remoteNodes;

    /**
     * Requests the initial device snapshot from a remote node.
     */
    static void requestNodeInitialData();

    /**
     * Requests the detailed device description from a remote node.
     */
    static void requestNodeDetailedData();

    /**
     * Sends a keep-alive request to registered remote nodes.
     */
    static void requestKeepAliveData();

    /**
     * Parses and routes a received UDP packet to the relevant RC-server processing routine.
     * @param msg Packet to process.
     */
    static void processUDPMessage(MessageUDP &msg);

    /**
     * Executes the next request waiting in the queue.
     * @param request Request to dispatch.
     * @return true when the request was accepted or completed; false when it could not be processed.
     */
    static bool processPendingRequest(RcRequest &request);

    /**
     * Interprets a received RC response packet and dispatches it to the registered handler.
     * @param msg UDP packet containing the response.
     */
    static void processReceivedRcResponse(MessageUDP &msg);

    /**
     * Handles the state in which the server asks remote nodes for initial device metadata.
     */
    static void handleRequestNodeInitialDataState();

    /**
     * Handles the state in which the server requests detailed device information from remote nodes.
     */
    static void handleRequestNodeDetailedDataState();

    /**
     * Handles the periodic keep-alive exchange used to validate remote node health.
     */
    static void handleKeepAliveState();

    /**
     * Handles the handshake exchange used during remote-node discovery.
     * @param msg Packet carrying the handshake payload.
     */
    static void handleHandShakeCommunication(MessageUDP &msg);

    /**
     * Checks whether a remote node is still alive based on the current message flow.
     * @param msg Packet emitted by the remote node.
     */
    static void handleSlaveAliveMonitoring(MessageUDP &msg);

    /**
     * Updates the node information when a detailed device-description message is received.
     * @param msg Packet containing the device description update.
     */
    static void handleDetailedDataUpdate(MessageUDP &msg);

    /**
     * Maps a specific packet ID to the server state that should process it.
     * @param msgID Message identifier emitted by the remote node.
     * @return Server state associated with the message.
     */
    static ServerState mapMsgIDToServerState(int msgID);

    /**
     * Parses the initial node payload embedded in a UDP packet.
     * @param msg Packet carrying the initial remote-node data.
     * @return Initial node snapshot extracted from the message.
     */
    static NodeInitialData getInitialDataFromPayload(MessageUDP &msg);

    /**
     * Pushes the current remote-device description into the shared signal layer.
     */
    static void updateDeviceDescriptionSignal();

    /**
     * Schedules a refresh of detailed remote-node data for the supplied MAC address.
     * @param mac MAC address of the remote node whose description should be refreshed.
     */
    static void triggerDDRefresh(uint64_t mac);

    /**
     * Removes stale nodes from the remote-device refresh set after the refresh cycle completes.
     * @param nodesToBeRemoved List of remote node MAC addresses that should be pruned.
     */
    static void handleDetailedDataRefreshMech(std::vector<uint64_t> &nodesToBeRemoved);

    /**
     * Generates a new unique request ID for the current outbound RC request.
     * @return New request identifier.
     */
    static uint8_t generateRequestId();

    /**
     * Updates the signal carrying the current remote-node information snapshot.
     */
    static void updateNetworkNodesInformationSignal();

public:
    /**
     * Initializes the remote-control server and its protocol state.
     */
    static void init();

    /**
     * Releases the server resources and resets the runtime state.
     */
    static void deinit();

    /**
     * Runs the RC server maintenance cycle, processing pending requests and remote state changes.
     */
    static void cyclic();

    /**
     * Receives and queues a UDP message for protocol processing.
     * @param msg Incoming UDP packet.
     */
    static void receiveUDP(MessageUDP &msg);

    /**
     * Creates and queues a new RC request for transmission to a remote node.
     * @param newRequest Request object to send.
     * @return Newly assigned request identifier.
     */
    static uint8_t createRcRequest(RcRequest &newRequest);

    /**
     * Registers a callback that will consume outbound responses for a specific request type.
     * @param request Request type to register.
     * @param receiverCallback Callback invoked when a matching response is received.
     * @return true when the callback was accepted successfully; false otherwise.
     */
    static bool registerResponseReceiver(RequestType request, std::function<bool(RcResponse &)> receiverCallback);

    /**
     * Triggers a refresh of the remote node metadata for the given MAC address.
     * @param mac MAC address of the remote node to refresh.
     */
    static void refreshRemoteNodeInfo(uint64_t mac);

    /**
     * Updates the stored information for a known remote node after a device description change.
     * @param deviceDescription New device description object.
     * @param newNodeHash Updated hash for the remote-node snapshot.
     */
    static void updateSlaveInformation(DeviceDescription &deviceDescription, uint16_t newNodeHash);
};

#endif
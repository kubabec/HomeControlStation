#ifndef NETWORK_DRIVER_H
#define NETWORK_DRIVER_H

#include <os/tools/WifiAdapter.hpp>
#include <os/tools/UdpAdapter.hpp>
#include <functional>
#include <queue>

/**
 * @file include/os/drivers/networkdriver.hpp
 * @brief Hardware driver interfaces for the Home Control Station OS.
 */

/**
 * @enum PacketRangeDefinition
 * @brief Groups communication packet ranges used by the OS transport layer.
 *
 * Each range indicates which message family is expected in a given UDP payload domain.
 */
enum PacketRangeDefinition {
    /**
     * Start of the service-call packet range.
     */
    SERVICE_RANGE_BEGIN = 0,
    /**
     * Start of the remote-control server packet range.
     */
    RCS_RANGE_BEGIN = 50,
    /**
     * Range reserved for digital button events.
     */
    DIGITAL_BUTTON_RANGE = 100,
    /**
     * Range reserved for display-control traffic.
     */
    DISPLAY_CONTROLS_RANGE = 150,
    /**
     * Begin of the user-data packet range.
     */
    USR_DATA_RANGE_BEGIN = 200
};

/**
 * Broadcast address used by the local LAN while the node is discovering or notifying peers.
 */
static const MessageUDP::IPAddr NETWORK_BROADCAST{192, 168, 1, 255};
/**
 * @class NetworkDriver
 * @brief Mediates UDP traffic, packet routing, and wireless reconnection for the OS.
 *
 * The driver maintains the per-packet dispatch table, queues outgoing messages, and ensures that
 * the node can reconnect and recover from transient network failures.
 */


class NetworkDriver
{
    /**
     * true when a valid set of Wi-Fi credentials has been loaded in memory.
     */
    static bool networkCredentialsAvailable;

    /**
     * Queue of outbound messages waiting to be sent by the networking layer.
     */
    static std::queue<MessageUDP> pendingToSendPackets;

    /**
     * Routes a received packet to its target subsystem callback according to the packet range.
     * @param packet Message to classify and dispatch.
     */
    static void mapReceivedPacketToInternalReceiver(MessageUDP& packet);

    /**
     * Callback executed when the driver has pending data to flush after a network event.
     */
    static std::function<void(void)> pendingPacketsBehavior;

    /**
     * Packet ID ranges that determine which callback should own an incoming message.
     */
    static std::vector<int> packetRanges;

    /**
     * List of functions that consume packets belonging to the tracked packet ranges.
     */
    static std::vector<std::function<void(MessageUDP&)>> packetReceivers;

public:
    /**
     * Initializes the network stack and registers the transport callbacks.
     */
    static void init();

    /**
     * Stops the networking services and clears driver state.
     */
    static void deinit();

    /**
     * Performs the periodic network maintenance cycle, including queue handling and reconnect logic.
     */
    static void cyclic();

    /**
     * Receives a UDP packet from the transport layer and forwards it for routing.
     * @param data Packet received from the socket.
     */
    static void udpReceive(MessageUDP data);

    /**
     * Sends a UDP message using the configured interface.
     * @param data Message to transmit.
     * @return true when the send operation succeeded; false otherwise.
     */
    static bool send(MessageUDP& data);

    /**
     * Broadcasts a UDP message to the LAN.
     * @param data Message payload to broadcast.
     * @return true when the broadcast was accepted and queued or transmitted successfully.
     */
    static bool sendBroadcast(MessageUDP& data);

    /**
     * Attempts to re-establish connectivity after a temporary connection loss.
     */
    static void networkReconnect();

    /**
     * Performs the legacy IP-detection workaround used to recover configuration after Wi-Fi startup.
     * @param ssid SSID used for the connection attempt.
     * @param pwd Password used for the connection attempt.
     */
    static void runIpDetectionTrick(String ssid, String pwd);
    


};


#endif
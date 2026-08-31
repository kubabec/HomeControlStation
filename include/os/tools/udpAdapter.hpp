#ifndef UDPADAPTER_H
#define UDPADAPTER_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include "MessageUDP.hpp"
/**
 * @class UDPAdapter
 * @brief Handles raw UDP traffic and forwards incoming frames to registered callbacks.
 *
 * The adapter exposes a simple send/receive API above the WiFiUDP socket so the other OS components
 * can exchange messages without managing socket details directly.
 */

class UDPAdapter
{
    /**
     * Local port used by the sender socket.
     */
    static const unsigned int senderPort;

    /**
     * Broadcast address used for LAN-wide delivery.
     */
    static const char * broadcastIP;

    /**
     * WiFiUDP socket instance used by the adapter.
     */
    static WiFiUDP udp;

    /**
     * Timestamp of the last outbound transmission used to throttle flooding.
     */
    static long lastSendTime;

    /**
     * Callback invoked when a UDP message is received by the adapter.
     */
    static std::function<void(MessageUDP&)> onReceiveCallback;

public:
    /**
     * Initializes the UDP socket and configures the network listener.
     */
    static void init();

    /**
     * Performs the periodic UDP maintenance loop and checks for incoming packets.
     */
    static void task();

    /**
     * Sends a MessageUDP packet over the current network interface.
     * @param msg Message to transmit.
     * @return true when the packet was queued or sent successfully.
     */
    static bool send(MessageUDP& msg);

    /**
     * Checks whether the send-rate limiter allows another transmission.
     * @return true when a packet can be sent now; false when the rate limit blocks it.
     */
    static bool sendingAllowed();

    /**
     * Registers a callback that will receive incoming UDP messages.
     * @param callback Function called for each received packet.
     */
    static void registerOnUdpReceiveCallback(std::function<void(MessageUDP&)> callback);
};


#endif
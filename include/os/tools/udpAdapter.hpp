#ifndef UDPADAPTER_H
#define UDPADAPTER_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include "MessageUDP.hpp"

class UDPAdapter
{

    static const unsigned int senderPort;
    static const char * broadcastIP;
    static WiFiUDP udp;
    static long lastSendTime;

    static std::function<void(MessageUDP&)> onReceiveCallback;

public:
    // UDP listener registration
    static void init();
    // Cyclic task
    static void task();

    // Interface to send message
    static bool send(MessageUDP& msg);

    static bool sendingAllowed();

    // Callback registration interface for incomming messages
    static void registerOnUdpReceiveCallback(std::function<void(MessageUDP&)> callback);
};


#endif
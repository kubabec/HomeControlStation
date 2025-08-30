#ifndef NETWORK_DRIVER_H
#define NETWORK_DRIVER_H

#include <os/tools/WifiAdapter.hpp>
#include <os/tools/UdpAdapter.hpp>
#include <functional>
#include <queue>

enum PacketRangeDefinition {
    SERVICE_RANGE_BEGIN = 0,
    RCS_RANGE_BEGIN = 50,
    DIGITAL_BUTTON_RANGE = 100,
    USR_DATA_RANGE_BEGIN = 150
};

static const MessageUDP::IPAddr NETWORK_BROADCAST{192, 168, 1, 255};


class NetworkDriver
{
    static bool networkCredentialsAvailable;
    static std::queue<MessageUDP> pendingToSendPackets;

    static void mapReceivedPacketToInternalReceiver(MessageUDP& packet);

    // Packet ID to callback function mapping

    static std::vector<int> packetRanges;
    static std::vector<std::function<void(MessageUDP&)>> packetReceivers;

public:
    static void init();
    static void deinit();
    static void cyclic();

    static void udpReceive(MessageUDP data);
    static bool send(MessageUDP& data);
    static bool sendBroadcast(MessageUDP& data);

    static void runIpDetectionTrick(String ssid, String pwd);
    


};


#endif
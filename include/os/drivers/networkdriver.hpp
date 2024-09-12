#ifndef NETWORK_DRIVER_H
#define NETWORK_DRIVER_H

#include <os/tools/WifiAdapter.hpp>
#include <os/tools/UdpAdapter.hpp>
#include <functional>

enum PacketRangeDefinition {
    SERVICE_RANGE_BEGIN = 0,
    RCS_RANGE_BEGIN = 50,
    USR_DATA_RANGE_BEGIN = 100
};

static const MessageUDP::IPAddr NETWORK_BROADCAST{192, 168, 1, 255};


class NetworkDriver
{
    static bool networkConnected;
    static bool networkConnectionRequested;


    static void mapReceivedPacketToInternalReceiver(MessageUDP& packet);

    // Packet ID to callback function mapping

    static std::vector<int> packetRanges;
    static std::vector<std::function<void(MessageUDP&)>> packetReceivers;

public:
    static void init();
    static void cyclic();

    static void udpReceive(MessageUDP data);
    static bool send(MessageUDP& data);
    static bool sendBroadcast(MessageUDP& data);
    


};


#endif
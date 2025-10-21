#include <os/tools/UdpAdapter.hpp>
#include "os/Logger.hpp"

const unsigned int UDPAdapter::senderPort = 9001;
const char *UDPAdapter::broadcastIP = "192.168.1.255";
WiFiUDP UDPAdapter::udp;
std::function<void(MessageUDP &)> UDPAdapter::onReceiveCallback;
long UDPAdapter::lastSendTime = 0;

void UDPAdapter::init()
{
    udp.begin(9001);
}

bool UDPAdapter::sendingAllowed()
{
    const uint32_t MIN_SEND_INTERVAL_MS = 5; // 3ms = ~333 logs/sec max per device

    if (millis() - lastSendTime >= MIN_SEND_INTERVAL_MS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void UDPAdapter::task()
{
    int packetSize = udp.parsePacket();

    if (packetSize > 0)
    {
        IPAddress senderIP = udp.remoteIP();
        //    Logger::log("Received new package");

        std::vector<uint8_t> dataBuffer;
        dataBuffer.resize(packetSize);
        int len = udp.read((char *)&dataBuffer.at(0), packetSize);

        /* All data read correctly */
        if (len == packetSize)
        {
            // Logger::log("Constructing MessageUDP...");
            MessageUDP receivedMessage = MessageUDP::fromUint8Vector(dataBuffer);
            //    MessageUDP::serialPrintMessageUDP(receivedMessage);

            // Check if on receive callback is assigned
            if (receivedMessage.isValid() && onReceiveCallback)
            {
                receivedMessage.setIpAddress({senderIP[0], senderIP[1], senderIP[2], senderIP[3]});

                // Pass received message to the callback
                onReceiveCallback(receivedMessage);
            }
            else
            {
                Logger::log("Message invalid or callback not assigned");
            }
        }
    }
}

bool UDPAdapter::send(MessageUDP &msg)
{
    bool transmissionSuccess = false;
    // MessageUDP::serialPrintMessageUDP(msg);

    // Logger::log("Sending via WiFi!");
    Logger::log("UDPAdapter:// Sending UDP packet (MsgId : " + String((int)msg.getId()) + ") to host " + msg.getIPAddress().toString());
    std::vector<uint8_t> &payloadRef = msg.getPayload();
    IPAddress receiverIP(
        msg.getIPAddress().octet1,
        msg.getIPAddress().octet2,
        msg.getIPAddress().octet3,
        msg.getIPAddress().octet4);

    int msgSize = msg.getSize();
    uint8_t *bufferToSend = (uint8_t *)malloc(msgSize);
    if (bufferToSend != nullptr)
    {

        msg.toByteArray(bufferToSend, msgSize);
        lastSendTime = millis();

        if(!udp.beginPacket(receiverIP, (uint16_t)msg.getPort()))
        {
            Logger::log("UDP beginPacket failed");
            free(bufferToSend);
            return false;
        }

        if(udp.write(bufferToSend, msgSize)  != msgSize)
        {
            Logger::log("UDP write failed");
            free(bufferToSend);
            return false;
        }

        if(!udp.endPacket()){
            Logger::log("UDP endPacket failed");
            free(bufferToSend);
            return false;
        }

        transmissionSuccess = true;
        free(bufferToSend);
    }

    return transmissionSuccess;

    // Logger::log("Packet sent.");
}

void UDPAdapter::registerOnUdpReceiveCallback(std::function<void(MessageUDP &)> callback)
{
    onReceiveCallback = callback;
}
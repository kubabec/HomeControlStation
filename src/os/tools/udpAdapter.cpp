#include <os/tools/UdpAdapter.hpp>
#include "os/Logger.hpp"

const unsigned int UDPAdapter::senderPort = 9001;
const char * UDPAdapter::broadcastIP = "192.168.1.255";
WiFiUDP UDPAdapter::udp;
std::function<void(MessageUDP&)> UDPAdapter::onReceiveCallback;


void UDPAdapter::init()
{
    udp.begin(9001);
}

void UDPAdapter::task()
{
    int packetSize = udp.parsePacket();

    if(packetSize > 0)
    {
        IPAddress senderIP = udp.remoteIP();
    //    Logger::log("Received new package");
        
        std::vector<uint8_t> dataBuffer;
        dataBuffer.resize(packetSize);
        int len = udp.read((char*)&dataBuffer.at(0), packetSize);

        /* All data read correctly */
        if(len == packetSize)
        {
            // Logger::log("Constructing MessageUDP...");
            MessageUDP receivedMessage = MessageUDP::fromUint8Vector(dataBuffer);
        //    MessageUDP::serialPrintMessageUDP(receivedMessage);

            // Check if on receive callback is assigned
            if(receivedMessage.isValid() && onReceiveCallback)
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

void UDPAdapter::send(MessageUDP& msg)
{
    // Logger::log();
    // Logger::log("Received new UDP Message!");
    //MessageUDP::serialPrintMessageUDP(msg);

    // Logger::log("Sending via WiFi!");
    std::vector<uint8_t>& payloadRef = msg.getPayload();
        IPAddress receiverIP(
        msg.getIPAddress().octet1, 
        msg.getIPAddress().octet2,
        msg.getIPAddress().octet3,
        msg.getIPAddress().octet4
    );

    int msgSize = msg.getSize();
    uint8_t* bufferToSend = (uint8_t*)malloc(msgSize);
    if(bufferToSend !=0){

        msg.toByteArray(bufferToSend,msgSize);
        udp.beginPacket(receiverIP, (uint16_t)msg.getPort()); //adres IP i port odiornika
        for(int i = 0; i < msgSize; i ++){
            udp.write(bufferToSend[i]);
        }
        udp.endPacket();
        free(bufferToSend);
        //Logger::log();

    }

    

    // Logger::log("Packet sent.");
}

void UDPAdapter::registerOnUdpReceiveCallback(std::function<void(MessageUDP&)> callback)
{
    onReceiveCallback = callback;
}
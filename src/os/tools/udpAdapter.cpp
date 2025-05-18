#include <os/tools/UdpAdapter.hpp>

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
    //    Serial.println("Received new package");
        
        std::vector<uint8_t> dataBuffer;
        dataBuffer.resize(packetSize);
        int len = udp.read((char*)&dataBuffer.at(0), packetSize);

        /* All data read correctly */
        if(len == packetSize)
        {
            // Serial.println("Constructing MessageUDP...");
            MessageUDP receivedMessage = MessageUDP::fromUint8Vector(dataBuffer);
        //    MessageUDP::serialPrintMessageUDP(receivedMessage);

            // Check if on receive callback is assigned
            if(receivedMessage.isValid() && onReceiveCallback)
            {
                // Pass received message to the callback
                onReceiveCallback(receivedMessage);
            }
            else
            {
                Serial.println("Message invalid or callback not assigned");
            }
        }   
    }
}

void UDPAdapter::send(MessageUDP& msg)
{
    // Serial.println();
    // Serial.println("Received new UDP Message!");
    //MessageUDP::serialPrintMessageUDP(msg);

    // Serial.println("Sending via WiFi!");
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
        //Serial.println();

    }

    

    // Serial.println("Packet sent.");
}

void UDPAdapter::registerOnUdpReceiveCallback(std::function<void(MessageUDP&)> callback)
{
    onReceiveCallback = callback;
}
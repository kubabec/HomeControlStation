#include <os/drivers/NetworkDriver.hpp>
#include <os/app/remoteControl/RemoteControlServer.hpp>
#include <os/app/remoteControl/RemoteControlClient.hpp>

bool NetworkDriver::networkConnected = false;
bool NetworkDriver::networkConnectionRequested = false;

std::vector<int> NetworkDriver::packetRanges;
std::vector<std::function<void(MessageUDP&)>> NetworkDriver::packetReceivers;

void dummyTobeRemoved(MessageUDP& data){

}

void NetworkDriver::deinit() {
    
}

void NetworkDriver::init()
{
/*******     WiFi connection section       ******/
    // Try to reach the network
    //WiFiAdapter::connectToNetwork( "GedUPC", "EmilaEryk2005");

    /* Try to access device configuration to extract WiFi credentials */
    try {
        std::any nodeConfiguration = DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION);
        NodeConfiguration config = std::any_cast<NodeConfiguration>(nodeConfiguration);

        // Serial.println("SSID Length : " + String(config.networkSSID.length()));
        // Serial.println("Password Length : " + String(config.networkPassword.length()));
        // Serial.println("Config available: " + String((int)config.networkCredentialsAvailable));

        // Is config valid?
        if(config.networkCredentialsAvailable)
        {
            WiFiAdapter::connectToNetwork(config.networkSSID, config.networkPassword);
        }else 
        {
            // Connect to defaults
            /* Host accesspoint */
            Serial.println("Starting AP due to invalid WiFi credentials");
            WiFiAdapter::createAccessPoint();
            //WiFiAdapter::connectToNetwork( "", "");
        }

    }catch (const std::bad_any_cast& e){ 
        //WiFiAdapter::connectToNetwork( "", "");
        Serial.println("Starting AP due to SIG_DEVICE_CONFIGURATION unreachable");
        WiFiAdapter::createAccessPoint();
    }
    


    // Save 'wasRequested' flag
    networkConnectionRequested = WiFiAdapter::wasConnectionRequested();


/*******     UDP communication section       ******/
    // Register callback that UDPAdapter will be able to call when new packet will arrive
    UDPAdapter::registerOnUdpReceiveCallback(NetworkDriver::udpReceive);

    // Initialize listening on the UDP port
    UDPAdapter::init();

    //packetReceivers.insert({RESPONSE_NODE_INITIAL_DATA, RemoteControlServer::receiveNodeInitialData});
    packetRanges.push_back(SERVICE_RANGE_BEGIN);
    packetReceivers.push_back(dummyTobeRemoved);

    std::any isRCServerRunning = DataContainer::getSignalValue(SIG_IS_RC_SERVER);
    bool isRcServer = std::any_cast<bool>(isRCServerRunning);

    packetRanges.push_back(RCS_RANGE_BEGIN);
    if(isRcServer) {
        packetReceivers.push_back(RemoteControlServer::receiveUDP);
    }else 
    {
        packetReceivers.push_back(RemoteControlClient::receiveUDP);
    }

    packetRanges.push_back(USR_DATA_RANGE_BEGIN);
    packetReceivers.push_back(dummyTobeRemoved);

}

void NetworkDriver::cyclic()
{
/*******     WiFi connection section       ******/
    // Update WiFi Adapter to be updated with connection lost status
    WiFiAdapter::task();

    // get 'isConnected' flag
    networkConnected = WiFiAdapter::isConnected();


/*******     UDP communication section       ******/
    // Update UDP task to be able to receive UDP packets
    UDPAdapter::task();

}


/*This function is called when new UDP packet arrived*/
/*It needs to be registered as onreceive callback for UDPAdapter*/
void NetworkDriver::udpReceive(MessageUDP data)
{
    mapReceivedPacketToInternalReceiver(data);
    //Serial.print("!!!!!! DUPA");
}

/*This is accessor function for UDPAdapter.send()*/
bool NetworkDriver::send(MessageUDP& data)
{
    // if(WiFiAdapter::isConnected()){
    //     UDPAdapter::send(data);
    //     return true;
    // }
    // else
    // {
    //     Serial.println("No network connection. Sending data failed.");
    //     return false;
    // }
}

bool NetworkDriver::sendBroadcast(MessageUDP& data)
{
    MessageUDP::IPAddr ipRef = data.getIPAddress();

    try {
        uint32_t deviceIP = std::any_cast<uint32_t>(
            DataContainer::getSignalValue(SIG_IP_ADDRESS)
        );
        uint8_t* ip = (uint8_t*)&deviceIP;

        ipRef.octet1 = ip[0];
        ipRef.octet2 = ip[1];
        ipRef.octet3 = ip[2];

        // Workaround for Iphone network
        if(ip[0] != 172){
            ipRef.octet4 = 255;
        }
        else 
        {
            ipRef.octet4 = 15;
        }
        
        data.setIpAddress(ipRef);
        // Serial.println("Sending broadcast to " + String((uint8_t)ipRef.octet1) + 
        // "." + String((uint8_t)ipRef.octet2) + 
        // "." + String((uint8_t)ipRef.octet3) + 
        // "." + String((uint8_t)ipRef.octet4));
        return send(data);
        
    } catch  (const std::bad_any_cast& e){ 
        Serial.println("Broadcasting UDP failed, no IP address available!");
        return false;
    }

}

void NetworkDriver::mapReceivedPacketToInternalReceiver(MessageUDP& packet)
{
    int packetId = packet.getId(); //Pobiera id odebranego pakietu

    //Przechodzi przez wektor packetRanges, który zawiera zakresy identyfikatorów pakietów dla różnych odbiorców wewnętrznych.
    for(uint8_t i=1; i<packetRanges.size(); i++){
        uint8_t nextRangeStart = packetRanges.at(i);
        if(packetId < nextRangeStart){
            //pakiet nalezy do zakresu spod indeksu i -1
            packetReceivers.at(i-1)(packet);
            return ;
        }
    } 
    Serial.println("Packet out of range received , Id : " + String(packetId));
    
    
}

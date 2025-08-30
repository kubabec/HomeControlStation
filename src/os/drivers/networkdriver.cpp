#include <os/drivers/NetworkDriver.hpp>
#include <os/app/remoteControl/RemoteControlServer.hpp>
#include <os/app/remoteControl/RemoteControlClient.hpp>
#include <os/app/DigitalButton/DigitalButtonReceiver.hpp>
#include <esp_wifi.h>

bool NetworkDriver::networkCredentialsAvailable = false;

std::vector<int> NetworkDriver::packetRanges;
std::queue<MessageUDP> NetworkDriver::pendingToSendPackets;
std::vector<std::function<void(MessageUDP&)>> NetworkDriver::packetReceivers;

void dummyTobeRemoved(MessageUDP& data){

}

void NetworkDriver::deinit() {
    
}

void NetworkDriver::init()
{
    Serial.println("NetworkDriver init ...");

    /* Try to access device configuration to extract WiFi credentials */
    try {
        std::any nodeConfiguration = DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION);
        NodeConfiguration config = std::any_cast<NodeConfiguration>(nodeConfiguration);
        networkCredentialsAvailable = config.networkCredentialsAvailable;

        // Is config valid?
        if(networkCredentialsAvailable)
        {   
            /* Try connect to known network with active wait flag */
            WiFiAdapter::connectToNetwork(config.networkSSID, config.networkPassword, true);
        }else 
        {
            // Connect to defaults
            /* Host accesspoint */
            Serial.println("Starting AP due to invalid WiFi credentials");
            WiFiAdapter::createAccessPoint();
        }

    }catch (const std::bad_any_cast& e){ 
        //WiFiAdapter::connectToNetwork( "", "");
        Serial.println("Starting AP due to SIG_DEVICE_CONFIGURATION unreachable");
        WiFiAdapter::createAccessPoint();
    }


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

    if(isRcServer){
        packetRanges.push_back(DIGITAL_BUTTON_RANGE);
        packetReceivers.push_back(DigitalButtonReceiver::receiveUDP);
    }

    packetRanges.push_back(USR_DATA_RANGE_BEGIN);
    packetReceivers.push_back(dummyTobeRemoved);


    // esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, mac.bytes);
    uint64_t _chipmacid = 0LL;
    esp_efuse_mac_get_default((uint8_t*) (&_chipmacid));


    Serial.println(_chipmacid);

    if(0LL != _chipmacid){
        DataContainer::setSignalValue(SIG_MAC_ADDRESS, static_cast<uint64_t>(_chipmacid));
    }


    Serial.println("... done");
}

void NetworkDriver::cyclic()
{
/*******     WiFi connection section       ******/
    // Update WiFi Adapter to be updated with connection lost status
    WiFiAdapter::task();


    if(networkCredentialsAvailable){ /* we shall monitor the connection in order to detect the failure*/
        static long long lastRetryTime = 0;
        if(!WiFiAdapter::isConnected()){ /* We are not connected, but shall be as credentials are known */
            if(abs(millis() - lastRetryTime > 10000)){
                NodeConfiguration config = std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
                WiFiAdapter::connectToNetwork(config.networkSSID, config.networkPassword, false);
                lastRetryTime = millis();
            }
        }
    }

/*******     UDP communication section       ******/
    // Update UDP task to be able to receive UDP packets
    UDPAdapter::task();

    if(pendingToSendPackets.size() > 0){
        // Send one pending packet per cyclic
        UDPAdapter::send(pendingToSendPackets.front());
        pendingToSendPackets.pop();
    }

}


/*This function is called when new UDP packet arrived*/
/*It needs to be registered as onreceive callback for UDPAdapter*/
void NetworkDriver::udpReceive(MessageUDP data)
{
    mapReceivedPacketToInternalReceiver(data);
}

/*This is accessor function for UDPAdapter.send()*/
bool NetworkDriver::send(MessageUDP& data)
{
    if(WiFiAdapter::isConnected()){
        // Serial.println("Sending data via UDP");
        pendingToSendPackets.push(std::move(data));
        // UDPAdapter::send(data);
        return true;
    }
    else
    {
        //Serial.println("No network connection. Sending data failed.");
        return false;
    }
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
    int packetId = packet.getId();

    /* Find the range of packetId */
    for(uint8_t i=1; i<packetRanges.size(); i++){
        uint8_t nextRangeStart = packetRanges.at(i);
        if(packetId < nextRangeStart){
            /* Call the receiver function */
            packetReceivers.at(i-1)(packet);
            return ;
        }
    } 
    Serial.println("Packet out of range received , Id : " + String(packetId));
    
    
}

#include "os/app/display/DisplayServer.hpp"

#include "os/drivers/networkdriver.hpp"

std::vector<Display> DisplayServer::knownDisplays;
unsigned long DisplayServer::lastDiscoveryTime = 0;
std::queue<MessageUDP> DisplayServer::receptionUdpQueue;

void DisplayServer::init()
{
    Logger::log("DisplayServer init ...");

    Logger::log("... done");
}

void DisplayServer::deinit()
{
    Logger::log("DisplayServer deinit ...");

    Logger::log("... done");
}

void DisplayServer::cyclic()
{
    discoverDisplays();

    processReceptionBuffer();
}

void DisplayServer::discoverDisplays(){
    if(millis() - lastDiscoveryTime < DISPLAY_DISCOVERY_FREQ_MS){
        return;
    }

    MessageUDP discoveryMsg((uint8_t)DisplayMessageIds::DISCOVER_DISPLAYS, NETWORK_BROADCAST, 9001);
    NetworkDriver::sendBroadcast(discoveryMsg);

    lastDiscoveryTime = millis();
}


void DisplayServer::processReceptionBuffer(){
    // Only process single message per cyclic to avoid long processing time in case of many messages received in short time
    if(receptionUdpQueue.size() > 0){
        MessageUDP msg = receptionUdpQueue.front();
        receptionUdpQueue.pop();
        if(msg.getId() == (uint8_t)DisplayMessageIds::DISPLAY_PRESENT){
            handleNewDisplay(msg);
        } 
        // TODO - handle other types of messages from display, e.g. data updates
    }
}

void DisplayServer::receiveUDP(MessageUDP &msg)
{
    // add received message to reception queue for further processing in cyclic
    receptionUdpQueue.push(msg);
}

void DisplayServer::handleNewDisplay(MessageUDP &msg){
    // Check if display is already know by unpacking data from message as DisplayInfo and add to knownDisplays if not present
    DisplayInfo newDisplayInfo;
    std::vector<uint8_t> &payload = msg.getPayload();
    if(payload.size() == sizeof(DisplayInfo)){
        memcpy(&newDisplayInfo, payload.data(), sizeof(DisplayInfo));
        Display newDisplay(newDisplayInfo);
        auto it = std::find(knownDisplays.begin(), knownDisplays.end(), newDisplay);
        if(it == knownDisplays.end()){
            Logger::log("DisplayServer:// New display discovered with MAC: " + String((unsigned long long)newDisplayInfo.macAddress));
            knownDisplays.push_back(newDisplay);
        }
    }else {
        Logger::log("DisplayServer:// Received DISPLAY_PRESENT message with invalid payload size");
    }
}
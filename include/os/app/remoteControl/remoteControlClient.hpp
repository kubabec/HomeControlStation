#ifndef REMOTE_CONTROL_CLIENT_H
#define REMOTE_CONTROL_CLIENT_H
#include <Arduino.h>
#include <os/drivers/NetworkDriver.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <map>
#include <os/datacontainer/SigMessages.hpp>
#include <os/app/config/ConfigProvider.hpp>
#include <os/app/remoteControl/rc_DataTypes.hpp>




class RemoteControlClient 
{
    static std::queue<MessageUDP> receivedBuffer;
    // deklaracja tablicy o nazwie requestReceivers
    // kazdy element tablicy zawiera element typu funkcja typu bool z parametrem RcRequest
    // i drugi element okreslajacy wielkosc tablicy
    static std::array<std::function<bool(SystemRequest&)>, REQ_COUNT> requestReceivers;
    static uint8_t localNodeId;
    static std::vector<SystemResponse> vecResponseMessage; //vector containing the responses to the requests
    
    static void handleNodeInitialDataState();
    static void handleNodeDetailedDataState();
    static void handleKeepAliveState();
    static void processUDPRequest(MessageUDP& msg);
    static void sendInitialDataResponse();
    static void sendDetailedDataResponse();
    static void sendKeepAlive();
    static void processGenericRequest(MessageUDP& msg);
    static bool processResponse();
    
public:
    static void init();
    static void deinit();
    static void cyclic();
    static void receiveUDP(MessageUDP& msg);
    static bool registerRequestReceiver(SystemRequestType request, std::function<bool(SystemRequest&)> receiverCallback);

    static bool sendResponse(SystemResponse& response);
    
};


#endif
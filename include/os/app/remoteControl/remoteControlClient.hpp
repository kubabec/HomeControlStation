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
    static std::array<std::function<bool(RcRequest&)>, REQ_COUNT> requestReceivers;
    static uint64_t localNodeMACAddress;
    static std::queue<RcResponse> vecResponseMessage; //vector containing the responses to the requests
    static std::queue<MessageUDP> pendingTxQueue;
    
    static void handleNodeInitialDataState();
    static void handleNodeDetailedDataState();
    static void handleKeepAliveState();
    static void processUDPRequest(MessageUDP& msg);
    static void sendInitialDataResponse();
    static void sendDetailedDataResponse(UdpFrames_RCS udpHeaderValue);
    
    static void sendKeepAlive();
    static void processGenericRequest(MessageUDP& msg);
    static bool processResponse();
    static void processPendingTxData();
    
public:
    static void init();
    static void deinit();
    static void cyclic();
    static void receiveUDP(MessageUDP& msg);
    static bool registerRequestReceiver(RequestType request, std::function<bool(RcRequest&)> receiverCallback);

    static bool sendResponse(RcResponse& response);
    
};


#endif
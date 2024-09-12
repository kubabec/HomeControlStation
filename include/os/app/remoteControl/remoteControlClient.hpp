#ifndef REMOTE_CONTROL_CLIENT_H
#define REMOTE_CONTROL_CLIENT_H
#include <Arduino.h>
#include <os/drivers/NetworkDriver.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <map>
#include <os/datacontainer/SigMessages.hpp>
#include <os/app/config/ConfigProvider.hpp>



class RemoteControlClient 
{
    static std::queue<MessageUDP> receivedBuffer;

    
    static void handleNodeInitialDataState();
    static void handleNodeDetailedDataState();
    static void handleKeepAliveState();
    static void processUDPRequest(MessageUDP& msg);
    static void sendInitialDataResponse();
    static void sendDetailedDataResponse();
    static void sendKeepAlive();
    
public:
    static void init();
    static void cyclic();
    static void receiveUDP(MessageUDP& msg);
    
};


#endif
#ifndef REMOTE_CONTROL_CLIENT_H
#define REMOTE_CONTROL_CLIENT_H
#include <Arduino.h>
#include <os/drivers/networkdriver.hpp>
#include <os/datacontainer/datacontainer.hpp>
#include <map>
#include <os/datacontainer/sigmessages.hpp>
#include <os/app/config/configprovider.hpp>



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
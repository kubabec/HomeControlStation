#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H

#include <os/app/remoteControl/rc_dataTypes.hpp>
#include <os/app/remoteControl/RC_publicDataTypes.hpp>
#include <os/tools/messageUDP.hpp>


class RequestProcessor
{
    RcRequest currentRequest ;
    unsigned long lastSendTime = 0;
    
    bool isProcessingStarted = false;

public:
   RequestProcessor();
   bool processReqest(RcRequest& newReqest, MessageUDP::IPAddr targetNodeIp);
   //void setCurrentRequest(RcRequest& newReqest);

};


#endif
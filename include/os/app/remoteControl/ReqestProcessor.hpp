#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H

#include <os/app/remoteControl/rc_dataTypes.hpp>
#include <os/app/remoteControl/RC_publicDataTypes.hpp>


class RequestProcessor
{
    RcRequest currentRequest ;
    unsigned long lastSendTime = 0;
    
    bool isProcessingStarted = false;

public:
   RequestProcessor();
   bool processReqest(RcRequest& newReqest);
   //void setCurrentRequest(RcRequest& newReqest);

};


#endif
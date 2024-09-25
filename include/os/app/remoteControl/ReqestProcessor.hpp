#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H

#include <os/app/remoteControl/rc_dataTypes.hpp>


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
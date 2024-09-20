#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H

#include <os/app/remoteControl/rc_dataTypes.hpp>


class RequestProcessor
{
    RcRequest currentRequest ;
    bool isProcessingStarted = false;

public:
   RequestProcessor();
   bool processReqest();
   void setCurrentRequest(RcRequest& newReqest);

};


#endif
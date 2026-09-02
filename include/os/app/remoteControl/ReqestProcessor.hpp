#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H

#include <os/app/remoteControl/rc_dataTypes.hpp>
#include <os/app/remoteControl/RC_publicDataTypes.hpp>
#include <os/tools/messageUDP.hpp>
/**
 * @class RequestProcessor
 * @brief Translates RC requests into outgoing UDP messages and tracks their delivery lifecycle.
 *
 * The processor keeps the currently active request object, manages retry timing, and sends the message
 * to the target node once the request is ready to be transmitted.
 */

class RequestProcessor
{
    /**
     * Request currently being processed by the RC protocol layer.
     */
    RcRequest currentRequest;

    /**
     * Timestamp of the most recent send attempt for the current request.
     */
    unsigned long lastSendTime = 0;

    /**
     * true once the processing state for the current request has started.
     */
    bool isProcessingStarted = false;

public:
   /**
    * Default constructor.
    */
   RequestProcessor();

   /**
    * Processes a new request by preparing it for transmission to the target node and UDP address.
    * @param newReqest Request object to dispatch.
    * @param targetNodeIp IP address of the destination node.
    * @return true when the request was accepted and sent or queued successfully.
    */
   bool processReqest(RcRequest& newReqest, MessageUDP::IPAddr targetNodeIp);
   //void setCurrentRequest(RcRequest& newReqest);

};


#endif
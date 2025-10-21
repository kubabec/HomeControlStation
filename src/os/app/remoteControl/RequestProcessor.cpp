#include <os/app/remoteControl/ReqestProcessor.hpp>
#include <Arduino.h>
#include <os/drivers/networkdriver.hpp>
#include "os/Logger.hpp"

RequestProcessor::RequestProcessor() {


}


bool RequestProcessor::processReqest(RcRequest& newReqest, MessageUDP::IPAddr targetNodeIp) {
    //new request asrrived
    //currentRequest.print();
    // Logger::log("Processing request with ID : " + String((int)newReqest.getRequestId()));
    
    /* Is it new request or already known? */
    if(currentRequest.getRequestId() != newReqest.getRequestId()) {

        // Logger::log("New request arrived, ID : " + String((int)newReqest.getRequestId()));

        /* Set newRequest as currently processed one for further function entries */
        //memcpy(&currentRequest, &newReqest, REQEST_SIZE);
        currentRequest = newReqest;
        
        /* Prepare UDP message with RC_REQUEST identifier */
        MessageUDP message(RC_REQUEST, targetNodeIp, 9001);

        // Logger::log("Constructing empty meesage completed");

        uint16_t reqestSize = currentRequest.getSize();
        // deklarujemy zmienną dataBuffer która jest pointerem typu uint8, malloc alokuje pamiec na stercie o rozmiarze requestSize i kastujemy na uint8 tzn informujemy kompilator ze bedziemy pod tym adresem przechowywać wartisci uint8
        
        currentRequest.setRequestSendCount(1);
        /* Calculate checksum byte based on the payload */
        currentRequest.calculateCrc();

        // Logger::log("calculateCrc() : ");

        uint8_t* dataBuffer = (uint8_t*) malloc(reqestSize);
        currentRequest.toByteArray(dataBuffer, reqestSize);

        // newReqest.print();
        /* Push fully prepared request to previously created empty UDP message  */

        if(message.pushData((byte*)(dataBuffer),reqestSize)){
            // Logger::log("Sending new request with ID "+String((int)currentRequest.getRequestId()));
            // currentRequest.print();
            /* Send UDP data */
            NetworkDriver::send(message);

            // Logger::log("Message sent");
            /* Increment request send counter for further entries of this function */
            lastSendTime = millis();
        }
        else{
            Logger::log("Message UDP construction failed, Request Id : " + String((int) currentRequest.getRequestId()));
        }

        free(dataBuffer);
        
    } else{/* Processing allready known request */
        
        /* Does message send retry count exceed maximum allowed number? */
        if(currentRequest.getRequestSendCount() >= 8 ){
            /* Max number reqests exceed , timeout request*/
            Logger::log("Request "+String((int)currentRequest.getRequestId())+" processing failed");
            /* clear the current request */
            currentRequest.clear();
            return false;
        } else {
            /* Send repeat request, when time to resend is over */
            if(millis() - lastSendTime > 1000){
                Logger::log("Sending repeat request for request "+String((int)currentRequest.getRequestId()));
                /* Send repeat request */
                MessageUDP message(RC_REQUEST,targetNodeIp, 9001);

                uint16_t reqestSize = currentRequest.getSize();
                currentRequest.setRequestSendCount(currentRequest.getRequestSendCount() + 1);

                currentRequest.calculateCrc();

                uint8_t* dataBuffer = (uint8_t*) malloc(reqestSize);
                currentRequest.toByteArray(dataBuffer, reqestSize);
                
                if(message.pushData((byte*)(dataBuffer),reqestSize)){
                    NetworkDriver::send(message);
                    lastSendTime = millis();
                }
                else{
                    Logger::log("Message UDP construction failed, Request Id : " + String((int) currentRequest.getRequestId()));
                }
                
                free(dataBuffer);
                
            }
        }
    }

    return true;
    
}

// void RequestProcessor::setCurrentRequest(RcRequest& newReqest) {
//     memcpy(&currentRequest, &newReqest, REQEST_SIZE);
//     currentRequest.print();

// }
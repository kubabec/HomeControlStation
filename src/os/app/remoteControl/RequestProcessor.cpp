#include <os/app/remoteControl/ReqestProcessor.hpp>
#include <Arduino.h>
#include <os/drivers/networkdriver.hpp>

RequestProcessor::RequestProcessor() {


}


bool RequestProcessor::processReqest(RcRequest& newReqest) {
    //new request asrrived
    //currentRequest.print();
    
    /* Is it new request or already known? */
    if(currentRequest.getRequestId() != newReqest.getRequestId()) {
        /* Set newRequest as currently processed one for further function entries */
        //memcpy(&currentRequest, &newReqest, REQEST_SIZE);
        currentRequest = newReqest;
        
        /* Prepare UDP message with RC_REQUEST identifier */
        MessageUDP message(RC_REQUEST,NETWORK_BROADCAST, 9001);

        uint8_t reqestSize = currentRequest.getSize();
        // deklarujemy zmienną dataBuffer która jest pointerem typu uint8, malloc alokuje pamiec na stercie o rozmiarze requestSize i kastujemy na uint8 tzn informujemy kompilator ze bedziemy pod tym adresem przechowywać wartisci uint8
        

        /* Calculate checksum byte based on the payload */
        currentRequest.calculateCrc();
        uint8_t* dataBuffer = (uint8_t*) malloc(reqestSize);
        currentRequest.toByteArray(dataBuffer, reqestSize);
        /* Push fully prepared request to previously created empty UDP message  */

        if(message.pushData((byte*)(dataBuffer),reqestSize)){
            Serial.println("Sending new request with ID "+String((int)currentRequest.getRequestId()));
            /* Send UDP data */
            NetworkDriver::sendBroadcast(message);
            /* Increment request send counter for further entries of this function */
            currentRequest.setRequestSendCount(1);
            lastSendTime = millis();
        }
        else{
            Serial.println("Message UDP construction failed, Request Id : " + String((int) currentRequest.getRequestId()));
        }

        free(dataBuffer);
        
    } else{/* Processing allready known request */
        
        /* Does message send retry count exceed maximum allowed number? */
        if(currentRequest.getRequestSendCount() >= 3 ){
            /* Max number reqests exceed , timeout request*/
            Serial.println("Request "+String((int)currentRequest.getRequestId())+" processing failed");
            /* clear the current request */
            currentRequest.clear();
            return false;
        } else {
            /* Send repeat request, when time to resend is over */
            if(millis() - lastSendTime > 5000){
                Serial.println("Sending repeat request for request "+String((int)currentRequest.getRequestId()));
                /* Send repeat request */
                MessageUDP message(RC_REQUEST,NETWORK_BROADCAST, 9001);

                uint8_t reqestSize = currentRequest.getSize();

                uint8_t* dataBuffer = (uint8_t*) malloc(reqestSize);
                currentRequest.toByteArray(dataBuffer, reqestSize);
                
                if(message.pushData((byte*)(dataBuffer),reqestSize)){
                    NetworkDriver::sendBroadcast(message);
                    currentRequest.setRequestSendCount(currentRequest.getRequestSendCount() + 1);
                    lastSendTime = millis();
                }
                else{
                    Serial.println("Message UDP construction failed, Request Id : " + String((int) currentRequest.getRequestId()));
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
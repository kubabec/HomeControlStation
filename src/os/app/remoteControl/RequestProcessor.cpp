#include <os/app/remoteControl/ReqestProcessor.hpp>
#include <Arduino.h>
#include <os/drivers/networkdriver.hpp>

RequestProcessor::RequestProcessor() {


}


bool RequestProcessor::processReqest(RcRequest& newReqest) {
    //new request asrrived
    //currentRequest.print();
    
    /* Is it new request or already known? */
    if(currentRequest.requestId != newReqest.requestId) {
        /* Set newRequest as currently processed one for further function entries */
        memcpy(&currentRequest, &newReqest, REQEST_SIZE);

        
        /* Prepare UDP message with RC_REQUEST identifier */
        MessageUDP message(RC_REQUEST,NETWORK_BROADCAST, 9001);

        uint8_t reqestSize = currentRequest.getSize();
        // deklarujemy zmienną dataBuffer która jest pointerem typu uint8, malloc alokuje pamiec na stercie o rozmiarze requestSize i kastujemy na uint8 tzn informujemy kompilator ze bedziemy pod tym adresem przechowywać wartisci uint8
        //uint8_t* dataBuffer = (uint8_t*) malloc(reqestSize);

        /* Calculate checksum byte based on the payload */
        currentRequest.calculateCrc();

        /* Push fully prepared request to previously created empty UDP message  */
        if(message.pushData((byte*)(&currentRequest),reqestSize)){
            /* Send UDP data */
            NetworkDriver::sendBroadcast(message);
            /* Increment request send counter for further entries of this function */
            currentRequest.requestSendCount ++;
            lastSendTime = millis();
        }
        else{
            Serial.println("Message UDP construction failed, Request Id : " + String((int) currentRequest.requestId));
        }
        
    } else{/* Processing allready known request */
        
        /* Does message send retry count exceed maximum allowed number? */
        if(currentRequest.requestSendCount >= 3){
            /* Max number reqests exceed , timeout request*/
            /* clear the current request */
            currentRequest = {};
            return false;
        } else {
            /* Send repeat request, when time to resend is over */
            if(millis() - lastSendTime > 3000){
                /* Send repeat request */
                MessageUDP message(RC_REQUEST,NETWORK_BROADCAST, 9001);

                uint8_t reqestSize = currentRequest.getSize();
                
                if(message.pushData((byte*)(&currentRequest),reqestSize)){
                    NetworkDriver::sendBroadcast(message);
                    currentRequest.requestSendCount ++;
                    lastSendTime = millis();
                }
                else{
                    Serial.println("Message UDP construction failed, Request Id : " + String((int) currentRequest.requestId));
                }
                

                
            }
        }
    }

    return true;
    
}

// void RequestProcessor::setCurrentRequest(RcRequest& newReqest) {
//     memcpy(&currentRequest, &newReqest, REQEST_SIZE);
//     currentRequest.print();

// }
#include <os/app/remoteControl/ReqestProcessor.hpp>
#include <Arduino.h>
#include <os/drivers/networkdriver.hpp>

RequestProcessor::RequestProcessor() {


}


bool RequestProcessor::processReqest(RcRequest& newReqest) {
    //new request asrrived
    //currentRequest.print();
    

    if(currentRequest.requestId != newReqest.requestId) {
        memcpy(&currentRequest, &newReqest, REQEST_SIZE);
        

        MessageUDP message(RC_REQUEST,NETWORK_BROADCAST, 9001);

        uint8_t reqestSize = currentRequest.getSize();
        // deklarujemy zmienną dataBuffer która jest pointerem typu uint8, malloc alokuje pamiec na stercie o rozmiarze requestSize i kastujemy na uint8 tzn informujemy kompilator ze bedziemy pod tym adresem przechowywać wartisci uint8
        //uint8_t* dataBuffer = (uint8_t*) malloc(reqestSize);
        currentRequest.calculateCrc();

        if(message.pushData((byte*)(&currentRequest),reqestSize)){
            NetworkDriver::sendBroadcast(message);
            currentRequest.requestSendCount ++;
            lastSendTime = millis();
        }
        else{
            Serial.println("Message UDP construction failed, Request Id : " + String((int) currentRequest.requestId));
        }
        
    } else{
        /* Processing allready known request */
        if(currentRequest.requestSendCount >= 3){
            /* Max number reqests exceed , timeout request*/
            currentRequest = {};
            return false;
        } else {
            /* Send repeat request, when time to resend is over */
            if(millis() - lastSendTime > 1000){
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
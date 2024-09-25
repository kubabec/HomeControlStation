#include <os/app/remoteControl/ReqestProcessor.hpp>
#include <Arduino.h>
#include <os/drivers/networkdriver.hpp>

RequestProcessor::RequestProcessor() {


}


bool RequestProcessor::processReqest(RcRequest& newReqest) {
    //new request asrrived
    if(currentRequest.requestId != newReqest.requestId) {
        memcpy(&currentRequest, &newReqest, REQEST_SIZE);
        currentRequest.print();

        MessageUDP message(RC_REQUEST,NETWORK_BROADCAST, 9001);

        uint8_t reqestSize = currentRequest.getSize();
        uint8_t* dataBuffer = (uint8_t*) malloc(reqestSize);

        if(currentRequest.toByteArray(dataBuffer, reqestSize)){
            if(message.pushData(dataBuffer,reqestSize)){
                NetworkDriver::sendBroadcast(message);
                currentRequest.requestSendCount ++;
                lastSendTime = millis();
            }
            else{
                Serial.println("Message UDP construction failed, Request Id : " + String((int) currentRequest.requestId));
            }
        }
        else{
            Serial.println("Unable to serialize Request Id : " + String((int) currentRequest.requestId));
        }

        free(dataBuffer);
    } else{
        /* Processing allready known request */
        if(currentRequest.requestSendCount >= 3){
            /* Max number reqests exceed , timeout request*/
            return false;
        } else {
            /* Send repeat request, when time to resend is over */
            if(millis() - lastSendTime > 300){
                /* Send repeat request */
                MessageUDP message(RC_REQUEST,NETWORK_BROADCAST, 9001);

                uint8_t reqestSize = currentRequest.getSize();
                uint8_t* dataBuffer = (uint8_t*) malloc(reqestSize);

                if(currentRequest.toByteArray(dataBuffer, reqestSize)){
                    if(message.pushData(dataBuffer,reqestSize)){
                        NetworkDriver::sendBroadcast(message);
                        currentRequest.requestSendCount ++;
                        lastSendTime = millis();
                    }
                    else{
                        Serial.println("Message UDP construction failed, Request Id : " + String((int) currentRequest.requestId));
                    }
                }
                else{
                    Serial.println("Unable to serialize Request Id : " + String((int) currentRequest.requestId));
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
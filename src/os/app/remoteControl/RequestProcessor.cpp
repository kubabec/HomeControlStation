#include <os/app/remoteControl/ReqestProcessor.hpp>
#include <Arduino.h>

RequestProcessor::RequestProcessor() {


}


bool RequestProcessor::processReqest() {

}

void RequestProcessor::setCurrentRequest(RcRequest& newReqest) {
    memcpy(&currentRequest, &newReqest, REQEST_SIZE);
    currentRequest.print();

}
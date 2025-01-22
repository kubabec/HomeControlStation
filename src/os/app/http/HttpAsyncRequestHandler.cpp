#include <os/app/http/HttpAsyncRequestHandler.hpp>



HTTPAsyncRequestHandler::AsyncHttpRequest HTTPAsyncRequestHandler::currentRequest;
String HTTPAsyncRequestHandler::jsonResponse = "";
std::map<uint8_t, String>* HTTPAsyncRequestHandler::roomMapping_ptr = nullptr;
std::map<uint8_t, std::vector<DeviceDescription*>>* HTTPAsyncRequestHandler::deviceToRoomMappingList_ptr = nullptr;

void HTTPAsyncRequestHandler::init(
    std::map<uint8_t, String>*  roomMapping, 
    std::map<uint8_t, std::vector<DeviceDescription*>>* deviceToRoomMappingList)
{
    roomMapping_ptr = roomMapping;
    deviceToRoomMappingList_ptr = deviceToRoomMappingList;

    currentRequestClear();

    Serial.println("HTTPAsyncRequestHandler initialized.");
}

HttpErrorCode HTTPAsyncRequestHandler::createRequest(
    AsyncRequestType requestType,
    uint8_t* requestParamsPtr,
    uint8_t paramsLength)
{
    /* no active request */
    if(currentRequest.state == ASYNC_NO_REQUEST){
        /* save request creation time */
        currentRequest.receivedTime = millis();
        /* save request metadata */
        currentRequest.state = ASYNC_REQUEST_RECEIVED;
        currentRequest.type = requestType;

        /* copy the parameters */
        if(paramsLength < MAX_PARAM_LENGTH_FOR_ASYNC_REQUEST && requestParamsPtr != nullptr){
            memcpy(currentRequest.requestData, requestParamsPtr, paramsLength);
        }

        // Serial.println("HTTPAsyncRequestHandler: New request created");
        // currentRequest.print();

        return e_HTTP_OK;
    }else {
        return e_HTTP_BUSY;
    }
}

void HTTPAsyncRequestHandler::currentRequestClear()
{
    currentRequest.receivedTime = 0;
    currentRequest.state = ASYNC_NO_REQUEST;
    currentRequest.type = ASYNC_TYPE_INVALID;

    memset(currentRequest.requestData, 0xFF, MAX_PARAM_LENGTH_FOR_ASYNC_REQUEST);
}


void HTTPAsyncRequestHandler::createServiceCall()
{
    ServiceParameters_set1 set1;
    ServiceParameters_set2 set2;
    ServiceParameters_set3 set3;

    switch(currentRequest.requestData[SERVICE_OVERLOADING_FUNCTION_INDEX])
    {
        /* No parameters service */
        case serviceCall_NoParams:
            (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))).serviceCall_NoParams(
                currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL],
                (DeviceServicesType)currentRequest.requestData[SERVICE_NAME_INDEX] 
            );
        break;

        /* Service with 1 parameter set */
        case serviceCall_1:
            memcpy(&set1, &currentRequest.requestData[SERVICE_NAME_INDEX+1], sizeof(ServiceParameters_set1));
            (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))).serviceCall_set1(
                currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL],
                (DeviceServicesType)currentRequest.requestData[SERVICE_NAME_INDEX],
                set1
            );
        break;

        case serviceCall_2:
            memcpy(&set2, &currentRequest.requestData[SERVICE_NAME_INDEX+1], sizeof(ServiceParameters_set2));
            (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))).serviceCall_set2(
                currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL],
                (DeviceServicesType)currentRequest.requestData[SERVICE_NAME_INDEX],
                set2
            );
        break;

        case serviceCall_3:
            memcpy(&set3, &currentRequest.requestData[SERVICE_NAME_INDEX+1], sizeof(ServiceParameters_set3));
            (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))).serviceCall_set3(
                currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL],
                (DeviceServicesType)currentRequest.requestData[SERVICE_NAME_INDEX],
                set3
            );

        break;

        default:
        break;
    }

}

AsyncRequestState HTTPAsyncRequestHandler::getProcessingState()
{
    return currentRequest.state;
}

void HTTPAsyncRequestHandler::processRequest()
{
    switch(currentRequest.type)
    {
        case ASYNC_TYPE_DEVICE_SERVICE_CALL:
            DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, static_cast<bool>(true));
            createServiceCall();
            currentRequest.state = ASYNC_REQUEST_PROCESSING;
        break;

        case ASYNC_GET_PAGE_CONTENT:
            currentRequest.state = ASYNC_REQUEST_COMPLETED;

        default : break;
    }
}

void HTTPAsyncRequestHandler::createMainPageContentJson()
{ 
//   Serial.println("HTTPAsyncRequestHandler: Starting JSON response preparation ...");
  jsonResponse += "{";
  uint8_t roomIteratorCount = 1;
  for(auto& room : *deviceToRoomMappingList_ptr)
  {
    uint8_t deviceIteratorCount = 1;

    if((*roomMapping_ptr).find(room.first) == (*roomMapping_ptr).end()){
      jsonResponse +="\""+String((int)room.first)+"\": [";
    }else
    {
      jsonResponse +="\""+(*roomMapping_ptr).find(room.first)->second+"\": [";
    }
    for(auto& deviceInThisRoom : room.second){
      jsonResponse +="{";
      jsonResponse +="\"id\":" + String((int)deviceInThisRoom->deviceId) + ",";
      jsonResponse +="\"name\":\"" + deviceInThisRoom->deviceName + "\",";
      if(deviceInThisRoom->isEnabled){
        jsonResponse +="\"status\":\"on\",";
      }else {
        jsonResponse +="\"status\":\"off\",";
      }
      jsonResponse +="\"hasBrightness\":" + String((int)deviceInThisRoom->customBytes[0]) + ",";
      jsonResponse +="\"brightness\":" + String((int)deviceInThisRoom->customBytes[1]);
      if(deviceIteratorCount < room.second.size()){
        jsonResponse +="},";
      }else {
        jsonResponse +="}";
      }
      deviceIteratorCount++;
    }
    
    if(roomIteratorCount < (*deviceToRoomMappingList_ptr).size()){
      jsonResponse +="],";
    }else 
    {
      jsonResponse +="]";
    }
    roomIteratorCount++;
  }
  jsonResponse +="}";
}

void HTTPAsyncRequestHandler::createJsonResponse()
{
    jsonResponse = "";
    switch(currentRequest.type){
        case ASYNC_TYPE_DEVICE_SERVICE_CALL:
            createMainPageContentJson();
            break;

        case ASYNC_GET_PAGE_CONTENT:
            createMainPageContentJson();
            break;

        default : break;
    }
}

void HTTPAsyncRequestHandler::mainFunction()
{
    if(currentRequest.state == ASYNC_REQUEST_RECEIVED){
        /* do the request processing here */
        processRequest();
    }

    if(currentRequest.state == ASYNC_REQUEST_PROCESSING){
        /* monitor UI blocked signal to detect that processing is over */
        bool isUiBlocked = std::any_cast<bool>(DataContainer::getSignalValue(SIG_IS_UI_BLOCKED));
        if(!isUiBlocked){
            currentRequest.state = ASYNC_REQUEST_COMPLETED;
        }
    }
}


String HTTPAsyncRequestHandler::getJsonResponse()
{
    /* request processing is completed */
    createJsonResponse();
    /* Clear request data, as final state -> response is requested */
    currentRequestClear();
    /* Return previously prepared JSON */
    // Serial.println("HTTPAsyncRequestHandler: Request processing finished!");
    return jsonResponse;
}
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
        currentRequest.state = ASYNC_REQUEST_PROCESSING;
        currentRequest.type = requestType;

        /* copy the parameters */
        if(paramsLength < MAX_PARAM_LENGTH_FOR_ASYNC_REQUEST && requestParamsPtr != nullptr){
            memcpy(currentRequest.requestData, requestParamsPtr, paramsLength);
        }

        // Serial.println("HTTPAsyncRequestHandler: Starting new request processing ...");
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

    ServiceRequestErrorCode serviceCallStatus = SERV_GENERAL_FAILURE;

    switch(currentRequest.requestData[SERVICE_OVERLOADING_FUNCTION_INDEX])
    {
        /* No parameters service */
        case serviceCall_NoParams:
            serviceCallStatus = (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))).serviceCall_NoParams(
                currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL],
                (DeviceServicesType)currentRequest.requestData[SERVICE_NAME_INDEX] 
            );
        break;

        /* Service with 1 parameter set */
        case serviceCall_1:
            memcpy(&set1, &currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL+1], sizeof(ServiceParameters_set1));
            serviceCallStatus = (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))).serviceCall_set1(
                currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL],
                (DeviceServicesType)currentRequest.requestData[SERVICE_NAME_INDEX],
                set1
            );
        break;

        case serviceCall_2:
            memcpy(&set2, &currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL+1], sizeof(ServiceParameters_set2));
            serviceCallStatus = (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))).serviceCall_set2(
                currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL],
                (DeviceServicesType)currentRequest.requestData[SERVICE_NAME_INDEX],
                set2
            );
        break;

        case serviceCall_3:
            memcpy(&set3, &currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL+1], sizeof(ServiceParameters_set3));
            serviceCallStatus = (std::any_cast <DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))).serviceCall_set3(
                currentRequest.requestData[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL],
                (DeviceServicesType)currentRequest.requestData[SERVICE_NAME_INDEX],
                set3
            );

        break;

        default:
        break;
    }

    /* we must check the status of service call */
    switch(serviceCallStatus){
        case SERV_SUCCESS:
            /* Request completed successfully */
            Serial.println("Service request processing finished.");
            currentRequest.state = ASYNC_REQUEST_COMPLETED;
            break;
        case SERV_PENDING:
            /* We are waiting for the response, service must be polled */
            currentRequest.state = ASYNC_REQUEST_PROCESSING;
            break;
        case SERV_BUSY:
            /* Trying to send request, but actually it is not possible */
            Serial.println("HTTPAsyncRequestHandler: Request call returned BUSY");
            /* Dropping the request */
            currentRequest.state = ASYNC_REQUEST_COMPLETED;
            /* This will just do not return any valid JSON response */
            currentRequest.type = ASYNC_TYPE_INVALID;
            break;

        case SERV_NOT_SUPPORTED:
            /* Device does not support this service call */
            Serial.println("HTTPAsyncRequestHandler: Request call returned NOT_SUPPORTED");
            currentRequest.state = ASYNC_REQUEST_COMPLETED;
            currentRequest.type = ASYNC_TYPE_INVALID;
            break;

        default:
            currentRequest.state = ASYNC_REQUEST_COMPLETED;
            currentRequest.type = ASYNC_TYPE_INVALID;
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
            createServiceCall();
        break;

        /* Page content request is immediatelly finished */
        case ASYNC_GET_PAGE_CONTENT:
            currentRequest.state = ASYNC_REQUEST_COMPLETED;

        case ASYNC_GET_NOTIFICATION_LIST:
            currentRequest.state = ASYNC_REQUEST_COMPLETED;

        case ASYNC_DOWNLOAD_CONFIGURATION:
            currentRequest.state = ASYNC_REQUEST_COMPLETED;
        case ASYNC_REDIRECT_TO_MAIN_PAGE:
            currentRequest.state = ASYNC_REQUEST_COMPLETED;

        default : break;
    }
}

String getHexColor(uint8_t r, uint8_t g, uint8_t b){
    String color = "";
    r < 16 ? color += "0" + String((int)r, HEX) : color += String((int)r, HEX) ;
    g < 16 ? color += "0" + String((int)g, HEX) : color += String((int)g, HEX) ;
    b < 16 ? color += "0" + String((int)b, HEX) : color += String((int)b, HEX) ;

    return color;
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
      jsonResponse +="\"devType\":" + String((int)deviceInThisRoom->deviceType) + ",";
      jsonResponse +="\"name\":\"" + deviceInThisRoom->deviceName + "\",";
      if(deviceInThisRoom->isEnabled){
        jsonResponse +="\"status\":\"on\",";
      }else {
        jsonResponse +="\"status\":\"off\",";
      }

      /*custom for types */
      jsonResponse +="\"hasBrightness\":" + String((int)deviceInThisRoom->customBytes[0]) + ",";
      if(deviceInThisRoom->deviceType == type_LED_STRIP){
        String rgbColor = getHexColor(
            deviceInThisRoom->customBytes[2],
            deviceInThisRoom->customBytes[3],
            deviceInThisRoom->customBytes[4]);
        jsonResponse +="\"avgColor\":\"#"+rgbColor+"\",";
      }

      if(deviceInThisRoom->deviceType == type_TEMP_SENSOR){
        float temperature = 0.f;
        memcpy(&temperature, &deviceInThisRoom->customBytes[3], sizeof(temperature));
        jsonResponse +="\"temp\":"+ String(temperature)+",";
        jsonResponse +="\"humid\":"+ String((int)deviceInThisRoom->customBytes[2])+",";
      }
      
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

void HTTPAsyncRequestHandler::createNotificationListContentJson()
{
    uint8_t activeNotificationCount = 
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).getActiveNotificationsCount();

    jsonResponse += "{";
    jsonResponse += "\"count\": "+String((int)activeNotificationCount)+",";
    jsonResponse += "\"notifications:\": [";
    
    for(uint8_t i = 0 ; i < activeNotificationCount; i++){
        /* get each notification */
        UserInterfaceNotification notification =
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).getOldestNotification();
        /* Add notification content to JSON */
        jsonResponse += notification.toJson();

        if(i < activeNotificationCount-1){
            jsonResponse += ",";
        }
    }
    jsonResponse += "]";
    jsonResponse += "}";
}

void HTTPAsyncRequestHandler::createDeviceConfigurationJson()
{
    String deviceCfg = std::any_cast<DeviceConfigManipulationAPI>
        (DataContainer::getSignalValue(SIG_SET_CONFIG_VIA_JSON_STRING)).getDeviceCfgJson();

    String localSetup = std::any_cast<DeviceConfigManipulationAPI>
        (DataContainer::getSignalValue(SIG_SET_DEVICES_CONFIG_VIA_JSON)).getDeviceCfgJson();

    String roomsCfg = std::any_cast<DeviceConfigManipulationAPI>
        (DataContainer::getSignalValue(CBK_GET_ROOMS_CFG_JSON)).getDeviceCfgJson();

    jsonResponse += "{"+localSetup+","+deviceCfg+","+roomsCfg+"}";
}

void HTTPAsyncRequestHandler::createRedirectToMainJson()
{
    jsonResponse += "{\"redirect\":\"mainPage\"}";
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
        
        case ASYNC_GET_NOTIFICATION_LIST:
            createNotificationListContentJson();
            break;
        
        case ASYNC_DOWNLOAD_CONFIGURATION:
            createDeviceConfigurationJson();
            break;

        case ASYNC_REDIRECT_TO_MAIN_PAGE:
            createRedirectToMainJson();
            break;

        default : break;
    }
}

void HTTPAsyncRequestHandler::mainFunction()
{
    if(currentRequest.state == ASYNC_REQUEST_PROCESSING){
        /* do the request processing here */
        processRequest();
        //Serial.println("HTTPAsyncRequestHandler: ...");
    }
}


String HTTPAsyncRequestHandler::getJsonResponse()
{
    //Serial.println("HTTPAsyncRequestHandler: Completed, returning JSON ...");
    /* request processing is completed */
    createJsonResponse();
    /* Clear request data, as final state -> response is requested */
    currentRequestClear();
    /* Return previously prepared JSON */
    // Serial.println("HTTPAsyncRequestHandler: Request processing finished!");
    return jsonResponse;
}
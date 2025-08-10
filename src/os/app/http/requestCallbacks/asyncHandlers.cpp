#include <os/app/http/httpserver.hpp>

void HomeLightHttpServer::parameterizedHandler_newSetupJson(String &request, WiFiClient &client)
{
  /* Call CBK_SET_CONFIG_VIA_STRING function with "header" parameter */
  std::any_cast<DeviceConfigManipulationAPI>(DataContainer::getSignalValue(SIG_SET_DEVICES_CONFIG_VIA_JSON)).setDeviceCfgViaJson(request);
}

void HomeLightHttpServer::parameterizedHandler_deviceSwitch(String &request, WiFiClient &client)
{
  pos1 = request.indexOf("stDvstte");
  pos2 = request.indexOf("state");
  pos3 = request.indexOf("&");
  String devId = request.substring(pos1 + 8, pos2);
  String state = request.substring(pos2 + 5, pos3);
  uint8_t deviceId = devId.toInt();
  uint8_t deviceState = state.toInt();

  uint8_t parameters[4];
  parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = deviceId; /* idx 0 */
  parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1; /* idx 1 */
  parameters[SERVICE_NAME_INDEX] = DEVSERVICE_STATE_SWITCH;       /* idx 2 */
  parameters[3] = deviceState;                                    /* idx 3 */

  HTTPAsyncRequestHandler::createRequest(
      ASYNC_TYPE_DEVICE_SERVICE_CALL,
      parameters,
      4);

  Serial.println("Async request processing started ");
  /* Request will be processed in next cycle of the HttpServer */
}

void HomeLightHttpServer::parameterizedHandler_loadDeviceConfiguration(String &request, WiFiClient &client)
{
  Serial.println("Loading configuration from file request ...");

  escapeSpecialCharsInJson(request);
  request.replace("loaddeicvcfg&", "");

  // Serial.println(request);

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  UserInterfaceNotification notification;

  if (success == DeserializationError::Code::Ok)
  {
    Serial.println("Json deserialized successfully");
    bool loadingFailure = false;
    loadingFailure |= std::any_cast<DeviceConfigManipulationAPI>(DataContainer::getSignalValue(SIG_SET_CONFIG_VIA_JSON_STRING)).loadConfigFromFile(doc);

    loadingFailure |= std::any_cast<DeviceConfigManipulationAPI>(DataContainer::getSignalValue(SIG_SET_DEVICES_CONFIG_VIA_JSON)).loadConfigFromFile(doc);

    loadingFailure |= std::any_cast<DeviceConfigManipulationAPI>(DataContainer::getSignalValue(CBK_GET_ROOMS_CFG_JSON)).loadConfigFromFile(doc);

    notification.title = "New configuration loaded";
    notification.type = UserInterfaceNotification::INFO;
    notification.body = "Config file loaded successfully. Device will be restarted.";
    HTTPAsyncRequestHandler::createRequest(ASYNC_REDIRECT_TO_MAIN_PAGE, nullptr, 0);
  }
  else
  {

    Serial.println("Json file loading error");
    notification.title = "Config file loading failure";
    notification.type = UserInterfaceNotification::ERROR;
    notification.body = "Problem occurred with configuration file loading.";
  }

  std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);

  /* successfully loaded */
  if (notification.type == UserInterfaceNotification::INFO)
  {
    std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(1000);
  }
}

void HomeLightHttpServer::parameterizedHandler_setStripColor(String &request, WiFiClient &client)
{
  escapeSpecialCharsInJson(request);
  request.replace("/setStripColor&", "");

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if (success == DeserializationError::Code::Ok)
  {
    uint16_t numberOfLeds = doc["color"].size();

    uint8_t *memory = (uint8_t *)malloc(numberOfLeds * 3 + 40);
    memory[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_3;
    memory[SERVICE_NAME_INDEX] = DEVSERVICE_SET_DETAILED_COLORS;

    *((uint16_t *)(memory + DYNAMIC_REQUEST_MEMORY_LENGTH_IDX)) = (numberOfLeds * sizeof(LedColor)) + sizeof(LedStripAnimationProperties);

    memory[DYNAMIC_REQUEST_DIRECTION_IDX] = e_IN_to_DEVICE;
    /* set the colors data */
    LedColor *ledValueAddr = (LedColor *)&memory[DYNAMIC_REQUEST_START_OF_DATA_IDX + sizeof(LedStripAnimationProperties)];

    String deviceId = String(doc["devId"]);
    String enAnim = String(doc["enableAnimation"]);
    String disAnim = String(doc["disableAnimation"]);
    String animSpeed = String(doc["speed"]);
    String liveAnim = String(doc["live"]);
    String liveSpeedStr = String(doc["liveSpd"]);
    /* Process JSON to extrac each device slot*/
    for (uint16_t i = 0; i < numberOfLeds; i++)
    {
      /*this exist for every slot*/
      String r = String(doc["color"][i][0]);
      String g = String(doc["color"][i][1]);
      String b = String(doc["color"][i][2]);

      if (r != "null" && g != "null" && b != "null")
      {
        ledValueAddr->r = r.toInt() <= 255 ? r.toInt() : 255;
        ledValueAddr->g = g.toInt() <= 255 ? g.toInt() : 255;
        ledValueAddr->b = b.toInt() <= 255 ? b.toInt() : 255;

        // Serial.println(String((int)ledValueAddr->r) + " " + String((int)ledValueAddr->g) + " " + String((int)ledValueAddr->b));
      }
      else
      {
        break;
      }

      /* go to next diode */
      ledValueAddr++;
    }

    if (deviceId != "null")
    {
      // Parse animation properties from json values
      uint8_t enableAnimation = enAnim.toInt() ? enAnim.toInt() : 0;
      uint8_t disableAnimation = disAnim.toInt() ? disAnim.toInt() : 0;
      uint8_t speed = animSpeed.toInt() ? animSpeed.toInt() : 0;
      uint8_t liveAnimation = liveAnim.toInt() ? liveAnim.toInt() : 0;
      uint8_t liveSpeed = liveSpeedStr.toInt() ? liveSpeedStr.toInt() : 0;
      LedStripAnimationProperties animationProperties = {
          .enableAnimation = enableAnimation,
          .disableAnimation = disableAnimation,
          .animationSpeed = speed,
          .liveAnimation = liveAnimation,
          .liveAnimationSpeed = liveSpeed,
        };
      // Set animation properties to the request
      memcpy(memory + DYNAMIC_REQUEST_START_OF_DATA_IDX, &animationProperties, sizeof(LedStripAnimationProperties));

      memory[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = deviceId.toInt();

      // Serial.println(request);
      HTTPAsyncRequestHandler::createRequest(
          ASYNC_TYPE_DEVICE_SERVICE_CALL,
          memory,
          (numberOfLeds * 3 + 40));
    }
    free(memory);
  }
}

void HomeLightHttpServer::parameterizedHandler_stripLoadFromMemory(String &request, WiFiClient &client)
{

  escapeSpecialCharsInJson(request);
  request.replace("stripLoadFromMemory&", "");

  Serial.println(request);
  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if (success == DeserializationError::Code::Ok)
  {
    String devIdStr = doc["devId"];
    String memorySlotStr = doc["slot"];

    if (devIdStr != "null" && memorySlotStr != "null")
    {
      Serial.println("Load strip from memory ...");
      uint8_t parameters[4];
      parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = devIdStr.toInt(); /* idx 0 */
      parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1;         /* idx 1 */
      parameters[SERVICE_NAME_INDEX] = DEVSERVICE_LED_STRIP_SWITCH_CONTENT;   /* idx 2 */
      parameters[3] = memorySlotStr.toInt();                                  /* idx 3 */

      HTTPAsyncRequestHandler::createRequest(
          ASYNC_TYPE_DEVICE_SERVICE_CALL,
          parameters,
          4);
    }
  }
}

void HomeLightHttpServer::parameterizedHandler_stripSaveCurrent(String &request, WiFiClient &client)
{
  escapeSpecialCharsInJson(request);
  request.replace("stripOverwriteSlot&", "");

  Serial.println(request);

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if (success == DeserializationError::Code::Ok)
  {
    String devId = doc["devId"];
    String slot = doc["slot"];

    if (devId != "null" && slot != "null")
    {

      Serial.println("Request: stripOverwriteSlot {devId, slot}");
      uint8_t parameters[4];
      parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = devId.toInt(); /* idx 0 */
      parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1;      /* idx 1 */
      parameters[SERVICE_NAME_INDEX] = DEVSERVICE_LED_STRIP_SAVE_CONTENT;  /* idx 2 */
      parameters[3] = slot.toInt();                                        /* idx 3 */

      HTTPAsyncRequestHandler::createRequest(
          ASYNC_TYPE_DEVICE_SERVICE_CALL,
          parameters,
          4);
    }
  }
}

void HomeLightHttpServer::parameterizedHandler_roomStateChange(String &request, WiFiClient &client)
{
  escapeSpecialCharsInJson(request);
  request.replace("stRmChng&", "");

  Serial.println(request);

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if (success == DeserializationError::Code::Ok)
  {
    String roomIdStr = doc["roomId"];
    String stateStr = doc["state"];

    if (roomIdStr != "null" && stateStr != "null")
    {
      uint8_t roomId = 255;
      // Try to find roomId in the mapping;
      for (auto &room : roomNamesMapping)
      {
        if (room.second == roomIdStr)
        {
          roomId = room.first;
          break;
        }
      }

      // Name not found in mapping - use the string as roomId
      if (roomId == 255)
      {
        roomId = roomIdStr.toInt();
      }

      // Serial.println("Room state change requested.");
      uint8_t parameters[4];
      parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = roomId;   /* idx 0 */
      parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1; /* idx 1 */
      parameters[SERVICE_NAME_INDEX] = DEVSERVICE_ROOM_STATE_CHANGE;  /* idx 2 */
      parameters[3] = stateStr == "true" ? 1 : 0;                     /* idx 3 */

      HTTPAsyncRequestHandler::createRequest(
          ASYNC_TYPE_DEVICE_SERVICE_CALL,
          parameters,
          4);
    }
  }
  else
  {
    Serial.println("Error with JSON parsing");
  }
}


void HomeLightHttpServer::parameterizedHandler_roomToggle(String& request, WiFiClient& client)
{
  escapeSpecialCharsInJson(request);
  request.replace("roomToggle&", "");

  Serial.println(request);

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if (success == DeserializationError::Code::Ok)
  {
    String roomIdStr = doc["roomId"];

    if (roomIdStr != "null")
    {
      uint8_t roomId = 255;
      // Try to find roomId in the mapping;
      for (auto &room : roomNamesMapping)
      {
        if (room.second == roomIdStr)
        {
          roomId = room.first;
          break;
        }
      }

      // Name not found in mapping - use the string as roomId
      if (roomId == 255)
      {
        roomId = roomIdStr.toInt();
      }

      // Need to evaluate toggle value
      std::vector<DeviceDescription> devicesCollection = 
            std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
      bool toggleValue = true;
      
      for(auto& device : devicesCollection)
      {
        if(device.roomId == roomId && (device.isEnabled == 1)) // compare to 1 as non-disablabe will return 2 or 3
        {
          toggleValue = false; // at least one device is ON, so we need to turn it OFF
          break;
        }
      }

      Serial.println("Room state change requested.");
      uint8_t parameters[4];
      parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = roomId;   /* idx 0 */
      parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1; /* idx 1 */
      parameters[SERVICE_NAME_INDEX] = DEVSERVICE_ROOM_STATE_CHANGE;  /* idx 2 */
      parameters[3] = toggleValue;                     /* idx 3 */

      HTTPAsyncRequestHandler::createRequest(
          ASYNC_TYPE_DEVICE_SERVICE_CALL,
          parameters,
          4);
    }
  }
  else
  {
    Serial.println("Error with JSON parsing");
  }
}

void HomeLightHttpServer::parameterizedHandler_segmentStateSwitch(String &request, WiFiClient &client)
{
  escapeSpecialCharsInJson(request);
  request.replace("segSwtch&", "");

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if (success == DeserializationError::Code::Ok)
  {
    String devIdStr = doc["devId"];
    String segmentIdStr = doc["seg"];
    String stateStr = doc["state"];

    if (devIdStr != "null" && segmentIdStr != "null" && stateStr != "null")
    {
      uint8_t parameters[3 + sizeof(ServiceParameters_set1)];
      ServiceParameters_set1 params;
      params.a = segmentIdStr.toInt();
      params.b = stateStr == "true" ? 1 : 0;                                  // 1 for ON, 0 for OFF
      parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = devIdStr.toInt(); /* idx 0 */
      parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1;         /* idx 1 */
      parameters[SERVICE_NAME_INDEX] = DEVSERVICE_SEGMENT_STATE_SWITCH;       /* idx 2 */
      memcpy(&parameters[3], &params, sizeof(ServiceParameters_set1));

      HTTPAsyncRequestHandler::createRequest(
          ASYNC_TYPE_DEVICE_SERVICE_CALL,
          parameters,
          3 + sizeof(ServiceParameters_set1));
    }
  }
}

void HomeLightHttpServer::parameterizedHandler_asyncSystemDetails(String &request, WiFiClient &client)
{
  /* this request is only handled when there is Active Cooling system present in HomeControl device */
  if (std::any_cast<bool>(DataContainer::getSignalValue(SIG_IS_ACTIVE_COOLING_SYSTEM_PRESENT)))
  {
    HTTPAsyncRequestHandler::createRequest(ASYNC_GET_SYSTEM_DETAILS, nullptr, 0);
  }
}

void HomeLightHttpServer::parameterizedHandler_ledsLiveSwitch(String &request, WiFiClient &client)
{
  escapeSpecialCharsInJson(request);
  request.replace("ledsLiveSwtch&", "");

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if (success == DeserializationError::Code::Ok)
  {
    String devIdStr = doc["devId"];
    String state = doc["state"];

    if (devIdStr != "null" && state != "null")
    {
      ServiceParameters_set1 params;
      params.a = state.toInt() ? state.toInt() : 0; 
      uint8_t parameters[3 + sizeof(ServiceParameters_set1)];
      parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = devIdStr.toInt(); /* idx 0 */
      parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1;  /* idx 1 */
      parameters[SERVICE_NAME_INDEX] = DEVSERVICE_LIVE_ANIMATION;             /* idx 2 */
      memcpy(&parameters[3], &params, sizeof(ServiceParameters_set1));

      HTTPAsyncRequestHandler::createRequest(
          ASYNC_TYPE_DEVICE_SERVICE_CALL,
          parameters,
          3 + sizeof(ServiceParameters_set1));
    }
  }
}

void HomeLightHttpServer::parameterizedHandler_getHash(String &request, WiFiClient &client)
{
    HTTPAsyncRequestHandler::createRequest(ASYNC_GET_HASH, nullptr, 0);
}

void HomeLightHttpServer::parameterizedHandler_getExtendedControls(String &request, WiFiClient &client)
{
  Serial.println("Extended controls requested");
  escapeSpecialCharsInJson(request);
  request.replace("/getExtendedControls&", "");

  Serial.println(request);

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if (success == DeserializationError::Code::Ok)
  {
    String deviceIdStr = doc["devId"];
    if (deviceIdStr != "null")
    {
      uint16_t deviceId = deviceIdStr.toInt();
      Serial.println("Advanced controls requested.");
      HTTPAsyncRequestHandler::createRequest(
          ASYNC_GET_ADVANCED_CONTROLS,
          (uint8_t *)&deviceId,
          sizeof(deviceId));
    }
  }
}

void HomeLightHttpServer::parameterizedHandler_downloadDeviceConfiguration(String &request, WiFiClient &client)
{

  HTTPAsyncRequestHandler::createRequest(
      ASYNC_DOWNLOAD_CONFIGURATION,
      nullptr,
      0);

  Serial.println("Device configuration download requested ...");
  /* Request will be processed in next cycle of the HttpServer */
}

void HomeLightHttpServer::parameterizedHandler_deviceBrightnessChange(String &request, WiFiClient &client)
{
  pos1 = request.indexOf("chngdvbr"); // Wyszukuje pozycję, na której występuje ciąg znaków 'bri' w nagłówku.
  pos2 = request.indexOf("DEV");      // Wyszukuje pozycję, na której występuje ciąg znaków 'DEV' w nagłówku.
  pos3 = request.indexOf("&");        // Wyszukuje pozycję, na której występuje ciąg znaków '&' w nagłówku.
  String brightnessString = request.substring(pos1 + 8, pos2);
  String idDeviceString = request.substring(pos2 + 3, pos3);

  uint8_t newbrightness = brightnessString.toInt();
  uint8_t idString = idDeviceString.toInt();

  uint8_t parameters[4];
  parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = idString; /* idx 0 */
  parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1; /* idx 1 */
  parameters[SERVICE_NAME_INDEX] = DEVSERVICE_BRIGHTNESS_CHANGE;  /* idx 2 */
  parameters[3] = newbrightness;                                  /* idx 3 */

  HTTPAsyncRequestHandler::createRequest(
      ASYNC_TYPE_DEVICE_SERVICE_CALL,
      parameters,
      4);
}

void HomeLightHttpServer::constantHandler_asyncGetPageContent(String &request, WiFiClient &client)
{
  HTTPAsyncRequestHandler::createRequest(ASYNC_GET_PAGE_CONTENT, nullptr, 0);
}

void HomeLightHttpServer::constantHandler_asyncGetNotifications(String &request, WiFiClient &client)
{
  HTTPAsyncRequestHandler::createRequest(ASYNC_GET_NOTIFICATION_LIST, nullptr, 0);
}

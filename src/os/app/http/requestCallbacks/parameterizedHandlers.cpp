#include <os/app/http/httpserver.hpp>

void HomeLightHttpServer::parameterizedHandler_newConfigApply(String& request, WiFiClient& client)
{
  // Configuration to be handled here

  Serial.println("Applying new config!");
  /* Call CBK_SET_CONFIG_VIA_STRING function with "header" parameter */
  client.println("<meta http-equiv='refresh' content='0;  url=http://"+ ipAddressString +"'>");
  client.println("</div></body></html>");
  client.flush();
  client.stop();
  if(std::any_cast<DeviceConfigManipulationAPI>
    (DataContainer::getSignalValue(SIG_SET_CONFIG_VIA_JSON_STRING)).setDeviceCfgViaJson(request)) {

        std::any_cast<std::function<void(uint16_t)>>
          (DataContainer::getSignalValue(CBK_RESET_DEVICE))(2000);
      }
}

void HomeLightHttpServer::parameterizedHandler_roomNameMappingApply(String& request, WiFiClient& client)
{
    SecurityAccessLevelType currentAccessLevel = 
        std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));

    UserInterfaceNotification notification;
    notification.title = "Room name management failure";
    notification.type = UserInterfaceNotification::WARNING;

    /* Check if device is unlocked that config can be modified */
    if(currentAccessLevel > e_ACCESS_LEVEL_NONE){
        escapeSpecialCharsInJson(request);
        request.replace("roomMappingApply&", "");

        // Serial.println("Json request:" + request);

        JsonDocument doc;
        DeserializationError success = deserializeJson(doc, request.c_str());
        if(success == DeserializationError::Code::Ok){
            String mappingsCount = String(doc["mappingsCount"]);
            uint8_t numberOfMappings = mappingsCount.toInt();

            for(uint8_t i = 0 ; i < numberOfMappings; i++)
            {
              String roomId   = String(doc["roomMapping"][i]["roomId"]);
              String roomName = String(doc["roomMapping"][i]["roomName"]);
              int roomIdInt = roomId.toInt();

              if(roomName.length() > 0 && roomNamesMapping.size() < MAX_NUMBER_OF_ROOM_NAME_TRANSLATIONS){
                if (roomNamesMapping.find(roomIdInt) != roomNamesMapping.end()) {
                  roomNamesMapping[roomIdInt] = roomName;
                }else {
                  roomNamesMapping.insert({roomIdInt, roomName});
                }
              }
            }
            client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"/roomAssignment'>");
            
            /* Trigger NVM save to have persistant room name values */
            std::any_cast<std::function<void()>>(DataContainer::getSignalValue(CBK_START_NVM_SAVE_TIMER))();

        }else {
            Serial.println("ConfigProvider://Problem with JSON parsing.");
            notification.body = "Room names JSON content cannot be correctly evaluated";
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
        }
    } else 
    {
        /* No access level to apply */
        notification.body = "Device cannot be locked to apply new configuration";
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
    }

}

void HomeLightHttpServer::parameterizedHandler_passwordApply(String& request, WiFiClient& client)
{

  Serial.println("Password apply processing");
  request = request.substring(String("passwordApply").length());
  try{
    /* Request Security access level change in OS via received password */
    std::any_cast<std::function<void(String)>>(
      DataContainer::getSignalValue(CBK_SECURITY_ACCESS_LEVEL_CHANGE_VIA_STRING))(request);
  }catch (std::bad_any_cast ex){

  }

  client.println("<meta http-equiv='refresh' content='1; url=http://"+ ipAddressString +"/config'>");
}

void HomeLightHttpServer::parameterizedHandler_ledStripColor(String& request, WiFiClient& client){
//   int pos1 = request.indexOf("ledStripColor?id="); 
//   int pos2 = request.indexOf("&r="); 
//   int pos3 = request.indexOf("&g=");
//   int pos4 = request.indexOf("&b=");       
//   String devId = request.substring(pos1+17 , pos2);
//   String red = request.substring(pos2+3 , pos3);
//   String green = request.substring(pos3+3 , pos4);
//   String blue = request.substring(pos4+3);

//   for(uint8_t i = 0 ; i < ledsCount; i++)
//   {
//     leds.red[i] = red.toInt();
//     leds.green[i] = green.toInt();
//     leds.blue[i] = blue.toInt();
//   }

//   Serial.println(devId);
//   Serial.println(red + ", " + green + " , " + blue);

//   client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");

}

void HomeLightHttpServer::parameterizedHandler_ledColor(String& request, WiFiClient& client){
//   int pos1 = request.indexOf("ledColor?id="); 
//   int pos2 = request.indexOf("&led="); 
//   int pos3 = request.indexOf("&r="); 
//   int pos4 = request.indexOf("&g=");
//   int pos5 = request.indexOf("&b=");       
//   String devId = request.substring(pos1+12 , pos2);
//   String ledIndex = request.substring(pos2+5 , pos3);
//   String red = request.substring(pos3+3 , pos4);
//   String green = request.substring(pos4+3 , pos5);
//   String blue = request.substring(pos5+3);

//   leds.red[ledIndex.toInt()] = red.toInt();
//   leds.green[ledIndex.toInt()] = green.toInt();
//   leds.blue[ledIndex.toInt()] = blue.toInt();
  

//   Serial.println(devId);
//   Serial.println(red + ", " + green + " , " + blue);

//   client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");

}


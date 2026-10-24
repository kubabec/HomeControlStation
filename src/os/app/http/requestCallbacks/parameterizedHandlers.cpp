#include <os/app/http/httpserver.hpp>
#include <os/app/DigitalEvent/DigitalEventReceiver.hpp>
#include <os/app/config/ConfigProvider.hpp>
#include <os/app/timeMaster.hpp>
#include "os/drivers/softwareupdater.hpp"
#include "os/Logger.hpp"

/**
 * @file src/os/app/http/requestCallbacks/parameterizedHandlers.cpp
 * @brief HTTP server implementation and request callbacks for the Home Control Station.
 */


void HomeLightHttpServer::parameterizedHandler_newConfigApply(String &request, WiFiClient &client)
{
  // Configuration to be handled here

  Logger::log("Applying new config!");
  /* Call CBK_SET_CONFIG_VIA_STRING function with "header" parameter */
  client.println("<meta http-equiv='refresh' content='0;  url=http://" + ipAddressString + "'>");
  client.println("</div></body></html>");
  client.flush();
  client.stop();
  if (std::any_cast<DeviceConfigManipulationAPI>(DataContainer::getSignalValue(SIG_SET_CONFIG_VIA_JSON_STRING)).setDeviceCfgViaJson(request))
  {

    std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(2000);
  }
}

void HomeLightHttpServer::parameterizedHandler_roomNameMappingApply(String &request, WiFiClient &client)
{
  SecurityAccessLevelType currentAccessLevel =
      std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));

  UserInterfaceNotification notification;
  notification.title = "Room name management failure";
  notification.type = UserInterfaceNotification::WARNING;

  /* Check if device is unlocked that config can be modified */
  if (currentAccessLevel > e_ACCESS_LEVEL_NONE)
  {
    escapeSpecialCharsInJson(request);
    request.replace("roomMappingApply&", "");

    // Logger::log("Json request:" + request);

    JsonDocument doc;
    DeserializationError success = deserializeJson(doc, request.c_str());
    if (success == DeserializationError::Code::Ok)
    {
      String mappingsCount = String(doc["mappingsCount"]);
      uint8_t numberOfMappings = mappingsCount.toInt();

      for (uint8_t i = 0; i < numberOfMappings; i++)
      {
        String roomId = String(doc["roomMapping"][i]["roomId"]);
        String roomName = String(doc["roomMapping"][i]["roomName"]);
        int roomIdInt = roomId.toInt();

        if (roomName.length() > 0 && roomNamesMapping.size() < MAX_NUMBER_OF_ROOM_NAME_TRANSLATIONS)
        {
          if (roomNamesMapping.find(roomIdInt) != roomNamesMapping.end())
          {
            roomNamesMapping[roomIdInt] = roomName;
          }
          else
          {
            roomNamesMapping.insert({roomIdInt, roomName});
          }
        }
      }
      client.println("<meta http-equiv='refresh' content='0; url=http://" + ipAddressString + "/roomAssignment'>");

      DataContainer::setSignalValue(SIG_ROOM_NAMES_MAPPING, RoomNamesMapping(roomNamesMapping));

      /* Trigger NVM save to have persistant room name values */
      std::any_cast<std::function<void()>>(DataContainer::getSignalValue(CBK_START_NVM_SAVE_TIMER))();
    }
    else
    {
      Logger::log("ConfigProvider://Problem with JSON parsing.");
      notification.body = "Room names JSON content cannot be correctly evaluated";
      std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
    }
  }
  else
  {
    /* No access level to apply */
    notification.body = "Device cannot be locked to apply new configuration";
    std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
  }
}

void HomeLightHttpServer::parameterizedHandler_newDigEvntTab(String &request, WiFiClient &client)
{
  SecurityAccessLevelType currentAccessLevel =
      std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));
  if (currentAccessLevel == e_ACCESS_LEVEL_SERVICE_MODE)
  {
    escapeSpecialCharsInJson(request);
    request.replace("/newDigEvntTab&", "");
    std::any localAny = DataContainer::getSignalValue(CBK_UPDATE_DIG_EVNT_TABLE);
    if (auto p = std::any_cast<std::function<void(String &)>>(&localAny))
    {
      (*p)(request);
      DigitalEventReceiver::deinit();
      if (!ConfigProvider::flushNvmData())
      {
        Logger::log("Digital event mapping NVM commit failed");
      }
      client.println("<meta http-equiv='refresh' content='0; url=/digBtn'>");
    }
    else
    {
    }
  }
}

void HomeLightHttpServer::parameterizedHandler_enablingConditions(String &request, WiFiClient &client)
{
  SecurityAccessLevelType currentAccessLevel =
      std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));
  if (currentAccessLevel == e_ACCESS_LEVEL_SERVICE_MODE)
  {
    escapeSpecialCharsInJson(request);
    request.replace("/saveEnablingConditions&", "");
    DigitalEventReceiver::updateEnablingConditionsViaJson(request);
    DigitalEventReceiver::deinit();
    if (!ConfigProvider::flushNvmData())
    {
      Logger::log("Enabling conditions NVM commit failed");
    }
    client.println("<meta http-equiv='refresh' content='0; url=/enablingConditions'>");
  }
}

void HomeLightHttpServer::parameterizedHandler_dayTimeConfig(String &request, WiFiClient &client)
{
  /* Expected: /saveDayTime&ms=HH:MM&me=HH:MM&es=HH:MM&ee=HH:MM&tz=N (empty HH:MM = not configured) */
  auto readParam = [&request](const char *key) -> String {
    const String token = String("&") + key + "=";
    const int start = request.indexOf(token);
    if (start < 0) return String("");
    const int valueStart = start + token.length();
    int end = request.indexOf('&', valueStart);
    if (end < 0) end = request.length();
    String value = request.substring(valueStart, end);
    value.trim();
    return value;
  };
  auto parseTime = [](const String &text, uint8_t &hh, uint8_t &mm) {
    hh = 255;
    mm = 255;
    if (text.length() != 5 || text.charAt(2) != ':') return;
    const int h = text.substring(0, 2).toInt();
    const int m = text.substring(3, 5).toInt();
    if (h < 0 || h > 23 || m < 0 || m > 59) return;
    hh = (uint8_t)h;
    mm = (uint8_t)m;
  };

  DayTimeRanges ranges;
  parseTime(readParam("ms"), ranges.morningStart_hh, ranges.morningStart_mm);
  parseTime(readParam("me"), ranges.morningEnd_hh, ranges.morningEnd_mm);
  parseTime(readParam("es"), ranges.eveningStart_hh, ranges.eveningStart_mm);
  parseTime(readParam("ee"), ranges.eveningEnd_hh, ranges.eveningEnd_mm);
  const int timeZone = readParam("tz").toInt();

  bool saved = false;
  std::any setter = DataContainer::getSignalValue(CBK_SET_DAY_TIME_CONFIG);
  if (DayTime::isTimeZoneValid(timeZone))
  {
    if (auto cbk = std::any_cast<std::function<bool(const DayTimeRanges &, int8_t)>>(&setter))
    {
      saved = (*cbk)(ranges, (int8_t)timeZone);
    }
  }

  if (!saved)
  {
    UserInterfaceNotification notif;
    notif.title = "Day time not saved";
    notif.body = "Invalid day time configuration. Morning start < Morning end <= Evening start < Evening end is required.";
    notif.type = UserInterfaceNotification::ERROR;
    std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
  }
  client.println("<meta http-equiv='refresh' content='0; url=/config'>");
}

void HomeLightHttpServer::parameterizedHandler_rtcEvents(String &request, WiFiClient &client)
{
  SecurityAccessLevelType currentAccessLevel =
      std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));
  if (currentAccessLevel == e_ACCESS_LEVEL_SERVICE_MODE)
  {
    escapeSpecialCharsInJson(request);
    request.replace("/saveRtcEvents&", "");
    if (!TimeMaster::updateRtcEventsViaJson(request))
      Logger::log("RTC event table validation or NVM commit failed");
    client.println("<meta http-equiv='refresh' content='0; url=/rtcEvents'>");
  }
}

void HomeLightHttpServer::parameterizedHandler_passwordApply(String &request, WiFiClient &client)
{

  Logger::log("Password apply processing");
  request = request.substring(String("passwordApply").length());
  std::any localAny = DataContainer::getSignalValue(CBK_SECURITY_ACCESS_LEVEL_CHANGE_VIA_STRING);
  if (auto p = std::any_cast<std::function<void(String)>>(&localAny))
  {
    /* Request Security access level change in OS via received password */
    (*p)(request);
  }

  else
  {
  }

  client.println("<meta http-equiv='refresh' content='1; url=http://" + ipAddressString + "/config'>");
}

void HomeLightHttpServer::parameterizedHandler_softwareUpdateInstall(String &request, WiFiClient &client)
{
  const String prefix = "softwareUpdateInstall&";
  const String version = request.substring(prefix.length());
  client.println(SoftwareUpdater::start(version) ? "{\"ok\":true}" : "{\"ok\":false}");
}

void HomeLightHttpServer::parameterizedHandler_ledStripColor(String &request, WiFiClient &client)
{
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

  //   Logger::log(devId);
  //   Logger::log(red + ", " + green + " , " + blue);

  //   client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
}

void HomeLightHttpServer::parameterizedHandler_ledColor(String &request, WiFiClient &client)
{
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

  //   Logger::log(devId);
  //   Logger::log(red + ", " + green + " , " + blue);

  //   client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
}

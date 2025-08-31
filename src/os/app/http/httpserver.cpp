#include <os/app/http/HttpServer.hpp>
#include <Regexp.h>
#include <ArduinoJson.h>
#include "os/app/http/ConfigPageHttp.h"
#include "os/app/http/PageHead.h"
#include "os/app/http/Style.h"
#include "os/app/http/JavaScript.h"
#include "os/app/http/tempGauge/tempGaugeJS.h"
#include "os/app/http/tempGauge/tempGaugeCSS.h"
#include "os/app/http/renderRoomsJS.h"


WiFiServer HomeLightHttpServer::server(80);
WiFiClient client;
HomeLightHttpServer::HttpServerNvmMetadata HomeLightHttpServer::nvmMetadata;
String HomeLightHttpServer::header = "";
unsigned long HomeLightHttpServer::currentTime = 0;
unsigned long HomeLightHttpServer::previousTime = 0;
const long HomeLightHttpServer::timeoutTime = 2000;
int HomeLightHttpServer::pos1= 100;
int HomeLightHttpServer::pos2 = 150; 
int HomeLightHttpServer::pos3 = 150;
SecurityAccessLevelType HomeLightHttpServer::secAccessLevel = e_ACCESS_LEVEL_NONE;

std::vector<DeviceDescription> HomeLightHttpServer::descriptionVector;
std::map<uint8_t, std::vector<DeviceDescription*>> HomeLightHttpServer::deviceToRoomMappingList;
std::map<uint8_t, String> HomeLightHttpServer::roomNamesMapping;
String HomeLightHttpServer::ipAddressString;



const char* labelStart = "<label>";
const char* labelEnd = "</label>";

std::vector<String> constantRequests = {
  "", /* Main page with no parameter */
  "config",
  "resetDevice",
  "localDevices",
  "roomAssignment",
  "masseraseviahttp",
  "asyncRequestTest",
  "networkInspection",
  "sysDetails"
};

std::vector<String> parameterizedRequests = {
  "newCfgApply",
  "roomMappingApply",
  "passwordApply",
  "ledStripColor",
  "ledColor"
};

std::vector<String> parameterizedAsyncRequests = {
  "stDvstte",
  "chngdvbr",
  "getPageContent",
  "getNotifications",
  "lclSetupJson",
  "dwlddevcfg",
  "loaddeicvcfg",
  "getExtendedControls",
  "setStripColor",
  "stripLoadFromMemory",
  "stripOverwriteSlot",
  "stRmChng",
  "segSwtch",
  "ledsLiveSwtch",
  "getHash",
  "roomToggle"
};


std::vector<std::pair<std::function<void(WiFiClient&)>, SecurityAccessLevelType>> constantRequestHandlers = {
  {HomeLightHttpServer::constantHandler_mainPage, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::constantHandler_configPage, e_ACCESS_LEVEL_AUTH_USER},
  {HomeLightHttpServer::constantHandler_resetDevice, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::constantHandler_devicesSetup, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::constantHandler_roomAssignment, e_ACCESS_LEVEL_AUTH_USER},
  {HomeLightHttpServer::constantHandler_massErase, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::constantHandler_asyncTest, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::constantHandler_networkInspecion, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::constantHandler_systemDetails, e_ACCESS_LEVEL_SERVICE_MODE},
};

std::vector<std::pair<std::function<void(String&, WiFiClient&)>, SecurityAccessLevelType>> parameterizedRequestHandlers = {
  {HomeLightHttpServer::parameterizedHandler_newConfigApply, e_ACCESS_LEVEL_AUTH_USER},
  {HomeLightHttpServer::parameterizedHandler_roomNameMappingApply, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_passwordApply, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_ledStripColor, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_ledColor, e_ACCESS_LEVEL_NONE},
};

std::vector<std::pair<std::function<void(String&, WiFiClient&)>, SecurityAccessLevelType>> parameterizedAsyncRequestHandlers = {
  {HomeLightHttpServer::parameterizedHandler_deviceSwitch, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_deviceBrightnessChange, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::constantHandler_asyncGetPageContent, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::constantHandler_asyncGetNotifications, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_newSetupJson, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::parameterizedHandler_downloadDeviceConfiguration, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::parameterizedHandler_loadDeviceConfiguration, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::parameterizedHandler_getExtendedControls, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_setStripColor, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_stripLoadFromMemory, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_stripSaveCurrent, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_roomStateChange, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_segmentStateSwitch, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_ledsLiveSwitch, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_getHash, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::parameterizedHandler_roomToggle, e_ACCESS_LEVEL_NONE}
};

void HomeLightHttpServer::escapeSpecialCharsInJson(String& json)
{
  json.replace("%7B", "{");
  json.replace("%22", "\"");
  json.replace("%7D", "}");
  json.replace("%20", " ");
}

void HomeLightHttpServer::cyclic()
{
  HTTPAsyncRequestHandler::mainFunction();
  /* Synchronous processing is only allowed when async request is not under processing */
  if(HTTPAsyncRequestHandler::getProcessingState() == ASYNC_NO_REQUEST){
    handleClientRequest();
  }else 
  {
    header = "";

    if(HTTPAsyncRequestHandler::getProcessingState() == ASYNC_REQUEST_COMPLETED){
      String jsonResponse = HTTPAsyncRequestHandler::getJsonResponse();
      // Serial.println("HTTPServer : // JSON Response: " + jsonResponse);

      if(client){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type:application/json");
        client.println();
        client.println(jsonResponse);
        client.stop();
      }
    }
  }
}

void HomeLightHttpServer::flushNvmData()
{
  /* Write NVM data for HttpServer application */
  uint16_t sizeOfNvm = (e_BLOCK_HTTP_LAST - e_BLOCK_HTTP_FIRST + 1) * PERSISTENT_DATABLOCK_SIZE;
  /* Allocate memory for NVM data */
  uint8_t* nvmData = (uint8_t*)malloc(sizeOfNvm);
  uint8_t offset = 0;

  if(packNvmData(nvmData, sizeOfNvm)){
    for(uint8_t blockID = e_BLOCK_HTTP_FIRST; blockID <= e_BLOCK_HTTP_LAST; blockID ++)
    {
        /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
        std::any_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(
            DataContainer::getSignalValue(CBK_SET_NVM_DATABLOCK)
        )(
            (PersistentDatablockID)blockID, // Datablock ID
            (uint8_t*)&nvmData[offset] // local memory buffer for datablock data
        );

        /* Shift the offset, that next datablock will be written next to previous in 'nvmData' */
        offset += PERSISTENT_DATABLOCK_SIZE;
    }
  }
  /* release heap buffer */
  free(nvmData);
}

void HomeLightHttpServer::deinit() {
  flushNvmData();
}

bool HomeLightHttpServer::packNvmData(uint8_t* nvmData, uint16_t length)
{
  bool retVal = false;
  if(nvmData != nullptr){
    nvmData[0] = 0xAB;
    nvmData[1] = (uint8_t)roomNamesMapping.size();
    for(uint8_t i = 2; i < 25; i++)
    {
      nvmData[i] = 0;
    }

    uint16_t offset = 25;
    for(auto& mapping: roomNamesMapping){
      /* safe mech */
      if(offset + 25 > length){
        break;
      }

      nvmData[offset] = mapping.first; /* room ID */
      uint8_t strArr[24] = {'\0'};
      mapping.second.toCharArray((char*)strArr, 24);
      memcpy(&nvmData[offset+1], strArr, 24);

      offset += 25;
    }

    retVal = true;
  }

  return retVal;
}

void HomeLightHttpServer::init()
{
  Serial.println("HomeLightHttpServer init ...");

  server.setTimeout(10);


  DeviceConfigManipulationAPI cfgControls = {
    .setDeviceCfgViaJson = nullptr,
    .getDeviceCfgJson = HomeLightHttpServer::getRoomsCfgJson,
    .loadConfigFromFile = HomeLightHttpServer::loadConfigFromFile
  };

  DataContainer::setSignalValue(
    CBK_GET_ROOMS_CFG_JSON, 
    static_cast<DeviceConfigManipulationAPI>(cfgControls));


  /* Read NVM data for HttpServer application */
  uint16_t sizeOfNvm = (e_BLOCK_HTTP_LAST - e_BLOCK_HTTP_FIRST + 1) * PERSISTENT_DATABLOCK_SIZE;
  /* Allocate memory for NVM data */
  uint8_t* nvmData = (uint8_t*)malloc(sizeOfNvm);
  uint8_t offset = 0;
  for(uint8_t blockID = e_BLOCK_HTTP_FIRST; blockID <= e_BLOCK_HTTP_LAST; blockID ++)
  {
      /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
      std::any_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(
          DataContainer::getSignalValue(CBK_GET_NVM_DATABLOCK)
      )(
          (PersistentDatablockID)blockID, // Datablock ID
          (uint8_t*)&nvmData[offset] // local memory buffer for datablock data
      );

      /* Shift the offset, that next datablock will be written next to previous in 'nvmData' */
      offset += PERSISTENT_DATABLOCK_SIZE;
  }

  restoreNvmData(nvmData, sizeOfNvm);

  /* release heap buffer */
  free(nvmData);


  DataContainer::subscribe(SIG_DEVICE_COLLECTION, HomeLightHttpServer::onDeviceDescriptionChange);

  DataContainer::subscribe(SIG_SECURITY_ACCESS_LEVEL, [](std::any signal){
    try{
      secAccessLevel = std::any_cast<SecurityAccessLevelType>(signal);
    }catch (std::bad_any_cast ex){ }
  });

  /* Get IP address from DataContainer to have it for further client redirections */
  DataContainer::subscribe(SIG_IP_ADDRESS_STRING, [&](std::any signal) {
    ipAddressString = std::any_cast<String>(signal);
  });
  ipAddressString = std::any_cast<String>(
    DataContainer::getSignalValue(SIG_IP_ADDRESS_STRING)
  );

  secAccessLevel = std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));

  server.setNoDelay(true);
  server.begin();
  currentTime = millis();
  //DataContainer::subscribe(CBK_DEVICE_ENABLE, "DeviceManager", ...);

  HTTPAsyncRequestHandler::init(&roomNamesMapping, &deviceToRoomMappingList);


  Serial.println("... done");
}

void HomeLightHttpServer::restoreNvmData(uint8_t* nvmData, uint16_t length)
{

  /* NVM expected pattern */
  /* 0xAB X 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 */
  /* ROOM_ID NAME_FIRST_BYTE ...22 ... NAME_LAST_BYTE   0 */
  /* ROOM_ID ....... 
  X times repeated */
  if(nvmData != nullptr){
    /* validate data correctness */
    if(nvmData[0] == 0xAB){ /* Magic number written during successfull nvmWrite() */
      nvmMetadata.restoredSuccessfully = 1;
    }

    if(nvmMetadata.restoredSuccessfully){
      nvmMetadata.numberOfTranslations = nvmData[1];

      /* 25 bytes offset is needed to have right padding */
      uint16_t offset = 25;
      if(nvmMetadata.numberOfTranslations <= MAX_NUMBER_OF_ROOM_NAME_TRANSLATIONS){
        for(uint8_t translationIndex = 0; translationIndex < nvmMetadata.numberOfTranslations; translationIndex++){
          /* safe mech */
          if((offset + 25) > length){
            break;
          }

          uint8_t roomId = nvmData[offset]; /* First byte */
          uint8_t strArr[24] = {'\0'};
          memcpy(strArr, &nvmData[offset+1], 23); /* max length to ensure '\0' at the end */
          /* Construct room name string */
          String roomName = String((char*)strArr);

          
          roomNamesMapping.insert({roomId,roomName});
          

          offset += 25;
        }
      }else { 
        nvmMetadata.numberOfTranslations  = 0; 
      }

    }else{
      Serial.println("HttpServer://Failure during NVM data restore try");
    }
  }
}

bool HomeLightHttpServer::processLinkAsyncRequest(WiFiClient& client)
{
  /* Retrieve request from pattern : 'GET /request HTTP/1.1' */
  String linkRequest = header.substring(
    String("GET /").length(), 
    (header.length() - (String(" HTTP/1.1").length()+2) )
  );

  if(processParameterizedAsyncRequests(linkRequest, client)){
    return true;
  }

  return false;
}

void HomeLightHttpServer::processLinkRequestData(WiFiClient& client)
{
  /* Retrieve request from pattern : 'GET /request HTTP/1.1' */
  String linkRequest = header.substring(
    String("GET /").length(), 
    (header.length() - (String(" HTTP/1.1").length()+2) )
  );

  Serial.println("Request : " + linkRequest);

  /* If request is not known in list of constant commands */
  if(!processConstantRequests(linkRequest, client))
  {
    /* Process request in terms of parameterized command (e.g. brightness&VALUE )*/
    if(!processParameterizedRequests(linkRequest, client))
    {
      Serial.println("Invalid request received : " + linkRequest);
    }
  }
}

bool HomeLightHttpServer::loadConfigFromFile(JsonDocument& doc){
  roomNamesMapping.clear();

  for(uint8_t i = 0 ; i < 15; i++){
    String roomId = String(doc["RoomsSetup"][i]["id"]);
    String roomName = String(doc["RoomsSetup"][i]["name"]);

    if(roomId != "null" && roomName != "null"){
      if(roomId.toInt() < 255 && roomName.length() < 25){
        roomNamesMapping.insert({roomId.toInt(), roomName});
      }
    }else 
    {
      break;
    }
  }

  /* no error */
  return false;
}


String HomeLightHttpServer::getRoomsCfgJson()
{
  if(roomNamesMapping.size() > 0){
    String roomConfigJson = "\"RoomsSetup\":[";


    for(auto& room : roomNamesMapping){
      roomConfigJson += "{\"id\":\""+String(room.first)+"\", \"name\":\""+room.second+"\"},";
    }

    roomConfigJson += "]]";
    roomConfigJson.replace(",]", "");

    return roomConfigJson;
  }

  return "";
}

bool HomeLightHttpServer::processConstantRequests(const String& request, WiFiClient& client)
{
  bool retVal = false;
  /* Iterate through known requests (commands) array */
  for(uint8_t knownRequest = 0; knownRequest < constantRequests.size(); knownRequest ++)
  {
    /* Does received request match to current known request from table? */
    if(request == constantRequests[knownRequest]) {
      /* Check if there is known handler function for matched request */
      if(knownRequest < constantRequestHandlers.size())
      {
        /* Check if access level allows to enter the request */
        if(secAccessLevel >= constantRequestHandlers.at(knownRequest).second){
          /* Run function assigned to received command */
          constantRequestHandlers.at(knownRequest).first(client);
          retVal = true;
        }else 
        {
          /* Redirect client to main page as security access is too low */
          client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
        }
        break;
      }
    }
  }

  /* Return FALSE in case of unknown request received, otherwise TRUE */
  return retVal;
}

bool HomeLightHttpServer::processParameterizedRequests(String& request, WiFiClient& client)
{
  MatchState matcher;
  bool retVal = false;

  /* Set received request as target for search engine */
  matcher.Target(const_cast<char*>(request.c_str()));

  /* Iterate through known requests (commands) array */
  for(uint8_t knownRequest = 0; knownRequest < parameterizedRequests.size(); knownRequest ++)
  {
    /* Try to find currently inspected knownRequest in the request string */
    char matchReqult = matcher.Match(parameterizedRequests[knownRequest].c_str());

    /* Substring with known command found in received 'request' */
    if(matchReqult > 0)
    {
      /* Handler for received request is not missing */
      if(knownRequest < parameterizedRequestHandlers.size())
      {
        /* Check if access level allows to enter the request */
        if(secAccessLevel >= parameterizedRequestHandlers.at(knownRequest).second){
          parameterizedRequestHandlers.at(knownRequest).first(request, client);
          retVal = true;

          Serial.println("Request handler found at : " + String((int)knownRequest));
        }else 
        {
          /* Redirect client to main page as security access is too low */
          client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
        }
        break;
      }
    }
  }

  return retVal;
}

bool HomeLightHttpServer::processParameterizedAsyncRequests(String& request, WiFiClient& client)
{
  MatchState matcher;
  bool retVal = false;

  /* Set received request as target for search engine */
  matcher.Target(const_cast<char*>(request.c_str()));

  /* Iterate through known requests (commands) array */
  for(uint8_t knownRequest = 0; knownRequest < parameterizedAsyncRequests.size(); knownRequest ++)
  {
    /* Try to find currently inspected knownRequest in the request string */
    char matchReqult = matcher.Match(parameterizedAsyncRequests[knownRequest].c_str());

    /* Substring with known command found in received 'request' */
    if(matchReqult > 0)
    {
      /* Handler for received request is not missing */
      if(knownRequest < parameterizedAsyncRequestHandlers.size())
      {
        /* Check if access level allows to enter the request */
        if(secAccessLevel >= parameterizedAsyncRequestHandlers.at(knownRequest).second){
          //Serial.println("Running async function handler at index : " + String((int)knownRequest) + " for request " + request);
          parameterizedAsyncRequestHandlers.at(knownRequest).first(request, client);
          retVal = true;
        }
        break;
      }
    }
  }

  return retVal;
}


void HomeLightHttpServer::handleClientRequest()
{
  client = server.available(); 
  if (client) {                             // If a new client connects,
    /* Synchronous request can only be accepted when there is no async */
    if(HTTPAsyncRequestHandler::getProcessingState() == ASYNC_NO_REQUEST){
      currentTime = millis();
      previousTime = currentTime;
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
        currentTime = millis();
        if (client.available()) {             
          char requestLine = client.read();          
          //Serial.write("request " + requestLine);                   
          header += requestLine;
          if (requestLine == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              if(processLinkAsyncRequest(client)){
                /* Received async request, further processing of this function must be dropped */
                header = "";
                client.setTimeout(10);
                return;
              }

              // // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html; charset=UTF-8\r\n");
              // client.println("Connection: close");
              client.println();
              client.println("<!DOCTYPE html>");
              client.println("<head>");
              client.println(pageHead);
              client.println(style_css);
              client.println(tempGaugeCSS);
              client.println(renderRoomsJS);
              client.println(javascript);
              client.println(tempGaugeJS);
              client.println("</head>");

              // Web Page Heading
              client.println("<body><div class=\"wrapper\">");
              client.println(popupContent);

              /* Process request only when user interface is NOT blocked */
              Serial.println("Processing link...");
              processLinkRequestData(client);

              // TEMPORARY DISABLED DUE TO PERFORMANCE ISSUES
              // client.println("<script>\
              //   setInterval(getNotifications, 9325);\
              // </script>");

              client.println("<footer><p>&copy; 2025 Home Control Station · Jakub Becmer · <a href=\"https://github.com/kubabec/HomeControlStation\">GitHub</a> · version 1.0</p></footer>");

              client.println("</div>");
              client.println("</body></html>");            
              client.println();
              break;


            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          }
        }
      }
      // Clear the header variable
      header = "";

      // Close the connection
      client.stop();
    }
  }
}

void HomeLightHttpServer::onDeviceDescriptionChange(std::any newDescriptionVector) 
{
  //Wlasny Http Servra wektor urzadzen On Off 
  descriptionVector = (std::any_cast<std::vector<DeviceDescription>>(newDescriptionVector));

  deviceToRoomMappingList.clear();

  for(auto& device : descriptionVector){
    if(deviceToRoomMappingList.find(device.roomId) == deviceToRoomMappingList.end())
    {
      /* First device in this room */
      std::vector<DeviceDescription*> roomDevicesVector = {&device};
      deviceToRoomMappingList.insert({device.roomId, roomDevicesVector});
    }else 
    {
      /* Add to already existing record, as there already are other devices in this room */
      deviceToRoomMappingList.find(device.roomId)->second.push_back(&device);

    }
  }

}

void HomeLightHttpServer::generateAsyncPageContentJson(WiFiClient& client)
{
  client.println("{");
  // Serial.println("{");
  uint8_t roomIteratorCount = 1;
  for(auto& room : deviceToRoomMappingList)
  {
    uint8_t deviceIteratorCount = 1;

    if(roomNamesMapping.find(room.first) == roomNamesMapping.end()){
      client.println("\""+String((int)room.first)+"\": [");
      // Serial.println("\""+String((int)room.first)+"\": [");
    }else
    {
      client.println("\""+roomNamesMapping.find(room.first)->second+"\": [");
      // Serial.println("\""+roomNamesMapping.find(room.first)->second+"\": [");
    }
    for(auto& deviceInThisRoom : room.second){
      client.println("{");
      // Serial.println("{");
      client.println("\"id\":" + String((int)deviceInThisRoom->deviceId) + ",");
      // Serial.println("\"id\":" + String((int)deviceInThisRoom->deviceId) + ",");
      client.println("\"name\":\"" + deviceInThisRoom->deviceName + "\",");
      // Serial.println("\"name\":\"" + deviceInThisRoom->deviceName + "\",");
      if(deviceInThisRoom->isEnabled){
        client.println("\"status\":\"on\",");
        // Serial.println("\"status\":\"on\",");
      }else {
        client.println("\"status\":\"off\",");
        // Serial.println("\"status\":\"off\",");
      }
      client.println("\"hasBrightness\":" + String((int)deviceInThisRoom->customBytes[0]) + ",");
      client.println("\"brightness\":" + String((int)deviceInThisRoom->customBytes[1]));
      // Serial.println("\"brightness\":" + String((int)deviceInThisRoom->customBytes[1]));
      if(deviceIteratorCount < room.second.size()){
        client.println("},");
        // Serial.println("},");
      }else {
        client.println("}");
        // Serial.println("}");
      }
      deviceIteratorCount++;
    }
    
    if(roomIteratorCount < deviceToRoomMappingList.size()){
      client.println("],");
      // Serial.println("],");
    }else 
    {
      client.println("]");
      // Serial.println("]");
    }
    roomIteratorCount++;
  }
  client.println("}");
  // Serial.println("}");
}

void generateExtraFieldsForOnOff(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client)
{
  client.println("<div class=\"extra-fields extra-43\">");
  client.println("<label>Brightness support:");
  client.println("<select id=\"brightnessSupported-"+String((int)slotNumber)+"\">");
  if(slot.customBytes[0]){
    client.println("<option value=\"0\" >No</option>");
    client.println("<option value=\"1\" selected>Yes</option>");
  }else {
    client.println("<option value=\"0\" selected>No</option>");
    client.println("<option value=\"1\">Yes</option>");
  }
  client.println("</select>");

  client.println("</label>");


  client.println("<label>Activation state:");
  client.println("<select id=\"activationState-"+String((int)slotNumber)+"\">");
  if(slot.customBytes[1]){
    client.println("<option value=\"0\" >LOW</option>");
    client.println("<option value=\"1\" selected>HIGH</option>");
  }else {
    client.println("<option value=\"0\" selected>LOW</option>");
    client.println("<option value=\"1\">HIGH</option>");
  }
  client.println("</select>");

  client.println("</label>");
  client.println("<label>Min PWM:");

  client.println("<input id=\"pwmMin-"+String((int)slotNumber)+"\" type='range' min='0' max='255' step=\"1\" value=\""+String((int)slot.customBytes[2]) +"\" >");

  client.println("</label>");
 

  client.println("<label>Max PWM:");
  client.println("<input id=\"pwmMax-"+String((int)slotNumber)+"\" type='range' min='0' max='255' step=\"1\" value=\""+String((int)slot.customBytes[3]) +"\" >");
    
  client.println("</label>");
  client.println("</div>");
}
void generateExtraFieldsForLedStrip(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client)
{
  client.println("<div class=\"extra-fields extra-44\">");
  client.println("<label>LEDs num.:");
  int ledsCount = 0;
  memcpy(&ledsCount, &slot.customBytes[0], sizeof(uint16_t));
  client.println("<input id=\"ledsCount-"+String((int)slotNumber)+"\" type=\"text\" placeholder=\"35\" value=\""+ 
  String((int)ledsCount) +"\">");

  client.println("</label>");

  client.println("<label>Sides flip:");
  client.println("<select id=\"ledsSideFlip-"+String((int)slotNumber)+"\">");
  if(slot.customBytes[2]){
    client.println("<option value=\"0\" >Not inversed</option>");
    client.println("<option value=\"1\" selected>Inversed</option>");
  }else {
    client.println("<option value=\"0\" selected>Not inversed</option>");
    client.println("<option value=\"1\">Inversed</option>");
  }
  client.println("</select>");

  client.println("</label>");


  client.println("<label>Current limiter</label>");
  int percentValue = (int)((float)slot.customBytes[3] / (float)2.55); // Convert to percent value
  Serial.println("Current limiter value: " + String((int)slot.customBytes[3]) + " -> " + String(percentValue) + "%");
  client.println("<label><input disabled type=\"text\" style=\"width:40px;\"  id=\"curLimVal-"+String((int)slotNumber)+"\" value=\""+String((int)percentValue)+"\">% ");
  client.println("<input id=\"curLim-"+String((int)slotNumber)+"\" type='range' min='15' max='255' value=\""+String((int)slot.customBytes[3]) +"\" onchange=\"updateCurLimVal('curLimVal-"+String((int)slotNumber)+"',this.value);\">");
  //client.println(String((int)slot.customBytes[3]) +"' onchange=\"updateCurLimVal('curLimVal-"+String((int)slotNumber)+"',this.value);\">");
  client.println("</label>");

  client.println("</div>");
}
void generateExtraFieldsForSegmentedLedStrip(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client)
{
  const String option1 = "<option value=\"0\" >Not inversed</option>";
  const String option2 = "<option value=\"1\" selected>Inversed</option>";
  const String option3 = "<option value=\"0\" selected>Not inversed</option>";
  const String option4 = "<option value=\"1\">Inversed</option>";



  client.println("<div class=\"extra-fields extra-46\">");

  for(uint8_t segmentIndex = 0; segmentIndex < 5; segmentIndex++)
  {
    client.println("<label>");
    client.println("<input id=\"seg"+String((int)(segmentIndex+1))+"Count-"+String((int)slotNumber)+"\" type=\"text\" placeholder=\"Diodes count (seg. "+String((int)(segmentIndex+1))+")\"");
    if(slot.customBytes[5 + segmentIndex] > 0){
      client.println(" value=\""+ String((int)slot.customBytes[5 + segmentIndex]) +"\">");
    }else {
      client.println(">");
    }
    client.println(" <select id=\"Seg"+String((int)(segmentIndex +1))+"Flip-"+String((int)slotNumber)+"\">");
    if(slot.customBytes[10 + segmentIndex]){
      client.println(option1);
      client.println(option2);
    }else {
      client.println(option3);
      client.println(option4);
    }
      
    client.println("</select></label>");
  }

  client.println("<label>Current limiter</label>");
  int percentValue = (int)((float)slot.customBytes[3] / (float)2.55); // Convert to percent value
  client.println("<label><input disabled type=\"text\" style=\"width:40px;\"  id=\"SegcurLimVal-"+String((int)slotNumber)+"\" value=\""+String((int)percentValue)+"\">% ");
  client.println("<input id=\"SegcurLim-"+String((int)slotNumber)+"\" type='range' min='15' max='255' value=\""+String((int)slot.customBytes[3]) +"\" onchange=\"updateCurLimVal('SegcurLimVal-"+String((int)slotNumber)+"',this.value);\">");
  

  client.println("</div>"); 
}

void generateExtraFieldsForDistanceSensor(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client)
{
  client.println("<div class=\"extra-fields extra-47\">");
  client.println("<label>Pin Tx:");
  client.println("<select id=\"pinTx-"+String((int)slotNumber)+"\" >");
  for (int i = 0; i < 32; i++) {
    String pinStr = "";
    if(slot.customBytes[4] == i)
    {
      client.println("<option value=\"" + String(i) + "\" selected>" + String(i) + "</option>");
    } else {
      client.println("<option value=\"" + String(i) + "\">" + String(i) + "</option>");
    }
  }
  client.println("</select>");
  client.println("</label>");
  //Pole wyboru dla pinu Rx
  client.println("<label>Pin Rx:");
  client.println("<select id=\"pinRx-"+String((int)slotNumber)+"\" >");
  for (int i = 0; i < 32; i++) {
    String pinStr = "";
    if(slot.customBytes[5] == i)
    {
      client.println("<option value=\"" + String(i) + "\" selected>" + String(i) + "</option>");
    } else {
      client.println("<option value=\"" + String(i) + "\">" + String(i) + "</option>");
    }
  }
  client.println("</select>");
  client.println("</label>");
  client.println("</div>"); 
}

void HomeLightHttpServer::generateConfigSlotUi(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client)
{
  client.println("<div class=\"device-container\" id=\"device-"+String((int)slotNumber)+"\">");
  
  if(slot.isActive)
  {
    /* Slot is active */
    client.println(labelStart);
    client.println("<input id=\"enabled"+String((int)slotNumber)+"\" type=\"checkbox\" checked> Memory slot " +String((int)slotNumber) + "<span class=\"status-text\">Enabled</span>");
    client.println(labelEnd);

  }else
  {
    /* Slot is inactive */
    client.println(labelStart);
    client.println("<input id=\"enabled"+String((int)slotNumber)+"\" type=\"checkbox\"> Memory slot " +String((int)slotNumber) + "<span class=\"status-text\">Enabled</span>");
    client.println(labelEnd);
  }

  /* intentionally hidden to prevent user from using IDs other than 0 - 5 */
  client.println("<input type=\"text\" style=\"visibility:hidden;display:none;\" id=\"identifier"+String(slotNumber)+"\"\
    value=\""+ String((int)slotNumber) + "\">");

  client.println(labelStart);
  client.println("Name:<input type=\"text\" maxlength=\"24\" id=\"name"+String(slotNumber)+"\"\
  value=\""+ String(slot.deviceName) +"\">");
  client.println(labelEnd);

  client.println(labelStart);
  client.println("Type: <select onchange=\"showExtraFields(this, 'device-"+String((int)slotNumber)+"');\" id=\"type"+String(slotNumber)+"\" >");

  if(slot.deviceType == type_ONOFFDEVICE){
    client.println("<option value=\"43\" selected>On/Off Device</option>");
  }else
  {
    client.println("<option value=\"43\">On/Off Device</option>");
  }
  
  if(slot.deviceType == type_LED_STRIP){
    client.println("<option value=\"44\" selected>LED strip</option>");
  }else
  {
    client.println("<option value=\"44\">LED strip</option>");
  }

  if(slot.deviceType == type_TEMP_SENSOR){
    client.println("<option value=\"45\" selected>Temperature sensor</option>");
  }else
  {
    client.println("<option value=\"45\">Temperature sensor</option>");
  }


  if(slot.deviceType == type_DISTANCE_SENSOR){
    client.println("<option value=\"47\" selected>Distance sensor</option>");
  }else
  {
    client.println("<option value=\"47\">Distance sensor</option>");
  }

  if(slot.deviceType == type_LED_STRIP_SEGMENTED){
    client.println("<option value=\"46\" selected>Segmented LED strip</option>");
  }else
  {
    client.println("<option value=\"46\">Segmented LED strip</option>");

  }

  if(
    slot.deviceType != type_ONOFFDEVICE &&
    slot.deviceType != type_LED_STRIP && 
    slot.deviceType != type_TEMP_SENSOR &&
    slot.deviceType != type_DISTANCE_SENSOR &&
    slot.deviceType != type_LED_STRIP_SEGMENTED){

    client.println("<option value=\"255\" selected>UNKNOWN</option>");
  }else {
    client.println("<option value=\"255\">UNKNOWN</option>");
  }
  client.println("</select>");

  client.println(labelEnd);

  client.println(labelStart);
  client.println("Pin:<select type=\"text\" id=\"pin"+String(slotNumber)+"\"\
  value=\""+ String((int)slot.pinNumber) +"\">");
  const std::array<int, 34> pinsAllowed = {1,2,3,4,5,6,7,8,9,10,12,13,14,22,23,24,27,26,25,33,32,35,34,15,18,19,21,39,40,41,42, 47, 48};

  for(auto& val : pinsAllowed){
    String pinStr = "";
    if(slot.pinNumber != val)
    {
      pinStr = "<option value=\"" + String((int)val) + "\">" + String((int)val) + "</option>";
    }
    else
    {
      pinStr = "<option value=\"" + String((int)val) + "\" selected>" + String((int)val) + "</option>";
    }
      client.println(pinStr);
  }
  client.println("</select>");
  client.println(labelEnd);

  client.println(labelStart);
  client.println("Room ID:<input type=\"text\" id=\"room"+String(slotNumber)+"\"\
  value=\""+ String((int)slot.roomId) +"\">");
  client.println(labelEnd);

  /*<!-- Extra fields for ON/OFF -->*/

  generateExtraFieldsForOnOff(slotNumber, slot, client);


  /*<!-- Extra fields for Distance Sensor -->*/
  generateExtraFieldsForDistanceSensor(slotNumber, slot, client);

  /*<!-- Extra fields for LED Strip -->*/
  generateExtraFieldsForLedStrip(slotNumber, slot, client);

  /*<!-- Extra fields for Temperature sensor -->*/
  // No extra fields for temperature sensor

  /*<!-- Extra fields for Segmented LED Strip -->*/
  generateExtraFieldsForSegmentedLedStrip(slotNumber, slot, client);



  client.println("</div>");

}

/* ========= CONFIG ==============*/

void HomeLightHttpServer::printConfigPage(WiFiClient& client)
{
  NodeConfiguration currentConfig = 
    std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));


  const String yesNotSelected = "<option value=\"yes\">Yes</option>";
  const String noNotSelected = "<option value=\"no\">No</option>";
  const String yesSelected = "<option selected=\"selected\" value=\"yes\">Yes</option>";
  const String noSelected = "<option selected=\"selected\" value=\"no\">No</option>";

  client.println("<div class=\"header\">General configuration</div>");

  try {
      auto getTimeCallback = std::any_cast<std::function<RtcTime()>>(DataContainer::getSignalValue(CBK_GET_CURRENT_TIME));        
      String currentTime = getTimeCallback().toString(); // Get the current time as a string

    client.printf("<div class=\"current-time\">System time: <span id=\"currentDateTime\">%s</span></div>\n", currentTime.c_str());

    // skrypt inicjalizujący
    client.println("<script>");
    client.println("document.addEventListener('DOMContentLoaded', function() {");
    // Przekazanie początkowego czasu z serwera jako argument
    client.println("  const initialTime = '" + currentTime + "';");
    // Wywołanie funkcji aktualizującej zegar
    client.println("  updateLocalDateTime(initialTime, 'currentDateTime');");
    client.println("});");
    client.println("</script>");
  } 
  catch(const std::bad_any_cast& e) {
    Serial.printf("Błąd typu: %s. Oczekiwano: std::string\n", e.what());
    client.println("<div class=\"error\">Błąd odczytu czasu</div>");
  }

  

  client.println("\
    <div class=\"container\">\
    <form onsubmit=\"return false;\">");
  
  /* exclusion for ServiceMode Access level only */
  if(secAccessLevel < e_ACCESS_LEVEL_SERVICE_MODE){
    client.println("<div class=\"access-level-hidder\" >");
  }
  /* intended TAB */
    /* Is Http Server configuration */

    /* Temporary disabled as there is no planned feature to disable HTTP server */
    // client.println("<label>HTTP Server:<select name=\"isHTTPServer\">");
    // if(currentConfig.isHttpServer)
    // {
    //   client.println(yesSelected);
    //   client.println(noNotSelected);
    // }else
    // {
    //   client.println(yesNotSelected);
    //   client.println(noSelected);
    // }
    // client.println("</select></label>");

    /* Is RC Server configuration */
    client.println("<label>Is Master:<select name=\"isRCServer\">");
    if(currentConfig.isRcServer)
    {
      client.println(yesSelected);
      client.println(noNotSelected);
    }else
    {
      client.println(yesNotSelected);
      client.println(noSelected);
    }
    client.println("</select></label>");

    /* Does user have admin rights to make configuration changes */
    client.println("<label>User is admin:<select name=\"isUserAsAdmin\">");
    if(currentConfig.isDefaultUserAdmin)
    {
      client.println(yesSelected);
      client.println(noNotSelected);
    }else
    {
      client.println(yesNotSelected);
      client.println(noSelected);
    }
    client.println("</select></label>");
    

    /* Device type  */
    const std::map<uint8_t, String> typeToNameMappings = {
      {9, "HomeControlStation v1 (StirTech)"},
      {1, "ESP32 devkit v1"},
      {2, "ESP32 S3"},
      {255, "UNDEFINED"}
    };
    client.println("<label>Type:<select name=\"nodetype\">");
    for(auto& type : typeToNameMappings){
      if(currentConfig.nodeType == type.first){
        client.println("<option selected value=\""+String((int)type.first)+"\">"+type.second+"</option>");
      }else {
        client.println("<option value=\""+String((int)type.first)+"\">"+type.second+"</option>");
      }
    }
    client.println("</select></label>");

  if(secAccessLevel < e_ACCESS_LEVEL_SERVICE_MODE){
    client.println("</div>");
  }

  /* Network SSID */
  client.println("<label>SSID:<input value=\"");
  client.println(currentConfig.networkSSID);
  client.println("\" type=\"text\" name=\"SSID\"></label>");

  /* Network Password */
  client.println("<label>WiFi Password:<input value=\"");
  client.println(currentConfig.networkPassword);
  client.println("\" type=\"text\" name=\"Password\"></label>");

  /* Network Password */
  client.println("<label>User Password:<input value=\"");
  client.println(currentConfig.panelPassword);
  client.println("\" type=\"text\" name=\"UserPassword\"></label>");

  /* Extended memory usage */
  ExtendedMemoryCtrlAPI extMemoryFunctions = 
        std::any_cast<ExtendedMemoryCtrlAPI>(DataContainer::getSignalValue(SIG_EXT_MEM_CTRL_API));
  // client.println("<label>Memory usage:<input disabled value=\"");
  // client.println(String((int)extMemoryFunctions.getCurrentMemoryUsage()) + " / 2 500 bytes");
  // client.println("\" type=\"text\" \"></label>");

  int memoryUsagePercent = (int)((float)extMemoryFunctions.getCurrentMemoryUsage() / 2000.f * 100.f);

  client.println("<label>Memory usage: <div class=\"memory-bar\" id=\"memoryBar\">");
  client.println("<div class=\"memory-bar-fill\" id=\"memoryBarFill\"></div>");
  client.println("</div></label>");
  client.println("<script>\
    function setMemoryBarValue(value) {\
      const bar = document.getElementById('memoryBarFill');\
      const clampedValue = Math.min(100, Math.max(0, value));\
      bar.style.width = clampedValue + '%';\
    }\
    setMemoryBarValue("+String((int)memoryUsagePercent)+");\
  </script>");

  /* Time when system was launched */

  RtcTime time = std::any_cast<RtcTime>(DataContainer::getSignalValue(SIG_STARTUP_TIME));
  client.println("<label>Launch time:<input value=\"");
  client.println(time.toString());
  client.println("\" type=\"text\" disabled></label>");

  /* Apply button*/
  client.println("<div class=\"error-button\" onclick=\"showMessage('Sure you wanna change Node settings? Device will be restarted afterwards.', applySettings)\">Apply</div><hr class=\"custom-hr\">");  
  

  /* display room settings only if there are devices already configured */
  std::vector<DeviceDescription>  deviceCollection = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));

  if(deviceCollection.size() > 0){
    /* Room mapping button */
    client.println("<div class=\"button-link\" onclick=\"goToRoomSettings()\">Room settings</div>");
  }

  if(currentConfig.isRcServer){
    client.println("<div class=\"button-link\" onclick=\"goToNetIns()\">Network inspection</div>");
  }
    
  /* Devices setup button */
  if(secAccessLevel >= e_ACCESS_LEVEL_SERVICE_MODE){
    client.println("<div class=\"button-link\" onclick=\"goToDevicesManagement()\">Devices management</div>");
    client.println("<hr class=\"custom-hr\">");

    /* Temporary disabled */
    // client.println("<div class=\"button-link\" onclick=\"downloadDeviceConfiguration()\">Download config file</div>");
    // client.println("<div class=\"button-link\" onclick=\"uploadConfigFile()\">Load configuration file</div>");
    // client.println("<hr class=\"custom-hr\">");

  /* Reboot button*/
  client.println("<div class=\"error-button\" style=\"background-color: yellow;\" onclick=\"showMessage('You really want to reboot this device?', resetDevice)\">Restart</div>");
  
    /* Clear all settings */
    client.println("<div class=\"error-button\" onclick=\"showMessage('Do you really wanna clear all node settings? WiFi configuration will also be cleared. Device will not restart automatically, you must reset device on your own when this option is selected!', massErase)\">Restore default</div>");
  }

  /* Common HTML tags closure */
  client.println("</form></div>");

  /* Display return button */
  client.println("<br><a href=\"http://"+ipAddressString+"\" class=\"button\">Home page</a><br>");

}

void HomeLightHttpServer::printSlotsConfigPage(WiFiClient& client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">Node Local Configuration</div>");


  ConfigSlotsDataType cfgSlots = std::any_cast<ConfigSlotsDataType>(DataContainer::getSignalValue(SIG_CONFIG_SLOTS));
  // ConfigSlotsDataType cfgSlots;
  // Serial.println("HTTPServer//:Printing config slots:");
  // for(auto& slot: cfgSlots.slots){
  //   slot.print();
  // }

  uint8_t slotIdx = 1;
  for(auto& slot : cfgSlots.slots)
  {
    generateConfigSlotUi(slotIdx, slot, client);
    slotIdx++;
  }

  client.println("<button class=\"button\" id=\"confirmationButton\" onclick=\"showMessage('Do you wanna change node devices configuration?\
   GPIO pin setup will be loaded according to options selected on this page.', createConfigurationStringJson);\">Save config</button>");
  client.println("<a href=\"/config\" class=\"button\">Config Page</a><br>");


  client.println("</div>");
}


/*** CONSTANT HANDLERS */
/** parameterized requests */ 


void HomeLightHttpServer::pending(WiFiClient& client){
  client.println("Loading, please wait . . . ");
  client.println("<meta http-equiv='refresh' content='1; url=http://"+ ipAddressString +"'>");
}

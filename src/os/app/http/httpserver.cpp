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
std::array<SystemErrorType, ERR_MONT_ERROR_COUNT> HomeLightHttpServer::systemErrorList;
uint8_t HomeLightHttpServer::activeErrorsCount = 0;
String HomeLightHttpServer::ipAddressString;


/*TESTBLOCK */
const uint8_t ledsCount = 50;
typedef struct{
  uint8_t status = 0;
  uint8_t red[ledsCount] = {0};
  uint8_t green[ledsCount] = {0};
  uint8_t blue[ledsCount] = {0};
}LedStripTestType;

LedStripTestType leds;

/*TESTBLOCK */

const char* labelStart = "<label>";
const char* labelEnd = "</label>";
// ERR_MON_UNEXPECTED_RESET = 1,
// ERR_MON_INVALID_NVM_DATA,
// ERR_MON_INVALID_LOCAL_CONFIG,
// ERR_MON_WRONG_CONFIG_STRING_RECEIVED,
// ERR_MON_WRONG_LOCAL_DEVICES_CONFIG_RECEIVED,
// ERR_MON_INVALID_ERROR_REPORTED,

std::vector<String> constantRequests = {
  "", /* Main page with no parameter */
  "config",
  "resetDevice",
  "errclrbtn",
  "localDevices",
  "roomAssignment",
  "masseraseviahttp",
  "asyncRequestTest"
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
};


std::vector<std::pair<std::function<void(WiFiClient&)>, SecurityAccessLevelType>> constantRequestHandlers = {
  {HomeLightHttpServer::constantHandler_mainPage, e_ACCESS_LEVEL_NONE},
  {HomeLightHttpServer::constantHandler_configPage, e_ACCESS_LEVEL_AUTH_USER},
  {HomeLightHttpServer::constantHandler_resetDevice, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::constantHandler_clearErrors, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::constantHandler_devicesSetup, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::constantHandler_roomAssignment, e_ACCESS_LEVEL_AUTH_USER},
  {HomeLightHttpServer::constantHandler_massErase, e_ACCESS_LEVEL_SERVICE_MODE},
  {HomeLightHttpServer::constantHandler_asyncTest, e_ACCESS_LEVEL_NONE},
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
  {HomeLightHttpServer::parameterizedHandler_stripSaveCurrent, e_ACCESS_LEVEL_NONE}
};

void escapeSpecialCharsInJson(String& json)
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

void HomeLightHttpServer::deinit() {

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

  DataContainer::subscribe(SIG_SYSTEM_ERROR_LIST, [](std::any signal) {
    systemErrorList = (std::any_cast<std::array<SystemErrorType, ERR_MONT_ERROR_COUNT>>(signal));
    activeErrorsCount = 0;    

    /* Count errors with occurrence > 0 */
    for(auto& error : systemErrorList)
    {
      if(error.occurrenceCount > 0)
      {
        activeErrorsCount ++;
      }
    }
  }
  );

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


  /* Explicit read of system error needed due to init order ->  ErrorMon -> ConfigProvider -> HttpServer */
  std::any errorsAsAny = DataContainer::getSignalValue(SIG_SYSTEM_ERROR_LIST);
  try {
    systemErrorList = std::any_cast<std::array<SystemErrorType, ERR_MONT_ERROR_COUNT>>(errorsAsAny);
    activeErrorsCount = 0;
    /* Count errors with occurrence > 0 */
    for(auto& error : systemErrorList)
    {
      if(error.occurrenceCount > 0)
      {
        activeErrorsCount ++;
      }
    }
  }catch (std::bad_any_cast ex)
  {
    Serial.println("Unable to access SYSTEM ERRORS");
  }


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

void HomeLightHttpServer::requestErrorList()
{
  
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
              client.println("Content-type:text/html");
              client.println("Connection: close");
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
            

              client.println("<div class=\"project-name\">Home Control Station<br>v1.0</div>");
              client.println("<script>\
                setInterval(getNotifications, 10000);\
              </script>");

              client.println("</div></body></html>");            
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

//funkcja rysujaca UI do sterowania dla 1 urzadzenia onoff
void HomeLightHttpServer::generateOnOffUi(DeviceDescription& description, WiFiClient& client) {
  client.println("<div class=\"container\" id=\"container"+String(description.deviceId)+"\">"); // container
  client.println("<div class=\"loading-overlay\" style=\"display: none;\">\
            <div class=\"spinner\"></div>\
            <div class=\"loading-text\">Loading...</div>\
        </div>");

  if(!description.isEnabled) {   
    client.println("<div class=\"header\">" + description.deviceName + "</div><div id=\"statusLight"+String(description.deviceId)+"\" class=\"status-light off\"></div>"); 
  }
  else {
    client.println("<div class=\"header\">" + description.deviceName + "</div><div id=\"statusLight"+String(description.deviceId)+"\" class=\"status-light on\"></div>"); 
  }

  /* Draw ON/OFF button depending on the current state */
  if(!description.isEnabled) {   
    client.println("<a class=\"button\" id=\"switchBtn"+String(description.deviceId)+"\" onclick=\"asyncDeviceStateSwitch("+String(description.deviceId)+", 1)\">ON</a>");
  }
  else {
    client.println("<a class=\"button\" id=\"switchBtn"+String(description.deviceId)+"\" onclick=\"asyncDeviceStateSwitch("+String(description.deviceId)+", 0)\">OFF</a>");
  }

  /* Draw Brightness range bar if device allows brightness change */
  /* is brigthness adjustable ? */
  if(description.customBytes[0]) {
    client.println("<br>");
    const String brightnessSlider1  = "<div class=\"header2\">Brightness</div><input type='range' min='0' max='100' value='";
    const String brightnessSlider2 = "' onchange=\"onRangeChanged(this.value, " + String(description.deviceId) + ")\" id=\"brightnessSlider"+String(description.deviceId)+"\">";
    client.println(brightnessSlider1 + String(description.customBytes[1]) + brightnessSlider2);
  }
  client.println("</div>"); // container 
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

  if(
    slot.deviceType != type_ONOFFDEVICE &&
    slot.deviceType != type_LED_STRIP && 
    slot.deviceType != type_TEMP_SENSOR){
    client.println("<option value=\"255\" selected>UNKNOWN</option>");
  }else {
    client.println("<option value=\"255\">UNKNOWN</option>");
  }
  client.println("</select>");

  client.println(labelEnd);

  client.println(labelStart);
  client.println("Pin:<select type=\"text\" id=\"pin"+String(slotNumber)+"\"\
  value=\""+ String((int)slot.pinNumber) +"\">");
  const std::array<int, 12> pinsAllowed = {1,2,3,4,5,6,7,8,9,10,41,42};

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
  /* some more extra fields for 43*/

  client.println("</div>");

  /*<!-- Extra fields for LED Strip -->*/
  client.println("<div class=\"extra-fields extra-44\">");
  client.println("<label>LEDs num.:");
  client.println("<input id=\"ledsCount-"+String((int)slotNumber)+"\" type=\"text\" placeholder=\"35\" value=\""+ 
  String((int)slot.customBytes[0]) +"\">");

  client.println("</label>");

  client.println("<label>Sides flip:");
  client.println("<select id=\"ledsSideFlip-"+String((int)slotNumber)+"\">");
  if(slot.customBytes[1]){
    client.println("<option value=\"0\" >No flip</option>");
    client.println("<option value=\"1\" selected>Last diode is strip begin</option>");
  }else {
    client.println("<option value=\"0\" selected>No flip</option>");
    client.println("<option value=\"1\">Last diode is strip begin</option>");
  }
  client.println("</select>");

  client.println("</label>");

  client.println("</div>");


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
  //client.println("<div class=\"current-time\">Aktualny czas: " + timeMaster.getFormattedTime() + "</div>");

  client.println("\
    <div class=\"container\">\
    <form onsubmit=\"return false;\">");
  
  /* exclusion for ServiceMode Access level only */
  if(secAccessLevel < e_ACCESS_LEVEL_SERVICE_MODE){
    client.println("<div class=\"access-level-hidder\" >");
  }
  /* intended TAB */
    /* Is Http Server configuration */
    client.println("<label>HTTP Server:<select name=\"isHTTPServer\">");
    if(currentConfig.isHttpServer)
    {
      client.println(yesSelected);
      client.println(noNotSelected);
    }else
    {
      client.println(yesNotSelected);
      client.println(noSelected);
    }
    client.println("</select></label>");

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
  client.println("<label>Memory usage:<input disabled value=\"");
  client.println(String((int)extMemoryFunctions.getCurrentMemoryUsage()) + " / 2 500 bytes");
  client.println("\" type=\"text\" \"></label>");

  /* Apply button*/
  client.println("<div class=\"error-button\" onclick=\"showMessage('Sure you wanna change Node settings? Device will be restarted afterwards.', applySettings)\">Apply</div><hr class=\"custom-hr\">");  
  

  /* display room settings only if there are devices already configured */
  std::vector<DeviceDescription>  deviceCollection = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));

  if(deviceCollection.size() > 0){
    /* Room mapping button */
    client.println("<div class=\"button-link\" onclick=\"goToRoomSettings()\">Room settings</div>");
  }
    
  /* Devices setup button */
  if(secAccessLevel >= e_ACCESS_LEVEL_SERVICE_MODE){
    client.println("<div class=\"button-link\" onclick=\"goToDevicesManagement()\">Devices management</div>");
    client.println("<hr class=\"custom-hr\">");
    client.println("<div class=\"button-link\" onclick=\"downloadDeviceConfiguration()\">Download config file</div>");
    client.println("<div class=\"button-link\" onclick=\"uploadConfigFile()\">Load configuration file</div>");
    client.println("<hr class=\"custom-hr\">");

  /* Reboot button*/
  client.println("<div class=\"error-button\" style=\"background-color: yellow;\" onclick=\"showMessage('You really want to reboot this device?', resetDevice)\">Restart</div>");
  
    /* Clear all settings */
    client.println("<div class=\"error-button\" onclick=\"showMessage('Do you really wanna clear all node settings? WiFi configuration will also be cleared. Device will not restart automatically, you must reset device on your own when this option is selected!', massErase)\">Restore default</div>");
  }

  /* Common HTML tags closure */
  client.println("</form></div>");

  printErrorTable(client);

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


void HomeLightHttpServer::printErrorTable(WiFiClient& client)
{
  if(activeErrorsCount > 0){
    client.println("<div class=\"error-table-container\"> <div class=\"error-header\">Error Log</div> <table class=\"error-table\">");
    client.println("<thead>\
                      <tr>\
                          <th>Code</th>\
                          <th>Count</th>\
                          <th>Comment</th>\
                          <th>Time</th>\
                      </tr>\
                  </thead><tbody>");
        

    for(uint i = 0 ; i < ERR_MONT_ERROR_COUNT; i++)
    {
      if(systemErrorList.at(i).occurrenceCount > 0) {
        client.println("<tr>\
                          <td>ERR-"+ String(i+1) +"</td>\
                          <td>"+ String((int)systemErrorList.at(i).occurrenceCount) +"</td>\
                          <td>"+ systemErrorList.at(i).comment +"</td>\
                          <td>" + systemErrorList.at(i).timeOfOccurrence + "</td>\
                      </tr>");

      }
    }
    client.println("</tbody></table>");

    /* Display error clear button */
    const char* errorClearBtn = "\
    <button class=\"error-button\" \
    onclick=\"showMessage('Wanna clear device errors?', '/errclrbtn')\">Clear errors</button>";

    if(secAccessLevel < e_ACCESS_LEVEL_SERVICE_MODE){
      client.println("<div class=\"access-level-hidder\">");
    }

    client.println(errorClearBtn);
    if(secAccessLevel < e_ACCESS_LEVEL_SERVICE_MODE){
      client.println("</div>");
    }

    client.println("</div>");
  }
}

/*** CONSTANT HANDLERS */

void printTestLedStrip(WiFiClient& client)
{
  client.println("<div class=\"popup-backdrop\"></div>\
    <div class=\"color-picker-popup\" id=\"colorPickerPopup\">\
        <div class=\"header2\">Set Diode Color</div>\
        <input type=\"color\" id=\"colorInput\" class=\"color-input\" />\
        <div class=\"popup-buttons\">\
            <button id=\"confirmColorButton\">Confirm</button>\
            <button id=\"cancelColorButton\">Cancel</button>\
        </div>\
    </div>");

  
  

  client.println("<div class=\"popup-backdrop\"></div>\
    <div class=\"color-picker-popup\" id=\"SaveFavouritesPopup\">\
        <div class=\"header2\">Save composition</div>\
          <button class=\"button\" id=\"compos1\">Slot 1</button>\
          <button class=\"button\" id=\"compos2\">Slot 2</button>\
          <button class=\"button\" id=\"compos3\">Slot 3</button>\
          <br>\
          <button class=\"button\" id=\"composSaveClose\">Close</button>\
    </div>");


  // const String brightnessSlider1  = "<div class=\"header2\">Brightness</div><input type='range' min='0' max='100' value='";
  // const String brightnessSlider2 = "' onchange=\"onRangeChanged(this.value, 6)\">";
  

  // client.println("<div class=\"container\">"); 
  // client.println("<div class=\"header\">TEST_LED_ON</div><div class=\"status-light on\"></div>");
  // client.println("\
  //               <div class=\"color-picker\"> \
  //                   <div class=\"color-display on\" id=\"colorDisplay1\" style=\"background-color:#f3bb11;\"></div> \
  //                   <input type=\"color\" id=\"colorInput1\" class=\"color-input\" value=\"#f3bb11\"> \
  //               </div>\
  //               <button class=\"button\" onclick=\"sendColor(1)\">Set Color</button>");
  // client.println("<a class=\"button\" href=\"/test\">OFF</a>\
  //               ");
  // client.println(brightnessSlider1 + 40 + brightnessSlider2);

  // client.println("</div>");

  String firstLedRed = leds.red[0] < 10 ? '0' + String((int)leds.red[0], HEX) : String((int)leds.red[0], HEX);
  String firstLedGreen = leds.green[0] < 10 ? '0' + String((int)leds.green[0], HEX) : String((int)leds.green[0], HEX);
  String firstLedBlue = leds.blue[0] < 10 ? '0' + String((int)leds.blue[0], HEX) : String((int)leds.green[0], HEX);


  client.println("<div class=\"container\">"); 
  client.println("<div class=\"header\">TEST_LED_OFF</div><div class=\"status-light off\"></div>");
  client.println("<div id=\"ledStrip3\" class=\"led-strip\" style=\"width:300px;\">");
  for(uint8_t i = 0; i < ledsCount; i++){
    String enableStatus = (leds.red[i] > 0 || leds.green[i] > 0 || leds.blue[i] > 0) ? "on" : "off";

    client.println("<div class=\"led "+enableStatus+"\" onClick=\"colorClickedAction("+String((int)i)+")\" style=\"width:6px;");
    if(enableStatus == "on"){
      client.println("background-color: rgb(\
      "+String((int)leds.red[i])+", \
      "+String((int)leds.green[i])+", \
      "+String((int)leds.blue[i])+");");
    }
    client.println("\"> </div>");
  }
  client.println("</div>");
  client.println("\
                <div class=\"color-picker\"> \
                    <input type=\"color\" id=\"colorInput2\" class=\"color-input\" value=\"#\
"+firstLedRed+"\
"+firstLedGreen+"\
"+firstLedBlue+"\"> \
                </div>\
                <button class=\"button\" onclick=\"sendColor(2)\">Set Color</button>\
                <button class=\"button\" onclick=\"openLedStripMemorySlots()\">Compositions</button>\
                <button class=\"button\" onclick=\"openSaveCompositions()\">Save composition</button>");
  client.println("<a class=\"button\" href=\"/test\">ON</a>\
                ");

  client.println("</div>");


  // client.println("<div class=\"container\">"); 
  // client.println("<div class=\"header\">TEST_LED_ON</div><div class=\"status-light on\"></div>");
  // // client.println("\
  // //               <div class=\"color-picker\"> \
  // //                   <div class=\"color-display on\" id=\"colorDisplay1\" style=\"background-color:#f3bb11;\"></div> \
  // //                   <input type=\"color\" id=\"colorInput1\" class=\"color-input\" value=\"#f3bb11\"> \
  // //               </div>");

  // client.println("\
  //               <div class=\"color-picker\"> \
  //                   <div class=\"color-display on\" id=\"colorDisplay1\" style=\"background-color:#f3bb11;\"></div> \
  //               </div>");
  // client.println("<div class=\"button-container\">\
  //                 <button class=\"button\" href=\"/test\">. . . </button>\
  //                 <button class=\"button\" href=\"/test\">OFF</button>\
  //                 </div>");
  // //client.println("<button class=\"button\" onclick=\"sendColor(1)\">Set Color</button>");

  // client.println(brightnessSlider1 + 40 + brightnessSlider2);
  // client.println("</div>");


  // client.println("<div class=\"container\">"); 
  // client.println("<div class=\"header\">TemperatureSensor</div><div class=\"status-light on\"></div>");
  // client.println("<div class=\"temperature-container\">\
  //       <div id=\"gauge2\" class=\"temperature-widget\">\
  //           <canvas style=\"max-width: 100px;\"></canvas>\
  //           <div class=\"temperature-value\">20°C</div>\
  //       </div>\
  //       <div id=\"humidity\" class=\"humidity-widget\">\
  //           <canvas></canvas>\
  //           <div class=\"value-display humidity-value\">50%</div>\
  //       </div>\
  //   </div>");

  // client.println("<div class=\"button-container\"><button class=\"button\" href=\"/test\">. . . </button></div>");


  // client.println("<script>\
  //    window.addEventListener('DOMContentLoaded', () => {\
  //           createGauge('gauge2');\
  //           setTemperature('gauge2', 0);\
  //           createHumidGauge('humidity');\
  //           setHumidity('humidity', 20);\
  //       });\
  //   </script>");

  // client.println("</div>");

  

}

void HomeLightHttpServer::constantHandler_mainPage(WiFiClient& client)
{
  // for(auto& room : deviceToRoomMappingList){
  //   if(roomNamesMapping.find(room.first) == roomNamesMapping.end()){
  //     client.println("<div class=\"room-container\"><div class=\"room-header\">Room ID: " + String((int)room.first) + "</div>");
  //   }else
  //   {
  //     client.println("<div class=\"room-container\"><div class=\"room-header\">"+ roomNamesMapping.find(room.first)->second + "</div>");
  //   }
  //   /* Generate UI for every available device in this room */
  //   for(auto& device : room.second)
  //   {

  //     if(device->deviceType == type_ONOFFDEVICE){
  //       generateOnOffUi(*device, client);
  //     }
  //   }
  //   client.println("</div>");
  // }

  client.println("<div id=\"rooms\"></div>");


  /* Password popup code */
  client.println("<div class=\"popup-overlay hidden-popup\" id=\"password-popup-overlay\">\
        <div class=\"popup-content\" id=\"password-popup-content\">\
            <div class=\"popup-header\">Enter Password</div>\
            <div class=\"popup-message\">Please provide your password to continue:</div>\
            <input type=\"password\" id=\"password-input\" class=\"popup-password-input\" placeholder=\"Password\" />\
            <button class=\"popup-button\" onclick=\"submitPassword()\">Submit</button>\
            <div class=\"popup-close\" id=\"password-popup-close\">&times;</div>\
        </div>\
  </div>");

  /* Advanced controls popup */
  client.println("<div class=\"popup-overlay hidden-popup\" id=\"advanced-ctrl-overlay\">\
    <div class=\"popup-content\" id=\"advanced-ctrl-popup\">\
        <div class=\"popup-header\" id=\"adv-ctrl-head\"></div>\
        <div class=\"popup-close\" id=\"advanced-ctrl-popup-close\">&times;</div>\
        <div class=\"popup-message\" id=\"advanced-ctrl-popup-msg\"></div>\
    </div>\
</div>");

client.println("<div class=\"popup-backdrop\"></div><script>var ledStripExtCtrlId = 255;</script>\
  <div class=\"color-picker-popup\" id=\"FavouritesPopup\">\
      <div class=\"header2\">Saved compositions</div><hr>\
        <div id=\"ledStripExt1\" class=\"color-display on\" style=\"background-color: rgb(130, 70, 170);\"></div>\
        <div class=\"button-container\"><button onclick=\"overWriteMemSlot(1, ledStripExtCtrlId);\" class=\"button\" id=\"overWrEx1\">Overwrite</button><button onclick=\"loadMemSlot(1, ledStripExtCtrlId);\" class=\"button\" id=\"loadEx1\">Load</button></div>\
        <hr><br>\
        <div id=\"ledStripExt2\" class=\"color-display on\" style=\"background-color: rgb(130, 70, 170);\"></div>\
        <div class=\"button-container\"><button onclick=\"overWriteMemSlot(2, ledStripExtCtrlId);\" class=\"button\" id=\"overWrEx2\">Overwrite</button><button onclick=\"loadMemSlot(2, ledStripExtCtrlId);\" class=\"button\" id=\"loadEx2\">Load</button></div>\
        <hr><br>\
        <div id=\"ledStripExt3\" class=\"color-display on\" style=\"background-color: rgb(130, 70, 170);\"></div>\
        <div class=\"button-container\"><button onclick=\"overWriteMemSlot(3, ledStripExtCtrlId);\" class=\"button\" id=\"overWrEx3\">Overwrite</button><button onclick=\"loadMemSlot(3, ledStripExtCtrlId);\" class=\"button\" id=\"loadEx3\">Load</button></div>\
        <button class=\"button\" id=\"composClose\">Close</button>\
  </div>");

  client.println("<script>document.getElementById(\"password-input\").addEventListener(\"keydown\", function(event) {\
  if (event.key === \"Enter\") {\
    submitPassword();\
  }\
  });</script>");


  // printTestLedStrip(client);

  /* Display configuration button */
  uint8_t nodeType = 
    std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION)).nodeType;
  const String configPageButtonText = nodeType != 255 ? "Settings" : "Configure";

  if(secAccessLevel == e_ACCESS_LEVEL_NONE){
    String configButtonLink = "\
    <br><button class=\"button\" onclick=\"showPasswordPopup()\">"+configPageButtonText+"</button><br>";
    client.println(configButtonLink);
  }else 
  {
    client.println("<br><a href=\"/config\" class=\"button\">"+configPageButtonText+"</a><br>");
  }



  client.println("<script>\
        let currentData = {};\
\
\
        async function fetchData() {\
            try {\
                const response = await fetch('/getPageContent');\
                const newData = await response.json();\
\
                if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                    currentData = newData;\
                    renderRooms(currentData);\
                    console.log(currentData);\
                }\
            } catch (error) {\
                console.error('Error fetching data:', error);\
            }\
        }\
\
\
        fetchData();\
        getNotifications();\
\
        setInterval(fetchData, 2500);\
\
\
\
    </script>");

            // 

}

void HomeLightHttpServer::constantHandler_clearErrors(WiFiClient& client)
{
  for(uint8_t i = 1; i <= ERR_MON_LAST_ERROR; i++)
  {  
    std::any_cast<std::function<void(ERR_MON_ERROR_TYPE errorCode)>>(
    DataContainer::getSignalValue(CBK_ERROR_CLEAR)
    )((ERR_MON_ERROR_TYPE)i);
  }

  client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
}

void HomeLightHttpServer::constantHandler_configPage(WiFiClient& client)
{
  client.println("<script>getNotifications();</script>");
  // Print config page if it is requested
  printConfigPage(client);
  
}

void HomeLightHttpServer::constantHandler_resetDevice(WiFiClient& client)
{
  /* Reset device callback */
  std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(100);
  client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
}

  

void HomeLightHttpServer::constantHandler_devicesSetup(WiFiClient& client)
{
  printSlotsConfigPage(client);
}

void HomeLightHttpServer::constantHandler_roomAssignment(WiFiClient& client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">Room name mapping</div>");
  
  uint8_t slotIndex = 1;
  std::vector<uint8_t> alreadyPrintedMappings;
  for(auto& room : deviceToRoomMappingList){
    client.println("<div class=\"container\">");
    client.println(labelStart);
    client.println("Room ID:<input disabled type=\"text\" maxlength=\"2\" id=\"roomMappingID"+String((int)slotIndex)+"\"\
    value=\""+ String((int)room.first) +"\">");
    client.println(labelEnd);

    String nameValue = "";
    /* name existing */
    if(roomNamesMapping.find(room.first) != roomNamesMapping.end())
    {
      nameValue = roomNamesMapping.find(room.first)->second;
      alreadyPrintedMappings.push_back(room.first);
    }

    client.println(labelStart);
    client.println("Name:<input type=\"text\" maxlength=\"24\" placeholder=\"Your custom name here\" id=\"roomMappingName"+String((int)slotIndex)+"\"\
    value=\""+ nameValue +"\">");
    client.println(labelEnd);

    if(nameValue != ""){
      client.println(labelStart);
      client.println("ON link:<input type=\"text\" disabled value=\"roomON/"+ nameValue +"\">");
      client.println(labelEnd);
      client.println(labelStart);
      client.println("OFF link:<input type=\"text\" disabled value=\"roomOFF/"+ nameValue +"\">");
      client.println(labelEnd);
    }

    client.println("</div>");
    slotIndex++;
  }

  /* There also can be room mappings of the roomIDs which are no longer present in deviceToRoomMappingList - device room was changed 
      or device was removed */
      /* This is why we must print also the rest of mappings which were not printed above */
  for(auto& mapping : roomNamesMapping){
    bool mappingAlreadyPrinted = false;
    for(auto& alreadyPrinted : alreadyPrintedMappings){
      if(mapping.first == alreadyPrinted){
        mappingAlreadyPrinted = true;
        break;
      }
    }
    if(mappingAlreadyPrinted){
      continue;
    }

    client.println("<div class=\"container\"><div class=\"header\">Empty (no devices)</div>");
    client.println(labelStart);
    client.println("Room ID:<input disabled type=\"text\" maxlength=\"2\" id=\"roomMappingID"+String((int)slotIndex)+"\"\
    value=\""+ String((int)mapping.first) +"\">");
    client.println(labelEnd);


    client.println(labelStart);
    client.println("Name:<input type=\"text\" maxlength=\"24\" placeholder=\"Your custom name here\" id=\"roomMappingName"+String((int)slotIndex)+"\"\
    value=\""+ mapping.second +"\">");
    client.println(labelEnd);

    client.println("</div>");
    slotIndex++;
  }

  client.println("<button class=\"button\" id=\"roomMappingApplyBtn\" onclick=\"roomMappingCreateString(" +String((int)deviceToRoomMappingList.size()) + ");\">Save</button>");
  client.println("<a href=\"/config\" class=\"button\">Config Page</a><br>");


  client.println("</div>");
}


void HomeLightHttpServer::constantHandler_massErase(WiFiClient& client)
{
  /* Erase flash callback */
  try{
    Serial.println("Erasing flash!");
    /* erase flash */
    std::any_cast<std::function<void(void)>>(DataContainer::getSignalValue(CBK_MASS_ERASE))();
    /* redirect */
    client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
    /* restart */
    std::any_cast<std::function<void(uint16_t)>>
    (DataContainer::getSignalValue(CBK_RESET_DEVICE))(2000);
  }catch (std::bad_any_cast ex)
  {

  }
}


void HomeLightHttpServer::constantHandler_asyncTest(WiFiClient& client)
{
  Serial.println("Asysnc request received!");
}


/** parameterized requests */ 

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
          (DataContainer::getSignalValue(CBK_RESET_DEVICE))(7000);
      }
}

void HomeLightHttpServer::parameterizedHandler_newSetupJson(String& request, WiFiClient& client)
{
  /* Call CBK_SET_CONFIG_VIA_STRING function with "header" parameter */
  std::any_cast<DeviceConfigManipulationAPI>
    (DataContainer::getSignalValue(SIG_SET_DEVICES_CONFIG_VIA_JSON)).setDeviceCfgViaJson(request);

}


void HomeLightHttpServer::parameterizedHandler_deviceSwitch(String& request, WiFiClient& client)
{
  pos1 = request.indexOf("stDvstte"); 
  pos2 = request.indexOf("state"); 
  pos3 = request.indexOf("&");         
  String devId = request.substring(pos1+8 , pos2);
  String state = request.substring(pos2+5 , pos3);
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
    4
  );



  Serial.println("Async request processing started ");
  /* Request will be processed in next cycle of the HttpServer */


}


void HomeLightHttpServer::parameterizedHandler_loadDeviceConfiguration(String& request, WiFiClient& client)
{
  Serial.println("Loading configuration from file request ...");

  escapeSpecialCharsInJson(request);
  request.replace("loaddeicvcfg&", "");


  // Serial.println(request);

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  UserInterfaceNotification notification;

  if(success == DeserializationError::Code::Ok){
    Serial.println("Json deserialized successfully");
    bool loadingFailure = false;
    loadingFailure |= std::any_cast<DeviceConfigManipulationAPI>
        (DataContainer::getSignalValue(SIG_SET_CONFIG_VIA_JSON_STRING)).loadConfigFromFile(doc);

    loadingFailure |= std::any_cast<DeviceConfigManipulationAPI>
        (DataContainer::getSignalValue(SIG_SET_DEVICES_CONFIG_VIA_JSON)).loadConfigFromFile(doc);

    loadingFailure |= std::any_cast<DeviceConfigManipulationAPI>
        (DataContainer::getSignalValue(CBK_GET_ROOMS_CFG_JSON)).loadConfigFromFile(doc);

    notification.title = "New configuration loaded";
    notification.type = UserInterfaceNotification::INFO;
    notification.body = "Config file loaded successfully. Device will be restarted.";
    HTTPAsyncRequestHandler::createRequest(ASYNC_REDIRECT_TO_MAIN_PAGE, nullptr, 0);

  }else {

    Serial.println("Json file loading error");
    notification.title = "Config file loading failure";
    notification.type = UserInterfaceNotification::ERROR;
    notification.body = "Problem occurred with configuration file loading.";
  }

  std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);

  /* successfully loaded */
  if(notification.type == UserInterfaceNotification::INFO){
    std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(5000);
  }
}


void HomeLightHttpServer::parameterizedHandler_setStripColor(String& request, WiFiClient& client){
  escapeSpecialCharsInJson(request);
  request.replace("/setStripColor&", "");

  uint16_t numberOfLeds = NUMBER_OF_DIODES;

  uint8_t* memory = (uint8_t*)malloc(340);
  memory[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_3;
  memory[SERVICE_NAME_INDEX] = DEVSERVICE_SET_DETAILED_COLORS;

  *((uint16_t*)(memory+DYNAMIC_REQUEST_MEMORY_LENGTH_IDX)) = (numberOfLeds * sizeof(LedColor));
  

  memory[DYNAMIC_REQUEST_DIRECTION_IDX] = e_IN_to_DEVICE;
  LedColor* ledValueAddr = (LedColor*) &memory[DYNAMIC_REQUEST_START_OF_DATA_IDX];

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if(success == DeserializationError::Code::Ok){
    String deviceId = String(doc["devId"]);
    /* Process JSON to extrac each device slot*/
    for(uint16_t i = 0; i < numberOfLeds; i++)
    {
        /*this exist for every slot*/
        String r = String(doc["color"][i][0]);
        String g = String(doc["color"][i][1]);
        String b = String(doc["color"][i][2]);

        

        if(r != "null" && g != "null" && b != "null"){
          ledValueAddr->r = r.toInt() <= 255 ? r.toInt() : 255;
          ledValueAddr->g = g.toInt() <= 255 ? g.toInt() : 255;
          ledValueAddr->b = b.toInt() <= 255 ? b.toInt() : 255;

          // Serial.println(String((int)ledValueAddr->r) + " " + String((int)ledValueAddr->g) + " " + String((int)ledValueAddr->b));
        }else {
          break;
        }

        /* go to next diode */
        ledValueAddr++;
    }

    if(deviceId != "null"){
      memory[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = deviceId.toInt();


      // Serial.println(request);
      HTTPAsyncRequestHandler::createRequest(
        ASYNC_TYPE_DEVICE_SERVICE_CALL,
        memory,
        340
      );  

    }

  }

  free(memory);
}


void HomeLightHttpServer::parameterizedHandler_stripLoadFromMemory(String& request, WiFiClient& client)
{

  escapeSpecialCharsInJson(request);
  request.replace("/stripLoadFromMemory&", "");

  Serial.println(request);
  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if(success == DeserializationError::Code::Ok){
    String devIdStr = doc["devId"];
    String memorySlotStr = doc["slot"];

    if(devIdStr != "null" && memorySlotStr != "null"){
      Serial.println("Load strip from memory ...");
      uint8_t parameters[4];
      parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = devIdStr.toInt(); /* idx 0 */
      parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1; /* idx 1 */
      parameters[SERVICE_NAME_INDEX] = DEVSERVICE_LED_STRIP_SWITCH_CONTENT;       /* idx 2 */
      parameters[3] = memorySlotStr.toInt();                                    /* idx 3 */

      HTTPAsyncRequestHandler::createRequest(
        ASYNC_TYPE_DEVICE_SERVICE_CALL,
        parameters,
        4
      );
    }
  }

}

void HomeLightHttpServer::parameterizedHandler_stripSaveCurrent(String& request, WiFiClient& client)
{
  escapeSpecialCharsInJson(request);
  request.replace("/stripOverwriteSlot&", "");

  Serial.println(request);

  JsonDocument doc;
  DeserializationError success = deserializeJson(doc, request.c_str());
  if(success == DeserializationError::Code::Ok){
    String devId = doc["devId"];
    String slot = doc["slot"];

    if(devId != "null" && slot != "null"){

      Serial.println("Request: stripOverwriteSlot {devId, slot}");
      uint8_t parameters[4];
      parameters[DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL] = devId.toInt(); /* idx 0 */
      parameters[SERVICE_OVERLOADING_FUNCTION_INDEX] = serviceCall_1; /* idx 1 */
      parameters[SERVICE_NAME_INDEX] = DEVSERVICE_LED_STRIP_SAVE_CONTENT;       /* idx 2 */
      parameters[3] = slot.toInt();                                    /* idx 3 */

      HTTPAsyncRequestHandler::createRequest(
        ASYNC_TYPE_DEVICE_SERVICE_CALL,
        parameters,
        4
      );
    }

  }
}

void HomeLightHttpServer::parameterizedHandler_getExtendedControls(String& request, WiFiClient& client){
    Serial.println("Extended controls requested");
    escapeSpecialCharsInJson(request);
    request.replace("/getExtendedControls&", "");

    Serial.println(request);

    JsonDocument doc;
    DeserializationError success = deserializeJson(doc, request.c_str());
    if(success == DeserializationError::Code::Ok){
      String deviceIdStr = doc["devId"];
      if(deviceIdStr != "null"){
        uint16_t deviceId = deviceIdStr.toInt();
        Serial.println("Advanced controls requested.");
        HTTPAsyncRequestHandler::createRequest(
            ASYNC_GET_ADVANCED_CONTROLS,
            (uint8_t*)&deviceId,
            sizeof(deviceId)
          );
      }
    }
}

void HomeLightHttpServer::parameterizedHandler_downloadDeviceConfiguration(String& request, WiFiClient& client)
{

  HTTPAsyncRequestHandler::createRequest(
    ASYNC_DOWNLOAD_CONFIGURATION,
    nullptr,
    0
  );

  Serial.println("Device configuration download requested ...");
  /* Request will be processed in next cycle of the HttpServer */

}

void HomeLightHttpServer::parameterizedHandler_deviceBrightnessChange(String& request, WiFiClient& client)
{
  pos1 = request.indexOf("chngdvbr"); //Wyszukuje pozycję, na której występuje ciąg znaków 'bri' w nagłówku.
  pos2 = request.indexOf("DEV"); //Wyszukuje pozycję, na której występuje ciąg znaków 'DEV' w nagłówku.
  pos3 = request.indexOf("&"); //Wyszukuje pozycję, na której występuje ciąg znaków '&' w nagłówku.             
  String brightnessString = request.substring(pos1+8, pos2); 
  String idDeviceString = request.substring(pos2+3, pos3);
  
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
    4
  );

}

void HomeLightHttpServer::pending(WiFiClient& client){
  client.println("Loading, please wait . . . ");
  client.println("<meta http-equiv='refresh' content='1; url=http://"+ ipAddressString +"'>");
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
  int pos1 = request.indexOf("ledStripColor?id="); 
  int pos2 = request.indexOf("&r="); 
  int pos3 = request.indexOf("&g=");
  int pos4 = request.indexOf("&b=");       
  String devId = request.substring(pos1+17 , pos2);
  String red = request.substring(pos2+3 , pos3);
  String green = request.substring(pos3+3 , pos4);
  String blue = request.substring(pos4+3);

  for(uint8_t i = 0 ; i < ledsCount; i++)
  {
    leds.red[i] = red.toInt();
    leds.green[i] = green.toInt();
    leds.blue[i] = blue.toInt();
  }

  Serial.println(devId);
  Serial.println(red + ", " + green + " , " + blue);

  client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");

}
void HomeLightHttpServer::parameterizedHandler_ledColor(String& request, WiFiClient& client){
  int pos1 = request.indexOf("ledColor?id="); 
  int pos2 = request.indexOf("&led="); 
  int pos3 = request.indexOf("&r="); 
  int pos4 = request.indexOf("&g=");
  int pos5 = request.indexOf("&b=");       
  String devId = request.substring(pos1+12 , pos2);
  String ledIndex = request.substring(pos2+5 , pos3);
  String red = request.substring(pos3+3 , pos4);
  String green = request.substring(pos4+3 , pos5);
  String blue = request.substring(pos5+3);

  leds.red[ledIndex.toInt()] = red.toInt();
  leds.green[ledIndex.toInt()] = green.toInt();
  leds.blue[ledIndex.toInt()] = blue.toInt();
  

  Serial.println(devId);
  Serial.println(red + ", " + green + " , " + blue);

  client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");

}


void HomeLightHttpServer::constantHandler_asyncGetPageContent(String& request, WiFiClient& client)
{
  HTTPAsyncRequestHandler::createRequest(ASYNC_GET_PAGE_CONTENT, nullptr, 0);
}


void HomeLightHttpServer::constantHandler_asyncGetNotifications(String& request, WiFiClient& client)
{
  HTTPAsyncRequestHandler::createRequest(ASYNC_GET_NOTIFICATION_LIST, nullptr, 0);
}

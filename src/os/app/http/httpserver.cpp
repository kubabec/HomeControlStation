#include <os/app/http/HttpServer.hpp>
#include <Regexp.h>


#include "os/app/http/ConfigPageHttp.h"
#include "os/app/http/PageHead.h"
#include "os/app/http/Style.h"
#include "os/app/http/JavaScript.h"

WiFiServer HomeLightHttpServer::server(80);
String HomeLightHttpServer::header = "";
unsigned long HomeLightHttpServer::currentTime = 0;
unsigned long HomeLightHttpServer::previousTime = 0;
const long HomeLightHttpServer::timeoutTime = 2000;
int HomeLightHttpServer::pos1= 100;
int HomeLightHttpServer::pos2 = 150;
int HomeLightHttpServer::pos3 = 150;

std::function<bool(uint8_t, bool)> HomeLightHttpServer::deviceEnableCallback;
std::function<bool(uint8_t, uint8_t)> HomeLightHttpServer::deviceBrightnessChangeCallback;
std::vector<OnOffDeviceDescription> HomeLightHttpServer::onOffDescriptionVector;
std::array<SystemErrorType, ERR_MONT_ERROR_COUNT> HomeLightHttpServer::systemErrorList;
uint8_t HomeLightHttpServer::activeErrorsCount = 0;
ConfigSlotsDataType HomeLightHttpServer::pinConfigSlotsCopy_HttpServer = {};
String HomeLightHttpServer::ipAddressString;


// ERR_MON_UNEXPECTED_RESET = 1,
// ERR_MON_INVALID_NVM_DATA,
// ERR_MON_INVALID_LOCAL_CONFIG,
// ERR_MON_WRONG_CONFIG_STRING_RECEIVED,
// ERR_MON_WRONG_LOCAL_DEVICES_CONFIG_RECEIVED,
// ERR_MON_INVALID_ERROR_REPORTED,
String errorCodeDescription[ERR_MONT_ERROR_COUNT] = 
{
  "Unexpected reset",
  "Invalid NVM data",
  "Invalid local devices config",
  "Wrong config string received",
  "Wrong local devices config string received",
  "Invalid error reported"
};

std::vector<String> constantRequests = {
  "", /* Main page with no parameter */
  "config",
  "errclrbtn",
  "localDevices",
  "masseraseviahttp"
};

std::vector<String> parameterizedRequests = {
  "apply",
  "localSetup",
  "dev",
  "?bri"
};

std::vector<std::function<void(WiFiClient&)>> constantRequestHandlers = {
  HomeLightHttpServer::constantHandler_mainPage,
  HomeLightHttpServer::constantHandler_configPage,
  HomeLightHttpServer::constantHandler_clearErrors,
  HomeLightHttpServer::constantHandler_devicesSetup,
  HomeLightHttpServer::constantHandler_massErase
};

std::vector<std::function<void(String&, WiFiClient&)>> parameterizedRequestHandlers = {
  HomeLightHttpServer::parameterizedHandler_newConfigApply,
  HomeLightHttpServer::parameterizedHandler_newDevicesSetup,
  HomeLightHttpServer::parameterizedHandler_deviceSwitch,
  HomeLightHttpServer::parameterizedHandler_deviceBrightnessChange
};


void HomeLightHttpServer::cyclic()
{
    // Do cyclic task here
    handleClientRequest();
}

void HomeLightHttpServer::deinit() {
    
}

void HomeLightHttpServer::init()
{
  //std::any deviceEnable = DataContainer::getSignalValue(CBK_DEVICE_ENABLE);
  //deviceEnableCallback = (std::any_cast<std::function<bool(uint8_t, bool)>>(deviceEnable));
  // pod signal kryje sie adres na funkcje w device manager której aliasem jest deviceEnableCallback
  DataContainer::subscribe(CBK_DEVICE_ENABLE, "HTTPServer", [](std::any signal) {
    deviceEnableCallback = (std::any_cast<std::function<bool(uint8_t, bool)>>(signal));
  }
  );

  DataContainer::subscribe(CBK_DEVICE_BRIGHTNESS_CHANGE, "HTTPServer", [](std::any signal) {
    deviceBrightnessChangeCallback = (std::any_cast<std::function<bool(uint8_t, uint8_t)>>(signal));
  }
  );

  DataContainer::subscribe(SIG_SYSTEM_ERROR_LIST, "HTTPServer", [](std::any signal) {
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



  DataContainer::subscribe(SIG_COLLECTION_ONOFF, "HTTPServer", HomeLightHttpServer::onDeviceDescriptionChange);
  DataContainer::subscribe(SIG_CONFIG_SLOTS, "HTTPServer", HomeLightHttpServer::onSlotConfigChange);


  /* Get IP address from DataContainer to have it for further client redirections */
  ipAddressString = std::any_cast<String>(
    DataContainer::getSignalValue(SIG_IP_ADDRESS_STRING)
  );

  server.setNoDelay(true);
  server.begin();
  currentTime = millis();
  //DataContainer::subscribe(CBK_DEVICE_ENABLE, "DeviceManager", ...);
}

void HomeLightHttpServer::requestErrorList()
{
  
}

void HomeLightHttpServer::processLinkRequestData(WiFiClient& client)
{
  /* Retrieve request from pattern : 'GET /request HTTP/1.1' */
  String linkRequest = header.substring(
    String("GET /").length(), 
    (header.length() - (String(" HTTP/1.1").length()+2) )
  );

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
        /* Run function assigned to received command */
        constantRequestHandlers.at(knownRequest)(client);
        retVal = true;
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
        parameterizedRequestHandlers.at(knownRequest)(request, client);
        retVal = true;
        break;
      }
    }
  }

  return retVal;
}

void HomeLightHttpServer::handleClientRequest()
{
  WiFiClient client = server.available(); 
  if (client) {                             // If a new client connects,
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
            client.println(javascript);
            client.println("</head>");



            // Web Page Heading
            client.println("<body><div class=\"wrapper\">");
            client.println(popupContent);

            processLinkRequestData(client);

            if(header.indexOf("GET /?pending") >= 0) //Ta linia sprawdza, czy w nagłówku żądania HTTP występuje fraza "GET /?bri"
            { 
                /* TODO :
                  jezeli sygnał SIG_CURRENT_REQUEST_PROCESSING_STATE nie jest pending to przekieruj na strone główną,
                  w każdym innym razie przekieruj ponownie na pending (/?pending) */
            }

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

void HomeLightHttpServer::onDeviceDescriptionChange(std::any newDescriptionVector) 
{
  //Wlasny Http Servra wektor urzadzen On Off 
  onOffDescriptionVector = (std::any_cast<std::vector<OnOffDeviceDescription>>(newDescriptionVector));
  for(auto& onOff : onOffDescriptionVector) {
    onOff.print();
  }
}

void HomeLightHttpServer::onSlotConfigChange(std::any newSlotConfig)
{
  pinConfigSlotsCopy_HttpServer = (std::any_cast<ConfigSlotsDataType>(newSlotConfig));
  for(auto& slot : pinConfigSlotsCopy_HttpServer.slots) {
    slot.print();
  }
}

//funkcja rysujaca UI do sterowania dla 1 urzadzenia onoff
void HomeLightHttpServer::generateOnOffUi(OnOffDeviceDescription& description, WiFiClient& client) {
  client.println("<div class=\"container\">"); // container
  /* Print device name */
  client.println("<div class=\"header\">" + description.deviceName + "</div>"); // HEADER / HEADER

  /* Draw ON/OFF button depending on the current state */
  if(!description.isEnabled) {    
    client.println("<a class=\"button\" href=\"/dev" + String(description.deviceId) + "state" + "1&\">OFF</a>");
  }
  else {
    client.println("<a class=\"button\"  href=\"/dev" + String(description.deviceId) +  "state" + "0&\" + &>ON</a>");
  }

  /* Draw Brightness range bar if device allows brightness change */
  if(description.brightnessIsAdjustable) {
    client.println("<br>");
    const String brightnessSlider1  = "<div class=\"header2\">Brightness</div><input type='range' min='0' max='100' value='";
    const String brightnessSlider2 = "' onchange=\"onRangeChanged(this.value, " + String(description.deviceId) + ")\">";
    client.println(brightnessSlider1 + String(description.currentBrightness) + brightnessSlider2);
  }
  client.println("</div>"); // container 
}


void HomeLightHttpServer::generateConfigSlotUi(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client)
{
  const char* labelStart = "<label>";
  const char* labelEnd = "</label>";

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

  if(slot.deviceType == 43){
    client.println("<option value=\"43\" selected>On/Off Device</option>");
  }else
  {
    client.println("<option value=\"43\">On/Off Device</option>");
  }
  
  if(slot.deviceType == 44){
    client.println("<option value=\"44\" selected>LED strip</option>");
  }else
  {
    client.println("<option value=\"44\">LED strip</option>");
  }

  if(slot.deviceType != 43 && slot.deviceType != 44){
    client.println("<option value=\"255\" selected>UNKNOWN</option>");
  }else {
    client.println("<option value=\"255\">UNKNOWN</option>");
  }
  client.println("</select>");

  client.println(labelEnd);

  client.println(labelStart);
  client.println("Pin:<select type=\"text\" id=\"pin"+String(slotNumber)+"\"\
  value=\""+ String((int)slot.pinNumber) +"\">");
  for(uint8_t i = 1 ; i <= 10 ; i ++){
    String pinStr = "";
    if(slot.pinNumber != i)
    {
      pinStr = "<option value=\"" + String(i) + "\">" + String(i) + "</option>";
    }
    else
    {
      pinStr = "<option value=\"" + String(i) + "\" selected>" + String(i) + "</option>";
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
  client.println("<select id=\"extra-43-"+String((int)slotNumber)+"\">");
  if(slot.customBytes[0]){
    client.println("<option value=\"0\" >No</option>");
    client.println("<option value=\"1\" selected>Yes</option>");
  }else {
    client.println("<option value=\"0\" selected>No</option>");
    client.println("<option value=\"1\">Yes</option>");
  }
  client.println("</select>");

  client.println("</label>");
  client.println("</div>");

  /*<!-- Extra fields for LED Strip -->*/
  client.println("<div class=\"extra-fields extra-44\">");
  client.println("<label>LEDs num.:");
  client.println("<input id=\"extra-44-"+String((int)slotNumber)+"\" type=\"text\" placeholder=\"35\">");

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

  client.println(configPageContent1);
  if(currentConfig.isHttpServer)
  {
    client.println(yesSelected);
    client.println(noNotSelected);
  }else
  {
    client.println(yesNotSelected);
    client.println(noSelected);
  }
  client.println(configPageContent2);
  if(currentConfig.isRcServer)
  {
    client.println(yesSelected);
    client.println(noNotSelected);
  }else
  {
    client.println(yesNotSelected);
    client.println(noSelected);
  }
  client.println(configPageContent3);
  client.println(currentConfig.networkSSID);
  client.println(configPageContent4);
  client.println(currentConfig.networkPassword);
  client.println(configPageContent5);
  client.println((int)currentConfig.nodeId);
  client.println(configPageContent6);
  client.println((int)currentConfig.nodeType);
  client.println(configPageContent7);


  printErrorTable(client);

  /* Display return button */
  client.println("<br><a href=\"http://"+ipAddressString+"\" class=\"button\">Home page</a><br>");

}

void HomeLightHttpServer::printSlotsConfigPage(WiFiClient& client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">Node Local Configuration</div>");

  uint8_t slotIdx = 1;
  for(auto slot : pinConfigSlotsCopy_HttpServer.slots)
  {
    generateConfigSlotUi(slotIdx, slot, client);
    slotIdx++;
  }

  client.println("<button class=\"button\" id=\"confirmationButton\" onclick=\"showPopup('Do you wanna change node devices configuration?\
   GPIO pin setup will be loaded according to options selected on this page.', createConfigurationString);\">Save config</button>");
  client.println("<a href=\"/config\" class=\"button\">Config Page</a><br>");


  client.println("</div>");
}

void HomeLightHttpServer::printErrorTable(WiFiClient& client)
{
  client.println("<div class=\"error-table-container\"> <div class=\"error-header\">Error Log</div> <table class=\"error-table\">");
  
  if(activeErrorsCount > 0){
  
    client.println("<thead>\
                      <tr>\
                          <th>Code</th>\
                          <th>Description</th>\
                          <th>Count</th>\
                          <th>Extra Data</th>\
                          <th>Time</th>\
                      </tr>\
                  </thead><tbody>");
        

    for(uint i = 0 ; i < ERR_MONT_ERROR_COUNT; i++)
    {
      if(systemErrorList.at(i).occurrenceCount > 0) {
        client.println("<tr>\
                          <td>ERR-"+ String(i+1) +"</td>\
                          <td>" + errorCodeDescription[i] + "</td>\
                          <td>"+ String((int)systemErrorList.at(i).occurrenceCount) +"</td>\
                          <td>"+ String((int)systemErrorList.at(i).extendedData) +"</td>\
                          <td>"+ String((int)systemErrorList.at(i).lastOccurrenceTime) +"</td>\
                      </tr>");

      }
    }
    client.println("</tbody></table>");

    /* Display error clear button */
    const char* errorClearBtn = "\
    <button class=\"error-button\" \
    onclick=\"showPopup('Wanna clear device errors?', '/errclrbtn')\">Clear errors</button>";
    client.println(errorClearBtn);

  }else 
  {
    /* no errors */
    client.println("No active errors.");
    client.println("</table>");
  }
  client.println("</div>");
}


/*** CONSTANT HANDLERS */

void HomeLightHttpServer::constantHandler_mainPage(WiFiClient& client)
{
  /* Generate UI for every available device */
  for(OnOffDeviceDescription& description : onOffDescriptionVector) 
  {
    generateOnOffUi(description, client);       
  }


  /* Display configuration button */
  const char* configButtonLink = "\
  <br><a href=\"/config\" class=\"button\">Settings</a><br>";
  client.println(configButtonLink);
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
  // Print config page if it is requested
  printConfigPage(client);
}

void HomeLightHttpServer::constantHandler_devicesSetup(WiFiClient& client)
{
  printSlotsConfigPage(client);
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
    // std::any_cast<std::function<void()>>
    // (DataContainer::getSignalValue(CBK_RESET_DEVICE))();
  }catch (std::bad_any_cast ex)
  {

  }
}



/** parameterized requests */ 

void HomeLightHttpServer::parameterizedHandler_newConfigApply(String& request, WiFiClient& client)
{
  // Configuration to be handled here

  Serial.println("Applying new config!");
  /* Call CBK_SET_CONFIG_VIA_STRING function with "header" parameter */
  std::any_cast<std::function<void(String&)>>
    (DataContainer::getSignalValue(CBK_SET_CONFIG_VIA_STRING))(request);

  std::any_cast<std::function<void()>>
    (DataContainer::getSignalValue(CBK_RESET_DEVICE))();

  client.println("<meta http-equiv='refresh' content='0;  url=http://"+ ipAddressString +"'>");
}

void HomeLightHttpServer::parameterizedHandler_newDevicesSetup(String& request, WiFiClient& client)
{
  Serial.println("Applying new device configuration!");
  /* Call CBK_SET_CONFIG_VIA_STRING function with "header" parameter */
  std::any_cast<std::function<void(String&)>>
    (DataContainer::getSignalValue(CBK_SET_DEVICES_CONFIG_VIA_STRING))(request);

  client.println("<meta http-equiv='refresh' content='0;  url=http://"+ ipAddressString +"'>");
}


void HomeLightHttpServer::parameterizedHandler_deviceSwitch(String& request, WiFiClient& client)
{
  pos1 = request.indexOf("dev"); 
  pos2 = request.indexOf("state"); 
  pos3 = request.indexOf("&");         
  String devId = request.substring(pos1+3 , pos2);
  String state = request.substring(pos2+5 , pos3);
  uint8_t deviceId = devId.toInt();
  uint8_t deviceState = state.toInt();
  
  deviceEnableCallback(deviceId, deviceState);
  Serial.println("->HTTP server - dostalem ID : " + String(deviceId));
  client.println("<meta http-equiv='refresh' content='0;  url=http://"+ ipAddressString +"'>");
}

void HomeLightHttpServer::parameterizedHandler_deviceBrightnessChange(String& request, WiFiClient& client)
{
  pos1 = request.indexOf("bri"); //Wyszukuje pozycję, na której występuje ciąg znaków 'bri' w nagłówku.
  pos2 = request.indexOf("DEV"); //Wyszukuje pozycję, na której występuje ciąg znaków 'DEV' w nagłówku.
  pos3 = request.indexOf("&"); //Wyszukuje pozycję, na której występuje ciąg znaków '&' w nagłówku.             
  String brightnessString = request.substring(pos1+3, pos2); 
  String idDeviceString = request.substring(pos2+3, pos3);
  
  uint8_t newbrightness = brightnessString.toInt(); 
  uint8_t idString = idDeviceString.toInt();

  deviceBrightnessChangeCallback(idString,newbrightness);
  // TODO : Przekieruj zawsze na pending a nie na stronę główną
  client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
}

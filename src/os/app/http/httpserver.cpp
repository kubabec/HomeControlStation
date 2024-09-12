#include <os/app/http/HttpServer.hpp>

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
String HomeLightHttpServer::ipAddressString;

void HomeLightHttpServer::cyclic()
{
    // Do cyclic task here
    handleClientRequest();
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

  DataContainer::subscribe(SIG_COLLECTION_ONOFF, "HTTPServer", HomeLightHttpServer::onDeviceDescriptionChange);


  /* Get IP address from DataContainer to have it for further client redirections */
  ipAddressString = std::any_cast<String>(
    DataContainer::getSignalValue(SIG_IP_ADDRESS_STRING)
  );

  server.setNoDelay(true);
  server.begin();
  currentTime = millis();
  //DataContainer::subscribe(CBK_DEVICE_ENABLE, "DeviceManager", ...);
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
        Serial.write("request " + requestLine);                   
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

            // NO config page requested?
            if(header.indexOf("GET /config") < 0){

              if(header.indexOf("GET /?pending") >= 0) //Ta linia sprawdza, czy w nagłówku żądania HTTP występuje fraza "GET /?bri"
              { 
                  /* TODO :
                    jezeli sygnał SIG_CURRENT_REQUEST_PROCESSING_STATE nie jest pending to przekieruj na strone główną,
                    w każdym innym razie przekieruj ponownie na pending (/?pending) */
              }
              // Check brightness changeCondition
              if(header.indexOf("GET /?bri") >= 0) //Ta linia sprawdza, czy w nagłówku żądania HTTP występuje fraza "GET /?bri"
              {
                pos1 = header.indexOf("bri"); //Wyszukuje pozycję, na której występuje ciąg znaków 'bri' w nagłówku.
                pos2 = header.indexOf("DEV"); //Wyszukuje pozycję, na której występuje ciąg znaków 'DEV' w nagłówku.
                pos3 = header.indexOf("&"); //Wyszukuje pozycję, na której występuje ciąg znaków '&' w nagłówku.             
                String brightnessString = header.substring(pos1+3, pos2); 
                String idDeviceString = header.substring(pos2+3, pos3);
                
                uint8_t newbrightness = brightnessString.toInt(); 
                uint8_t idString = idDeviceString.toInt();

                deviceBrightnessChangeCallback(idString,newbrightness);
                // TODO : Przekieruj zawsze na pending a nie na stronę główną
                client.println("<meta http-equiv='refresh' content='0; url=http://"+ ipAddressString +"'>");
              }
              
              // Check timer changeCondition
              if(header.indexOf("GET /?tim") >= 0) {
                pos1 = header.indexOf("tim");
                pos2 = header.indexOf("&");
                String timerString = header.substring(pos1+1, pos2);
                uint8_t newtimer = timerString.toInt(); 
                client.println("<meta http-equiv='refresh' content='0;  url=http://"+ ipAddressString +"'>");
              }
        
              if(header.indexOf("GET /dev") >= 0) {
                
                pos1 = header.indexOf("dev"); 
                pos2 = header.indexOf("state"); 
                pos3 = header.indexOf("&");         
                String devId = header.substring(pos1+3 , pos2);
                String state = header.substring(pos2+5 , pos3);
                uint8_t deviceId = devId.toInt();
                uint8_t deviceState = state.toInt();
                
                deviceEnableCallback(deviceId, deviceState);
                Serial.println("->HTTP server - dostalem ID : " + String(deviceId));
                client.println("<meta http-equiv='refresh' content='0;  url=http://"+ ipAddressString +"'>");
              } 

              if(header.indexOf("GET /apply") >= 0)
              {
                // Configuration to be handled here

                Serial.println("Applying new config!");
                /* Call CBK_SET_CONFIG_VIA_STRING function with "header" parameter */
                std::any_cast<std::function<void(String&)>>
                  (DataContainer::getSignalValue(CBK_SET_CONFIG_VIA_STRING))(header);

                client.println("<meta http-equiv='refresh' content='0;  url=http://"+ ipAddressString +"'>");
              }

              /* Generate UI for every available device */
              for(OnOffDeviceDescription& description : onOffDescriptionVector) 
              {
                generateOnOffUi(description, client);       
              }


              /* Display configuration button */
              const char* configButtonLink = "\
              <br><a href=\"/config\" class=\"button\">Configuration</a><br>";
              client.println(configButtonLink);

            }else
            {
              // Print config page if it is requested
              printConfigPage(client);
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


/* ========= CONFIG POC ==============*/

void HomeLightHttpServer::printConfigPage(WiFiClient& client)
{
  NodeConfiguration currentConfig = 
    std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));

  const String yesNotSelected = "<option value=\"yes\">Yes</option>";
  const String noNotSelected = "<option value=\"no\">No</option>";
  const String yesSelected = "<option selected=\"selected\" value=\"yes\">Yes</option>";
  const String noSelected = "<option selected=\"selected\" value=\"no\">No</option>";

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

  /* Display return button */
  client.println("<br><a href=\"http://"+ipAddressString+"\" class=\"button\">Home page</a><br>");

}
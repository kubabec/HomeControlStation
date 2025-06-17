#include <os/app/http/httpserver.hpp>
#include <Esp.h>

void HomeLightHttpServer::constantHandler_mainPage(WiFiClient& client)
{
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
      <div class=\"header2\">Saved compositions</div>\
        <div id=\"ledStripExt1\" class=\"color-display\" style=\"background-color: rgb(130, 70, 170);\"></div>\
        <div class=\"button-container\"><button onclick=\"overWriteMemSlot(1, ledStripExtCtrlId);\" class=\"icon-btn save\" id=\"overWrEx1\"></button><button onclick=\"loadMemSlot(1, ledStripExtCtrlId);\" class=\"icon-btn play\" id=\"loadEx1\"></button></div>\
        <br>\
        <div id=\"ledStripExt2\" class=\"color-display\" style=\"background-color: rgb(130, 70, 170);\"></div>\
        <div class=\"button-container\"><button onclick=\"overWriteMemSlot(2, ledStripExtCtrlId);\" class=\"icon-btn save\"id=\"overWrEx2\"></button><button onclick=\"loadMemSlot(2, ledStripExtCtrlId);\" class=\"icon-btn play\" id=\"loadEx2\"></button></div>\
        <br>\
        <div id=\"ledStripExt3\" class=\"color-display\" style=\"background-color: rgb(130, 70, 170);\"></div>\
        <div class=\"button-container\"><button onclick=\"overWriteMemSlot(3, ledStripExtCtrlId);\" class=\"icon-btn save\" id=\"overWrEx3\"></button><button onclick=\"loadMemSlot(3, ledStripExtCtrlId);\" class=\"icon-btn play\" id=\"loadEx3\"></button></div>\
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
    <button class=\"button\" onclick=\"showPasswordPopup()\">"+configPageButtonText+"</button>";
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
        hidePopup('advanced-ctrl-overlay', 'advanced-ctrl-popup');\
        getNotifications();\
\
        setInterval(fetchData, 2000);\
\
\
\
    </script>");

            // 

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
    client.println("<label>");
    client.println("Room ID:<input disabled type=\"text\" maxlength=\"2\" id=\"roomMappingID"+String((int)slotIndex)+"\"\
    value=\""+ String((int)room.first) +"\">");
    client.println("</label>");

    String nameValue = "";
    /* name existing */
    if(roomNamesMapping.find(room.first) != roomNamesMapping.end())
    {
      nameValue = roomNamesMapping.find(room.first)->second;
      alreadyPrintedMappings.push_back(room.first);
    }

    client.println("<label>");
    client.println("Name:<input type=\"text\" maxlength=\"24\" placeholder=\"Your custom name here\" id=\"roomMappingName"+String((int)slotIndex)+"\"\
    value=\""+ nameValue +"\">");
    client.println("</label>");

    if(nameValue != ""){
      client.println("<label>");
      client.println("ON link:<input type=\"text\" disabled value=\"roomON/"+ nameValue +"\">");
      client.println("</label>");
      client.println("<label>");
      client.println("OFF link:<input type=\"text\" disabled value=\"roomOFF/"+ nameValue +"\">");
      client.println("</label>");
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
    client.println("<label>");
    client.println("Room ID:<input disabled type=\"text\" maxlength=\"2\" id=\"roomMappingID"+String((int)slotIndex)+"\"\
    value=\""+ String((int)mapping.first) +"\">");
    client.println("</label>");


    client.println("<label>");
    client.println("Name:<input type=\"text\" maxlength=\"24\" placeholder=\"Your custom name here\" id=\"roomMappingName"+String((int)slotIndex)+"\"\
    value=\""+ mapping.second +"\">");
    client.println("</label>");

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
    (DataContainer::getSignalValue(CBK_RESET_DEVICE))(1000);
  }catch (std::bad_any_cast ex)
  {

  }
}


void HomeLightHttpServer::constantHandler_asyncTest(WiFiClient& client)
{
  Serial.println("Asysnc request received!");
}


void HomeLightHttpServer::constantHandler_networkInspecion(WiFiClient& client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">Network inspection view</div>");


  try{
    std::vector<NetworkNodeInfo> networkNodes = std::any_cast<std::vector<NetworkNodeInfo> >(DataContainer::getSignalValue(SIG_NETWORK_NODES_INFO));
    client.println("<table class=\"table-graphite\">");
    client.println("<thead><tr><th>Type</th><th>IP</th><th>Web interface</th></tr></thead>");
    client.println("<tbody>");
    for(auto& node : networkNodes){
      String nodeType = node.nodeType == NetworkNodeInfo::NodeType::Master ? "Master" : "Slave";
      String nodeIpString = String((int)node.nodeIP.octet1)+"."+String((int)node.nodeIP.octet2)+"."+String((int)node.nodeIP.octet3)+"."+String((int)node.nodeIP.octet4);
      client.println("<tr><td>"+nodeType+"</td><td>"+nodeIpString+"</td><td><a class=\"button\" href=\"http://"+nodeIpString+"/\">Go</a></td></tr>");
    }
    client.println("</tbody>");

    client.println("</table>");
  } catch (std::bad_any_cast ex)
  {
    client.println("<div>Data not available.</div>");
  }

  client.println("<a href=\"/config\" class=\"button\">Config Page</a><br>");

  client.println("</div>");

}

void HomeLightHttpServer::constantHandler_systemDetails(WiFiClient& client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">System details</div>");

  std::vector<NetworkNodeInfo> networkNodes = std::any_cast<std::vector<NetworkNodeInfo> >(DataContainer::getSignalValue(SIG_NETWORK_NODES_INFO));
  client.println("<table class=\"table-graphite\">");
  client.println("<thead><tr><th>Parameter</th><th>Value</th></tr></thead>");
  client.println("<tbody>");
  client.println("<tr><td>free RAM</td><td>"+String((int)ESP.getFreeHeap())+" bytes</td></tr>");
  client.println("<tr><td>CPU frequency</td><td>"+String((int)ESP.getCpuFreqMHz())+" MHz</td></tr>");
  client.println("<tr><td><div id=\"systmp\">Temperature</div></td><td>");

  client.println("<div class=\"memory-bar\"><div class=\"memory-bar-fill\" id=\"tempBarFill\"></div>");
  client.println("</div></div>");
  client.println("<script>\
    function setTempBarValue(value) {\
      const bar = document.getElementById('tempBarFill');\
      const clampedValue = Math.min(100, Math.max(0, value));\
      bar.style.width = clampedValue + '%';\
      const tempDiv = document.getElementById('systmp');\
      tempDiv.innerHTML = 'Temperature: ' + value + '°C';\
    }\
  </script>");
  client.println("</td></tr></tbody>");

  client.println("</table>");

  client.println("<a href=\"/config\" class=\"button\">Config Page</a><br>");

  client.println("</div>");

  client.println("<script>\
\
\
        async function fetchSys() {\
            try {\
                const response = await fetch('/gtSysDet');\
                const sysDat = await response.json();\
                setTempBarValue(sysDat.temp);\
                console.log('System data fetched:', sysDat);\
            } catch (error) {\
                console.error('Error sys fetching data:', error);\
            }\
        }\
\
\
        fetchSys();\
\
        setInterval(fetchSys, 500);\
\
\
\
    </script>");

}



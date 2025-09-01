#include <os/app/http/httpserver.hpp>
#include <Esp.h>

void HomeLightHttpServer::constantHandler_mainPage(WiFiClient &client)
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

  if (secAccessLevel == e_ACCESS_LEVEL_NONE)
  {
    String configButtonLink = "\
    <button class=\"button\" onclick=\"showPasswordPopup()\">" +
                              configPageButtonText + "</button>";
    client.println(configButtonLink);
  }
  else
  {
    client.println("<br><a href=\"/config\" class=\"button\">" + configPageButtonText + "</a><br>");
  }

  client.println("<script>\
        let currentData = {};\
\
\
        async function fetchData() {\
  if(interfaceVisible){\
            try {\
                const response = await fetch('/getPageContent');\
                const [newData, hashObj] = await response.json();\
                if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                    currentData = newData;\
                    hash = hashObj.hash;\
                    renderRooms(currentData);\
                }\
            } catch (error) {\
                console.error('Error fetching data:', error);\
            }\
  }else {\
    renderRooms({});\
  }\
        }\
\
let hash = 0;\
    async function getHash(){\
  if(interfaceVisible){\
        try {\
            const response = await fetch('/getHash');\
            const newHash = await response.json();\
            if(newHash.hash != hash){\
                hash = newHash.hash;\
                fetchData();}\
        } catch (error) {\
            console.error('Error fetching hash:', error);\
        }\
  }else {\
    renderRooms({});\
  }\
    }\
\
        getHash();\
        hidePopup('advanced-ctrl-overlay', 'advanced-ctrl-popup');\
        getNotifications();\
\
        setInterval(getHash, 2000);\
\
\
\
    </script>");

  //
}

void HomeLightHttpServer::constantHandler_configPage(WiFiClient &client)
{
  client.println("<script>getNotifications();</script>");
  // Print config page if it is requested
  printConfigPage(client);
}

void HomeLightHttpServer::constantHandler_resetDevice(WiFiClient &client)
{
  /* Reset device callback */
  std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(100);
  client.println("<meta http-equiv='refresh' content='0; url=http://" + ipAddressString + "'>");
}

void HomeLightHttpServer::constantHandler_devicesSetup(WiFiClient &client)
{
  printSlotsConfigPage(client);
}

void HomeLightHttpServer::constantHandler_roomAssignment(WiFiClient &client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">Room name mapping</div>");

  uint8_t slotIndex = 1;
  std::vector<uint8_t> alreadyPrintedMappings;
  for (auto &room : deviceToRoomMappingList)
  {
    client.println("<div class=\"container\">");
    client.println("<label>");
    client.println("Room ID:<input disabled type=\"text\" maxlength=\"2\" id=\"roomMappingID" + String((int)slotIndex) + "\"\
    value=\"" + String((int)room.first) +
                   "\">");
    client.println("</label>");

    String nameValue = "";
    /* name existing */
    if (roomNamesMapping.find(room.first) != roomNamesMapping.end())
    {
      nameValue = roomNamesMapping.find(room.first)->second;
      alreadyPrintedMappings.push_back(room.first);
    }

    client.println("<label>");
    client.println("Name:<input type=\"text\" maxlength=\"24\" placeholder=\"Your custom name here\" id=\"roomMappingName" + String((int)slotIndex) + "\"\
    value=\"" + nameValue +
                   "\">");
    client.println("</label>");

    client.println("<label>");
    client.println("Toggle command:<input type=\"text\" disabled value=\"home.local/roomToggle&{roomId:" + String((int)room.first) + "}\">");

    client.println("</div>");
    slotIndex++;
  }

  /* There also can be room mappings of the roomIDs which are no longer present in deviceToRoomMappingList - device room was changed
      or device was removed */
  /* This is why we must print also the rest of mappings which were not printed above */
  for (auto &mapping : roomNamesMapping)
  {
    bool mappingAlreadyPrinted = false;
    for (auto &alreadyPrinted : alreadyPrintedMappings)
    {
      if (mapping.first == alreadyPrinted)
      {
        mappingAlreadyPrinted = true;
        break;
      }
    }
    if (mappingAlreadyPrinted)
    {
      continue;
    }

    client.println("<div class=\"container\"><div class=\"header\">Empty (no devices)</div>");
    client.println("<label>");
    client.println("Room ID:<input disabled type=\"text\" maxlength=\"2\" id=\"roomMappingID" + String((int)slotIndex) + "\"\
    value=\"" + String((int)mapping.first) +
                   "\">");
    client.println("</label>");

    client.println("<label>");
    client.println("Name:<input type=\"text\" maxlength=\"24\" placeholder=\"Your custom name here\" id=\"roomMappingName" + String((int)slotIndex) + "\"\
    value=\"" + mapping.second +
                   "\">");
    client.println("</label>");

    client.println("</div>");
    slotIndex++;
  }

  client.println("<button class=\"button\" id=\"roomMappingApplyBtn\" onclick=\"roomMappingCreateString(" + String((int)deviceToRoomMappingList.size()) + ");\">Save</button>");
  client.println("<a href=\"/config\" class=\"button\">BACK</a><br>");

  client.println("</div>");
}

void HomeLightHttpServer::constantHandler_massErase(WiFiClient &client)
{
  /* Erase flash callback */
  try
  {
    Serial.println("Erasing flash!");
    /* erase flash */
    std::any_cast<std::function<void(void)>>(DataContainer::getSignalValue(CBK_MASS_ERASE))();
    /* redirect */
    client.println("<meta http-equiv='refresh' content='0; url=http://" + ipAddressString + "'>");
    /* restart */
    std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(1000);
  }
  catch (std::bad_any_cast ex)
  {
  }
}

void HomeLightHttpServer::constantHandler_asyncTest(WiFiClient &client)
{
  Serial.println("Asysnc request received!");
}

void HomeLightHttpServer::constantHandler_networkInspecion(WiFiClient &client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">Network inspection view</div>");

  try
  {
    std::vector<NetworkNodeInfo> networkNodes = std::any_cast<std::vector<NetworkNodeInfo>>(DataContainer::getSignalValue(SIG_NETWORK_NODES_INFO));
    client.println("<table class=\"table-graphite\">");
    client.println("<thead><tr><th>Type</th><th>IP</th><th>Web interface</th></tr></thead>");
    client.println("<tbody>");
    for (auto &node : networkNodes)
    {
      String nodeType = node.nodeType == NetworkNodeInfo::NodeType::Master ? "Master" : "Slave";
      String nodeIpString = String((int)node.nodeIP.octet1) + "." + String((int)node.nodeIP.octet2) + "." + String((int)node.nodeIP.octet3) + "." + String((int)node.nodeIP.octet4);
      client.println("<tr><td>" + nodeType + "</td><td>" + nodeIpString + "</td><td><a class=\"button\" href=\"http://" + nodeIpString + "/\">Go</a></td></tr>");
    }
    client.println("</tbody>");

    client.println("</table>");
  }
  catch (std::bad_any_cast ex)
  {
    client.println("<div>Data not available.</div>");
  }

  client.println("<a href=\"/config\" class=\"button\">BACK</a><br>");

  client.println("</div>");
}

void HomeLightHttpServer::constantHandler_systemDetails(WiFiClient &client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">System details</div>");

  std::vector<NetworkNodeInfo> networkNodes = std::any_cast<std::vector<NetworkNodeInfo>>(DataContainer::getSignalValue(SIG_NETWORK_NODES_INFO));
  client.println("<table class=\"table-graphite\">");
  client.println("<thead><tr><th>Parameter</th><th>Value</th></tr></thead>");
  client.println("<tbody>");
  client.println("<tr><td>free RAM</td><td>" + String((int)ESP.getFreeHeap()) + " bytes</td></tr>");
  client.println("<tr><td>CPU frequency</td><td>" + String((int)ESP.getCpuFreqMHz()) + " MHz</td></tr>");
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

  client.println("<a href=\"/config\" class=\"button\">BACK</a><br>");

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

void HomeLightHttpServer::constantHandler_digitalButtons(WiFiClient &client)
{

  std::vector<std::pair<uint64_t, DigitalButton::ButtonEvent>> digitalButtonsMapping =
      std::any_cast<std::vector<std::pair<uint64_t, DigitalButton::ButtonEvent>>>(DataContainer::getSignalValue(SIG_DIGITAL_BTN_MAPPING));

  client.println("\
    <table class=\"table-graphite\" id=\"data-table\" aria-describedby=\"table-desc\">\
    <thead><tr><th style=\"width:25%\">Button ID</th><th style=\"width:25%\">Affects</th><th style=\"width:25%\">Affected Item</th><th style=\"width:25%\">Action</th><th style=\"width:90px\">&nbsp;</th>\
    </tr></thead><tbody>");

  for (auto &mapping : digitalButtonsMapping)
  {
    client.println("<tr><td><input type=\"text\" class=\"cell-id\" value=\"" + String((int)mapping.first) + "\" /></td>");

    if (mapping.second.affectedType == DigitalButton::AffectedType::DEVICE)
    {
      client.println("<td>\
          <select class=\"cell-type\">\
            <option value=\"" +
                     String((int)DigitalButton::AffectedType::ROOM) + "\">Room</option>\
            <option value=\"" +
                     String((int)DigitalButton::AffectedType::DEVICE) + "\" selected>Device</option>\
          </select>\
        </td>");
    }
    else
    {
      client.println("<td>\
          <select class=\"cell-type\">\
            <option value=\"" +
                     String((int)DigitalButton::AffectedType::ROOM) + "\" selected>Room</option>\
            <option value=\"" +
                     String((int)DigitalButton::AffectedType::DEVICE) + "\">Device</option>\
          </select>\
        </td>");
    }

    client.println("<td><input type=\"text\" class=\"cell-target-id\" value=\"" + String((int)mapping.second.affectedId) + "\" /></td>");

    if (mapping.second.actionType == DigitalButton::ActionType::TOGGLE)
    {
      client.println("<td>\
          <select class=\"cell-action\">\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::TOGGLE) + "\" selected>TOGGLE</option>\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::ON) + "\" >ON</option>\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::OFF) + "\">OFF</option>\
          </select>\
        </td>");
    }
    else if (mapping.second.actionType == DigitalButton::ActionType::ON)
    {
      client.println("<td>\
          <select class=\"cell-action\">\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::TOGGLE) + "\">TOGGLE</option>\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::ON) + "\" selected>ON</option>\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::OFF) + "\">OFF</option>\
          </select>\
        </td>");
    }
    else
    {
      client.println("<td>\
          <select class=\"cell-action\">\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::TOGGLE) + "\">TOGGLE</option>\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::ON) + "\" >ON</option>\
            <option value=\"" +
                     String((int)DigitalButton::ActionType::OFF) + "\" selected>OFF</option>\
          </select>\
        </td>");
    }

    client.println("<td><button type=\"button\" class=\"error-button\">Remove</button></td></tr>");
  }
  client.println("</tbody></table>");

  client.println("\
    <div class=\"controls\">\
    <button id=\"add-row\" class=\"button\">(+) New mapping</button>\
    <button id=\"submit-btn\" class=\"button\">Save table</button>\
  </div>\
    <a href=\"/config\" class=\"button\">BACK</a>\
    ");

  client.println("\
    <template id=\"row-template\">\
    <tr>\
      <td><input type=\"text\" class=\"cell-id\" value=\"\" /></td>\
      <td>\
        <select class=\"cell-type\">\
          <option value=\"" +
                 String((int)DigitalButton::AffectedType::ROOM) + "\">Room</option>\
          <option value=\"" +
                 String((int)DigitalButton::AffectedType::DEVICE) + "\">Device</option>\
        </select>\
      </td>\
      <td><input type=\"text\" class=\"cell-target-id\" value=\"0\" /></td>\
      <td>\
        <select class=\"cell-action\">\
          <option value=\"" +
                 String((int)DigitalButton::ActionType::TOGGLE) + "\">TOGGLE</option>\
          <option value=\"" +
                 String((int)DigitalButton::ActionType::ON) + "\">ON</option>\
          <option value=\"" +
                 String((int)DigitalButton::ActionType::OFF) + "\">OFF</option>\
        </select>\
      </td>\
      <td><button type=\"button\" class=\"error-button\">Remove</button></td></tr>\
  </template>\
    ");

  const char *skript = R"SCRIPT(
<script>
  (function(){
  const table = document.getElementById('data-table');
  const tbody = table.querySelector('tbody');
  const addBtn = document.getElementById('add-row');
  const submitBtn = document.getElementById('submit-btn');
  const jsonOut = document.getElementById('json-output');
  const template = document.getElementById('row-template');)SCRIPT";

  const char *skript2 = R"SCRIPT2(
  // pomocnik: tworzy <select class="cell-target-id"> z podanych opcji
  function buildTargetSelect(options, selectedValue){
    const sel = document.createElement('select');
    sel.className = 'cell-target-id';
    if(options.length === 0){
      const opt = document.createElement('option');
      opt.value = '';
      opt.textContent = '(brak elementów)';
      sel.appendChild(opt);
      return sel;
    }
    options.forEach(o => {
      const opt = document.createElement('option');
      opt.value = o.value;
      opt.textContent = o.label;
      if(selectedValue !== undefined && String(selectedValue) === String(o.value)) opt.selected = true;
      sel.appendChild(opt);
    });
    return sel;
  }

  // zamienia środkowe pole (input lub select) na select oparty o aktualny type
  function populateTargetForRow(tr){
    const typeEl = tr.querySelector('.cell-type');
    if(!typeEl) return;
    // bierzemy tekst opcji (np. "Room" / "Device") — to jest prostsze i pasuje do twojego markupu
    const typeText = (typeEl.options[typeEl.selectedIndex] && typeEl.options[typeEl.selectedIndex].text) || typeEl.value;
    const td = (tr.querySelector('.cell-target-id') || {}).closest ? tr.querySelector('.cell-target-id').closest('td') : tr.children[2];
    // zachowaj aktualną wartość (jeśli była inputem lub selectem)
    const current = tr.querySelector('.cell-target-id') ? tr.querySelector('.cell-target-id').value : undefined;
    const list = elementsByType[typeText] || [];
    const newSelect = buildTargetSelect(list, current);
    if(td){
      td.innerHTML = '';
      td.appendChild(newSelect);
    }
  }

  // Dodaj wiersz (klonuje template), a potem inicjalizuje target select
  function addRow(values){
    const clone = template.content.firstElementChild.cloneNode(true);
    // ustaw wartości z values jeśli podano (opcjonalne)
    if(values){
      const id = clone.querySelector('.cell-id');
      const type = clone.querySelector('.cell-type');
      const target = clone.querySelector('.cell-target-id');
      const action = clone.querySelector('.cell-action');
      if(id && values.id !== undefined) id.value = values.id;
      if(type && values.type !== undefined) {
        // wartości typu w Twoim markupie to liczby/enumy, ale text opcji to "Room"/"Device"
        // jeśli values.type jest tekstem, ustawiamy według tekstu; jeśli liczbą - możesz dopasować serwerowo
        // tutaj ustawiamy value (serwer i HTML już generują odpow. option value)
        type.value = values.type;
      }
      if(target && values.targetId !== undefined) target.value = values.targetId;
      if(action && values.action !== undefined) action.value = values.action;
    }
    tbody.appendChild(clone);
    // zainicjalizuj środkowy select dla nowego wiersza
    populateTargetForRow(tbody.lastElementChild);
  }

  // usuwanie wiersza (delegacja)
  tbody.addEventListener('click', function(e){
    if(e.target && e.target.classList.contains('error-button')){
      const tr = e.target.closest('tr');
      if(tr) tr.remove();
    }
  });

  // reaguj na zmianę type i aktualizuj odpowiadający target select
  tbody.addEventListener('change', function(e){
    if(e.target && e.target.classList.contains('cell-type')){
      const tr = e.target.closest('tr');
      if(tr) populateTargetForRow(tr);
    }
  });

  // inicjalizacja istniejących wierszy po załadowaniu
  (function init(){
    Array.from(tbody.querySelectorAll('tr')).forEach(tr => populateTargetForRow(tr));
  })();

  // obsługa przycisku dodawania
  if(addBtn) addBtn.addEventListener('click', () => addRow());

  // zbieranie danych do tablicy obiektów
  function collectRows(){
    return Array.from(tbody.querySelectorAll('tr')).map(tr => {
      const idEl = tr.querySelector('.cell-id');
      const typeEl = tr.querySelector('.cell-type');
      const targetEl = tr.querySelector('.cell-target-id'); // teraz może być select
      const actionEl = tr.querySelector('.cell-action');
      return {
        id: idEl ? String(idEl.value) : '',
        type: typeEl ? String(typeEl.value) : '',
        targetId: targetEl ? String(targetEl.value) : '',
        action: actionEl ? String(actionEl.value) : ''
      };
    });
  }

  // submit -> loguj i aktualizuj <pre> jeśli istnieje
  if(submitBtn) submitBtn.addEventListener('click', function(){
    const data = collectRows();
    console.log('Wyeksportowane dane:', data);
    if(jsonOut) jsonOut.textContent = JSON.stringify(data, null, 2);
    var url = '/newDigBtnTab&' + JSON.stringify(data);

    const xhr = new XMLHttpRequest();
        xhr.timeout = 10000;
        xhr.open("POST", url, true);
        xhr.onreadystatechange = function() {
            if (xhr.readyState === 4) {
                if (xhr.status === 200) {
                    console.log('Table sent');
                }
            }
            url = '/';
            window.location.href = url;
        };
        xhr.send();
  });

})();
</script>
)SCRIPT2";

  client.println(skript);
  const auto &roomsWithNames = roomNamesMapping;
  const auto &roomsWithoutNames = deviceToRoomMappingList;
  const auto &deviceDescriptions = descriptionVector;

  client.println("const elementsByType = {\
    \"Room\": [");
  int iterator = 1;
  for (auto &room : roomsWithNames)
  {
    client.println("{ value: \"" + String((int)room.first) + "\", label: \"" + room.second + "\"}");
    // if(iterator != roomsWithNames.size()){
    client.println(",");
    // }
    iterator++;
  }
  iterator = 1;
  for (auto &room : roomsWithoutNames)
  {
    client.println("{ value: \"" + String((int)room.first) + "\", label: \"" + String((int)room.first) + "\"}");
    if (iterator != roomsWithoutNames.size())
    {
      client.println(",");
    }
    iterator++;
  }

  client.println("],\
    \"Device\": [");
  iterator = 1;
  for (auto &device : deviceDescriptions)
  {
    String roomName = "";
    if (roomNamesMapping.find(device.roomId) != roomNamesMapping.end())
    {
      roomName = roomNamesMapping.at(device.roomId);
    }
    else
    {
      roomName = "RoomID:" + String((int)device.roomId);
    }

    client.println("{ value: \"" + String((int)device.deviceId) + "\", label: \"" + device.deviceName + " (" + roomName + ")\"}");
    if (iterator != deviceDescriptions.size())
    {
      client.println(",");
    }
    iterator++;
  }
  client.println("] };");
  client.println(skript2);
}

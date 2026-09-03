#include <os/app/http/httpserver.hpp>
#include "generated/GeneratedDigitalEventActions.hpp"
#include "generated/GeneratedEnablingConditions.hpp"
#include "os/app/DigitalEvent/DigitalEventReceiver.hpp"
#include "os/app/timeMaster.hpp"
#include "os/Logger.hpp"
#include <Esp.h>

/**
 * @file src/os/app/http/requestCallbacks/constantHandlers.cpp
 * @brief HTTP server implementation and request callbacks for the Home Control Station.
 */


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
            if (!response.ok) throw new Error(`HTTP ${response.status}`);\
                const [newData, , notificationCnt] = await response.json();\
            if (!newData || typeof newData !== 'object' || Array.isArray(newData)) throw new Error('Invalid device data');\
            document.body.classList.remove('connection-lost');\
                if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                    currentData = newData;\
                    renderRooms(currentData);\
                }\
                if(notificationCnt.ntcnt > 0){\
                    getNotifications();\
                }\
            } catch (error) {\
              document.body.classList.add('connection-lost');\
                console.error('Error fetching data:', error);\
            }\
  }else {\
    renderRooms({});\
  }\
        }\
\
    fetchData();\
        hidePopup('advanced-ctrl-overlay', 'advanced-ctrl-popup');\
        getNotifications();\
\
    setInterval(fetchData, 1000);\
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
  std::any localAnyMassErase = DataContainer::getSignalValue(CBK_MASS_ERASE);
  if (auto p = std::any_cast<std::function<void(void)>>(&localAnyMassErase))
  {
    Logger::log("Erasing flash!");
    /* erase flash */
    (*p)();
    /* redirect */
    client.println("<meta http-equiv='refresh' content='0; url=http://" + ipAddressString + "'>");
    /* restart */
    std::any localAny = DataContainer::getSignalValue(CBK_RESET_DEVICE);
    if (auto p2 = std::any_cast<std::function<void(uint16_t)>>(&localAny))
    {
      (*p2)(1000);
    }
    else
    {
    }
  }
  else
  {
  }
}

void HomeLightHttpServer::constantHandler_asyncTest(WiFiClient &client)
{
  Logger::log("Asysnc request received!");
}

void HomeLightHttpServer::constantHandler_networkInspecion(WiFiClient &client)
{
  client.println("<div class=\"wrapper\">\
        <div class=\"header\">Network inspection view</div>");

  std::any localAny = DataContainer::getSignalValue(SIG_NETWORK_NODES_INFO);
  if (auto p = std::any_cast<std::vector<NetworkNodeInfo>>(&localAny))
  {
    std::vector<NetworkNodeInfo> networkNodes = *p;
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
  else
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

void HomeLightHttpServer::constantHandler_digitalEvents(WiFiClient &client)
{

  std::vector<std::pair<uint64_t, DigitalEvent::Event>> digitalEventsMapping =
      std::any_cast<std::vector<std::pair<uint64_t, DigitalEvent::Event>>>(DataContainer::getSignalValue(SIG_DIGITAL_EVNT_MAPPING));
  const auto &mappingConditions = DigitalEventReceiver::getMappingConditions();

  client.println("\
    <table class=\"table-graphite\" id=\"data-table\" aria-describedby=\"table-desc\">\
    <thead><tr><th>Mapping ID</th><th>Event ID</th><th>Device</th><th>Action</th><th>Enabling conditions (AND)</th><th style=\"width:90px\">&nbsp;</th>\
    </tr></thead><tbody>");

  for (auto &mapping : digitalEventsMapping)
  {
    uint8_t deviceType = 255;
    for (const auto &device : descriptionVector)
    {
      if (device.deviceId == mapping.second.deviceId) deviceType = device.deviceType;
    }
    const auto *action = GeneratedDigitalEventActions::find(deviceType, mapping.second.actionId);
    String selectedConditions;
    auto assignment = std::find_if(mappingConditions.begin(), mappingConditions.end(), [&mapping](const auto &item) { return item.mappingId == mapping.second.mappingId; });
    if (assignment != mappingConditions.end())
    {
      for (uint8_t conditionId : assignment->conditionIds)
      {
        if (conditionId != 0) selectedConditions += (selectedConditions.length() ? "," : "") + String((int)conditionId);
      }
    }

    client.println("<tr><td><input type=\"text\" class=\"cell-mapping-id\" readonly value=\"" + String((int)mapping.second.mappingId) + "\" /></td>");
    client.println("<td><input type=\"text\" class=\"cell-event-id\" value=\"" + String((uint64_t)mapping.first) + "\" /></td>");
    client.println("<td><select class=\"cell-device\" data-selected=\"" + String((int)mapping.second.deviceId) + "\"></select></td>");
    client.println("<td><button type=\"button\" class=\"button cell-action\" data-action-id=\"" + String((int)mapping.second.actionId) + "\">" + String(action ? action->label : "Choose action") + "</button></td>");
    client.println("<td><div class=\"cell-conditions\" data-selected=\"" + selectedConditions + "\"></div></td>");
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
      <td><input type=\"text\" class=\"cell-mapping-id\" readonly /></td>\
      <td><input type=\"text\" class=\"cell-event-id\" /></td>\
      <td><select class=\"cell-device\"></select></td>\
      <td><button type=\"button\" class=\"button cell-action\" data-action-id=\"\">Choose action</button></td>\
      <td><div class=\"cell-conditions\"></div></td>\
      <td><button type=\"button\" class=\"error-button\">Remove</button></td></tr>\
  </template>\
  <div class=\"popup-overlay hidden-popup\" id=\"action-popup\">\
    <div class=\"popup-content\"><div class=\"popup-header\">Choose device action</div>\
      <div id=\"action-popup-options\"></div><button type=\"button\" class=\"button\" id=\"action-popup-close\">Cancel</button>\
    </div>\
  </div>\
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
    var url = '/newDigEvntTab&' + JSON.stringify(data);

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
  client.println("const digitalEventDevices = [");
  for (const auto &device : descriptionVector)
  {
    String label = device.deviceName;
    label.replace("\\", "\\\\");
    label.replace("\"", "\\\"");
    client.println("{id:" + String((int)device.deviceId) + ",type:" + String((int)device.deviceType) + ",label:\"" + label + "\"},");
  }
  client.println("]; const digitalEventActions = [");
  for (const auto &action : GeneratedDigitalEventActions::kActions)
  {
    String label = action.label;
    label.replace("\\", "\\\\");
    label.replace("\"", "\\\"");
    client.println("{type:" + String((int)action.deviceType) + ",id:" + String((int)action.id) + ",label:\"" + label + "\"},");
  }
  client.println("]; const configuredConditions = [");
  for (const auto &condition : DigitalEventReceiver::getEnablingConditions())
  {
    auto device = std::find_if(descriptionVector.begin(), descriptionVector.end(), [&condition](const auto &item) { return item.deviceId == condition.deviceId; });
    if (device == descriptionVector.end()) continue;
    const auto *predicate = GeneratedEnablingConditions::find(device->deviceType, condition.predicateId);
    if (predicate == nullptr) continue;
    String label = String((int)condition.conditionId) + ": " + device->deviceName + " / " + predicate->label;
    label.replace("\\", "\\\\");
    label.replace("\"", "\\\"");
    client.println("{id:" + String((int)condition.conditionId) + ",label:\"" + label + "\"},");
  }
  client.println("]; ");
  client.println(R"SCRIPT3(
  let activeActionRow = null;
  const actionPopup = document.getElementById('action-popup');
  const actionOptions = document.getElementById('action-popup-options');

  function fillDeviceSelect(select) {
    const selected = select.dataset.selected || select.value;
    select.innerHTML = '';
    digitalEventDevices.forEach(device => {
      const option = document.createElement('option');
      option.value = device.id;
      option.textContent = device.label;
      option.dataset.type = device.type;
      option.selected = String(device.id) === String(selected);
      select.appendChild(option);
    });
  }

  function fillConditionSelects(row) {
    const container = row.querySelector('.cell-conditions');
    const selected = (container.dataset.selected || '').split(',').filter(Boolean);
    container.innerHTML = '';
    for (let index = 0; index < 3; index++) {
      const select = document.createElement('select');
      select.className = 'cell-condition';
      const none = document.createElement('option'); none.value = ''; none.textContent = index === 0 ? 'No condition' : 'No additional condition'; select.appendChild(none);
      configuredConditions.forEach(condition => {
        const option = document.createElement('option'); option.value = condition.id; option.textContent = condition.label;
        option.selected = String(condition.id) === String(selected[index]); select.appendChild(option);
      });
      container.appendChild(select);
    }
  }

  function openActionPopup(row) {
    activeActionRow = row;
    const device = digitalEventDevices.find(item => String(item.id) === row.querySelector('.cell-device').value);
    actionOptions.innerHTML = '';
    digitalEventActions.filter(action => device && action.type === device.type).forEach(action => {
      const button = document.createElement('button');
      button.type = 'button';
      button.className = 'button';
      button.textContent = action.label;
      button.onclick = () => {
        const target = activeActionRow.querySelector('.cell-action');
        target.dataset.actionId = action.id;
        target.textContent = action.label;
        actionPopup.classList.remove('visible-popup');
        actionPopup.classList.add('hidden-popup');
        actionPopup.querySelector('.popup-content').classList.remove('show');
      };
      actionOptions.appendChild(button);
    });
    if (!actionOptions.children.length) actionOptions.textContent = 'This device does not expose event-triggered actions.';
    actionPopup.classList.remove('hidden-popup');
    actionPopup.classList.add('visible-popup');
    actionPopup.querySelector('.popup-content').classList.add('show');
  }

  function nextMappingId() {
    const used = new Set(Array.from(tbody.querySelectorAll('.cell-mapping-id')).map(input => Number(input.value)));
    for (let id = 1; id <= 255; id++) if (id !== 11 && id !== 12 && !used.has(id)) return id;
    return 0;
  }

  Array.from(tbody.querySelectorAll('tr')).forEach(row => { fillDeviceSelect(row.querySelector('.cell-device')); fillConditionSelects(row); });
  addBtn.onclick = () => {
    const row = template.content.firstElementChild.cloneNode(true);
    row.querySelector('.cell-mapping-id').value = nextMappingId();
    fillDeviceSelect(row.querySelector('.cell-device'));
    fillConditionSelects(row);
    tbody.appendChild(row);
  };
  tbody.onclick = event => {
    const row = event.target.closest('tr');
    if (event.target.classList.contains('error-button')) row.remove();
    if (event.target.classList.contains('cell-action')) openActionPopup(row);
  };
  tbody.onchange = event => {
    if (!event.target.classList.contains('cell-device')) return;
    const row = event.target.closest('tr');
    const action = row.querySelector('.cell-action');
    action.dataset.actionId = '';
    action.textContent = 'Choose action';
    openActionPopup(row);
  };
  document.getElementById('action-popup-close').onclick = () => {
    actionPopup.classList.remove('visible-popup');
    actionPopup.classList.add('hidden-popup');
    actionPopup.querySelector('.popup-content').classList.remove('show');
  };
  submitBtn.onclick = () => {
    const data = Array.from(tbody.querySelectorAll('tr')).map(row => ({
      mappingId: Number(row.querySelector('.cell-mapping-id').value),
      eventId: String(row.querySelector('.cell-event-id').value),
      deviceId: Number(row.querySelector('.cell-device').value),
      actionId: Number(row.querySelector('.cell-action').dataset.actionId),
      conditionIds: Array.from(row.querySelectorAll('.cell-condition')).map(select => Number(select.value)).filter(Boolean)
    }));
    if (data.some(item => !item.mappingId || !item.eventId || !item.deviceId || !item.actionId)) {
      alert('Complete every mapping before saving.');
      return;
    }
    const xhr = new XMLHttpRequest();
    xhr.timeout = 10000;
    xhr.open('POST', '/newDigEvntTab&' + JSON.stringify(data), true);
    xhr.onreadystatechange = () => { if (xhr.readyState === 4) window.location.href = '/digBtn'; };
    xhr.send();
  };
  })();
  )SCRIPT3");
  client.println("</script>");
}

void HomeLightHttpServer::constantHandler_enablingConditions(WiFiClient &client)
{
  const auto &conditions = DigitalEventReceiver::getEnablingConditions();
  client.println("<div class=\"header\">Enabling Conditions</div>");
  client.println("<table class=\"table-graphite\" id=\"condition-table\"><thead><tr><th>Condition ID</th><th>Device</th><th>Condition</th><th>&nbsp;</th></tr></thead><tbody>");
  for (const auto &condition : conditions)
  {
    client.println("<tr><td><input class=\"condition-id\" type=\"number\" min=\"1\" max=\"255\" value=\"" + String((int)condition.conditionId) + "\"></td>");
    client.println("<td><select class=\"condition-device\" data-selected=\"" + String((int)condition.deviceId) + "\"></select></td>");
    client.println("<td><select class=\"condition-predicate\" data-selected=\"" + String((int)condition.predicateId) + "\"></select></td>");
    client.println("<td><button type=\"button\" class=\"error-button\">Remove</button></td></tr>");
  }
  client.println("</tbody></table><div class=\"controls\"><button type=\"button\" id=\"add-condition\" class=\"button\">(+) New condition</button><button type=\"button\" id=\"save-conditions\" class=\"button\">Save table</button></div><a href=\"/config\" class=\"button\">BACK</a>");
  client.println("<template id=\"condition-row-template\"><tr><td><input class=\"condition-id\" type=\"number\" min=\"1\" max=\"255\"></td><td><select class=\"condition-device\"></select></td><td><select class=\"condition-predicate\"></select></td><td><button type=\"button\" class=\"error-button\">Remove</button></td></tr></template>");

  client.println("<script>const conditionDevices=[");
  for (const auto &device : descriptionVector)
  {
    String label = device.deviceName;
    label.replace("\\", "\\\\");
    label.replace("\"", "\\\"");
    client.println("{id:" + String((int)device.deviceId) + ",type:" + String((int)device.deviceType) + ",label:\"" + label + "\"},");
  }
  client.println("];const devicePredicates=[");
  for (const auto &predicate : GeneratedEnablingConditions::kPredicates)
  {
    String label = predicate.label;
    label.replace("\\", "\\\\");
    label.replace("\"", "\\\"");
    client.println("{type:" + String((int)predicate.deviceType) + ",id:" + String((int)predicate.id) + ",label:\"" + label + "\"},");
  }
  client.println(R"SCRIPT(];
(function(){
const tbody=document.querySelector('#condition-table tbody');
const template=document.getElementById('condition-row-template');
function fillPredicates(row){
  const device=conditionDevices.find(item=>String(item.id)===row.querySelector('.condition-device').value);
  const select=row.querySelector('.condition-predicate');
  const selected=select.dataset.selected||select.value;
  select.innerHTML='';
  devicePredicates.filter(item=>device&&item.type===device.type).forEach(item=>{
    const option=document.createElement('option');option.value=item.id;option.textContent=item.label;
    option.selected=String(item.id)===String(selected);select.appendChild(option);
  });
}
function initialize(row){
  const select=row.querySelector('.condition-device');const selected=select.dataset.selected||select.value;
  select.innerHTML='';conditionDevices.forEach(item=>{const option=document.createElement('option');option.value=item.id;option.textContent=item.label;option.selected=String(item.id)===String(selected);select.appendChild(option);});
  fillPredicates(row);
}
Array.from(tbody.children).forEach(initialize);
tbody.onclick=event=>{if(event.target.classList.contains('error-button'))event.target.closest('tr').remove();};
tbody.onchange=event=>{if(event.target.classList.contains('condition-device'))fillPredicates(event.target.closest('tr'));};
document.getElementById('add-condition').onclick=()=>{
  if(tbody.children.length>=20){alert('A maximum of 20 enabling conditions is supported.');return;}
  const row=template.content.firstElementChild.cloneNode(true);const used=new Set(Array.from(tbody.querySelectorAll('.condition-id')).map(input=>Number(input.value)));
  for(let id=1;id<=255;id++){if(!used.has(id)){row.querySelector('.condition-id').value=id;break;}}
  initialize(row);tbody.appendChild(row);
};
document.getElementById('save-conditions').onclick=()=>{
  const data=Array.from(tbody.children).map(row=>({conditionId:Number(row.querySelector('.condition-id').value),deviceId:Number(row.querySelector('.condition-device').value),predicateId:Number(row.querySelector('.condition-predicate').value)}));
  const ids=data.map(item=>item.conditionId);if(data.some(item=>!item.conditionId||!item.deviceId||!item.predicateId)||new Set(ids).size!==ids.length){alert('Use unique condition IDs and complete every row.');return;}
  const xhr=new XMLHttpRequest();xhr.open('POST','/saveEnablingConditions&'+JSON.stringify(data),true);xhr.onreadystatechange=()=>{if(xhr.readyState===4)window.location.href='/enablingConditions';};xhr.send();
};
})();
)SCRIPT");
  client.println("</script>");
}

void HomeLightHttpServer::constantHandler_unmappedEvents(WiFiClient &client)
{
  client.println("<div class=\"wrapper\"><div class=\"header\">Unmapped events</div>");
  client.println("<table class=\"table-graphite\" id=\"unmapped-event-table\"><thead><tr><th>Source</th><th>Event ID</th><th>&nbsp;</th></tr></thead><tbody>");

  const auto &events = DigitalEventReceiver::getUnmappedEvents();
  if (events.empty())
  {
    client.println("<tr><td colspan=\"3\">No unmapped events have occurred during this power cycle.</td></tr>");
  }
  for (size_t index = 0; index < events.size(); ++index)
  {
    String source = events[index].source;
    source.replace("&", "&amp;");
    source.replace("<", "&lt;");
    source.replace(">", "&gt;");
    source.replace("\"", "&quot;");
    String eventId = String((unsigned long long)events[index].eventId);
    client.println("<tr><td>" + source + "</td><td><input id=\"unmapped-event-" + String(index) +
                   "\" type=\"text\" readonly value=\"" + eventId +
                   "\"></td><td><button type=\"button\" class=\"button\" onclick=\"copyEventId('unmapped-event-" +
                   String(index) + "')\">Copy</button></td></tr>");
  }

  client.println("</tbody></table><a href=\"/digBtn\" class=\"button\">Event mappings</a>"
                 "<a href=\"/config\" class=\"button\">BACK</a></div>");
  client.println(R"SCRIPT(
<script>
function copyEventId(id) {
  const input = document.getElementById(id);
  input.select();
  input.setSelectionRange(0, input.value.length);
  if (navigator.clipboard && window.isSecureContext) navigator.clipboard.writeText(input.value);
  else document.execCommand('copy');
}
</script>
)SCRIPT");
}

void HomeLightHttpServer::constantHandler_rtcEvents(WiFiClient &client)
{
  client.println("<div class=\"header\">RTC Events</div>");
  client.println("<table class=\"table-graphite\" id=\"rtc-event-table\"><thead><tr><th>Event ID</th><th>Repeats</th><th>Schedule</th><th>&nbsp;</th></tr></thead><tbody></tbody></table>");
  client.println("<div class=\"controls\"><button type=\"button\" id=\"add-rtc-event\" class=\"button\">(+) New RTC event</button><button type=\"button\" id=\"save-rtc-events\" class=\"button\">Save table</button></div><a href=\"/config\" class=\"button\">BACK</a>");
  client.println("<script>const initialRtcEvents=[");
  for (const auto &event : TimeMaster::getRtcEvents())
  {
    client.println("{eventId:'" + String((unsigned long long)event.eventId) + "',anchorEpoch:" +
                   String((unsigned long)event.anchorEpoch) + ",intervalHours:" +
                   String((int)event.intervalHours) + ",recurrence:" +
                   String((int)event.recurrence) + ",weekdayMask:" +
                   String((int)event.weekdayMask) + "},");
  }
  client.println(R"SCRIPT(];
(function(){
const tbody=document.querySelector('#rtc-event-table tbody');
const modes=[['0','One shot'],['1','Every N hours'],['2','Every day'],['3','Selected weekdays']];
const dayNames=['Sun','Mon','Tue','Wed','Thu','Fri','Sat'];
const epochToDateTime=epoch=>new Date(epoch*1000).toISOString().slice(0,16);
const epochToTime=epoch=>new Date(epoch*1000).toISOString().slice(11,16);
function wallEpoch(value){
  const match=value.match(/^(\d{4})-(\d{2})-(\d{2})T(\d{2}):(\d{2})$/);
  return match?Math.floor(Date.UTC(+match[1],+match[2]-1,+match[3],+match[4],+match[5])/1000):0;
}
function addRow(data={eventId:'',anchorEpoch:0,intervalHours:1,recurrence:0,weekdayMask:0}){
  const row=document.createElement('tr');
  row.innerHTML='<td><input class="rtc-event-id" type="text" inputmode="numeric" pattern="[0-9]+" value="'+data.eventId+'"></td>'+
    '<td><select class="rtc-mode">'+modes.map(mode=>'<option value="'+mode[0]+'"'+(+mode[0]===+data.recurrence?' selected':'')+'>'+mode[1]+'</option>').join('')+'</select></td>'+
    '<td><div class="rtc-once"><input class="rtc-date-time" type="datetime-local"></div>'+
    '<div class="rtc-interval"><label>Start <input class="rtc-interval-start" type="datetime-local"></label><label>Every <input class="rtc-hours" type="number" min="1" max="168" value="'+(data.intervalHours||1)+'"> hours</label></div>'+
    '<div class="rtc-daily"><input class="rtc-time" type="time"></div>'+
    '<div class="rtc-weekly"><input class="rtc-weekly-time" type="time"><div class="rtc-days">'+dayNames.map((name,index)=>'<label><input type="checkbox" value="'+index+'" '+((data.weekdayMask&(1<<index))?'checked':'')+'>'+name+'</label>').join('')+'</div></div></td>'+
    '<td><button type="button" class="error-button rtc-remove">Remove</button></td>';
  const dateTime=data.anchorEpoch?epochToDateTime(data.anchorEpoch):'';
  const time=data.anchorEpoch?epochToTime(data.anchorEpoch):'12:00';
  row.querySelector('.rtc-date-time').value=dateTime;
  row.querySelector('.rtc-interval-start').value=dateTime;
  row.querySelector('.rtc-time').value=time;
  row.querySelector('.rtc-weekly-time').value=time;
  tbody.appendChild(row);updateVisibleFields(row);
}
function updateVisibleFields(row){
  const mode=+row.querySelector('.rtc-mode').value;
  ['rtc-once','rtc-interval','rtc-daily','rtc-weekly'].forEach((name,index)=>row.querySelector('.'+name).style.display=mode===index?'block':'none');
}
initialRtcEvents.forEach(addRow);
tbody.onchange=event=>{if(event.target.classList.contains('rtc-mode'))updateVisibleFields(event.target.closest('tr'));};
tbody.onclick=event=>{if(event.target.classList.contains('rtc-remove'))event.target.closest('tr').remove();};
document.getElementById('add-rtc-event').onclick=()=>{if(tbody.children.length>=10){alert('A maximum of 10 RTC events is supported.');return;}addRow();};
document.getElementById('save-rtc-events').onclick=()=>{
  const records=Array.from(tbody.children).map(row=>{
    const recurrence=+row.querySelector('.rtc-mode').value;
    let dateTime='';let weekdayMask=0;
    if(recurrence===0)dateTime=row.querySelector('.rtc-date-time').value;
    if(recurrence===1)dateTime=row.querySelector('.rtc-interval-start').value;
    if(recurrence===2)dateTime='2000-01-02T'+row.querySelector('.rtc-time').value;
    if(recurrence===3){dateTime='2000-01-02T'+row.querySelector('.rtc-weekly-time').value;row.querySelectorAll('.rtc-days input:checked').forEach(day=>weekdayMask|=1<<+day.value);}
    return {eventId:row.querySelector('.rtc-event-id').value.trim(),recurrence,anchorEpoch:wallEpoch(dateTime),intervalHours:recurrence===1?+row.querySelector('.rtc-hours').value:0,weekdayMask};
  });
  const ids=records.map(record=>record.eventId);
  if(records.some(record=>!/^[0-9]+$/.test(record.eventId)||record.eventId==='0'||!record.anchorEpoch||(record.recurrence===1&&(record.intervalHours<1||record.intervalHours>168))||(record.recurrence===3&&!record.weekdayMask))||new Set(ids).size!==ids.length){alert('Complete every schedule and use unique, non-zero event IDs.');return;}
  const xhr=new XMLHttpRequest();xhr.open('POST','/saveRtcEvents&'+JSON.stringify(records),true);xhr.onreadystatechange=()=>{if(xhr.readyState===4)window.location.href='/rtcEvents';};xhr.send();
};
})();
)SCRIPT");
  client.println("</script>");
}

#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H


const char* javascript = "\
<script>\
let isNotificationPollingActive = 1;\
    function applySettings() {\
        var isHTTPServer = document.querySelector('select[name=\"isHTTPServer\"]').value;\
        var isRCServer = document.querySelector('select[name=\"isRCServer\"]').value;\
        var isUserAdmin = document.querySelector('select[name=\"isUserAsAdmin\"]').value;\
        var nodeType = document.querySelector('select[name=\"nodetype\"]').value;\
        var SSID = document.querySelector('input[name=\"SSID\"]').value;\
        var Password = document.querySelector('input[name=\"Password\"]').value;\
        var PanelPassword = document.querySelector('input[name=\"UserPassword\"]').value;\
\
        let finalJson = {\
        httpActive:isHTTPServer,\
        rcServerActive:isRCServer,\
        usrAdmin:isUserAdmin,\
        type:nodeType,\
        network:SSID,\
        netPwd:Password,\
        cfgPwd:PanelPassword\
        };\
\
        let jsonString = JSON.stringify(finalJson);\
        console.log(\"Wygenerowany Config:\", jsonString);\
        var url = '/newCfgApply&' + jsonString;\
        window.location.href = url;\
    }\
    function goToDevicesManagement() {\
        var url = `/localDevices`;\
        window.location.href = url;\
    }\
    function goToRoomSettings() {\
        var url = `/roomAssignment`;\
        window.location.href = url;\
    }\
    function massErase(){\
        var url = `/masseraseviahttp`;\
        window.location.href = url;\
    }\
    function resetDevice(){\
        var url = `/resetDevice`;\
        window.location.href = url;\
    }\
    function onRangeTimChanged(value) {\
        value = parseInt(value);\
        window.location.assign(\"/?tim\"+value+\"&\");\
    }\
    function onRangeChanged(value, devId) {\
        value = parseInt(value);\
        var url = \"/chngdvbr\"+value+\"DEV\"+devId+\"&\";\
        const xhr = new XMLHttpRequest();\
        xhr.timeout = 10000;\
        const container = document.getElementById('container' + devId);\
        showLoading(container);\
        xhr.open(\"GET\", url, true);\
        xhr.onreadystatechange = function() {\
            if (xhr.readyState === 4) { \
                const statusElement = document.getElementById(\"status\");\
                if (xhr.status === 200) { \
                    const newData = JSON.parse(xhr.responseText);\
                    if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                        currentData = newData;\
                        renderRooms(currentData);\
                        console.log(currentData);\
                    }\
                } else { \
                    console.log('Error with AJAX request');\
                }\
            }\
            hideLoading(container);\
        };\
        xhr.send();\
    }\
    function toggleDeviceConfig(checkbox) {\
        var container = checkbox.closest('.device-container');\
        var statusText = container.querySelector('.status-text');\
        if (checkbox.checked) {\
            container.classList.remove('disabled');\
            statusText.textContent = 'Enabled';\
            container.querySelectorAll('input[type=\"text\"]').forEach(input => input.disabled = false);\
        } else {\
            container.classList.add('disabled');\
            statusText.textContent = 'Disabled';\
            container.querySelectorAll('input[type=\"text\"]').forEach(input => input.disabled = true);\
        }\
    }\
    function showExtraFields(select, deviceId) {\
        const deviceContainer = document.getElementById(deviceId);\
        const extraFields = deviceContainer.querySelectorAll('.extra-fields');\
        extraFields.forEach(field => {\
            field.classList.remove('visible');\
        });\
        const selectedValue = select.value;\
        const fieldsToShow = deviceContainer.querySelector(`.extra-${selectedValue}`);\
        if (fieldsToShow) {\
            fieldsToShow.classList.add('visible');\
        }\
    };\
    function getOnOffConfigurationJson(id){\
        var enable = \"enabled\" + id;\
        var enableValue = document.getElementById(enable).checked;\
        var dataName = document.getElementById('name' + id).value;\
        var dataPin = document.getElementById('pin' + id).value;\
        var dataRoom = document.getElementById('room' + id).value;\
        var brightnessSupport =  document.getElementById('brightnessSupported-' + id).value;\
        var activeSt = document.getElementById('activationState-' + id).value;\
        var pwmMin = document.getElementById('pwmMin-' + id).value;\
        var pwmMax = document.getElementById('pwmMax-' + id).value;\
        return {\
        type:\"OnOff\",\
        id:id,\
        enabled:enableValue,\
        name:dataName,\
        pin:dataPin,\
        room:dataRoom,\
        briSup:brightnessSupport,\
        activeState:activeSt,\
        PwmMin:pwmMin,\
        PwmMax:pwmMax\
      };\
    }\
    function getLedStripConfigurationJson(id){\
        var enable = \"enabled\" + id;\
        var enableValue = document.getElementById(enable).checked;\
        var dataName = document.getElementById('name' + id).value;\
        var dataPin = document.getElementById('pin' + id).value;\
        var dataRoom = document.getElementById('room' + id).value;\
        var ledsCnt =  document.getElementById('ledsCount-' + id).value;\
        var sideFlip =  document.getElementById('ledsSideFlip-' + id).value;\ 
        var curLim =  document.getElementById('curLimVal-' + id).realValue;\ 
        return {\
        type:\"LedStrip\",\
        id:id,\
        enabled:enableValue,\
        name:dataName,\
        pin:dataPin,\
        room:dataRoom,\
        ledCount:ledsCnt,\
        sideFlp:sideFlip,\
        currLim:curLim\
        };\
    }\
    function getTempSensorConfigurationJson(id){\
        var enable = \"enabled\" + id;\
        var enableValue = document.getElementById(enable).checked;\
        var dataName = document.getElementById('name' + id).value;\
        var dataPin = document.getElementById('pin' + id).value;\
        var dataRoom = document.getElementById('room' + id).value;\
        return {\
        type:\"TempSensor\",\
        id:id,\
        enabled:enableValue,\
        name:dataName,\
        pin:dataPin,\
        room:dataRoom,\
      };\
    }\
    function getEmptyConfigurationJson(id){\
        return {\
        type:\"Empty\",\
        id:id,\
        enabled:false,\
      };\
    }\
    function createConfigurationStringJson()\
    {\
        let devices = [];\
\
        for (let i = 1; i <= 6; i++) {\
            var deviceTypeValue = document.getElementById('type' + i).value;\
\
            if(deviceTypeValue == 43){\
                devices.push(getOnOffConfigurationJson(i));\
            } else if(deviceTypeValue == 44) {\
                devices.push(getLedStripConfigurationJson(i));\
            } else if(deviceTypeValue == 45) {\
                devices.push(getTempSensorConfigurationJson(i));\
            } else {\
                devices.push(getEmptyConfigurationJson(i));\
            }\
        }\
\
        let finalJson = {\
            devices: devices\
        };\
    \
        let jsonString = JSON.stringify(finalJson);\
        console.log(\"Wygenerowany JSON:\", jsonString);\
        var url = '/lclSetupJson&' + jsonString;\
\
        const xhr = new XMLHttpRequest();\
        const container = document.getElementById('popup-content');\
        showLoading(container);\
        xhr.timeout = 10000;\
        xhr.open(\"POST\", url, true);\
        xhr.onreadystatechange = function() {\
            if (xhr.readyState === 4) { \
                if (xhr.status === 200) { \
                    console.log('Config changed');\
                } else { \
                    console.log('Error with AJAX request');\
                }\
            }\
            hideLoading(container);\
            url = '/';\
            window.location.href = url;\
        };\
        xhr.send();\
    }\
    function roomMappingCreateString(count)\
    {\
        let roomMappings = [];\
        var mappingStr = '';\
        var lengthCount = 0;\
        for (let i = 1; i <= count; i++) {\
            var dataId = document.getElementById(\"roomMappingID\" + i).value;\
            var dataName = document.getElementById(\"roomMappingName\" + i).value;\
            \
            roomMappings.push({\
            roomId:dataId,\
            roomName:dataName\
            });\
        }\
        let finalJson = {\
            mappingsCount:count,\
            roomMapping:roomMappings\
        };\
        let jsonString = JSON.stringify(finalJson);\
        console.log(\"Wygenerowany room mapping JSON:\", jsonString);\
        url = '/roomMappingApply&' + jsonString;\
        window.location.href = url;\
    };\
    window.onload = function() {\
        document.querySelectorAll('.device-container').forEach(container => {\
            var checkbox = container.querySelector('input[type=\"checkbox\"]');\
            toggleDeviceConfig(checkbox);\
            checkbox.addEventListener('change', function() {\
                toggleDeviceConfig(this);\
            });\
        });\
\
        showExtraFields(document.getElementById('type1'), 'device-1');\
        showExtraFields(document.getElementById('type2'), 'device-2');\
        showExtraFields(document.getElementById('type3'), 'device-3');\
        showExtraFields(document.getElementById('type4'), 'device-4');\
        showExtraFields(document.getElementById('type5'), 'device-5');\
        showExtraFields(document.getElementById('type6'), 'device-6');\
    };\
\
\
        function showMessage(message, action) {\
            const popupMessage = document.getElementById('popup-message');\
            const popupButton = document.getElementById('popup-button');\
\
            popupMessage.textContent = message;\
\
            if (typeof action === 'string') {\
                popupButton.onclick = function () {\
                    window.location.href = action;\
                };\
            } else if (typeof action === 'function') {\
                popupButton.onclick = function () {\
                    action();\
                };\
            }\
\
            showPopup('popup-overlay', 'popup-content');\
\
            document.getElementById('popup-overlay-close').onclick = function () {\
                hidePopup('popup-overlay', 'popup-content');\
            };\
        }\
        function showPopup(overlay, content) {\
            const popupOverlay = document.getElementById(overlay);\
            const popupContent = document.getElementById(content);\
\
            popupOverlay.classList.remove('hidden-popup');\
            popupOverlay.classList.add('visible-popup');\
            popupContent.classList.add('show');\
        }\
\
        function hidePopup(overlay, content) {\
            const popupOverlay = document.getElementById(overlay);\
            const popupContent = document.getElementById(content);\
\
            popupOverlay.classList.remove('visible-popup');\
            popupOverlay.classList.add('hidden-popup');\
            popupContent.classList.remove('show');\
            isNotificationPollingActive = 1;\
        };\
        function showPasswordPopup() {\
            const passwordPopupOverlay = document.getElementById('password-popup-overlay');\
            const passwordPopupContent = document.getElementById('password-popup-content');\
\
            passwordPopupOverlay.classList.remove('hidden-popup');\
            passwordPopupOverlay.classList.add('visible-popup');\
            passwordPopupContent.classList.add('show');\
\
            document.getElementById('password-popup-close').onclick = function () {\
                hidePopup('password-popup-overlay', 'password-popup-content');\
            };\
            setTimeout(function() {\
                document.getElementById(\"password-input\").focus();\
            }, 0);\
        }\
        function showAdvancedControls() {\
            const popupOverlay = document.getElementById('advanced-ctrl-overlay');\
            const popupContent = document.getElementById('advanced-ctrl-popup');\
            const popupMessage = document.getElementById('advanced-ctrl-popup-msg');\
\
            document.getElementById('advanced-ctrl-popup-close').onclick = function () {\
                hidePopup('advanced-ctrl-overlay', 'advanced-ctrl-popup');\
            };\
\
            popupOverlay.classList.remove('hidden-popup');\
            popupOverlay.classList.add('visible-popup');\
            popupContent.classList.add('show');\
            popupMessage.innerHTML = '';\
\
        }\
\
        function submitPassword() {\
            const passwordValue = document.getElementById('password-input').value;\
            alert(`Password entered: ${passwordValue}`);\
            hidePopup(document.getElementById('password-popup-overlay'), document.getElementById('password-popup-content'));\
        }\
        function submitPassword() {\
            const passwordValue = document.getElementById('password-input').value;\
            hidePopup('password-popup-overlay', 'password-popup-content');\
            url = '/passwordApply' + passwordValue;\
            window.location.href = url;\
        }\
        function hexToRgb(hex) {\
            const match = hex.match(/^#([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})$/i);\
            if (match) {\
                return {\
                    r: parseInt(match[1], 16),\
                    g: parseInt(match[2], 16),\
                    b: parseInt(match[3], 16)\
                };\
            }\
            return null;\
        }\
        function sendColor(stripId) {\
            const colorInput = document.getElementById(`colorInput${stripId}`);\
            const color = colorInput.value;\
            const rgbColor = hexToRgb(color);\
            if (rgbColor) {\
                const url = `/ledStripColor?id=${stripId}&r=${rgbColor.r}&g=${rgbColor.g}&b=${rgbColor.b}`;\
                window.location.href = url;\
            }\
        }\
        function openColorPicker(index) {\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            selectedLedIndex = index;\
            colorInput.value = 0xFF00FA;\
            colorPickerPopup.style.display = \"flex\";\
            backdrop.style.display = \"block\";\
        }\
\
        function closePopup() {\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            colorPickerPopup.style.display = \"none\";\
            backdrop.style.display = \"none\";\
        }\
        function openLedStripMemorySlots() {\
            const FavouritesPopup = document.getElementById(\"FavouritesPopup\");\
            const composClose = document.getElementById(\"composClose\");\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            composClose.addEventListener(\"click\", closeCompositions);\
            FavouritesPopup.style.display = \"flex\";\
            backdrop.style.display = \"block\";\
        }\
\
        function closeCompositions() {\
            const FavouritesPopup = document.getElementById(\"FavouritesPopup\");\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            FavouritesPopup.style.display = \"none\";\
            backdrop.style.display = \"none\";\
        }\
        function openSaveCompositions() {\
            const FavouritesPopup = document.getElementById(\"SaveFavouritesPopup\");\
            const composClose = document.getElementById(\"composSaveClose\");\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            composClose.addEventListener(\"click\", closeSaveCompositions);\
            colorInput.value = 0xFF00FA;\
            FavouritesPopup.style.display = \"flex\";\
            backdrop.style.display = \"block\";\
        }\
\
        function closeSaveCompositions() {\
            const FavouritesPopup = document.getElementById(\"SaveFavouritesPopup\");\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            FavouritesPopup.style.display = \"none\";\
            backdrop.style.display = \"none\";\
        }\
        function rgbToHex(rgb) {\
            return rgb;\
        }\
        function colorClickedAction(index) {\
            const ledStrip = document.getElementById(\"ledStrip\");\
            const colorPickerPopup = document.getElementById(\"colorPickerPopup\");\
            const colorInput = document.getElementById(\"colorInput\");\
            const confirmButton = document.getElementById(\"confirmColorButton\");\
            const cancelButton = document.getElementById(\"cancelColorButton\");\
\
            openColorPicker(index);\
\
            confirmButton.addEventListener(\"click\", () => {\
            const rgbColor = hexToRgb(colorInput.value);\
            closePopup();\
\
            const url = `/ledColor?id=2&led=${selectedLedIndex}&r=${rgbColor.r}&g=${rgbColor.g}&b=${rgbColor.b}`;\
                window.location.href = url;\
            });\
            cancelButton.addEventListener(\"click\", closePopup);\
        }\
\
  function asyncDeviceStateSwitch(device, state){\
    const xhr = new XMLHttpRequest();\
    xhr.timeout = 10000;\
    var url = '/stDvstte' + device.toString() + 'state';\
    const container = document.getElementById('container' + device);\
    showLoading(container);\
    if(state === 1){\
        url = url + '1&';\
    }else {\
        url = url + '0&';\
    }\
    xhr.open(\"GET\", url, true);\
    xhr.onreadystatechange = function() {\
        if (xhr.readyState === 4) { \
            const statusElement = document.getElementById(\"status\");\
            if (xhr.status === 200) { \
                const newData = JSON.parse(xhr.responseText);\
                if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                    currentData = newData;\
                    renderRooms(currentData);\
                }\
            } else { \
                console.log('Error with AJAX request');\
            }\
        }\
        hideLoading(container);\
    };\
    xhr.send();\
  }\
  function handleJsonResponse(responseText = \"\"){\
    const response = JSON.parse(responseText);\
    if(response.status === \"succ\"){\
        if(response.type === \"switch\"){\
            var switchBtn = document.getElementById('switchBtn' + response.id);\
            var statusLight = document.getElementById('statusLight' + response.id);\
            if(response.state === \"on\"){\
                switchBtn.innerHTML = \"OFF\";\
                switchBtn.removeAttribute(\"onclick\");\
                switchBtn.onclick = function() { asyncDeviceStateSwitch(response.id, 0); };\
                statusLight.classList.remove('off');\
                statusLight.classList.add('on');\
            }else {\
                switchBtn.innerHTML = \"ON\";\
                switchBtn.removeAttribute(\"onclick\");\
                switchBtn.onclick = function() { asyncDeviceStateSwitch(response.id, 1); };\
                statusLight.classList.remove('on');\
                statusLight.classList.add('off');\
            }\
            console.log(\"Success\")\
        }else if(response.type === \"brightnessChange\"){\
            var slider = document.getElementById('brightnessSlider' + response.id);\
            slider.value = response.level;\
            console.log(\"Success\")\
        }\
    }\
  }\
  function showLoading(container) {\
    container.classList.add(\"loading\");\
    const overlay = container.querySelector(\".loading-overlay\");\
    if (overlay) {\
        overlay.style.display = \"flex\";\
    }\
}\
\
function hideLoading(container) {\
    container.classList.remove(\"loading\");\
    const overlay = container.querySelector(\".loading-overlay\");\
    if (overlay) {\
        overlay.style.display = \"none\";\
    }\
}\
function displayNotifications(data){\
    console.log(data);\
    if (data.count > 0) {\
        isNotificationPollingActive = 0;\
        const notificationsList = document.getElementById('notificationsList');\
\
        showPopup('notifications-popup-overlay', 'notificationsPopup');\
\
        notificationsList.innerHTML = \"\";\
\
        data[\"notifications:\"].forEach(notification => {\
            const notificationDiv = document.createElement('div');\
            notificationDiv.className = 'notification';\
            switch (notification.type){\
            case 0:\
                notificationDiv.classList.add('info');\
                break;\
            case 1:\
                notificationDiv.classList.add('warning');\
                break;\
            case 2:\
                notificationDiv.classList.add('error');\
                break;\
            default:\
            }\
\
            const titleDiv = document.createElement('div');\
            titleDiv.className = 'notification-title';\
            titleDiv.textContent = notification.title;\
\
            const timeDiv = document.createElement('div');\
            timeDiv.className = 'notification-body';\
            timeDiv.textContent = notification.time;\
\
            const bodyDiv = document.createElement('div');\
            bodyDiv.textContent = notification.body;\
\
            notificationDiv.appendChild(titleDiv);\
            notificationDiv.appendChild(timeDiv);\
            notificationDiv.appendChild(bodyDiv);\
\
            notificationsList.appendChild(notificationDiv);\
        });\
\
        document.getElementById('notification-popup-overlay-close').onclick = function () {\
            hidePopup('notifications-popup-overlay', 'notificationsPopup');\
        };\
    }\
}\
\
        \
function fetchDataxxx() {\
                const xhr = new XMLHttpRequest();\
                var url = '/getPageContent';\
                xhr.open(\"GET\", url, true);\
                xhr.onreadystatechange = function() {\
                    if (xhr.readyState === 4) { \
                        if (xhr.status === 200) { \
                            console.log(xhr.responseText);\
                            const response = JSON.parse(xhr.responseText);\
                            console.log(response);\
                        } else { \
                            console.log('Error with AJAX request');\
                        }\
                    }\
                };\
                xhr.send();\
            }\
\
async function getNotifications() {\
    if (isNotificationPollingActive) {\
        try {\
                console.log(\"Trying to get notifications\");\
                const response = await fetch('/getNotifications');\
                const newData = await response.json();\
    \
                displayNotifications(newData);\
        } catch (error) {\
            console.error('Error fetching notifications data:', error);\
        }\ 
    };\
}\
function downloadDeviceConfiguration(){\
    const xhr = new XMLHttpRequest();\
    var urlreq = '/dwlddevcfg';\
    xhr.open(\"GET\", urlreq, true);\
    xhr.onreadystatechange = function() {\
        if (xhr.readyState === 4) { \
            if (xhr.status === 200) { \
                console.log(xhr.responseText);\
                    const element = document.createElement('a'); \
                    element.setAttribute('href', 'data:text/plain;charset=utf-8,' + (xhr.responseText));\
                    element.setAttribute('download', 'HomeControlStationConfig.json');\
                    document.body.appendChild(element);\
                    element.click();\
                    document.body.removeChild(element);\
            } else { \
                console.log('Error with AJAX request');\
            }\
        }\
    };\
    xhr.send();\
}\
function uploadConfigFile() {\
  const input = document.createElement('input');\
  input.type = 'file';\
  input.accept = '.json';\
  input.addEventListener('change', function (event) {\
    const file = event.target.files[0];\
    if (file) {\
      const reader = new FileReader();\
      reader.onload = function (e) {\
      var url = '/loaddeicvcfg&' + e.target.result;\
      const xhr = new XMLHttpRequest();\
      xhr.open(\"GET\", url, true);\
      xhr.onreadystatechange = function() {\
        if (xhr.readyState === 4) { \
            if (xhr.status === 200) { \
                window.location.href = '/';\
            } else { \
                console.log('Error with AJAX request');\
            }\
        }\
      };\
      xhr.send();\
    };\
      reader.readAsText(file);\
    }\
  });\
  input.click();\
}\
function getExtendedControlsRequest(id, devContainer){\
    let json = {\"devId\":id};\
    let jsonString = JSON.stringify(json);\
    var url = '/getExtendedControls&' + jsonString;\
    showLoading(devContainer);\
    const xhr = new XMLHttpRequest();\
    xhr.open(\"POST\", url, true);\
    xhr.onreadystatechange = function() {\
        if (xhr.readyState === 4) { \
            if (xhr.status === 200) { \
                console.log('Advanced controls respose:' + xhr.responseText);\
                showAdvancedControls();\
                window.eval(xhr.responseText);\
            } else { \
                console.log('Error with AJAX request');\
            }\
        }\
        hideLoading(devContainer);\
    };\
    xhr.send();\
}\
function overWriteMemSlot(memSlotId, ledStripDevId){\
    let json = {\"devId\":ledStripDevId, \"slot\":memSlotId};\
    let jsonString = JSON.stringify(json);\
    var url = '/stripOverwriteSlot&' + jsonString;\
    const xhr = new XMLHttpRequest();\
    xhr.open(\"POST\", url, true);\
    xhr.onreadystatechange = function() {\
        if (xhr.readyState === 4) { \
            if (xhr.status === 200) { \
                const newData = JSON.parse(xhr.responseText);\
                if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                    currentData = newData;\
                    renderRooms(currentData);\
                }\
            } else { \
                console.log('Error with AJAX request');\
            }\
        }\
    };\
    xhr.send();\
    hidePopup('advanced-ctrl-overlay', 'advanced-ctrl-popup');\
    closeCompositions();\
}\
function loadMemSlot(memSlotId, ledStripDevId){\
    let json = {\"devId\":ledStripDevId, \"slot\":memSlotId};\
    let jsonString = JSON.stringify(json);\
    var url = '/stripLoadFromMemory&' + jsonString;\
    const xhr = new XMLHttpRequest();\
    xhr.open(\"POST\", url, true);\
    xhr.onreadystatechange = function() {\
        if (xhr.readyState === 4) { \
            if (xhr.status === 200) { \
                const newData = JSON.parse(xhr.responseText);\
                if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                    currentData = newData;\
                    renderRooms(currentData);\
                }\
            } else { \
                console.log('Error with AJAX request');\
            }\
        }\
    };\
    xhr.send();\
    hidePopup('advanced-ctrl-overlay', 'advanced-ctrl-popup');\
    closeCompositions();\
}\
function updateCurLimVal(id, val) {\
    var value = Math.round((val/255)*100);\
    document.getElementById(id).value=value;\
    document.getElementById(id).realValue=val;\
}\
\
\
function updateLocalDateTime(initialDateTime, elementId = 'currentDateTime') {\
    const dateTimeElement = document.getElementById(elementId);\
    if (!dateTimeElement) {\
        console.error(`Element o ID \"${elementId}\" nie istnieje`);\
        return null;\
    }\
    \
    let currentDate;\
    try {\
        const dateParts = initialDateTime.split(' ')[0].split('.');\
        const timeParts = initialDateTime.split(' ')[1].split(':');\
        \
        const year = parseInt(dateParts[0], 10);\
        const month = parseInt(dateParts[1], 10) - 1;\
        const day = parseInt(dateParts[2], 10);\
        const hours = parseInt(timeParts[0], 10);\
        const minutes = parseInt(timeParts[1], 10);\
        const seconds = parseInt(timeParts[2], 10);\
        \
        currentDate = new Date(year, month, day, hours, minutes, seconds);\
        \
        if (isNaN(currentDate.getTime())) {\
            throw new Error('Nieprawidłowa data');\
        }\
    } catch (error) {\
        console.warn('Błąd parsowania daty:', error);\
        currentDate = new Date();\
    }\
    \
    function updateDateTime() {\
        currentDate.setSeconds(currentDate.getSeconds() + 1);\
        \
        const year = currentDate.getFullYear();\
        const month = (currentDate.getMonth() + 1).toString().padStart(2, '0');\
        const day = currentDate.getDate().toString().padStart(2, '0');\
        const hours = currentDate.getHours().toString().padStart(2, '0');\
        const minutes = currentDate.getMinutes().toString().padStart(2, '0');\
        const seconds = currentDate.getSeconds().toString().padStart(2, '0');\
        \
        const formattedDateTime = `${year}.${month}.${day} ${hours}:${minutes}:${seconds}`;\
        dateTimeElement.textContent = formattedDateTime;\
    }\
    \
    updateDateTime();\
    return setInterval(updateDateTime, 1000);\
}\
\
\
</script>";

#endif


        

        